#ifndef TREE_HPP
#define TREE_HPP

#include <functional>
#include "tree_node.hpp"
#include "tree_iterator.hpp"
#include "const_tree_iterator.hpp"

namespace namestnikov
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class Tree
  {
  public:
    using pair_key_t = std::pair< const Key, Value >;
    using node_t = detail::TreeNode< Key, Value >;
    using iterator = IteratorTree< Key, Value, Compare >;
    using const_iterator = ConstIteratorTree< Key, Value, Compare >;
    Tree():
      root_(nullptr),
      size_(0),
      compare_(Compare())
    {}
    Tree(const Tree & other):
      root_(other.root_),
      size_(other.size_),
      compare_(other.compare_)
    {
      try
      {
        for (auto it = other.begin(); it != other.end(); ++it)
        {
          insert(it->first, it->second);
        }
      }
      catch(...)
      {
        clear();
        throw;
      } 
    }
    Tree(Tree && other):
      root_(other.root_),
      size_(other.size_),
      compare_(other.compare_)
    {
      other.root_ = nullptr;
      other.size_ = 0;
    }
    Tree & operator=(const Tree & other)
    {
      if (this != std::addressof(other))
      {
        Tree< Key, Value, Compare > temp(other);
        swap(temp);
      }
      return *this;
    }
    Tree & operator=(Tree && other)
    {
      if (this != std::addressof(other))
      {
        swap(other);
      }
      return *this;
    }
    const Value & at(const Key & key)
    {
      node_t * result = search(root_, key);
      if (result)
      {
        return result->data.second;
      }
      else
      {
        throw std::out_of_range("Can not give you access to this element");
      }
    }
    Value & operator[](const Key & key)
    {
      node_t * result = search(root_, key);
      if (result)
      {
        return result->data.second;
      }
      else
      {
        insert(key, Value());
        return search(root_, key)->data.second;
      }
    }
    void rotate_left(node_t * node)
    {
      node_t * newRoot = node->right;
      node->right = newRoot->left;
      if (newRoot->left)
      {
        newRoot->left->parent = node;
      }
      newRoot->parent = node->parent;
      if (node->isRoot())
      {
        root_ = newRoot;
      }
      else
      {
        if (node->isLeftChild())
        {
          node->parent->left = newRoot;
        }
        else
        {
          node->parent->right = newRoot;
        }
      }
      newRoot->left = node;
      node->parent = newRoot;
      node->height = node->height + 1 - std::min(newRoot->height, 0);
      newRoot->height = newRoot->height + 1 + std::max(node->height, 0);
    }
    void rotate_right(node_t * node)
    {
      node_t * newRoot = node->left;
      node->left = newRoot->right;
      if (newRoot->right)
      {
        newRoot->right->parent = node;
      }
      newRoot->parent = node->parent;
      if (node->isRoot())
      {
        root_ = newRoot;
      }
      else
      {
        if (node->isLeftChild())
        {
          node->parent->left = newRoot;
        }
        else
        {
          node->parent->right = newRoot;
        }
      }
      newRoot->right = node;
      node->parent = newRoot;
      node->height = node->height - 1 - std::max(newRoot->height, 0);
      newRoot->height = newRoot->height - 1 + std::min(node->height, 0);
    }
    void print_impl(node_t * root)
    {
      if (root->left != nullptr)
      {
        print_impl(root->left);
      }
      std::cout << " " << root->data.second << " ";
      if (root->right != nullptr)
      {
        print_impl(root->right);
      }
    }
    void print()
    {
      print_impl(root_);
    }
    void balance(node_t * node)
    {
      if (node->height < 0)
      {
        if (node->right->height > 0)
        {
          rotate_right(node->right);
        }
        rotate_left(node);
      }
      else
      {
        if (node->left->height < 0)
        {
          rotate_left(node->left);
        }
        rotate_right(node);
      }
    }
    void getNewBalance(node_t * node)
    {
      if ((node->height > 1) || (node->height < -1))
      {
        balance(node);
        return;
      }
      if (node->parent != nullptr)
      {
        if (node->isLeftChild())
        {
          node->parent->height += 1;
        }
        else if (node->isRightChild())
        {
          node->parent->height -= 1;
        }
        if (node->parent->height != 0)
        {
          getNewBalance(node->parent);
        }
      }
    }
    node_t * find_min(node_t * node) const
    {
      node_t * temp = node;
      while (temp->left)
      {
        temp = temp->left;
      }
      return temp;
    }
    iterator begin() const
    {
      node_t * result = find_min(root_);
      return iterator(result);
    }
    iterator end() const
    {
      return iterator();
    }
    const_iterator cbegin() const noexcept
    {
      node_t * result = find_min(root_);
      return const_iterator(result);
    }
    const_iterator cend() const noexcept
    {
      return const_iterator();
    }
    const_iterator find(const Key & key) const
    {
      node_t * temp = root_;
      while ((temp) && (temp->data.first != key))
      {
        if (compare_(temp->data.first, key))
        {
          temp = temp->right;
        }
        else
        {
          temp = temp->left;
        }
      }
      return const_iterator(temp);
    }
    void insert(const Key & key, const Value & val, node_t * root)
    {
      if (compare_(key, root->data.first))
      {
        if (root->left)
        {
          insert(key, val, root->left);
        }
        else
        {
          root->left = new node_t(key, val, root);
          getNewBalance(root->left);

        }
      }
      else
      {
        if (root->right)
        {
          insert(key, val, root->right);
        }
        else
        {
          root->right = new node_t(key, val, root);
          getNewBalance(root->right);
        }
      }
    }
    void insert(const Key & key, const Value & val)
    {
      if (root_)
      {
        insert(key, val, root_);
        ++size_;
      }
      else
      {
        root_ = new node_t(key, val);
        ++size_;
      }
    }
    node_t * search(const Key & key) const
    {
      return search(root_, key);
    }
    node_t * search(node_t * node, const Key & key) const
    {
      if ((node == nullptr) || (node->data.first == key))
      {
        return node;
      }
      if (compare_(key, node->data.first))
      {
        return search(node->left, key);
      }
      else
      {
        return search(node->right, key);
      }
    }
    bool contains(const Key & key) const
    {
      return search(root_, key) != nullptr;
    }
    void fixHeight(node_t * node)
    {
      int rightHeight = getHeight(node->right);
      int leftHeight = getHeight(node->left);
      node->height = std::max(rightHeight, leftHeight) + 1;
    }
    int getHeight(node_t * root)
    {
      return root ? (root->height) : 0;
    }
    void updateHeight(node_t * node)
    {
      node->height = 1 + std::max(node->getHeight(node->left), node->getHeight(node->right));
    }
    int getBalance(node_t * node)
    {
      return getHeight(node->right) - getHeight(node->left);
    }
    bool empty() const noexcept
    {
      return (size_ == 0);
    }
    size_t size() const noexcept
    {
      return size_;
    }
    void swap(Tree & other) noexcept
    {
      std::swap(root_, other.root_);
      std::swap(size_, other.size_);
      std::swap(compare_, other.compare_);
    }
    ~Tree()
    {
      clear();
    }
  private:
    node_t * root_;
    size_t size_;
    Compare compare_;
    void clear()
    {
      clear(root_);
      root_ = nullptr;
      size_ = 0;
    }
    void clear(node_t * node)
    {
      if (node)
      {
        clear(node->left);
        clear(node->right);
        delete node;
      }
    }
  };
}

#endif