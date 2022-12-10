#include "node.h"
#include "huffman.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include "vector"
#include "queue"
#include "algorithm"
#include <limits.h>
#include <bitset>

namespace mse
{

using queue_t = std::priority_queue<Node::pointer , std::vector<Node::pointer>, LowestPriority>;

std::ifstream ::pos_type get_size(const std::string& fn) {
  std::ifstream ifs(fn, std::ifstream::ate);
  return ifs.tellg();
}


void read_file(const std::string& filename, std::vector<int>& frequency) {
  int filesize = get_size(filename);
  if (filesize < 0) {
    std::cerr << "error in " << __PRETTY_FUNCTION__ << std::strerror(errno);
    return;
  }

  std::ifstream ifs(filename, std::ifstream::binary);
  if (!ifs) {
    std::cerr << "error in " << __PRETTY_FUNCTION__ << std::strerror(errno);
    return;
  }

// std::count << "Reading file " << filename << ":" << std::endl;
  int jj = 0;
  while (true) {
    char ch;
    ifs.read(&ch, 1);
    if (ifs.eof()) {
      break;
    }

    frequency[static_cast<unsigned char>(ch)]++;

//    if (jj % 20 == 0){
//      int value = (jj + filesize % 20) * 100.0 / filesize;
//      std::cout << "\r" << value << "%" << std::flush;
//
//    }
    ++jj;
    //    std::cout << (int)ch << ", ";
  }

//  int size = frequency.size();
//  for(int i = 0; i < size; ++i) {
//    if (frequency[i] != 0) {
//      std::cout << "[" << i << "] = " << frequency[i] << ", \t" << " \n";
//    }
//  }
//
//  std::cout << std::endl;

  return ;
}

void make_code(Node::pointer& node, std::string str, std::vector<std::string>& codes) {
  if (node->left != nullptr) {
    make_code(node->left, str + "0", codes);
  }

  if (node->right != nullptr) {
    make_code(node->right, str + "1", codes);
  }

  if (node->left == nullptr && node->right == nullptr) {
    node->code(str);
    codes[node->get_byte()] = str;
  // std::count << "leaf: " << *node << " code: " << node->code() << "\n";
  }
}

void fill_queue(std::vector<int>& frequency, queue_t& queue) {
  uchar byte = 0;
  for_each(frequency.begin(), frequency.end(), [&queue, &byte](const auto& value) {
    if (value != 0) {
      Node::pointer node = std::make_shared<Node>(byte, value);
      queue.push(node);
    }
    ++byte;
  });
}

void build_tree(queue_t& queue) {
  while (queue.size() > 1)
  {
    Node::pointer x = queue.top();
    queue.pop();

    Node::pointer y = queue.top();
    queue.pop();

    std::string name = x->get_name() + y->get_name();

    Node::pointer z = std::make_shared<Node>(name, x->get_frequency() + y->get_frequency());

    z->left = x;
    z->right = y;

    x->parent = z;
    y->parent = z;

    queue.push(z);
  }
}

std::string message2code(const std::string& filename, const std::vector<std::string>& codes) {
  std::string msg{""};
  std::ifstream ifs(filename, std::ifstream::binary);
  if (!ifs) {
    std::cerr << "error in " << __PRETTY_FUNCTION__ << std::strerror(errno);
    return msg;
  }
  while (true) {
    char ch;
    ifs.read(&ch, 1);
    if(ifs.eof()) {
      break ;
    }
    msg += codes[static_cast<uchar>(ch)];
  }
  return msg;
}

void write_file(const std::string& filename, std::vector<int>& frequency, const queue_t& queue, const std::string& message) {
  std::ofstream ofs(filename, std::ofstream::binary);
  if (!ofs) {
    std::cerr << "error in " << __PRETTY_FUNCTION__ << std::strerror(errno);
    return;
  }
  uchar count = std::count_if(frequency.begin(), frequency.end(), [](const int& value) {return (value != 0);});
//  std::cout << "Count: " << (int)count << std::endl;

  ofs.write(reinterpret_cast<char *>(&count), sizeof count);

  std::for_each(frequency.begin(), frequency.end(), [index = uchar(0), &ofs](int& value) mutable {
    if (value != 0) {
      ofs.write(reinterpret_cast<char*>(&index), sizeof index);
      ofs.write(reinterpret_cast<char*>(&value), sizeof value);
    }
    ++index;
  });

  int byte_count = message.size() / CHAR_BIT;
  uchar modulo = message.size() % CHAR_BIT;
// std::count << "byte count: " << byte_count << " of size " << message.size() << std::endl;
// std::count << "modulo: " << (int)modulo << " of size " << message.size() << std::endl;

  ofs.write(reinterpret_cast<char*>(&byte_count), sizeof byte_count);
  ofs.write(reinterpret_cast<char*>(&modulo), sizeof modulo);

  int i = 0;
  for(; i < byte_count; ++i) {
    std::bitset<CHAR_BIT> b(message.substr(i * CHAR_BIT, CHAR_BIT));
    uchar value = static_cast<uchar>(b.to_ulong());
    ofs.write(reinterpret_cast<char*>(&value), sizeof value);
  }

//  for(; i < byte_count * CHAR_BIT; i += CHAR_BIT) {
//    std::bitset<CHAR_BIT> b(message.substr(i, CHAR_BIT));
//    uchar value = b.to_ulong();
//    ofs.write((char*) &value, sizeof(uchar));
//  }

  if (modulo > 0) {
    std::bitset<CHAR_BIT> b(message.substr(i * CHAR_BIT, modulo));
    uchar value = static_cast<uchar>(b.to_ulong());
    ofs.write(reinterpret_cast<char*>(&value), sizeof value);
  }

  queue.top();
// std::count << message << "\n";

}

void read_decoding_file(std::string& filename, std::vector<int>& frequency, std::string& message) {
  std::ifstream ifs(filename, std::ifstream::binary);
  if (!ifs) {
    std::cerr << "error in " << __PRETTY_FUNCTION__ << std::strerror(errno);
    return;
  }

  uchar count = 0;
  ifs.read(reinterpret_cast<char *>(&count), sizeof count);
// std::count << "New count: " << (int)count << "\n";

  int i = 0;
  while (i < count) {
    uchar ch;
    ifs.read(reinterpret_cast<char *>(&ch), sizeof ch);

    int f = 0;
    ifs.read(reinterpret_cast<char *>(&f), sizeof f);
    frequency[ch] = f;
    ++i;
  }

//  std::for_each(frequency.begin(), frequency.end(), [index = uchar(0)] (const int& value) mutable  {
//    if (value != 0) {
//      std::cout << "[" << static_cast<char>(index) << "] = " << value << std::endl;
//    }
//    ++index;
//  });

  int byte_count = 0;
  uchar modulo = 0;
  ifs.read(reinterpret_cast<char*>(&byte_count), sizeof byte_count);
  ifs.read(reinterpret_cast<char*>(&modulo), sizeof modulo);
//  std::cout << "New byte count: " << byte_count << "\n";
//  std::cout << "modulo: " << (int)modulo << "\n";

  i = 0;
  for(; i < byte_count; ++i) {
    uchar byte;
    ifs.read(reinterpret_cast<char*>(&byte), sizeof byte);
    std::bitset<CHAR_BIT> b(byte);
    message += b.to_string();
  }

  if (modulo > 0) {
    uchar byte;
    ifs.read(reinterpret_cast<char*>(&byte), sizeof byte);
    std::bitset<CHAR_BIT> b(byte);
    message += b.to_string().substr(CHAR_BIT - modulo, CHAR_BIT);
  }

// std::count << message << std::endl;

// std::count << frequency[0] << message << "\n";
  return ;
}


void make_char(const Node::pointer& root, const std::string& message, std::string& text) {
  Node::pointer node = root;
  for(size_t i = 0; i < message.size(); ++i) {
    char ch = message[i];
    if (ch == '0') {
      if (node->left != nullptr) {
        node = node->left;
        if (node->is_leaf()) {
          text += node->get_byte();
          node = root;
        }
      }
    }
    else if (ch == '1') {
      if (node->right != nullptr) {
        node = node->right;
        if (node->is_leaf()) {
          text += node->get_byte();
          node = root;
        }
      }

    }
  }

// std::count << "Text: [" << text << "]" << std::endl;
  return ;
}

void write_decoding_file(const std::string& filename, const std::string& text) {
  std::ofstream ofs(filename, std::ofstream::binary);
  if (!ofs) {
    std::cerr << "error in " << __PRETTY_FUNCTION__ << std::strerror(errno);
    return;
  }
  ofs << text;
}


void huffman(int command, std::string& in_file, std::string& out_file)
{
  if (command == 1) { // code
    std::vector<int> frequency(0x100, 0);
    //  std::string in_file = "/media/ruslan515/DATA_LINUX/ITMO/I_semestr/C++/hw_05_copy/test/text.txt";
    //  std::string out_file = "/media/ruslan515/DATA_LINUX/ITMO/I_semestr/C++/hw_05_copy/test/text.out";
    read_file(in_file, frequency);

    //  std::cout << frequency.size() << std::endl;
    queue_t queue;
    fill_queue(frequency, queue);
    build_tree(queue);

    //  int size = frequency.size();
    //  for (int i = 0; i < size; ++i) {
    //    if (frequency[i] != 0) {
    //      Node node(i, frequency[i]);
    //      queue.push(node);
    //    }
    //  }

    //  while (!queue.empty()) {
    //    std::cout << *queue.top() << " ";
    //    queue.pop();
    //  }

    std::vector<std::string > codes(0x100, "");
    Node::pointer root = queue.top();
    make_code(root, "", codes);

    std::string message = message2code(in_file, codes);
  // std::count << message << std::endl;

    write_file(out_file, frequency, queue, message);

  }
  else if (command == 2) {  // decode
    std::vector<int> frequency2(0x100, 0);
    std::string message2 = "";
    read_decoding_file(in_file, frequency2, message2);

    queue_t queue2;
    fill_queue(frequency2, queue2);
    build_tree(queue2);
    Node::pointer root2 = queue2.top();
    std::string text = "";
    make_char(root2, message2, text);
    write_decoding_file(out_file, text);
  }
  else if (command == 3) {  // code with -v
    std::cout << 3;
  }
  else if (command == 4) {  // decode with -v
    std::cout << 4;
  }

  return;
}

} // namespace mse
