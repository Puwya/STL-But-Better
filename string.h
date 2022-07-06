#ifndef STRING_H_
#define STRING_H_
#include <cstring>
#include <iostream>
#include <string>

namespace los {
class String {
 public:
  // Constructors & Destructors
  String() : m_str(nullptr) {}
  String(const char* str);
  String(const String &rhs);
  // using move implementation for r value
  String(String &&rValue);
  ~String();

  // Methods
  void print() const;
  // Getters & Setters
  // YOU HAVE TO DEALLOCATE MEMORY AFTER
  char* get_m_str() const { return this->m_str; }
  void set_m_str(const char* str) {
    delete[] this->m_str;
    this->m_str = new char[std::strlen(str) + 1];
    std::strcpy(this->m_str, str);
  }
  // Overloaded operators
  void operator=(const String &rhs);
  void operator=(String &&rValue);
  String operator+(const String &rhs);
  friend void operator+=(String &obj, const String &rhs);
  friend std::ostream &operator<<(std::ostream &os, const String &obj);
  friend std::istream &operator>>(std::istream &is, String &obj);

  // Attributes
 private:
  char* m_str;
};
// Method Implementations
// String literal constructor
String::String(const char* str) : m_str(nullptr) {
  this->m_str = new char[std::strlen(str) + 1];
  std::strcpy(this->m_str, str);
}
// Copy Constructor
String::String(const String &rhs) : m_str(nullptr) {
  this->m_str = new char[std::strlen(rhs.m_str) + 1];
  std::strcpy(this->m_str, rhs.m_str);
}
// Move Constructor
String::String(String &&rValue) : m_str(nullptr) {
  this->m_str = rValue.m_str;
  rValue.m_str = nullptr;
}
// Destructor
String::~String() { delete[] m_str; }
// Print Method
void String::print() const {
  if (this->m_str != nullptr) {
    std::cout << this->m_str << "\n";
  } else {
    std::cout << "nullptr\n";
  }
}
// Copy Assignment Operator
void String::operator=(const String &rhs) {
  delete[] this->m_str;
  this->m_str = new char[std::strlen(rhs.m_str) + 1];
  std::strcpy(this->m_str, rhs.m_str);
}
// Move Assignment Operator
void String::operator=(String &&rValue) {
  delete[] this->m_str;
  this->m_str = rValue.m_str;
  rValue.m_str = nullptr;
}
// + Operator
String String::operator+(const String &rhs) {
  String temp{std::strcat(this->m_str, rhs.m_str)};
  return temp;
}
// += Operator
void operator+=(String &obj, const String &rhs) { obj.operator+(rhs); }
// Multiply string times user specified amount
String operator*(String &obj, const int multiplyBy) {
  char* temp = new char[std::strlen(obj.get_m_str()) * multiplyBy + 1];
  // Cat cstrings together n times
  for (int i = 0; i < multiplyBy; i++) {
    std::strcat(temp, obj.get_m_str());
  }
  obj.set_m_str(temp);
  return obj;
}
void operator*=(String &obj, const int multiplyBy) { obj = obj * multiplyBy; }
// Insertion Operator
std::ostream &operator<<(std::ostream &os, const String &obj) {
  if (obj.m_str != nullptr) {
    os << obj.m_str;
  } else {
    os << "nullptr\n";
  }
  return os;
}
// Extraction Operator
std::istream &operator>>(std::istream &is, String &obj) {
  std::string temp;
  is >> temp;
  obj.set_m_str(temp.c_str());
  return is;
}
}  // namespace los
#endif  // STRING_H_