#ifndef FABLE_ENGINE_CWSTRINGPARSER_H
#define FABLE_ENGINE_CWSTRINGPARSER_H
#include "engine/CBaseClass.h"
#include <stddef.h>
class CWParsedItem;
// Original Ego_r4ebab/FableWin CWStringParser,16 bytes. Native00a0cdd0 offsets4/8/c.
class CWStringParser:public CBaseClass {
public:
 virtual ~CWStringParser();
private:
 const wchar_t* String;
 unsigned long StringPos,Line;
 bool ReadNextNumber(CWParsedItem*);
};
FABLE_STATIC_ASSERT(sizeof(CWStringParser)==16);
#endif
