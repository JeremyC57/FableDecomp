// CGenericVar::ReadFromParserAsBool
// Leaf: ReadFromParserAsBool
#include "fable_string.h"
#define CStringParser CStringParserDonor
#include "engine/CStringParser.h"
#undef CStringParser
// Native parser is0x28 vs donor0x30 due to the native list. Only the true thiscall member is used.
struct CParsedItem;
struct CStringParser {CStringParser();void*VTable;unsigned char NativeState[0x24];bool ReadNextItem(CParsedItem&);}; // ctor PDB00492230, ReadNextItem009BA080/PDB00B0EB60
struct CParsedItem {CParsedItem(){} long Type;long Value;CCharString String;const CCharString& GetSymbol()const;}; // real ctor PDB01911740 constructs only String; GetSymbol009B9880/PDB00B0E400
extern char EmptyString[]; //0129AAF4
__forceinline CCharString::operator const char*()const{return !storage_?EmptyString:storage_->text;}
extern "C" int __cdecl _stricmp(const char*,const char*); // MSVCR71.dll IAT014400EC via00BFEBA8
__forceinline bool operator==(const CCharString&a,const CCharString&b){return _stricmp(a,b)==0;}
struct CGenericVar {CGenericVar();long Padding;long Type;CCharString ValString;long ValSLONG;unsigned long ValULONG;bool ValBool;float ValFloat;bool ReadFromParserAsBool(CStringParser&);}; // actual ctor PDB030A8B30; bool+14h
struct CGenericVarRef {CGenericVarRef(bool&);long Type;union{bool*ValBool;void*Value;};bool ReadFromParserAsBool(CStringParser&);bool ReadFromParserAsSLONG(CStringParser&);bool ReadFromParserAsFloat(CStringParser&);bool ReadFromParserAsString(CStringParser&);bool ReadFromParser(CStringParser&);}; // actual bool-ref ctor PDB018A3D80

bool CGenericVar::ReadFromParserAsBool(CStringParser& parser){
 CParsedItem item;
 if(parser.ReadNextItem(item)&&item.Type==2){
 bool same;{CCharString text("TRUE",-1);same=item.GetSymbol()==text;}
 if(same){ValBool=true;return true;}
 {CCharString text("FALSE",-1);same=item.GetSymbol()==text;}
 if(same){ValBool=false;return true;}
 }return false;
}