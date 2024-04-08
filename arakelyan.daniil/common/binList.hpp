#ifndef BINLIST_HPP
#define BINLIST_HPP

#include <iostream>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include "iterator.hpp"
#include "constIterator.hpp"
#include "node.hpp"

namespace arakelyan
{
  template < class T >
  struct BinList
  {
    using iterator = Iterator< T >;
    using const_iterator =  ConstIterator< T >;

    BinList();//fine
    BinList(const T &val, size_t size);//fine
    BinList(const BinList< T > &otherLs);//fine
    BinList(std::initializer_list< T > otherLs);//fine
    BinList(iterator it_start, iterator it_end);//fine
    ~BinList();

    BinList< T > &operator=(const BinList< T > &otherLs);//fine
    BinList< T > &operator=(BinList< T > &&otherLs);//fine
    BinList< T > &operator=(std::initializer_list< T > otherLs);//fine

    size_t get_size() const noexcept;//fine
    bool empty() const noexcept;//fine

    iterator begin() noexcept;//fine
    const_iterator cbegin() const noexcept;//fine
    iterator end() noexcept;//fine
    const_iterator cend() const noexcept;//fine

    T &front();//fine
    T &back();//fine
    const T &front() const;//fine
    const T &back() const;//fine

    void push_back(const T &el);//fine
    void push_front(const T &el);//fine

    void pop_front();//fine
    void pop_back();//fine

    void assign(const T &val, size_t size);//fine
    void assign(iterator it_start, iterator it_end);//fine
    void assign(std::initializer_list< T > otherLs);//fine

    void clear() noexcept;//fine

    void swap(BinList &ls) noexcept;//fine

    void remove(const T &val);//fine
    template < class Unarypredicate >
    void remove_if(Unarypredicate p);

    iterator insert(const_iterator it_pos, const T &val);//fine

    iterator erase(iterator it_pos);//fine
    iterator erase(iterator it_start, iterator it_end);//fine

    void splice(iterator it_this, BinList< T > &otherLs);//fine
    void splice(const_iterator it_this, std::initializer_list< T > otherLs);//fine
    void splice(const_iterator it_this, BinList< T > &otherLs, const_iterator it_other);//fine

    void reverse() noexcept;//fine

    bool operator==(const BinList< T > &otherLs) const;
    bool operator!=(const BinList< T > &otherLs) const;
    bool operator<(const BinList< T > &otherLs) const;
    bool operator>(const BinList< T > &otherLs) const;
    bool operator<=(const BinList< T > &otherLs) const;
    bool operator>=(const BinList< T > &otherLs) const;

  private:
    using Node = details::Node< T >;
    Node *head_;
    Node *tail_;
    size_t size_;
  };
}

template < class T >
arakelyan::BinList< T >::BinList():
  head_(nullptr),
  tail_(nullptr),
  size_(0)
{}

template < class T >
arakelyan::BinList< T >::BinList(const T &val, size_t size):
  BinList()
{
  for (size_t i = 0; i < size; ++i)
  {
    push_back(val);
  }
}

template < class T >
arakelyan::BinList< T >::BinList(const BinList<T> &otherLs):
  BinList()
{
  auto it = otherLs.cbegin();
  while (size_ != otherLs.size_)
  {
    push_back(*it);
    ++it;
  }
}

template < class T >
arakelyan::BinList< T >::BinList(std::initializer_list< T > otherLs):
  BinList()
{
  for (auto it = otherLs.begin(); it != otherLs.end(); ++it)
  {
    push_back(*it);
  }
}

template < class T >
arakelyan::BinList< T >::BinList(iterator it_start, iterator it_end):
  BinList()
{
  for (; it_start != it_end; ++it_start)
  {
    push_back(*it_start);
  }
}

template < class T >
size_t arakelyan::BinList< T >::get_size() const noexcept
{
  return size_;
}

template < class T >
bool arakelyan::BinList< T >::empty() const noexcept
{
  return head_ == nullptr;
}

template < class T >
arakelyan::Iterator< T > arakelyan::BinList< T >::begin() noexcept
{
  return Iterator< T >(head_);
}

