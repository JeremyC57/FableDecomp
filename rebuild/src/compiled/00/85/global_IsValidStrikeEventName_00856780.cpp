// 00856780 IsValidStrikeEventName
// Flags: /O2 /Oy /W3 
// Leaf: IsValidStrikeEventName
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): the eight string handles are the global const CCharStrings of the combat-action TU
//   (Ego_r.pdb + FableWin.pdb `dynamic initializer for '...'`, initializers 0x0121E7A0..0x0121E8A0 landed by agentFAM09):
//   0x013BAC64 STRIKE_EVENT_WEAPON_START_IDENTIFIER (was ComboStrikes::PrimaryPrefix), 0x013BAC68 STRIKE_EVENT_WEAPON_END_IDENTIFIER
//   (SecondaryPrefix), 0x013BAC6C STRIKE_EVENT_START_IDENTIFIER (PrimaryAlternative), 0x013BAC70 STRIKE_EVENT_END_IDENTIFIER
//   (SecondaryAlternative), 0x013BAC78 STRIKE_EVENT_BEGIN_LEGACY (BothFlags), 0x013BAC7C STRIKE_EVENT_END_LEGACY (SecondFlag),
//   0x013BAC80 SECONDARY_STRIKE_EVENT_BEGIN_LEGACY (FirstFlag), 0x013BAC84 SECONDARY_STRIKE_EVENT_END_LEGACY (NeitherFlag). The row's
//   string handle model moved out of the stand-in namespace so the objects bind ?NAME@@3VCCharString@@B. Still open: the function is
//   the anonymous-namespace ?IsValidStrikeEventName@?A0x...@@YI_NABVCCharString@@AA_N111@Z (Ego_r.pdb), graded here inside the stand-in
//   namespace ComboStrikes, and the Compare / EmptyString stand-ins (0x00411570 / 0x0129AAF4) keep their row-local spellings.
#include "fable_compiler.h"
#include <string.h>
namespace ComboStrikes {
// Layout: CCharString one-pointer handle; CBasicString<char> data+0,length+4. Globals13bac64..84 are string handles.
template<class T>struct CBasicString {T*Data;unsigned Length;static int __fastcall Compare(const T*,const T*);};
extern const char EmptyString[];
}
class CCharString{public:ComboStrikes::CBasicString<char>*Data;
 FABLE_FORCEINLINE const char*c_str()const{return !Data?ComboStrikes::EmptyString:Data->Data;}
 FABLE_FORCEINLINE unsigned size()const{return !Data?0:Data->Length;}
 FABLE_FORCEINLINE bool StartsWith(const CCharString&prefix)const{return strncmp(c_str(),prefix.c_str(),prefix.size())==0;}
 FABLE_FORCEINLINE bool operator==(const CCharString&v)const{if(Data==v.Data)return true;if(!Data||!v.Data)return false;if(Data->Length!=v.Data->Length)return false;return ComboStrikes::CBasicString<char>::Compare(Data->Data,v.Data->Data)==0;}
};
extern const CCharString STRIKE_EVENT_WEAPON_START_IDENTIFIER,STRIKE_EVENT_WEAPON_END_IDENTIFIER,STRIKE_EVENT_START_IDENTIFIER,STRIKE_EVENT_END_IDENTIFIER;   // 0x013BAC64 / 68 / 6C / 70
extern const CCharString STRIKE_EVENT_BEGIN_LEGACY,STRIKE_EVENT_END_LEGACY,SECONDARY_STRIKE_EVENT_BEGIN_LEGACY,SECONDARY_STRIKE_EVENT_END_LEGACY;   // 0x013BAC78 / 7C / 80 / 84
namespace ComboStrikes {
FABLE_FORCEINLINE bool Equal(CBasicString<char>*a,CBasicString<char>*b){if(a==b)return true;if(!a||!b)return false;if(a->Length!=b->Length)return false;return CBasicString<char>::Compare(a->Data,b->Data)==0;}
bool __fastcall IsValidStrikeEventName(const CCharString&name,bool&first,bool&prefix,bool&named,bool&second){
 prefix=false;bool primary=name.StartsWith(STRIKE_EVENT_WEAPON_START_IDENTIFIER);bool secondary=name.StartsWith(STRIKE_EVENT_WEAPON_END_IDENTIFIER);
 if(!primary&&!secondary){primary=name.StartsWith(STRIKE_EVENT_START_IDENTIFIER);secondary=name.StartsWith(STRIKE_EVENT_END_IDENTIFIER);}else prefix=true;
 if(primary||secondary){named=false;first=primary;return true;}
 named=true;CBasicString<char>*cached=name.Data;
 if(Equal(cached,STRIKE_EVENT_BEGIN_LEGACY.Data)){first=true;second=true;return true;}
 if(Equal(cached,SECONDARY_STRIKE_EVENT_BEGIN_LEGACY.Data)){first=true;second=false;return true;}
 if(Equal(cached,STRIKE_EVENT_END_LEGACY.Data)){first=false;second=true;return true;}
 if(Equal(cached,SECONDARY_STRIKE_EVENT_END_LEGACY.Data)){first=false;second=false;return true;}
 return false;
}
}
