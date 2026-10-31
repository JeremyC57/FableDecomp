// 0072aaf0 CTCSpecialAbilities::SetupMultiStrike
// Codex parked-50 best attempt: DIFFER(66v66); residue: 66/66 bytes; retail loads AddTC receiver before pushes, compiler after pushes. Named string and input-reading callee context did not change it.
// Leaf: ?SetupMultiStrike@CTCSpecialAbilities@@AAE_NXZ
// Flags:
#define CClass CClassLayout
#include "engine/CClass.h"
#undef CClass
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define CTCBase CTCBaseLayout
#include "engine/CTCBase.h"
#undef CTCBase
#define CCharString CCharStringLayout
#include "fable_string.h"
#undef CCharString
class CCharString{
    unsigned char unobservedString[sizeof(CCharStringLayout)];
public:CCharString(const char*,long=-1);~CCharString();
};
#include "engine/CBaseClassNonCopyable.h"
#pragma pack(push,1)
class CClass:public CBaseObject{public:unsigned char unobservedClass[sizeof(CClassLayout)-sizeof(CBaseObject)];virtual ~CClass();};
#pragma pack(pop)
namespace std{
template<unsigned long N>struct _Base_bitset{unsigned long _M_w[N];};
template<unsigned long N>class bitset:public _Base_bitset<(N+31)/32>{};
}
class CTCBase;class CTCParamsBase;
enum ETCInterfaceType{};
// Layout evidence: native tests UsingComponents+20, word4 bit7. Donor6854-6855
// and RTTI2015 prove CThing's genuine base and prefix; unread remainder omitted.
#pragma pack(push,1)
class CThing:public CClass{
// Layout: unused alignment and UniqueID stay opaque; UsingComponents is at20.
protected:unsigned char unobservedBeforeBits[0x20-sizeof(CClass)];std::bitset<274>UsingComponents;
public:
    bool HasTC(ETCInterfaceType type)const{return(UsingComponents._M_w[(unsigned long)type>>5]&(1UL<<((unsigned long)type&31)))!=0;}
    CTCBase*AddTC(const CCharString&,bool,const CTCParamsBase*);
};
#pragma pack(pop)
class CTCParamsBase:public CBaseClassNonCopyable{};
class CTCBase:public CBaseClassNonCopyable{
protected:CThing&ParentThing;private:bool ToBeRemoved,IsInGlobalUpdateTCs;unsigned char unobservedAlignment[2];
};
// Layout evidence: no normal CTCSpecialAbilities engine header exists; RTTI1940
// proves CTCBase inheritance and native+4 proves ParentThing. No other fields used.
class CTCSpecialAbilities:public CTCBase{private:bool SetupMultiStrike();};
// Fidelity: same-TU analysis stand-in, not a reconstructed AddTC implementation.
// Reads the string input without modifying it; its runtime body is not delivered as a helper row.
#pragma auto_inline(off)
CTCBase* CThing::AddTC(const CCharString& name,bool active,const CTCParamsBase* params){
 if(*(const void* const*)&name==params || !active) return 0;
 return reinterpret_cast<CTCBase*>(this);
}
#pragma auto_inline(on)
bool CTCSpecialAbilities::SetupMultiStrike(){
    if(!ParentThing.HasTC((ETCInterfaceType)135)){
        ParentThing.AddTC("CTCMultiStrike",true,0);
        return true;
    }
    return false;
}
