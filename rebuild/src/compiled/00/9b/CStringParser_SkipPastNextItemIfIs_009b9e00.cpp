// CStringParser::SkipPastNextItemIfIs(const CCharString&) -- consume a matching next token, restoring cursor on failure
// Leaf: ?SkipPastNextItemIfIs@CStringParser@@QAE_NABVCCharString@@@Z
// Retail: 0x009B9E00,219 bytes,thiscall
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_TOKEN_MOVE
#define FABLE_RETAIL_CSTRINGPARSER_MOVESTRINGPOS
#define FABLE_RETAIL_CCHARSTRING_GETCHAR
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CStringParser.h"
bool CStringParser::SkipPastNextItemIfIs(const CCharString& str){
 CCursorPos old_pos=CursorPos;
 SkipWhitespace();
 long c1=0;
 while(c1<str.GetLength()&&str.GetChar(c1)==String[GetStringPos()+c1])++c1;
 if(c1==str.GetLength()){MoveStringPos(c1);return true;}
 CursorPos=old_pos;return false;
}