/*
Author: github.com/annadostoevskaya
File: unique_ptr.h
Date: 20/09/23 18:26:11

Description: custom `std::unique_ptr` class. It's no ship code! 
  We have little problem with unique_ptr<T[]> specialization!
  Also, we don't implement swap for this class! 
*/

template <typename T>
class unique_ptr
{
  T* pointer_;
public:
  unique_ptr() : pointer_(nullptr) {}
  unique_ptr(T* pointer) : pointer_(pointer) {}
  unique_ptr(unique_ptr<T>&& uniq)
  {
    pointer_ = uniq.pointer_;
    uniq.pointer_ = nullptr;
  }
  
  ~unique_ptr() { delete pointer_; }
  unique_ptr(unique_ptr& uniq) = delete;
  unique_ptr& operator=(const unique_ptr& uniq) = delete;
  T* operator->() const { return pointer_; }
  T& operator*() const { return *pointer_; }
  void reset(T* pointer) 
  { 
    if (pointer_) delete pointer_;
    pointer_ = pointer; 
  }
  T* release() 
  { 
    T* pointer = pointer_;
    pointer_ = nullptr;
    return pointer; 
  }
  T* get() { return pointer_; }
  const T* get() const { return pointer_; }
};
