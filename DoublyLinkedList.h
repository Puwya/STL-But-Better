#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_
#include <initializer_list>
#include <iostream>
#include <memory>
#include <string>

template <typename T>
struct Node {
  T val_;
  std::unique_ptr<Node<T>> next_;
  Node<T>* prev_;
};

template <typename T>
class DList {
 public:
  DList(std::initializer_list<T> init) : size_(init.size()) {
    head_ = std::make_unique<Node<T>>();
    Node<T>* node = head_.get();
    Node<T>* prevNode = node;
    node->val_ = *init.begin();

    std::for_each(init.begin() + 1, init.end(), [&](const T &val) {
      node->next_ = std::make_unique<Node<T>>();
      node = node->next_.get();
      node->val_ = val;
      node->prev_ = prevNode;
      prevNode = node;
    });
    tail_ = node;
    tail_->next_ = nullptr;
    head_->prev_ = nullptr;
  }

  DList(const int &size) : size_(size), tail_(nullptr) {
    if (this->isEmpty()) return;
    head_ = std::make_unique<Node<T>>();
    Node<T>* node = head_.get();
    Node<T>* prevNode = node;

    for (int i = 1; i < size_; i++) {
      node->next_ = std::make_unique<Node<T>>();
      node = node->next_.get();
      node->prev_ = prevNode;
      prevNode = node;
    }
    tail_ = node;
    tail_->next_ = nullptr;
    head_->prev_ = nullptr;
  }
  // Copy Constructor
  DList(const DList &rhs) : size_(rhs.size_) { *this = rhs; }
  // Copy Assignment
  DList &operator=(const DList &rhs) {
    if (this == &rhs) return *this;
    head_.reset();
    tail_ = nullptr;
    if (rhs.isEmpty()) {
      size_ = rhs.size_;
      return *this;
    }

    head_ = std::make_unique<Node<T>>();
    Node<T>* node = head_.get();
    Node<T>* prevNode = node;

    Node<T>* rhsNode = rhs.head_.get();
    node->val_ = rhsNode->val_;
    rhsNode = rhsNode->next_.get();

    while (rhsNode != nullptr) {
      node->next_ = std::make_unique<Node<T>>();
      node = node->next_.get();
      node->val_ = rhsNode->val_;
      node->prev_ = prevNode;
      prevNode = node;
      rhsNode = rhsNode->next_.get();
    }
    tail_ = node;
    tail_->next_ = nullptr;
    head_->prev_ = nullptr;

    return *this;
  }
  // Move Constructor
  DList(DList &&rhs) noexcept : size_(rhs.size_) { *this = std::move(rhs); }
  // Move Assignment
  DList &operator=(DList &&rhs) {
    if (this == &rhs) return *this;
    head_.reset();
    tail_ = nullptr;
    if (rhs.isEmpty()) {
      size_ = rhs.size_;
      return *this;
    }
    head_ = std::move(rhs.head_);
    tail_ = rhs.tail_;
    rhs.tail_ = nullptr;
    rhs.size_ = 0;

    return *this;
  }
  // Smart Pointers handling deallocation
  ~DList() {}

  // Creates new list to keep *this intact
  DList reverse() const {
    Node<T>* node = tail_;
    DList<T> tList(this->size_);

    for (int i = 0; i < tList.size(); i++) {
      tList[i] = node->val_;
      node = node->prev_;
    }
    return tList;
  }

  bool remove(const int &index) {
    if (this->isEmpty() || index == size_) return false;
    if (index == 0) {
      if (head_->next_ == nullptr) {
        head_.reset();
        tail_ = nullptr;
      } else {
        head_ = std::move(head_->next_);
        head_->prev_ = nullptr;
      }
      return true;
    }
    if (index == size_ - 1) {
      tail_ = tail_->prev_;
      tail_->next_.reset();
      return true;
    }

    Node<T>* node = head_.get();
    int counter = 0;

    while (node != nullptr) {
      if (counter == index - 1) break;
      ++counter;
      node = node->next_.get();
    }

    std::unique_ptr<Node<T>> __node;
    __node = std::move(node->next_->next_);
    node->next_.reset();
    try {
      __node->prev_ = node;
    } catch (...) {
    }
    node->next_ = std::move(__node);
    return true;
  }

  void insert(const int &index, const T &val) {
    if (this->isEmpty()) throw std::string("Inserting into empty list");
    if (index < -1 || index > size_) throw std::string("Accessing invalid memory");

    if (index - 1 == -1) {
      std::unique_ptr<Node<T>> node = std::make_unique<Node<T>>();
      node->val_ = val;
      head_->prev_ = node.get();
      node->next_ = std::move(head_);
      head_ = std::move(node);
      head_->prev_ = nullptr;
      ++size_;
      return;
    }

    Node<T>* node = head_.get();
    int counter = 0;
    while (node != nullptr) {
      if (counter == index - 1) {
        break;
      }
      ++counter;
      node = node->next_.get();
    }
    std::unique_ptr<Node<T>> __node(node->next_.release());
    node->next_ = std::make_unique<Node<T>>();
    node->next_->val_ = val;
    node->next_->prev_ = node;
    node->next_->next_ = std::move(__node);
    node->next_->next_->prev_ = node->next_.get();
    ++size_;
  }

  void print() const {
    if (this->isEmpty()) return;
    Node<T>* node = head_.get();

    std::cout << "[ ";
    while (node != nullptr) {
      std::cout << node->val_ << ((node->next_ != nullptr) ? ", " : " ");
      node = node->next_.get();
    }
    std::cout << "]\n";
  }

  int size() const { return size_; }
  bool isEmpty() const { return (size_ <= 0); }

  T &operator[](const int &index) {
    if (this->isEmpty()) throw std::string("Accessing invalid memory");
    Node<T>* node = head_.get();
    int counter = 0;

    while (node != nullptr) {
      if (counter == index) {
        return node->val_;
      }
      ++counter;
      node = node->next_.get();
    }
    throw std::string("Accessing invalid memory!");
  }

 private:
  int size_;
  std::unique_ptr<Node<T>> head_;
  Node<T>* tail_;
};

#endif  // LINKED_LIST_H_