template < class T >
arakelyan::ConstIterator< T > arakelyan::BinList< T >::cbegin() const noexcept
{
  return ConstIterator< T >(head_);
}

template < class T >
arakelyan::Iterator< T > arakelyan::BinList< T >::end() noexcept
{
  return Iterator< T >(tail_->nextNode);
}

template < class T >
arakelyan::ConstIterator< T > arakelyan::BinList< T >::cend() const noexcept
{
  return ConstIterator< T >(tail_->nextNode);
}

template < class T >
void arakelyan::BinList< T >::splice(iterator it_this, BinList<T> &otherLs)
{
  assert(!otherLs.empty());
  if (it_this == end())
  {
    tail_->nextNode = otherLs.head_;
    otherLs.head_->prevNode = tail_;
    tail_ = otherLs.tail_;
  }
  else if (it_this == begin())
  {
    auto it = begin();
    ++it;
    head_->nextNode = otherLs.head_;
    otherLs.head_->prevNode = head_;
    otherLs.tail_->nextNode = it.node;
    it.node->prevNode = otherLs.tail_;
  }
  else
  {
    auto it = it_this;
    auto it_next = ++it;
    otherLs.head_->prevNode = it_this.node;
    it_this.node->nextNode = otherLs.head_;
    otherLs.tail_->nextNode = it_next.node;
    if (it_next != end())
    {
      it_next.node->prevNode = otherLs.tail_;
    }
    else
    {
      tail_ = otherLs.tail_;
    }
    size_ += otherLs.size_;
  }
  otherLs.head_ = nullptr;
  otherLs.tail_ = nullptr;
}

template < class T >
void arakelyan::BinList< T >::splice(const_iterator it_this, std::initializer_list< T > otherLs)
{
  if (it_this == cend())
  {
    auto it = otherLs.begin();
    while (it != otherLs.end())
    {
      push_back(*it);
      ++it;
    }
  }
  else if (it_this == cbegin())
  {
    auto it = otherLs.end();
    --it;
    while (it != otherLs.begin())
    {
      push_front(*it);
      --it;
    }
    push_front(*it);
  }
  else
  {
    auto it = it_this;
    auto it_other = otherLs.begin();
    while (it_other != otherLs.end())
    {
      insert(it, *it_other);
      ++it;
      ++it_other;
    }
  }
}

template < class T >
void arakelyan::BinList< T >::splice(const_iterator it_this, BinList< T > &otherLs, const_iterator it_other) //ne to
{
  details::Node< T > *nodeOtherLs = it_other.node;
  nodeOtherLs->nextNode->prevNode = nodeOtherLs->prevNode;
  nodeOtherLs->prevNode->nextNode = nodeOtherLs->nextNode;
  --otherLs.size_;
  if (it_this == cend())
  {
    push_back(*it_other);
  }
  else
  {
    insert(it_this, *it_other);
  }
}

template < class T >
void arakelyan::BinList< T >::reverse() noexcept
{
  assert(!empty());
  details::Node< T > *node = head_;
  node->prevNode = tail_->nextNode;
  node->nextNode->prevNode = node;
  tail_->nextNode = nullptr;
  while (node != nullptr)
  {
    details::Node< T > *tempNode = node->nextNode;
    std::swap(node->prevNode, node->nextNode);
    node = tempNode;
  }
  std::swap(head_, tail_);
}

template < class T >
T &arakelyan::BinList< T >::front()
{
  assert(!empty());
  return head_->value;
}

template < class T >
T &arakelyan::BinList< T >::back()
{
  assert(!empty());
  return tail_->value;
}

template < class T >
const T &arakelyan::BinList< T >::front() const
{
  assert(!empty());
  return head_->value;
}

template < class T >
const T &arakelyan::BinList< T >::back() const
{
  assert(!empty());
  return tail_->value;
}

