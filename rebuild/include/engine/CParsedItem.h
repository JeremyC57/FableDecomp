#ifndef FABLE_ENGINE_CPARSEDITEM_H
#define FABLE_ENGINE_CPARSEDITEM_H
#include "fable_string.h"
enum EParsedItemType {PARSED_ITEM_NULL=0,PARSED_ITEM_SYMBOL=1,PARSED_ITEM_STRING=2,PARSED_ITEM_INTEGER=3,PARSED_ITEM_FLOAT=4};
// Layout: original Type+0, Integer/Float union+4, actual String+8, full size12.
class CParsedItem {
private:EParsedItemType Type;union{long Integer;float Float;};CCharString String;
#ifdef FABLE_RETAIL_CPARSEDITEM
// Retail class branch (Codex pass 13, row 0x009BB110): Ego_r TPI 0xC77F / FableWin 0xC4A2 mark the ctor, copy and dtor
// compiler-generated (attrs 0x103), so the retail class declares none of them.
public:const CCharString& GetString() const;
// chunk 751 (row 0x009BA650 CStringParser::ReadAsFloat): the Ego_r.pdb accessors ?IsFloat@CParsedItem@@QBE_NXZ,
// ?IsInteger@CParsedItem@@QBE_NXZ, ?GetFloat@CParsedItem@@QBEMXZ, ?GetInteger@CParsedItem@@QBEJXZ (inline in retail: the
// integer arm keeps the long returned by GetInteger in a frame word across the item's destructor and `fild`s it after).
bool IsFloat() const { return Type==PARSED_ITEM_FLOAT; }
bool IsInteger() const { return Type==PARSED_ITEM_INTEGER; }
float GetFloat() const { return Float; }
long GetInteger() const { return Integer; }
// Chunk 756 (row 0x009B9700): Ego_r.pdb ?InitAsInteger@CParsedItem@@QAEXPBDK@Z (out of line, 0x009B9700).
void InitAsInteger(const char* text,unsigned long length);
// Chunk 756 (row 0x009B9770): Ego_r.pdb ?InitAsFloat@CParsedItem@@QAEXPBDK@Z (out of line, 0x009B9770).
void InitAsFloat(const char* text,unsigned long length);
#else
public:CParsedItem(){} const CCharString& GetString() const;
#endif
};
FABLE_STATIC_ASSERT(sizeof(CParsedItem)==12);
#endif
