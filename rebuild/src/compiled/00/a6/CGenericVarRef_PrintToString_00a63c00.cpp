// CGenericVarRef::PrintToString -- format a referenced integer, float, bool, or string
// Leaf: PrintToString
// Retail00A63C00,180bytes including embedded fiveentry jump table.
// Actual string header lacks the native public copy ctor; a zero-state derived API façade
// exposes that ctor without replicating string layout. Its implicit destructor calls the actual base string destructor.
#include "fable_string.h"
struct NativeStringCopy:CCharString{NativeStringCopy(const CCharString&);}; //0099EC30 actual CCharString copy ctor
void __cdecl Format(CCharString*,const char*,...); //0099F1F0 CCharString::Format (varargs cdecl this on stack)
struct CGenericVarRef{long Type;void*PData;void PrintToString(CCharString&)const;};
void CGenericVarRef::PrintToString(CCharString&out)const{
 switch(Type){
 case 3:Format(&out,"%f",*static_cast<const float*>(PData));break;
 case 5:{NativeStringCopy copy(*static_cast<const CCharString*>(PData));out=static_cast<const CCharString&>(copy);break;}
 case 4:if(*static_cast<const bool*>(PData))out="TRUE";else out="FALSE";break;
 case 1:Format(&out,"%d",*static_cast<const long*>(PData));break;
 }
}