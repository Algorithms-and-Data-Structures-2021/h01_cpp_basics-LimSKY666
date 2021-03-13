#include "book_store.hpp"

#include <algorithm>  // copy
#include <stdexcept>  // invalid_argument

// 1. реализуйте функцию ...
ResizeStorageStatus resize_storage(Book *&storage, int size, int new_capacity) {
  if (storage == nullptr) {
      return ResizeStorageStatus::NULL_STORAGE;
  }
  if (new_capacity <= size) {
      return ResizeStorageStatus::INSUFFICIENT_CAPACITY;
  }
  if (size < 0) {
      return  ResizeStorageStatus::NEGATIVE_SIZE;
  }
  if (storage != nullptr && new_capacity >= size && size > 0) {
    auto newStorage = new Book[new_capacity];
    std::copy(storage, storage+size, newStorage);
    delete[] storage;
    storage = newStorage;
  }
  return ResizeStorageStatus::SUCCESS;
}

// 2. реализуйте конструктор ...
BookStore::BookStore(const std::string &name) : name_{name} {
  // валидация аргумента
  if (name.empty()) {
    throw std::invalid_argument("BookStore::name must not be empty");
  }
    name_ = name;
    storage_capacity_ = kInitStorageCapacity;
    storage_ = new Book[storage_capacity_];
  // здесь мог бы быть ваш сотрясающий землю и выделяющий память код ...
}

// 3. реализуйте деструктор ...
BookStore::~BookStore() {
    if (storage_ != nullptr) {
        delete[] storage_;
        storage_ = nullptr;
    }
    storage_capacity_ = 0;
    storage_size_ = 0;
}

// 4. реализуйте метод ...
void BookStore::AddBook(const Book &book) {
  if (storage_size_ == storage_capacity_) {
        resize_storage_internal(storage_capacity_ + kCapacityCoefficient);
    // Tip 1: используйте функцию resize_storage_internal, задав новый размер хранилища
    // Tip 2: не забудьте обработать статус вызова функции
  }
  storage_[storage_size_] = book;
  storage_size_+=1;
}

// РЕАЛИЗОВАНО

const std::string &BookStore::GetName() const {
  return name_;
}

int BookStore::GetSize() const {
  return storage_size_;
}

int BookStore::GetCapacity() const {
  return storage_capacity_;
}

const Book *BookStore::GetBooks() const {
  return storage_;
}

ResizeStorageStatus BookStore::resize_storage_internal(int new_capacity) {
  // изменяем размеры хранилища с копированием старых данных в хранилище нового объема
  const ResizeStorageStatus status = resize_storage(storage_, storage_size_, new_capacity);

  // если все прошло ОК, то сохраняем новый объем хранилища в объекте
  if (status == ResizeStorageStatus::SUCCESS) {
    storage_capacity_ = new_capacity;
  }

  return status;
}