/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   // TODO: decide the initial value for _isSorted
   DList() {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () { _node = _node->_next; return *(this); }
      iterator operator ++ (int) { iterator temp = *this; _node = _node->_next; return temp; }
      iterator& operator -- () { _node = _node->_prev; return *(this); }
      iterator operator -- (int) { iterator temp = *this; _node = _node->_prev; return temp; }

      iterator& operator = (const iterator& i) { _node = i._node; return *(this); }

      bool operator != (const iterator& i) const { return _node != i._node; }
      bool operator == (const iterator& i) const { return _node == i._node; }

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const { return empty() ? iterator(_head) : iterator(_head->_next); }
   iterator end() const { return iterator(_head); }
   bool empty() const { return (_head->_prev == _head && _head->_next == _head) ? true : false; }
   size_t size() const { 
      size_t num = 0;
      for (iterator i = begin(); i != end(); ++i) {
         ++num;
      }
      return num;
   }

   void push_back(const T& x) {
      DListNode<T>* newObj = new DListNode<T>(x, _head->_prev, _head);
      _head->_prev->_next = newObj;
      _head->_prev = newObj;  
    }
   void pop_front() { erase(begin()); }
   void pop_back() { erase(--end()); }

   // return false if nothing to erase
   bool erase(iterator pos) { 
      if (empty()) {
         return false;
      } else {
         pos._node->_prev->_next = pos._node->_next;
         pos._node->_next->_prev = pos._node->_prev;
         delete pos._node;
         return true;
      } 
   }

   bool erase(const T& x) {
      if (empty()) {
         return false;
      } else {
         for (iterator i = begin(); i != end(); ++i) {
            if (*i == x) {
               erase(i);
               return true;
            }
         }
      }
   }

   void clear() {
      DListNode<T>* pos(_head->_next);
      while (pos != _head) {
         pos = pos->_next;
         delete pos->_prev;  
      }
      _head->_next = _head->_prev = _head;
   }  // delete all nodes except for the dummy node

   void sort() const {
      iterator pos = begin();
      for (size_t i = 0; i < size() - 1; ++i) {
         for (size_t j = 0; j < size() - 1 - i; ++j) {
            if (pos._node->_data > pos._node->_next->_data) {
               T temp = pos._node->_data;
               pos._node->_data = pos._node->_next->_data;
               pos._node->_next->_data = temp;  
            }
            ++pos;
         }
         pos = begin();  
      }
      _isSorted = true;
   }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] helper functions; called by public member functions
};

#endif // DLIST_H
