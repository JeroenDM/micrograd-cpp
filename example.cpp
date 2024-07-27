#include <cstdint>
#include <cstdio>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "engine.h"

using namespace micrograd;

struct PairHasher final {
public:
  template <class TFirst, class TSecond>
  size_t operator()(const std::pair<TFirst, TSecond> &p) const noexcept {
    uintmax_t hash = std::hash<TFirst>{}(p.first);
    hash <<= sizeof(uintmax_t) * 4;
    hash ^= std::hash<TSecond>{}(p.second);
    return std::hash<uintmax_t>{}(hash);
  }
};

class Graph {
public:
  void add_node(ValueRef node, std::string options = "[]") {
    nodes.insert(id(node));
    data.insert({id(node), options});
  };

  void add_edge(ValueRef x, ValueRef y) { edges.insert({id(x), id(y)}); }

  void print_dot() {
    std::printf("digraph Micrograd{\n");
    for (auto n : nodes) {
      //   std::printf("%lu[shape=rectangle,label=\"%s\"] \n", n,
      //   data[n]->label.c_str());
      std::printf("%lu%s\n", n, data[n].c_str());
    }
    for (auto &[x, y] : edges) {
      std::printf("%lu -> %lu\n", x, y);
    }
    std::printf("}\n");
  }

private:
  std::unordered_set<Id> nodes{};
  std::unordered_set<std::pair<Id, Id>, PairHasher> edges{};
  std::unordered_map<Id, std::string> data;
};

void build_graph(Graph &g, ValueRef node) {
  std::string label{"[shape=rectangle,label=\""};
  label += node->label;
  label += "\"]";
  g.add_node(node, label);

  //   if (node->op != '0') {
  //     std::string op_label{"[shape=circle,label=\""};
  //     op_label += std::string{node->op};
  //     op_label += "\"]";
  //     auto op_node = Value(0.0);
  //     g.add_node(op_node, op_label);
  //     g.add_edge(op_node, node);
  //     for (ValueRef child : node->children) {
  //       g.add_edge(op_node, child);
  //       build_graph(g, child);
  //     }
  //   } else {
  for (ValueRef child : node->children) {
    g.add_edge(node, child);
    build_graph(g, child);
  }
  //   }
}

void print_graph(ValueRef root) {
  Graph g;
  build_graph(g, root);
  g.print_dot();
}

int main() {
  auto x = Value(-4.0);
  auto z = 2 * x + 2 + x;
  auto q = relu(z) + z * x;
  auto h = relu(z * z);
  auto y = h + q + q * x;
  // auto y.backward();
  // auto xmg, ymg = x, y;

  x->label = "x";
  z->label = "z";
  q->label = "q";
  h->label = "h";
  y->label = "y";

  //   std::printf("res = %f\n", y->data);

  print_graph(y);

  return 0;
}
