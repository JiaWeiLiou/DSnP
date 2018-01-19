/****************************************************************************
  FileName     [ array.h ]
  PackageName  [ util ]
  Synopsis     [ Define dynamic array package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef ARRAY_H
#define ARRAY_H

#include <cassert>
#include <algorithm>

using namespace std;

// NO need to implement class ArrayNode
//
template <class T>
class Array
{
public:
   // TODO: decide the initial value for _isSorted
   Array() : _data(0), _size(0), _capacity(0) { _isSorted = false; }
   ~Array() { delete [] _data; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class Array;

   public:
      iterator(T* n= 0): _node(n) {}
      iterator(const iterator& i): _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return *_node; }
      T& operator * () { return (*_node); }
      iterator& operator ++ () { ++_node; return (*this); }
      iterator operator ++ (int) { iterator temp = *this; ++_node; return (*this); }
      iterator& operator -- () { --_node; return (*this); }
      iterator operator -- (int) { iterator temp = *this; --_node; return (*this); }

      iterator operator + (int i) const { return iterator(_node + i); }
      iterator& operator += (int i) { _node += i; return (*this); }

      iterator& operator = (const iterator& i) { _node = i._node; return (*this); }

      bool operator != (const iterator& i) const { return (_node != i._node); }
      bool operator == (const iterator& i) const { return (_node == i._node); }

   private:
      T*    _node;
   };

   // TODO: implement these functions
   iterator begin() const { return (_capacity == 0) ? 0 : iterator(_data); }
   iterator end() const { return (_capacity == 0) ? 0 : iterator(_data + _size); }
   bool empty() const { return !_size; }
   size_t size() const { return _size; }

   T& operator [] (size_t i) { return _data[i]; }
   const T& operator [] (size_t i) const { return _data[i]; }

   void push_back(const T& x) {
      if (_capacity == 0) {
         reserve(1);  
      } else if (_capacity == _size) {
         reserve(_capacity * 2);  
      }
      *(_data + _size) = x;
      ++_size;
   }
   
   void pop_front() { erase(begin()); }
   void pop_back() { erase(--end()); }

   bool erase(iterator pos) {
      if (empty()) {
         return false;  
      } else {
         *pos = *(--end());
         --_size;
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
         return false;  
      }
   }

   void clear() { _size = 0; }

   // [Optional TODO] Feel free to change, but DO NOT change ::sort()
   void sort() const { if (!empty()) ::sort(_data, _data + _size); }

   // Nice to have, but not required in this homework...
   void reserve(size_t n) {
      if (n <= _capacity) {
      } else {
         T* reallocData = new T[n];
         _capacity = n;
         for (size_t i = 0; i < _size; ++i) {
            *reallocData = *(_data + i);  
         }
         delete [] _data;
         _data = reallocData;  
      }
   }
   void resize(size_t n) {
      if (n <= _size) {
         for (size_t i = n; i < _size; ++i) {
            (_data + i) = T();  
         } 
      } else {
         reserve(n);
         for (size_t i = _size; i < n; ++i) {
            _data[i] = T();
         }  
      }
      _size = n;  
   }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   T*            _data;
   size_t        _size;       // number of valid elements
   size_t        _capacity;   // max number of elements
   mutable bool  _isSorted;   // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] Helper functions; called by public member functions
};

#endif // ARRAY_H
