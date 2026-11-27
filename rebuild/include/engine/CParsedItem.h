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
#ifdef FABLE_RETAIL_SYMBOLMAP_ENUM_PASS29
public:
    EParsedItemType GetType() const { return Type; }
#endif
#ifdef FABLE_RETAIL_PROCESS_COMMAND_LINE_PASS29
public: CCharString ConvertToString() const;
#endif
public:const CCharString& GetString() const;
// chunk 751 (row 0x009BA650 CStringParser::ReadAsFloat): the Ego_r.pdb accessors ?IsFloat@CParsedItem@@QBE_NXZ,
// ?IsInteger@CParsedItem@@QBE_NXZ, ?GetFloat@CParsedItem@@QBEMXZ, ?GetInteger@CParsedItem@@QBEJXZ (inline in retail: the
// integer arm keeps the long returned by GetInteger in a frame word across the item's destructor and `fild`s it after).
bool IsFloat() const { return Type==PARSED_ITEM_FLOAT; }
bool IsInteger() const { return Type==PARSED_ITEM_INTEGER; }
#ifdef FABLE_RETAIL_CPARSEDITEM_GETVALUE_OUTOFLINE
// NF129 (q4657, row 0x00A641A0 CGenericVar::ReadFromParserAsFloat): the same Ego_r.pdb accessors called out of line, as that TU does
// (?GetInteger@CParsedItem@@QBEJXZ 0x009B9890 `mov eax,[ecx+4]`, ?GetFloat@CParsedItem@@QBEMXZ 0x009B98A0 `fld [ecx+4]`). Declarations only.
float GetFloat() const;                                                  // 0x009B98A0
long GetInteger() const;                                                 // 0x009B9890
#else
float GetFloat() const { return Float; }
long GetInteger() const { return Integer; }
#endif
// Chunk 756 (row 0x009B9700): Ego_r.pdb ?InitAsInteger@CParsedItem@@QAEXPBDK@Z (out of line, 0x009B9700).
void InitAsInteger(const char* text,unsigned long length);
// Chunk 756 (row 0x009B9770): Ego_r.pdb ?InitAsFloat@CParsedItem@@QAEXPBDK@Z (out of line, 0x009B9770).
void InitAsFloat(const char* text,unsigned long length);
#ifdef FABLE_RETAIL_CPARSEDITEM_SYMBOL
// Chunk 778 (row 0x00615E70): Ego_r.pdb ?IsSymbol@CParsedItem@@QBE_NXZ (inline in retail: `cmp [item],1`) and
// ?GetSymbol@CParsedItem@@QBEABVCCharString@@XZ (out of line, 0x009B9870 `lea eax,[ecx+8]`).
bool IsSymbol() const { return Type==PARSED_ITEM_SYMBOL; }
const CCharString& GetSymbol() const;
#endif
#ifdef FABLE_RETAIL_CPARSEDITEM_ISSTRING
// hand3c3 opt-in (row 0x009ACEE0 CDefinitionManager::ParseEnumForSymbols): Ego_d.pdb / FableWin.pdb ?IsString@CParsedItem@@QBE_NXZ
// (FableWin 0x028B9200), inline in retail (0x009ACFD1 `cmp [item],2`), the twin of IsSymbol. No layout change.
bool IsString() const { return Type==PARSED_ITEM_STRING; }
#endif
#else
public:CParsedItem(){} const CCharString& GetString() const;
#endif
};
FABLE_STATIC_ASSERT_AT(33, sizeof(CParsedItem)==12);
#endif