template < class T >
void arakelyan::BinList< T >::push_back(const T &el)
{
  details::Node< T > *node = new details::Node< T >(el);
  if ((head_ == nullptr) && (tail_ == nullptr))
  {
    head_ = node;
    tail_ = node;
  }
  else
  {
    tail_->nextNode = node;
    node->prevNode = tail_;
    tail_ = node;
  }
  ++size_;
}

template < class T >
void arakelyan::BinList< T >::push_front(const T &el)
{
  details::Node< T > *node = new details::Node< T >(el);
  node->nextNode = head_;
  if (head_ == nullptr && tail_ == nullptr)
  {
    head_ = node;
    tail_ = node;
  }
  head_ = node;
  ++size_;
}

template < class T >
arakelyan::Iterator< T > arakelyan::BinList< T >::insert(const_iterator it_pos, const T &val)
{
  if (it_pos == cbegin())
  {
    push_front(val);
    return begin();
  }
  else if (it_pos == cend())
  {
    push_back(val);
    return begin();
  }
  else
  {
    details::Node< T > *newNode = new details::Node< T >(val);
    details::Node< T > *nextNode = it_pos.node->nextNode;
    it_pos.node->nextNode = newNode;
    newNode->prevNode = it_pos.node;
    newNode->nextNode = nextNode;
    nextNode->prevNode = newNode;
    ++size_;
    return Iterator< T >(newNode);
  }
}

template < class T >
void arakelyan::BinList< T >::clear() noexcept
{
  while (head_)
  {
    pop_front();
  }
}

template < class T >
void arakelyan::BinList< T >::pop_front()
{
  assert(!empty());
  details::Node< T > *node = head_->nextNode;
  if (node != nullptr)
  {
    node->prevNode = nullptr;
  }
  else
  {
    tail_ = nullptr;
  }
  delete head_;
  head_ = node;
  --size_;
}

template < class T >
void arakelyan::BinList< T >::pop_back()
{
  assert(!empty());
  details::Node< T > *node = tail_->prevNode;
  if (node != nullptr)
  {
    node->nextNode = nullptr;
  }
  else
  {
    head_ = nullptr;
  }
  delete tail_;
  tail_ = node;
  --size_;
}

template < class T >
void arakelyan::BinList< T >::remove(const T &val)
{
  assert(!empty());
  details::Node< T > *node = head_;
  while (node)
  {
    if (node->value == val)
    {
      if (node == head_)
      {
        pop_front();
        node = head_->nextNode;
      }
      else if (node == tail_)
      {
        pop_back();
        return;
      }
      else
      {
        details::Node< T > *prevNode = node->prevNode;
        details::Node< T > *nextNode = node->nextNode;
        details::Node< T > *nodeToDel = node;
        delete nodeToDel;
        node = nextNode;
        nextNode->prevNode = prevNode;
        prevNode->nextNode = nextNode;
      }
    }
    else
    {
      node = node->nextNode;
    }
  }
}

template < class T >
template < class UnaryPredicate >
void arakelyan::BinList< T >::remove_if(UnaryPredicate p)
{
  details::Node< T > *node = head_;
  while (node)
  {
    if (p(node->value))
    {
      if (node == head_)
      {
        pop_front();
      }
      else if (node == tail_)
      {
        pop_back();
      }
      else
      {
        details::Node< T > *prevNode = node->prevNode;
        details::Node< T > *nextNode = node->nextNode;
        details::Node< T > *nodeToDel = node;
        delete nodeToDel;
        node = nextNode;
        nextNode->prevNode = prevNode;
        prevNode->nextNode = nextNode;
      }
    }
    else
    {
      node = node->nextNode;
    }
  }
}

template < class T >
void arakelyan::BinList< T >::swap(BinList< T > &otherLs) noexcept
{
  std::swap(head_, otherLs.head_);
  std::swap(tail_, otherLs.tail_);
  std::swap(size_, otherLs.size_);
}

