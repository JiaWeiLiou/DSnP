/****************************************************************************
  FileName     [ cmdReader.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Define command line reader member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cassert>
#include <cstring>
#include "cmdParser.h"

using namespace std;

//----------------------------------------------------------------------
//    Extrenal funcitons
//----------------------------------------------------------------------
void mybeep();
char mygetc(istream&);
ParseChar getChar(istream&);


//----------------------------------------------------------------------
//    Member Function for class Parser
//----------------------------------------------------------------------
void CmdParser::readCmd()
{
   if (_dofile.is_open()) {
      readCmdInt(_dofile);
      _dofile.close();
   }
   else
      readCmdInt(cin);
}

void CmdParser::readCmdInt(istream& istr)
{
   resetBufAndPrintPrompt();

   while (1) {
      ParseChar pch = getChar(istr);
      if (pch == INPUT_END_KEY) break;
      switch (pch) {
         case LINE_BEGIN_KEY :
         case HOME_KEY       : moveBufPtr(_readBuf); break;
         case LINE_END_KEY   :
         case END_KEY        : moveBufPtr(_readBufEnd); break;
         case BACK_SPACE_KEY : /* TODO */ if (moveBufPtr(_readBufPtr - 1)) { deleteChar(); } break;
         case DELETE_KEY     : deleteChar(); break;
         case NEWLINE_KEY    : addHistory();
                               cout << char(NEWLINE_KEY);
                               resetBufAndPrintPrompt(); break;
         case ARROW_UP_KEY   : moveToHistory(_historyIdx - 1); break;
         case ARROW_DOWN_KEY : moveToHistory(_historyIdx + 1); break;
         case ARROW_RIGHT_KEY: /* TODO */ moveBufPtr(_readBufPtr + 1); break;
         case ARROW_LEFT_KEY : /* TODO */ moveBufPtr(_readBufPtr - 1); break;
         case PG_UP_KEY      : moveToHistory(_historyIdx - PG_OFFSET); break;
         case PG_DOWN_KEY    : moveToHistory(_historyIdx + PG_OFFSET); break;
         case TAB_KEY        : /* TODO */ insertChar(' ', char(8) - (_readBufPtr - _readBuf) % char(8)); break;
         case INSERT_KEY     : // not yet supported; fall through to UNDEFINE
         case UNDEFINED_KEY:   mybeep(); break;
         default:  // printable character
            insertChar(char(pch)); break;
      }
      #ifdef TA_KB_SETTING
      taTestOnly();
      #endif
   }
}


// This function moves _readBufPtr to the "ptr" pointer
// It is used by left/right arrowkeys, home/end, etc.
//
// Suggested steps:
// 1. Make sure ptr is within [_readBuf, _readBufEnd].
//    If not, make a beep sound and return false. (DON'T MOVE)
// 2. Move the cursor to the left or right, depending on ptr
// 3. Update _readBufPtr accordingly. The content of the _readBuf[] will
//    not be changed
//
// [Note] This function can also be called by other member functions below
//        to move the _readBufPtr to proper position.
bool CmdParser::moveBufPtr(char* const ptr)
{
   // TODO...
   if (ptr < _readBuf || ptr > _readBufEnd) {
      mybeep();
      return false;
   } else if (ptr < _readBufPtr) {
      for (char i = 0; i < _readBufPtr - ptr; ++i) {
         cout << char(8);
      }
   } else {
      for (char i = 0; i < ptr - _readBufPtr; ++i) {
         cout << *(_readBufPtr + i);
      }
   }
   _readBufPtr = ptr;
   return true;
}


// [Notes]
// 1. Delete the char at _readBufPtr
// 2. mybeep() and return false if at _readBufEnd
// 3. Move the remaining string left for one character
// 4. The cursor should stay at the same position
// 5. Remember to update _readBufEnd accordingly.
// 6. Don't leave the tailing character.
// 7. Call "moveBufPtr(...)" if needed.
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteChar()---
//
// cmd> This is he command
//              ^
//
bool CmdParser::deleteChar()
{
   // TODO...
   if (_readBufPtr == _readBufEnd) {
      mybeep();
      return false;
   }
   for (char i = 0; i <= _readBufEnd - _readBufPtr; ++i) {
      *(_readBufPtr + i) = *(_readBufPtr + i + 1);
   }
   --_readBufEnd;
   for (char i = 0; i < _readBufEnd - _readBufPtr; ++i) {
      cout << *(_readBufPtr + i);
   }
   cout << " "; //tailing character
   for (char i = 0; i <= _readBufEnd - _readBufPtr; ++i) { 
      cout << char(8); 
   }
   return true;
}

