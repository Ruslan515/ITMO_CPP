//
// Created by ruslan515 on 20.11.22.
//

#include <iostream>
#include <memory>

#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H

using uchar = unsigned char;

class Node {
public:
  using pointer = std::shared_ptr<Node>;

  pointer left {nullptr};
  pointer right {nullptr};
  pointer parent{nullptr};

  Node() = default;
  Node(uchar uch, int f): ch(uch), frequency(f) {}
  Node(const std::string& n, int f): name(n), frequency(f) {}

  bool operator<(const Node& oth) const {
    return frequency < oth.frequency;
  }

  friend std::ostream& operator<<(std::ostream& os, const Node& node);

  int get_frequency() const {
    return frequency;
  }

  std::string code() const {
    return code_string;
  }

  void code(const std::string& c) {
    code_string = c;
  }

  std::string get_name() const {
    if (ch == 0) {
      return name;
    }
    else {
      if (ch == 10) {
        return "\\n";
      }
      return std::string (1, static_cast<char>(ch));
    }
  }

  uchar get_byte() const {
    return ch;
  }

  bool is_leaf() const {
    return (left == nullptr && right == nullptr);
  }

private:
  std::string name {""};
  uchar ch {0};
  int frequency {0};
  std::string code_string {""};
};

std::ostream& operator<<(std::ostream& os, const Node& node) {
  return os << "[" << node.get_name() << "] = " << node.frequency;
}

class LowestPriority {
public:
  bool operator()(const Node::pointer& left, const Node::pointer& right) const {
    return left->get_frequency() > right->get_frequency();
  }
};

#endif // HUFFMAN_NODE_H
