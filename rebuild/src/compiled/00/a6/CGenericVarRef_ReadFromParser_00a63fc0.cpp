// CGenericVarRef::ReadFromParser
// Leaf: ReadFromParser
#include "fable_string.h"
#define CStringParser CStringParserDonor
#include "engine/CStringParser.h"
#undef CStringParser
// Native parser is0x28 vs donor0x30 due to the native list. Only the true thiscall member is used.
class CParsedItem;
class CStringParser { public:CStringParser();void*VTable;unsigned char NativeState[0x24];bool ReadNextItem(CParsedItem&);}; // ctor PDB00492230, ReadNextItem009BA080/PDB00B0EB60
class CParsedItem { public:CParsedItem(){} long Type;long Value;CCharString String;const CCharString& GetSymbol()const;}; // real ctor PDB01911740 constructs only String; GetSymbol009B9880/PDB00B0E400
extern char EmptyString[]; //0129AAF4
inline CCharString::operator const char*()const{return storage_?storage_->text:EmptyString;}
extern "C" int __cdecl _stricmp(const char*,const char*); // MSVCR71.dll IAT014400EC via00BFEBA8
inline bool operator==(const CCharString&a,const CCharString&b){return _stricmp(a,b)==0;}
class CGenericVar { public:CGenericVar();long Padding;long Type;CCharString ValString;long ValSLONG;unsigned long ValULONG;bool ValBool;float ValFloat;bool ReadFromParserAsBool(CStringParser&);}; // actual ctor PDB030A8B30; bool+14h
struct CGenericVarRef {CGenericVarRef(bool&);long Type;union{bool*ValBool;void*Value;};bool ReadFromParserAsBool(CStringParser&);bool ReadFromParserAsSLONG(CStringParser&);bool ReadFromParserAsFloat(CStringParser&);bool ReadFromParserAsString(CStringParser&);bool ReadFromParser(CStringParser&);}; // actual bool-ref ctor PDB018A3D80

bool CGenericVarRef::ReadFromParser(CStringParser& p){switch(Type){case 3:return ReadFromParserAsFloat(p);case 5:return ReadFromParserAsString(p);case 4:return ReadFromParserAsBool(p);case 1:return ReadFromParserAsSLONG(p);default:return false;}}