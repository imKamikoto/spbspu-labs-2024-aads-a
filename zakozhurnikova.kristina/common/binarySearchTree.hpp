#ifndef BINARYSEARCHTREE_HPP
#define BINARYSEARCHTREE_HPP
#include <algorithm>
#include <constIteratorTree.hpp>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <treeNode.hpp>
#include <queue.hpp>

namespace zakozhurnikova
{
  template < typename Key, typename Value, typename Compare = std::less< Key > >
  struct BinarySearchTree
  {
    using Node = detail::TreeNode< Key, Value >;
    BinarySearchTree():
      root_(nullptr),
      size_(0),
      compare_(Compare())
    {}

    BinarySearchTree(const BinarySearchTree& rhs):
      root_(nullptr),
      size_(0),
      compare_(rhs.compare_)
    {
      try
      {
        for (auto it = rhs.cbegin(); it != rhs.cend(); ++it)
        {
          push(it->first, it->second);
        }
      }
      catch (...)
      {
        clear();
        throw;
      }
    }

    BinarySearchTree(BinarySearchTree&& rhs):
      root_(rhs.root_),
      size_(rhs.size_),
      compare_(rhs.compare_)
    {
      rhs.root_ = nullptr;
      rhs.size_ = 0;
    }

    BinarySearchTree& operator=(const BinarySearchTree& rhs)
    {
      if (this != std::addressof(rhs))
      {
        BinarySearchTree< Key, Value > temp(rhs);
        swap(temp);
      }
      return *this;
    }

    BinarySearchTree& operator=(BinarySearchTree&& rhs)
    {
      if (this != std::addressof(rhs))
      {
        clear();
        swap(rhs);
      }
      return *this;
    }

    ~BinarySearchTree()
    {
      clear();
    }

    int size() const noexcept
    {
      return this->size_;
    }

    void swap(BinarySearchTree< Key, Value, Compare >& other) noexcept
    {
      std::swap(root_, other.root_);
      std::swap(size_, other.size_);
      std::swap(compare_, other.compare_);
    }

    bool empty() const noexcept
    {
      return size_ == 0;
    }

    void push(const Key& key, const Value& val)
    {
      if (root_)
      {
        put(key, val, root_);
      }
      else
      {
        root_ = new Node(key, val);
      }
      size_ = size_ + 1;
    }

    void put(const Key& key, const Value& val, Node* currentNode)
    {
      if (compare_(key, currentNode->data.first))
      {
        if (currentNode->hasLeftChild())
        {
          put(key, val, currentNode->leftChild);
        }
        else
        {
          currentNode->leftChild = new Node(key, val, currentNode);
          updateBalance(currentNode->leftChild);
        }
      }
      else
      {
        if (currentNode->hasRightChild())
        {
          put(key, val, currentNode->rightChild);
        }
        else
        {
          currentNode->rightChild = new Node(key, val, currentNode);
          updateBalance(currentNode->rightChild);
        }
      }
    }

    void del(const Key& key)
    {
      if (size_ > 1)
      {
        Node* nodeToRemove = get(key, root_);
        if (nodeToRemove)
        {
          remove(nodeToRemove);
          size_ = size_ - 1;
        }
        else
        {
          std::cerr << "Error, key not in tree" << '\n';
        }
      }
      else if (size_ == 1 && root_->data.first == key)
      {
        root_ = nullptr;
        size_ = size_ - 1;
      }
      else
      {
        std::cerr << "Error, key not in tree" << '\n';
      }
    }
    Node* getLowestRight(Node* prev) const
    {
      Node* lowest = prev;
      if (!lowest)
      {
        return nullptr;
      }
      while (lowest->rightChild != nullptr)
      {
        lowest = lowest->rightChild;
      }
      return lowest;
    }

    ConstIteratorTree< Key, Value, Compare > cbeginR() const noexcept
    {
      return ConstIteratorTree< Key, Value, Compare >(getLowestRight(root_));
    }

    ConstIteratorTree< Key, Value, Compare > cbegin() const noexcept
    {
      return ConstIteratorTree< Key, Value, Compare >(getLowest(root_));
    }
    ConstIteratorTree< Key, Value, Compare > cend() const noexcept
    {
      return ConstIteratorTree< Key, Value, Compare >();
    }

