/****************************************************************************
  FileName     [ p2Table.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define member functions of class Row and Table ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2016-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include "p2Table.h"
#include <fstream>
#include <sstream>
#include <istream>
#include <iomanip>
#include <set>

using namespace std;

//read /n or /r or /n/r 
istream& safeGetline(istream& is, string& t) {
   t.clear();
   // The characters in the stream are read one-by-one using a streambuf.
   // That is faster than reading them one-by-one using the istream.
   // Code that uses streambuf this way must be guarded by a sentry object.
   // The sentry object performs various tasks,
   // such as thread synchronization and updating the stream state.

   istream::sentry se(is, true);
   streambuf *sb = is.rdbuf();

   while (1) {
      int c = sb->sbumpc();
      switch (c) {
         case '\n':
            return is;
         case '\r':
            if (sb->sgetc() == '\n')
               sb->sbumpc();
            return is;
         case EOF:
         // Also handle the case when the last line has no line ending
            if (t.empty())
               is.setstate(std::ios::eofbit);
            return is;
         default:
            t += (char)c;
      }
   }
}

// Implement member functions of class Row and Table here
bool Table::read(const string& csvFile) {
   fstream file;
   size_t _nRows = 1;
   _nCols = 1;

   // calculate .csv file rows and cols
   file.open(csvFile);
   if (file.is_open() == false) { return false; }
   char ch;
   while (file.get(ch)) {
      if (ch == ',' && _nRows == 1) { ++_nCols; }
      if (ch == '\r' || ch == '\n') { ++_nRows; }
   }
   file.close();

   // read .csv data
   string line;
   file.open(csvFile);

   while(!safeGetline(file, line).eof()) {		
      Row rowData(_nCols);
      size_t preLoc = 0; //start from 0
      string value;
      for (size_t i = 0; i != _nCols - 1; ++i) {
         size_t Loc = line.find(',', preLoc);
         if (preLoc == Loc) { 
            rowData[i] = INT_MAX; 
         } else {
            value = line.substr(preLoc, Loc - preLoc);
            rowData[i] = stoi(value);
         }
         preLoc = Loc + 1;
      }
      if (preLoc == line.size()) {
         rowData[_nCols - 1] = INT_MAX;
      } else {
         value = line.substr(preLoc, line.size() - preLoc + 1);
         rowData[_nCols - 1] = stoi(value);
      }
      _rows.push_back(rowData);
   }
   file.close();
   return true; // TODO
}

void Table::print() {
   for (size_t i = 0; i != _rows.size(); ++i) {
      for (size_t j = 0; j != _nCols; ++j) {
         if (_rows[i][j] == INT_MAX) {
            cout << setw(4) << '.';
         } else {
            cout << setw(4) << _rows[i][j];
         }
      }
         cout << endl;
   }
}

void Table::add(Row newRow) {
   _rows.push_back(newRow);
}

void Table::sum(size_t column) {
   int sumNum = 0;
   size_t emptyNum = 0;
   for (size_t i = 0; i != _rows.size(); ++i) {
      if (_rows[i][column] != INT_MAX) {
         sumNum = sumNum + _rows[i][column];
      } else {
         ++emptyNum;
      }
   }

   if (emptyNum == _rows.size()) {
      cout << "Error: This is a NULL column!!" << endl;
   } else {
      cout << "The summation of data in column #" << column << " is "<< sumNum <<"." << endl;
   }
}

void Table::avg(size_t column) {
   int sumNum = 0;
   size_t emptyNum = 0;
   for (size_t i = 0; i != _rows.size(); ++i) {
      if (_rows[i][column] != INT_MAX) {
         sumNum = sumNum + _rows[i][column];
      } else {
         ++emptyNum;
      }
   }

   double avgNum = (double)sumNum / (double)(_rows.size() - emptyNum);

   if (emptyNum == _rows.size()) {
      cout << "Error: This is a NULL column!!" << endl;
   } else {
      cout << "The average of data in column #" << column << " is " << fixed << setprecision(1) << avgNum << "." << endl;
   }
}

void Table::max(size_t column) {
   int maxNum = INT_MIN;
   size_t emptyNum = 0;
   for (size_t i = 0; i != _rows.size(); ++i) {
      if (_rows[i][column] != INT_MAX) {
         if (_rows[i][column] > maxNum) {
            maxNum = _rows[i][column];
         } else {
            ++emptyNum;
         }
      }
   }

   if (emptyNum == _rows.size()) {
      cout << "Error: This is a NULL column!!" << endl;
   } else {
      cout << "The maximum of data in column #" << column << " is " << maxNum << "." << endl; 
   }
}

void Table::min(size_t column) {
   int minNum = INT_MAX - 1;
   size_t emptyNum = 0;
   for (size_t i = 0; i != _rows.size(); ++i) {
      if (_rows[i][column] != INT_MAX) {
         if (_rows[i][column] < minNum) {
            minNum = _rows[i][column];
         }
      } else {
         ++emptyNum;
      }
   }

   if (emptyNum == _rows.size()) {
      cout << "Error: This is a NULL column!!" << endl;
   } else {
      cout << "The minimum of data in column #" << column << " is " << minNum << "." << endl;
   }
}

void Table::dist(size_t column) {
   set<int> s;
   size_t emptyNum = 0;
   for (size_t i = 0; i != _rows.size(); ++i) {
      if (_rows[i][column] != INT_MAX) {
         s.insert(_rows[i][column]);
      } else {
         ++emptyNum;
      }
   }

   if (emptyNum == _rows.size()) {
      cout << "Error: This is a NULL column!!" << endl;
   } else {
      cout << "The distinct count of data in column #" << column << " is " << s.size() << "." << endl;
   }
}
