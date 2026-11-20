// CGenericVarRef::PrintToString -- format a referenced integer, float, bool, or string
// Leaf: PrintToString
// Retail00A63C00,180bytes including embedded fiveentry jump table.
// The string case copies through the retail CCharString copy ctor 0x0099EC30 (Ego_r.pdb ??0CCharString@@QAE@ABV0@@Z,
//   fable_string.h FABLE_RETAIL_CCHARSTRING; NF129 q4458) and destroys the temporary with ~CCharString 0x0099EAE0.
// Naming (NF129, 2026-11-20, q6512): 0x0099F1F0 is Ego_r.pdb 0x00AFB180 + FableWin.pdb 0x02F61B20 ?Sprintf@CCharString@@QAAXPBDZZ
//   (fable_string.h FABLE_RETAIL_CCHARSTRING_SPRINTF; varargs cdecl, this pushed first).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SPRINTF
#include "fable_string.h"
struct CGenericVarRef{long Type;void*PData;void PrintToString(CCharString&)const;};
void CGenericVarRef::PrintToString(CCharString&out)const{
 switch(Type){
 case 3:out.Sprintf("%f",*static_cast<const float*>(PData));break;
 case 5:{CCharString copy(*static_cast<const CCharString*>(PData));out=copy;break;}
 case 4:if(*static_cast<const bool*>(PData))out="TRUE";else out="FALSE";break;
 case 1:out.Sprintf("%d",*static_cast<const long*>(PData));break;
 }
}