    Value& operator[](const Key& key)
    {
      Node* traverser = get(key, root_);
      if (traverser)
      {
        return traverser->data.second;
      }
      else
      {
        push(key, Value());
        return get(key, root_)->data.second;
      }
    }

    Value& at(const Key& key)
    {
      Node* traverser = get(key, root_);
      if (traverser)
      {
        return traverser->data.second;
      }
      throw std::out_of_range("No such element");
    }

    template < typename F >
    F traverse_lnr(F f, std::string& result) const
    {
      for (auto it = cbegin(); it != cend(); ++it)
      {
        f(it->first);
        result += it->second + ' ';
      }
      if (!empty())
      {
        result.pop_back();
      }
      else
      {
        result = "<EMPTY>";
      }
      return f;
    }

    template < typename F >
    F traverse_rnl(F f, std::string& result) const
    {
      for (auto it = cbeginR(); it != cend(); --it)
      {
        f(it->first);
        result += it->second + ' ';
      }
      if (!empty())
      {
        result.pop_back();
      }
      else
      {
        result = "<EMPTY>";
      }
      return f;
    }

    template < typename F >
    F traverse_breadth(F f, std::string& result) const
    {
      if (empty())
      {
        result = "<EMPTY>";
        return f;
      }
      Queue< const Node* > queue;
      queue.push(root_);
      while (!queue.empty())
      {
        const Node* curr = queue.top();
        queue.drop();
        f(curr->data.first);
        result += curr->data.second + ' ';
        if (curr->leftChild)
        {
          queue.push(curr->leftChild);
        }
        if (curr->rightChild)
        {
          queue.push(curr->rightChild);
        }
      }
      if (!empty())
      {
        result.pop_back();
      }
      else
      {
        result = "<EMPTY>";
      }
      return f;
    }

    ConstIteratorTree< Key, Value > find(const Key& key) const
    {
      Node* wanted = root_;
      while (wanted)
      {
        if (wanted->data.first == key)
        {
          break;
        }
        else if (compare_(wanted->data.first, key))
        {
          wanted = wanted->rightChild;
        }
        else
        {
          wanted = wanted->leftChild;
        }
      }
      return ConstIteratorTree< Key, Value >(wanted);
    }

  private:
    Node* root_;
    size_t size_;
    Compare compare_;

    void clear()
    {
      clear(root_);
      root_ = nullptr;
    }

    void clear(Node* node)
    {
      if (node != nullptr)
      {
        clear(node->leftChild);
        clear(node->rightChild);
        delete node;
      }
    }

    Value& get(const Key& key)
    {
      Node* res = get(key, root_);
      if (res)
      {
        return res->data.second;
      }
      else
      {
        return res->data.second;
      }
    }

    Node* get(const Key& key, Node* currentNode)
    {
      if (!currentNode)
      {
        return nullptr;
      }
      else if (currentNode->data.first == key)
      {
        return currentNode;
      }
      else if (compare_(key, currentNode->data.first))
      {
        return get(key, currentNode->leftChild);
      }
      else
      {
        return get(key, currentNode->rightChild);
      }
    }

