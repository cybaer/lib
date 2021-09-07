/*
 *  static_list.h
 *
 *  Created on: 10.06.2021
 *      Author: cybaer
 *  Double linked list for already existing (also static) objects.
 *  There is no 'new' and 'delete'. The linked object have to be derived from 'Node'.
 *  bBased on ideas from Maxim Egorushkin posted on stack overflow
 *  https://stackoverflow.com/questions/55402803/linked-list-iterator-implementation-c
 */

#ifndef STATICLIST_H_
#define STATICLIST_H_

template<class T>
class StaticList;

class Iterator;

class Node 
{
  friend class Iterator;
  template<class T> friend class StaticList;

public:
  Node()
  : next_(this), prev_(this)
  {}

  ~Node() { unlink(); }

protected:
  Node *next_, *prev_;

  void push_back(Node* n) 
  {
    if(n == nullptr) return;
    n->next_ = this;
    n->prev_ = prev_;
    prev_->next_ = n;
    prev_ = n;
  }

  void unlink() 
  {
    Node *next = next_;
    Node *prev = prev_;

    next->prev_ = prev;
    prev->next_ = next;
    next_ = this;
    prev_ = this;
  }
private:
  Node (const Node&);
};

class Iterator 
{
public:
  Iterator& operator++() 
  {
      node_ = node_->next_;
      return *this;
  }

  Iterator& operator--() 
  {
      node_ = node_->prev_;
      return *this;
  }

  bool operator==(Iterator b) const { return node_ == b.node_; }
  bool operator!=(Iterator b) const { return node_ != b.node_; }

protected:
  Node* node_;

  Iterator(Node* node)
  : node_(node)
  {}
};

template<class T>
class StaticList 
{ 
  template<class U>
  class IteratorT : public Iterator 
  {
    friend class StaticList<T>;
    T* node() const { return static_cast<T*>(node_); }
  public:
    U& operator*() const { return *node(); }
    U* operator->() const { return node(); }
    operator IteratorT<U const>() const { return node_; } // iterator to const_iterator conversion
    IteratorT(Node* node) 
    : Iterator(node)
    {}
  };

  Node list_;

public:
  using iterator = IteratorT<T>;
  using const_iterator = IteratorT<T const>;

  bool empty() const { return list_.next_ == &list_; }
  iterator begin() { return list_.next_; }
  iterator end() { return &list_; }
  iterator last() { return list_.prev_; }
  void push_back(T& t) { list_.push_back(&t); }
};

#endif
