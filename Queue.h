#ifndef QUEUE_H_
#define QUEUE_H_
#include <memory>
#include <string>

template <typename T>
class Queue {
 public:
  Queue(int capacity) : capacity_(capacity), size_(0), tail_(nullptr) {
    queue_ = std::make_unique<T[]>(capacity_);
    tail_ = &queue_[0];
  }

  Queue(const Queue &rhs) : capacity_(rhs.capacity_), size_(rhs.size()), tail_(nullptr) {
    try {
      queue_ = std::make_unique<T[]>(capacity_);
      tail_ = &queue_[size_ - 1];

      for (int i = 0; i < size_; i++) {
        queue_[i] = rhs.queue_[i];
      }
    } catch (...) {
      std::cout << "Memory allocation unsuccessful\n";
    }
  }  // Copy constructor

  Queue &operator=(const Queue &rhs) {
    try {
      size_ = rhs.size();
      capacity_ = rhs.capacity_;
      queue_ = std::make_unique<T[]>(capacity_);
      tail_ = &queue_[size_ - 1];

      for (int i = 0; i < size_; i++) {
        queue_[i] = rhs.queue_[i];
      }
    } catch (...) {
      std::cout << "Memory allocation unsuccessful\n";
    }
    return *this;
  }  // Copy assignment

  Queue(Queue &&rhs) noexcept
      : capacity_(rhs.capacity_), size_(rhs.size_), tail_(rhs.tail_) {
    this->queue_ = std::move(rhs.queue_);
    rhs.tail_ = nullptr;
    rhs.size_ = 0;
  }  // Move constructor

  Queue &operator=(Queue &&rhs) noexcept {
    this->size_ = rhs.size();
    this->capacity_ = rhs.capacity_;
    this->tail_ = rhs.tail_;
    this->queue_ = std::move(rhs.queue_);

    rhs.tail_ = nullptr;
    rhs.size_ = 0;
    return *this;
  }  // Move assignment
  ~Queue() {}

  void print() {
    if (this->isEmpty()) return;

    std::cout << "[ ";
    for (int i = 0; i < size_; i++) {
      std::cout << queue_[i] << ", ";
    }
    std::cout << "]\n";
  }

  bool isEmpty() const { return (size_ == 0); }
  bool isFull() const { return (size_ >= capacity_); }
  int size() const { return size_; }

  T dequeue() {
    if (size_ <= 0) {
      std::cout << "dequeue attempting to access empty queue\n";
      throw std::string("dequeue attempting to access empty queue");
    }
    T val = *tail_;
    --size_;
    if (size_ == 0) {
      tail_ = nullptr;
    } else {
      tail_ = &queue_[size_ - 1];
    }
    return val;
  }

  void enqueue(const T &val) {
    if (this->isFull()) {
      std::cout << "Queue Full\n";
      return;
    }
    if (this->isEmpty() || size_ == 1) {
      if (size_ == 1) {
        queue_[1] = queue_[0];
        queue_[0] = val;
      } else {
        queue_[0] = val;
      }
      ++size_;
      tail_ = &queue_[size_ - 1];
      return;
    }

    ++size_;
    T leftValue = queue_[0], rightValue;
    for (int i = 1; i < size_; i++) {
      rightValue = queue_[i];
      queue_[i] = leftValue;
      leftValue = rightValue;
    }
    queue_[0] = val;
    tail_ = &queue_[size_ - 1];
  }
  T front() const { return *tail_; }

 private:
  int size_;
  int capacity_;
  T* tail_;
  std::unique_ptr<T[]> queue_;
};

#endif  // QUEUE_H_