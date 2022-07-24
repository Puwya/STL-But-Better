#ifndef LINKED_QUEUE_H_
#define LINKED_QUEUE_H_
#include <memory>
#include <string>

template <typename T>
struct Node {
  T val_;
  Node* next_;
  Node(const T &val, Node* node) : val_(val), next_(node) {}
  Node() : next_(nullptr) {}
};

template <typename T>
class LinkedQueue {
 public:
  LinkedQueue(int capacity)
      : capacity_(capacity), size_(0), front_(nullptr), head_(nullptr) {}
  LinkedQueue(const LinkedQueue &rhs) : LinkedQueue(rhs.capacity_) { *this = rhs; }
  LinkedQueue &operator=(const LinkedQueue &rhs) {
    if (this == &rhs) return *this;
    this->~LinkedQueue();
    if (rhs.isEmpty()) {
      capacity_ = rhs.capacity_;
      return *this;
    }
    try {
      Node<T>* rhsNode = rhs.head_;
      Node<T>* node = nullptr;
      head_ = new Node<T>(rhsNode->val_, nullptr);
      front_ = head_;
      rhsNode = rhsNode->next_;

      while (rhsNode != nullptr) {
        node = new Node<T>(rhsNode->val_, nullptr);
        front_->next_ = node;
        front_ = node;
        rhsNode = rhsNode->next_;
      }
      size_ = rhs.size_;
    } catch (...) {
      std::cout << "Failed Allocation\n";
    }
    return *this;
  }
  LinkedQueue(LinkedQueue &&rhs) noexcept : LinkedQueue(rhs.capacity_) {
    *this = std::move(rhs);
  }
  LinkedQueue &operator=(LinkedQueue &&rhs) noexcept {
    if (this == &rhs) return *this;
    this->~LinkedQueue();
    if (rhs.isEmpty()) {
      capacity_ = rhs.capacity_;
      return *this;
    }
    head_ = rhs.head_;
    Node<T>* node = rhs.head_;
    while (node != nullptr) {
      front_ = node;
      node = node->next_;
    }
    rhs.head_ = nullptr;
    rhs.front_ = nullptr;
    rhs.size_ = 0;
    return *this;
  }

  ~LinkedQueue() {
    for (int i = 0; i < size_; i++) {
      this->dequeue();
    }
  }

  void print() {
    if (this->isEmpty()) return;
    Node<T>* node = head_;

    std::cout << "[ ";
    while (node != nullptr) {
      std::cout << node->val_ << ", ";
      node = node->next_;
    }
    std::cout << "]\n";
  }

  bool isEmpty() const { return (head_ == nullptr); }
  bool isFull() const { return (size_ >= capacity_); }
  int size() const { return size_; }

  T dequeue() {
    if (this->isEmpty()) {
      throw std::string("dequeue attempting to access empty queue");
    }
    Node<T>* node = head_;
    T val = front_->val_;

    if (size_ != 1) {
      while (node->next_->next_ != nullptr) {
        node = node->next_;
      }
    } else {
      head_ = nullptr;
    }

    delete front_;
    front_ = node;
    front_->next_ = nullptr;
    --size_;
    return val;
  }

  void enqueue(const T &val) {
    if (this->isFull()) {
      std::cout << "Queue Full\n";
      return;
    }
    Node<T>* node = head_;

    try {
      head_ = new Node<T>(val, node);
      if (head_->next_ == nullptr) {
        front_ = head_;
      }
      ++size_;
    } catch (...) {
      std::cout << "Failed allocation\n";
    }
  }
  T front() const {
    if (this->isEmpty()) throw std::string("front attempting to access empty queue");
    return *front_;
  }

 private:
  int size_;
  int capacity_;
  Node<T>* front_;
  Node<T>* head_;
};

#endif  // LINKED_QUEUE_H_