// 1. Insert character 'ch' for "repeat" times at _readBufPtr
// 2. Move the remaining string right for "repeat" characters
// 3. The cursor should move right for "repeats" positions afterwards
// 4. Default value for "repeat" is 1. You should assert that (repeat >= 1).
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling insertChar('k', 3) ---
//
// cmd> This is kkkthe command
//                 ^
//
void CmdParser::insertChar(char ch, int repeat)
{
   // TODO...
   for (char i = (_readBufEnd - _readBufPtr); i >= 0; --i) {
      *(_readBufPtr + i + repeat) = *(_readBufPtr + i);
   }
	 for (char i = 0; i < repeat; ++i) {
      *(_readBufPtr + i) = ch;
   }
	 _readBufPtr += repeat;
	 _readBufEnd += repeat;
	 for (char i = -repeat; i < _readBufEnd - _readBufPtr; ++i) {
      cout << *(_readBufPtr + i);
   }
	 for (char i = 0; i < _readBufEnd - _readBufPtr; ++i) {
      cout << char(8);
   }
   assert(repeat >= 1);
}

// 1. Delete the line that is currently shown on the screen
// 2. Reset _readBufPtr and _readBufEnd to _readBuf
// 3. Make sure *_readBufEnd = 0
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteLine() ---
//
// cmd>
//      ^
//
void CmdParser::deleteLine()
{
   // TODO...
   moveBufPtr(_readBuf);
	 char length = _readBufEnd - _readBuf;
	 for (char i = 0; i < length; ++i) {
      deleteChar();
   }
}


// This functions moves _historyIdx to index and display _history[index]
// on the screen.
//
// Need to consider:
// If moving up... (i.e. index < _historyIdx)
// 1. If already at top (i.e. _historyIdx == 0), beep and do nothing.
// 2. If at bottom, temporarily record _readBuf to history.
//    (Do not remove spaces, and set _tempCmdStored to "true")
// 3. If index < 0, let index = 0.
//
// If moving down... (i.e. index > _historyIdx)
// 1. If already at bottom, beep and do nothing
// 2. If index >= _history.size(), let index = _history.size() - 1.
//
// Assign _historyIdx to index at the end.
//
// [Note] index should not = _historyIdx
//
void CmdParser::moveToHistory(int index)
{
   // TODO...
   size_t historySize = _history.size();
	 
   // modify to real index
   if (_tempCmdStored) {
      --historySize;
   }

   // moving up
	 if (index < _historyIdx) {
      // at top
	    if (_historyIdx == 0) {
			   mybeep();
			   return;
      // at bottom
	    } else if ((size_t)_historyIdx == historySize) {
	       if (!_tempCmdStored) {
				    _history.push_back(_readBuf);
				    _tempCmdStored = true;
			   } else {
            _history[_history.size() - 1].assign(_readBuf); // replacing current string
         }
      // index < 0
		  } else if (index < 0) {
         index = 0;
      }
   // moving down
	 } else if (index > _historyIdx) {
      // at bottom
	    if ((size_t)_historyIdx == historySize) {
			   mybeep();
			   return;
      // index >= _history.size()
		  } else if ((size_t)index > historySize) {
         index = historySize;
      }
	 }

   //assign _historyIdx to index
	 _historyIdx = index;
   retrieveHistory();
	 if ((size_t)_historyIdx == historySize && _tempCmdStored) {
	    _history.pop_back();
			_tempCmdStored = false;
	 }
}


// This function adds the string in _readBuf to the _history.
// The size of _history may or may not change. Depending on whether 
// there is a temp history string.
//
// 1. Remove ' ' at the beginning and end of _readBuf
// 2. If not a null string, add string to _history.
//    Be sure you are adding to the right entry of _history.
// 3. If it is a null string, don't add anything to _history.
// 4. Make sure to clean up "temp recorded string" (added earlier by up/pgUp,
//    and reset _tempCmdStored to false
// 5. Reset _historyIdx to _history.size() // for future insertion
//
void CmdParser::addHistory()
{
   // TODO...
	 string readBufString(_readBuf, _readBufEnd - _readBuf);
   // remove ' ' at the beginning and end of _readBuf
	 size_t s = 0;
   size_t e = readBufString.size() - 1;
	 while (1) {
      if (readBufString[s] == ' ') ++s;
		  else break;
	 }
	 while (1) {
	    if (readBufString[e] == ' ') --e;
		  else break;
	 }

   // clean up temp recorded string and reset _tempCmdStored to false
   if (_tempCmdStored) {
      _history.pop_back();
      _tempCmdStored = false;
   }
   
   string addString;
   if (s <= readBufString.size() - 1 && e >= 0) {
      addString = readBufString.substr(s, e - s + 1);
   }

   // not a null string
   if (addString.size() != 0) {
	    _history.push_back(addString);
      // reset _historyIdx to _history.size()
	    _historyIdx = _history.size();
   }
}


// 1. Replace current line with _history[_historyIdx] on the screen
// 2. Set _readBufPtr and _readBufEnd to end of line
//
// [Note] Do not change _history.size().
//
void CmdParser::retrieveHistory()
{
   deleteLine();
   strcpy(_readBuf, _history[_historyIdx].c_str());
   cout << _readBuf;
   _readBufPtr = _readBufEnd = _readBuf + _history[_historyIdx].size();
}
