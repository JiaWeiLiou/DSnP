/****************************************************************************
  FileName     [ p2Main.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define main() function ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2016-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cstdlib>
#include "p2Table.h"

using namespace std;

int main()
{
   Table table;

   // Read in the csv file. Do NOT change this part of code.
   string csvFile;
   cout << "Please enter the file name: ";
   cin >> csvFile;
   if (table.read(csvFile))
      cout << "File \"" << csvFile << "\" was read in successfully." << endl;
   else {
      cerr << "Failed to read in file \"" << csvFile << "\"!" << endl;
      exit(-1); // csvFile does not exist.
   }

   // TODO read and execute commands
   string input;
   while (getline(cin, input)) {

      /*find action input*/
      size_t Loc = input.find(' ');
      size_t endLoc = input.size();
      string action = input.substr(0, Loc);

      /*find column input || find number input*/
      int column = 0;
      size_t _nCols = table.nCols();
  
      if (action == "SUM" || action == "AVE" || action == "MAX" || action == "MIN" || action == "DIST") {
         column = stoi(input.substr(Loc + 1, endLoc - Loc - 1));
      }

      Row newRow(_nCols);
      if (action == "ADD") {
         size_t nextLoc = 0;
         for (size_t i = 0; i != _nCols; ++i) {
            nextLoc = input.find(' ', Loc + 1);
            if (input.substr(Loc + 1, nextLoc - Loc - 1) == ".") {
               newRow[i] = INT_MAX;
            } else {
               newRow[i] = stoi(input.substr(Loc + 1, nextLoc - Loc - 1));
            }
            Loc = nextLoc;
         }
      }

      if (action == "PRINT") {
         table.print();
      } else if (action == "ADD") {
         table.add(newRow);
      } else if (action == "SUM") {
         table.sum(column);
      } else if (action == "AVE") {
         table.avg(column);
      } else if (action == "MAX") {
         table.max(column);
      } else if (action == "MIN") {
         table.min(column);
      } else if (action == "DIST") {
         table.dist(column);
      } else if (action == "EXIT") {
         break;
      }
   }
   return 0;
}