template < class T >
arakelyan::Iterator< T > arakelyan::BinList< T >::erase(iterator it_pos)
{
  if (it_pos == begin())
  {
    pop_front();
    return begin();
  }
  else if (it_pos == end())
  {
    pop_back();
    return end();
  }
  else
  {
    details::Node< T > *prevNode = it_pos.node->prevNode;
    details::Node< T > *nextNode = it_pos.node->nextNode;
    prevNode->nextNode = nextNode;
    nextNode->prevNode = prevNode;
    delete it_pos.node;
    return Iterator< T >(nextNode);
  }
}

template < class T >
arakelyan::Iterator< T > arakelyan::BinList< T >::erase(iterator it_start, iterator it_end)
{
  if (it_start == begin() && it_end == end())
  {
    clear();
    return end();
  }
  else if (it_start == begin() && it_end != end())
  {
    while (it_start != it_end && it_start != end())
    {
      ++it_start;
      pop_front();
    }
    return it_start;
  }
  else if (it_start != begin() && it_end == end())
  {
    while (it_start != it_end)
    {
      erase(it_start);
    }
    return it_start;
  }
  while (it_start != it_end && it_start != end())
  {
    erase(it_start);
  }
  return it_end;
}

template < class T >
void arakelyan::BinList< T >::assign(const T &val, const size_t size)
{
  clear();
  for (size_t i = 0; i < size; ++i)
  {
    push_back(val);
  }
}

template < class T >
void arakelyan::BinList< T >::assign(std::initializer_list< T > otherLs)
{
  clear();
  for (auto it = otherLs.begin(); it != otherLs.end(); ++it)
  {
    push_back(*it);
  }
}

template < class T >
void arakelyan::BinList< T >::assign(iterator it_start, iterator it_end)
{
  clear();
  for (auto it = it_start; it != it_end; ++it)
  {
    push_back(*it);
  }
}

template < class T >
arakelyan::BinList< T > &arakelyan::BinList< T >::operator=(const arakelyan::BinList< T > &otherLs)
{
  BinList< T > tempList(otherLs);
  if (this != std::addressof(tempList))
  {
    swap(tempList);
  }
  return *this;
}

template < class T >
arakelyan::BinList< T > &arakelyan::BinList< T >::operator=(BinList< T > &&otherLs)
{
  if (this != &otherLs)
  {
    clear();
    swap(std::move(otherLs));
  }
  return *this;
}

template < class T >
arakelyan::BinList< T > &arakelyan::BinList< T >::operator=(std::initializer_list< T > otherLs)
{
  clear();
  auto otherIt = otherLs.begin();
  while (otherIt != otherLs.end())
  {
    push_back(*otherIt++);
  }
  return *this;
}

template < class T >
bool arakelyan::BinList< T >::operator==(const BinList< T > &otherLs) const
{
  if (size_ != otherLs.size_)
  {
    return false;
  }
  auto it = cbegin();
  auto otherIt = otherLs.cbegin();
  while (it != cend() && otherIt != otherLs.cend())
  {
    if (*it != *otherIt)
    {
      return false;
    }
    ++it;
    ++otherIt;
  }
  return true;
}

template < class T >
bool arakelyan::BinList< T >::operator!=(const BinList< T > &otherLs) const
{
  return !(*this == otherLs);
}

template < class T >
bool arakelyan::BinList< T >::operator<(const BinList< T > &otherLs) const
{
  auto it = cbegin();
  auto otherIt = otherLs.cbegin();
  while (it != cend() && otherIt != otherLs.cend())
  {
    if (*it < *otherIt)
    {
      return true;
    }
    else if (*it > *otherIt)
    {
      return false;
    }
    ++it;
    ++otherIt;
  }
  return (it == cend() && otherIt != otherLs.cend());
}

template < class T >
bool arakelyan::BinList< T >::operator>(const BinList< T > &otherLs) const
{
  return !(*this < otherLs);
}

template < class T >
bool arakelyan::BinList< T >::operator<=(const BinList< T > &otherLs) const
{
  return !(*this > otherLs);
}

template < class T >
bool arakelyan::BinList< T >::operator>=(const BinList< T > &otherLs) const
{
  return !(*this < otherLs);
}

template < class T >
arakelyan::BinList< T >::~BinList()
{
  clear();
}

#endif