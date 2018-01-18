/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>

using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode
{
   // TODO: design your own class!!
   friend class BSTree<T>;
   friend class BSTree<T>::iterator;

   BSTreeNode(const T& d, BSTreeNode<T>* l = 0, BSTreeNode<T>* r = 0:
      _data(d), _left(l), _right(r) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T               _data;
   BSTreeNode<T>*  _left;
   BSTreeNode<T>*  _right;
};


template <class T>
class BSTree
{
   // TODO: design your own class!!
public:
   BSTree() {
      _head = new BSTreeNode<T>(T());
      _head->_left = _head->_right = _head; // _head is a dummy node
   }
   ~BSTree() { clear(); delete _head; }

   class iterator
   {
      friend class BSTree;

   public:
      iterator(BSTreeNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {}

      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () { _node = _node->_left; return *(this); } // to _left
      iterator operator ++ (int) { _node = _node->_right; return *(this); } // to _right
      iterator& operator -- () { _node = _node->_prev; return *(this); }  //
      iterator operator -- (int) { iterator temp = *this; _node = _node->_prev; return temp; }  //

      iterator& operator = (const iterator& i) { _node = i._node; return *(this); }

      bool operator != (const iterator& i) const { return _node != i._node; }
      bool operator == (const iterator& i) const { return _node == i._node; }

   private:
      BSTreeNode<T>* _node;
   };

   iterator begin() const { return empty() ? iterator(_head) : iterator(_head->_left); }
   iterator end() const { return iterator(_head); }
   bool empty() const { return (_head->_left == _head) ? true : false; }
   size_t size() const {
      size_t num = 0;
      for (iterator i = begin(); i != end(); ++i) {
         ++num;
      }
      return num;
   }

   void insert(const T& x) { //
      DListNode<T>* newObj = new DListNode<T>(x, _head->_prev, _head);
      _head->_prev->_next = newObj;
      _head->_prev = newObj;  
    }
   void pop_front() { erase(begin()); } //
   void pop_back() { erase(--end()); }  //

   bool erase(iterator pos) { // 
      if (empty()) {
         return false;
      } else {
         pos._node->_prev->_next = pos._node->_next;
         pos._node->_next->_prev = pos._node->_prev;
         delete pos._node;
         return true;
      } 
   }

   bool erase(const T& x) { //
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

   void clear() { //
      DListNode<T>* pos(_head->next);
      while (pos != _head) {
         pos = pos->_next;
         delete pos->_prev;  
      }
      _head->_next = _head->_prev = _head;
   }  // delete all nodes except for the dummy node

   void sort() const {  //
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
   BSTreeNode<T>*  _head;     // = dummy node if list is empty

};

#endif // BST_H
