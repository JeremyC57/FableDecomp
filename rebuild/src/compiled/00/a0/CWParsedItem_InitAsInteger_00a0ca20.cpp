// 00a0ca20 CWParsedItem::InitAsInteger
// Leaf: ?InitAsInteger@CWParsedItem@@QAEXPB_WK@Z
// Flags: /Zc:wchar_t
#include <stddef.h>
#include "fable_string.h"
enum EParsedItemType{};
// Layout: the PDB leaves the numeric union at+4 unnamed. Use an unnamed storage word and a typed local projection of that original union.
class CWParsedItem{public:EParsedItemType Type;unsigned int:32;CWideString String;
 void InitAsInteger(const wchar_t*,unsigned long);void InitAsFloat(const wchar_t*,unsigned long);
};
void CWParsedItem::InitAsInteger(const wchar_t*text,unsigned long length){
 unsigned long i=0;long&value=*reinterpret_cast<long*>(reinterpret_cast<unsigned char*>(this)+4);value=0;bool negative=false;wchar_t c=text[0];
 if(i<length){do{if(c==L'-')negative=true;else value=value*10+c-L'0';c=text[++i];}while(i<length);
  if(negative==true){Type=(EParsedItemType)3;value=-value;}else Type=(EParsedItemType)3;
 }else Type=(EParsedItemType)3;
}