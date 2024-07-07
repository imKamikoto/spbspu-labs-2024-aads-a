#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include "node.hpp"

namespace vyzhanov
{
  template< typename T >
  class List;

  template< typename T >
  class BiIterator: public std::iterator< std::bidirectional_iterator_tag, T >
  {
    friend List< T >;
    using iterator = BiIterator< T >;
    BiIterator() = default;
    BiIterator(const iterator&) = default;
    ~BiIterator() = default;

    iterator& operator=(const iterator&) = default;
    bool operator==(const iterator&) const;
    bool operator!=(const iterator&) const;

    iterator& operator++();
    iterator& operator--();
    iterator operator++(int);
    iterator operator--(int);

    T& operator*();
    T* operator->();
    const T& operator*() const;
    const T* operator->() const;
  private:
    Node< T >* node_;
    explicit BiIterator(Node< T >* node);
  };

  template< typename T >
  BiIterator< T >::BiIterator() :
    node_(nullptr)
  {}

  template< typename T >
  BiIterator< T >::BiIterator(Node< T >* node) :
    node_(node)
  {}

  template < typename T >
  bool BiIterator< T >::operator==(const iterator& obj) const
  {
    return node_ == obj.node_;
  }

  template< typename T >
  bool BiIterator< T >::operator!=(const iterator& obj) const
  {
    return !(node_ == obj.node_);
  }

  template< typename T >
  BiIterator< T >& BiIterator< T >::operator++()
  {
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  BiIterator< T >& BiIterator< T >::operator--()
  {
    node_ = node_->prev_;
    return *this;
  }

  template< typename T >
  BiIterator< T > BiIterator< T >::operator++(int)
  {
    BiIterator< T > temp = *this;
    node_ = node_->next_;
    return temp;
  }

  template< typename T >
  BiIterator< T > BiIterator< T >::operator--(int)
  {
    BiIterator< T > temp = *this
    node_ = node_->prev_;
    return *this;
  }

  template< class T >
  T& BiIterator< T >::operator*()
  {
    return node_->data_;
  }

  template< class T >
  T* BiIterator< T >::operator->()
  {
    return std::addressof(node_->data_);
  }

  template< class T >
  const T& BiIterator< T >::operator*() const
  {
    return node_->data_;
  }

  template< class T >
  const T* BiIterator< T >::operator->() const
  {
     return std::addressof(node_->data_);
  }

}

#endif