    void rebalance(Node* node)
    {
      if (node->balanceFactor < 0)
      {
        if (node->rightChild->balanceFactor > 0)
        {
          node->rightChild->rotateRight(&root_);
          node->rotateLeft(&root_);
        }
        else
        {
          node->rotateLeft(&root_);
        }
      }
      else
      {
        if (node->leftChild->balanceFactor < 0)
        {
          node->leftChild->rotateLeft(&root_);
          node->rotateRight(&root_);
        }
        else
        {
          node->rotateRight(&root_);
        }
      }
    }
    void updateBalance(Node* node)
    {
      if (node->balanceFactor > 1 || node->balanceFactor < -1)
      {
        rebalance(node);
        return;
      }
      if (node->parent != nullptr)
      {
        if (node->isLeftChild())
        {
          node->parent->balanceFactor += 1;
        }
        else if (node->isRightChild())
        {
          node->parent->balanceFactor -= 1;
        }
        if (node->parent->balanceFactor != 0)
        {
          updateBalance(node->parent);
        }
      }
    }
    Node* getLowest(Node* prev) const
    {
      Node* lowest = prev;
      if (!lowest)
      {
        return nullptr;
      }
      while (lowest->leftChild != nullptr)
      {
        lowest = lowest->leftChild;
      }
      return lowest;
    }
    void updateRemoveBalance(Node* balanceParent, int oldBal)
    {
      if (balanceParent->balanceFactor == 0)
      {
        if (!balanceParent->isRoot())
        {
          int bal = balanceParent->parent->balanceFactor;
          if (balanceParent->isLeftChild())
          {
            --balanceParent->parent->balanceFactor;
          }
          else
          {
            ++balanceParent->parent->balanceFactor;
          }
          updateRemoveBalance(balanceParent->parent, bal);
        }
      }
      else if (1 < balanceParent->balanceFactor || -1 > balanceParent->balanceFactor)
      {
        if (!balanceParent->isRoot())
        {
          Node* grandParent = balanceParent->parent;
          bool leftChild = balanceParent->isLeftChild();
          rebalance(balanceParent);
          int newBal = 0;
          if (leftChild)
          {
            newBal = grandParent->leftChild->balanceFactor;
          }
          else
          {
            newBal = grandParent->rightChild->balanceFactor;
          }
          newBal = std::abs(newBal) - std::abs(oldBal);
          if (std::abs(newBal) - std::abs(oldBal) != 0)
          {
            int balGrand = grandParent->balanceFactor;
            if (leftChild)
            {
              --grandParent->balanceFactor;
            }
            else
            {
              ++grandParent->balanceFactor;
            }
            updateRemoveBalance(grandParent, balGrand);
          }
        }
        else
        {
          rebalance(balanceParent);
        }
      }
      return;
    }

    void remove(Node* currentNode)
    {
      if (currentNode->isLeaf())
      {
        int oldBal = currentNode->parent->balanceFactor;
        if (currentNode->isLeftChild())
        {
          currentNode->parent->leftChild = nullptr;
          --currentNode->parent->balanceFactor;
        }
        else
        {
          currentNode->parent->rightChild = nullptr;
          ++currentNode->parent->balanceFactor;
        }
        Node* balanceParent = currentNode->parent;
        delete currentNode;
        updateRemoveBalance(balanceParent, oldBal);
      }
      else if (currentNode->hasBothChildren())
      {
        Node* succ = getLowest(currentNode->rightChild);
        currentNode->data.first = succ->data.first;
        currentNode->data.second = succ->data.second;
        remove(succ);
      }
      else
      {
        if (currentNode->hasLeftChild())
        {
          int oldBal = currentNode->parent->balanceFactor;
          if (currentNode->isLeftChild())
          {
            currentNode->leftChild->parent = currentNode->parent;
            currentNode->parent->leftChild = currentNode->leftChild;
            --currentNode->parent->balanceFactor;
          }
          else if (currentNode->isRightChild())
          {
            currentNode->leftChild->parent = currentNode->parent;
            currentNode->parent->rightChild = currentNode->leftChild;
            ++currentNode->parent->balanceFactor;
          }
          updateRemoveBalance(currentNode->parent, oldBal);

          if (currentNode->isRoot())
          {
            root_ = currentNode->leftChild;
            root_->parent = nullptr;
          }
        }
        else
        {
          int oldBal = currentNode->parent->balanceFactor;
          if (currentNode->isLeftChild())
          {
            currentNode->rightChild->parent = currentNode->parent;
            currentNode->parent->leftChild = currentNode->rightChild;
            --currentNode->parent->balanceFactor;
          }
          else if (currentNode->isRightChild())
          {
            currentNode->rightChild->parent = currentNode->parent;
            currentNode->parent->rightChild = currentNode->rightChild;
            ++currentNode->parent->balanceFactor;
          }
          updateRemoveBalance(currentNode->parent, oldBal);

          if (currentNode->isRoot())
          {
            root_ = currentNode->rightChild;
            root_->parent = nullptr;
          }
        }
        delete currentNode;
      }
    }
  };
}

#endif
