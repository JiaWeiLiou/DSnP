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

#define REPLACE(A,B) if (B) B->_parent = A->_parent; \
  							if (A->_parent) \
  							(A->_parent->_right == A) ? (A->_parent->_right = B) : (A->_parent->_left = B); \
  							else _root = B

#define TOMAX(A) while (A->_right) A = A->_right
#define TOMIN(A) while (A->_left) A = A->_left

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

   BSTreeNode(const T& d, BSTreeNode<T>* p = 0, BSTreeNode<T>* r = 0, BSTreeNode<T>* l = 0):
      _data(d), _parent(p), _right(r), _left(l) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T               _data;
   BSTreeNode<T>*  _parent;
   BSTreeNode<T>*  _right;
   BSTreeNode<T>*  _left;
};


template <class T>
class BSTree
{
   // TODO: design your own class!!
public:
   BSTree() : _size(0) {
	    _dummy = new BSTreeNode<T>(T());
		  _root = _dummy;
   }
   ~BSTree() { clear(); delete _dummy; }

   class iterator
   {
      friend class BSTree;

   public:
      iterator(BSTreeNode<T>* n = 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {}

      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () { //
         if(_node->_right) {
            TOMIN(_node->_right); // find leftmost
         } else {
            // check for leftmost
            while(_node->_parent || _node != _node->_parent->_left) {
               _node = _node->_parent;
            }
            _node = _node->_parent;
         }
      	return *(this);
      }
      iterator operator ++ (int) {
         iterator temp = *(this);
      	 ++(*this);
      	 return temp;
      }
      iterator& operator -- () { //
         if(_node->_left) {
            TOMAX(_node->_left);
         } else {
            while(_node->_parent || _node != _node->_parent->_right) {
               _node = _node->_parent;
            }
            _node = _node->_parent;
         }
      	return *(this);
      }
      iterator operator -- (int) {
         iterator temp = *(this);
      	 --(*this);
      	 return temp;
      }

      iterator& operator = (const iterator& i) { _node = i._node; return *(this); }

      bool operator != (const iterator& i) const { return _node != i._node; }
      bool operator == (const iterator& i) const { return _node == i._node; }

   private:
      BSTreeNode<T>* _node;
   };

   iterator begin() const {
      BSTreeNode<T>* temp = _root;
      TOMIN(temp);
   	  return iterator(temp);
   }
   iterator end() const { return iterator(_dummy); }
   bool empty() const { return !_size; }
   size_t size() const { return _size; }

   void insert(const T& x) {
      BSTreeNode<T>* temp = _root;
   	  while (1) {
   		   if (temp == _dummy) {
   			    BSTreeNode<T>* newObj = new BSTreeNode<T>(x, _dummy->_parent, _dummy);
   			    if (temp == _root) {
               _root = newObj;
            } else {
               _dummy->_parent->_right = newObj;
            }
   			    _dummy->_parent = newObj;
   			    break;
   		   } else if (x < temp->_data) {
   			    if (temp->_left) {
               temp = temp->_left;
            } else {
   				     BSTreeNode<T>* newObj = new BSTreeNode<T>(x, temp);
   				     temp->_left = newObj;
   				     break;
   			    }
   		   } else {
   	        if (temp->_right) {
               temp = temp->_right;
   			    } else {
   				     BSTreeNode<T>* newObj = new BSTreeNode<T>(x, temp);
   				     temp->_right = newObj;
   				     break;
   			    }
   		   }
   	  }
   	  ++_size; 
   }
   void pop_front() { erase(begin()); }
   void pop_back() { erase(--end()); }

   bool erase(iterator pos) {
      if (pos == end()) return false;
   	  BSTreeNode<T>* node = pos._node;
   	  if (node->_right && node->_right != _dummy) {
   		   if (node->_left) {
   			    T s = (++pos)._node->_data;
   			    erase(pos);
   			    node->_data = s;
   			    ++_size;
   		   } else {
   			    REPLACE(node, node->_right);
   			    delete node;
   		   }
   	  } else if (node->_left) {
   		   REPLACE(node, node->_left);
   		   if (node->_right == _dummy) {
            BSTreeNode<T>* n = node->_left;
            TOMAX(n);
            n->_right = _dummy;
            _dummy->_parent = n;
   		   }
   		   delete node;
   	  } else {
         if(node->_right == _dummy) { 
            REPLACE(node, _dummy);
         } else { 
            REPLACE(node, node->_left);
         }
   		   delete node;
   	  }
   	  --_size;
   	  return true; 
   }

   bool erase(const T& x) {
      for (iterator h = begin(); h != end(); ++h) {
         if (h._node->_data == x) {
            erase(h);
            return true;
         }
      }
      return false;
   }

   void clear() {
      for (iterator i = begin(); i != end(); ++i) {
         erase(i);
      }
   }  // delete all nodes except for the dummy node

   void sort() const {}
   void print() const {}

private:
   BSTreeNode<T>*  _root;
   BSTreeNode<T>*  _dummy;  //point to the end of node
   size_t _size;

};

#endif // BST_H
