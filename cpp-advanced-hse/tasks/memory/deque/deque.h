#pragma once

#include <cstring>
#include <initializer_list>
#include <algorithm>
#include <deque>
#include <iostream>

#define DEFAULT_SIZE 10

class Deque {
public:
    Deque();
    Deque(const Deque& rhs);
    explicit Deque(const size_t size);
    Deque(Deque&& rhs);

    Deque(std::initializer_list<int> list);
    ~Deque() {
        Clear();
    }

    Deque& operator=(Deque rhs) {
        Swap(rhs);
        return *this;
    }

    void Swap(Deque& rhs) {
        std::swap(data_, rhs.data_);
        std::swap(pfirstbl_, rhs.pfirstbl_);
        std::swap(capacity_, rhs.capacity_);
        std::swap(plastind_, rhs.plastind_);
        std::swap(pfirstind_, rhs.pfirstind_);
        std::swap(sizeb_, rhs.sizeb_);
    }

    void PushBack(int value) {
        //  std::cout<<"Push Back"<<std::endl;
        if (plastind_ == kBlockSize - 1 || sizeb_ == 0) {
            if (sizeb_ == capacity_) {
                Reallocate(capacity_ * 2);
            }
            ++sizeb_;
            data_[Plastbl()] = new int[kBlockSize];
        }
        (++plastind_) %= kBlockSize;
        data_[Plastbl()][plastind_] = value;
    }

    void PopBack() {

        if (plastind_ == 0) {
            delete[] data_[Plastbl()];
            --sizeb_;
            plastind_ = kBlockSize - 1;
        } else {
            plastind_--;
        }
    }

    void PushFront(int value) {
        if (pfirstind_ == 0 || sizeb_ == 0) {
            if (sizeb_ == capacity_) {
                Reallocate(capacity_ * 2);
            }
            ++sizeb_;
            pfirstbl_ = (pfirstbl_ == 0) ? (capacity_ - 1) : (pfirstbl_ - 1);
            data_[pfirstbl_] = new int[kBlockSize];
        }
        pfirstind_ = (pfirstind_ == 0) ? (kBlockSize - 1) : (pfirstind_ - 1);
        data_[pfirstbl_][pfirstind_] = value;
    }

    void PopFront() {
        if (pfirstind_ == kBlockSize - 1) {
            delete[] data_[pfirstbl_];
            --sizeb_;
            pfirstind_ = 0;
            (++pfirstbl_) %= capacity_;
        } else {
            pfirstind_++;
        }
    }

    int& operator[](size_t ind);
    int operator[](size_t ind) const;

    size_t Size() const;

    void Clear();

private:
    void Reallocate(const size_t new_capacity);
    inline size_t Plastbl() {
        return (sizeb_ == 0) ? pfirstbl_ : (pfirstbl_ + sizeb_ - 1) % capacity_;
    };

    static constexpr int kBlockSize = 4;

    int** data_;
    size_t pfirstbl_;
    size_t pfirstind_;
    size_t plastind_;
    size_t capacity_;
    size_t sizeb_;
};

Deque::Deque()
    : data_(nullptr), pfirstbl_(0), pfirstind_(1), plastind_(0), capacity_(0), sizeb_(0) {
}

Deque::Deque(const size_t size)
    : data_(nullptr), pfirstbl_(0), pfirstind_(1), plastind_(0), capacity_(0), sizeb_(0) {
    Reallocate((size + kBlockSize - 1) / kBlockSize);
    for (size_t i = 0; i < size; i++) {
        PushBack(0);
    }
}
Deque::Deque(const Deque& rhs)
    : data_(nullptr),
      pfirstbl_(0),
      pfirstind_(rhs.pfirstind_),
      plastind_(rhs.plastind_),
      capacity_(0),
      sizeb_(rhs.sizeb_) {
    Reallocate(rhs.sizeb_);
    size_t cur_block = rhs.pfirstbl_;
    for (size_t i = 0; i < rhs.sizeb_; i++) {
        data_[i] = new int[kBlockSize];
        memcpy(data_[i], rhs.data_[cur_block], sizeof(int) * kBlockSize);
        (++cur_block) %= rhs.capacity_;
    }
}
Deque::Deque(Deque&& rhs)
    : data_(rhs.data_),
      pfirstbl_(rhs.pfirstbl_),
      pfirstind_(rhs.pfirstind_),
      plastind_(rhs.plastind_),
      capacity_(rhs.capacity_),
      sizeb_(rhs.sizeb_) {
    rhs.data_ = nullptr;
    rhs.pfirstind_ = 1;
    rhs.plastind_ = 0;
    rhs.capacity_ = 0;
    rhs.sizeb_ = 0;
    rhs.pfirstbl_ = 0;
}
Deque::Deque(std::initializer_list<int> list)
    : data_(nullptr), pfirstbl_(0), pfirstind_(1), plastind_(0), capacity_(0), sizeb_(0) {
    auto n = list.size();
    Reallocate((n + kBlockSize - 1) / kBlockSize);
    for (auto p : list) {
        PushBack(p);
    }
}

void Deque::Reallocate(size_t new_capacity) {
    // std::cout<<"Reallocate"<<std::endl;
    if (new_capacity == 0 && capacity_ == 0) {
        new_capacity = 2;
    }
    if (new_capacity <= capacity_) {
        return;
    }

    int** buff = new int*[new_capacity]();
    for (size_t i = 0; i < sizeb_; i++) {
        buff[i] = data_[pfirstbl_];
        (++pfirstbl_) %= capacity_;
    }
    capacity_ = new_capacity;
    if (data_ != nullptr) {
        delete[] data_;
    }
    data_ = buff;
    pfirstbl_ = 0;
}

size_t Deque::Size() const {
    // std::cout<<"Size"<<std::endl;
    if (sizeb_ == 0 || (sizeb_ == 1 && pfirstind_ > plastind_)) {
        return 0;
    }
    return sizeb_ * kBlockSize - (pfirstind_ + kBlockSize - plastind_ - 1);
}

int& Deque::operator[](size_t ind) {
    size_t lft = ind % kBlockSize;
    size_t bl = (lft + pfirstind_ >= kBlockSize) ? ind / kBlockSize + 1 : ind / kBlockSize;
    (bl += pfirstbl_) %= capacity_;
    size_t in = (pfirstind_ + lft) % kBlockSize;
    return data_[bl][in];
}
int Deque::operator[](size_t ind) const {
    size_t lft = ind % kBlockSize;
    size_t bl = (lft + pfirstind_ >= kBlockSize) ? ind / kBlockSize + 1 : ind / kBlockSize;
    (bl += pfirstbl_) %= capacity_;
    size_t in = (pfirstind_ + lft) % kBlockSize;
    return data_[bl][in];
}
void Deque::Clear() {
    size_t i = 0;
    while (i < sizeb_) {
        delete[] data_[pfirstbl_];
        i++;
        (++pfirstbl_) %= capacity_;
    }
    if (data_) {
        delete[] data_;
        data_ = nullptr;
    }
    sizeb_ = 0;
    capacity_ = 0;
    pfirstbl_ = 0;
    pfirstind_ = 1;
    plastind_ = 0;
}