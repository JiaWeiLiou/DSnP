/****************************************************************************
  FileName     [ p2Table.h]
  PackageName  [ p2 ]
  Synopsis     [ Header file for class Row and Table ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2016-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#ifndef P2_TABLE_H
#define P2_TABLE_H

#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <climits>

using namespace std;

class Row
{
public:
   // TODO: define constructor and member functions on your own
   
   // Constructor
   Row(size_t i) {
      _len = i;
      _data = new int [i];
   }
   
   // Copy constructor
   Row(const Row &row) {
      _len = row._len;
      _data = new int [_len];
      for (size_t i = 0; i != _len;++i) {
      _data[i] = row._data[i]; 
      }
   }
   
   // Deconstructor
   ~Row() {
      delete [] _data;
      _data = NULL;
   }
   
   // Basic access functions
   const int operator[] (size_t i) const { return _data[i]; }
   int& operator[] (size_t i) { return _data[i]; }

private:
   int *_data; // DO NOT change this definition. Use it to store data.
   size_t _len; // Row length
};

class Table
{
public:
   // TODO: define constructor and member functions on your own 
   bool read(const string&);

   // Basic access functions
   size_t nCols() const { return _nCols; }
   const Row& operator[] (size_t i) const { return _rows[i]; }
   Row& operator[] (size_t i) { return _rows[i]; }
   
   // Basic operational function
   void print();
   void add(Row newRow);
   void sum(size_t _nCols);
   void avg(size_t _nCols);
   void max(size_t _nCols);
   void min(size_t _nCols);
   void dist(size_t _nCols);

private:
   size_t       _nCols; // You should record the number of columns.
   vector<Row>  _rows;  // DO NOT change this definition. Use it to store rows.
};

#endif // P2_TABLE_H
