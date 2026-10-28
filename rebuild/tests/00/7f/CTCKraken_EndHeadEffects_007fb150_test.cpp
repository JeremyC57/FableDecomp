#include <stdio.h>
#include <string.h>
// CTCKraken::EndHeadEffects() -- Kill the live effect and clear its intelligent pointer.
// Leaf: ?EndHeadEffects@CTCKraken@@QAEXXZ
// Identity: Own CreateHeadEffect007fbb00 and InitTentaclesAndHead007fc710; PDB EndHeadEffects and PHeadEffect+40.
#ifndef _MSC_VER
#define __forceinline inline
#endif
// Layout: CThing active header is absent (donor-only header quarantined). Retail Dead is bit0 at+0x91; no other fields accessed.
class CBaseObject;
class CThing {public:unsigned char Prefix[0x91];unsigned char Dead:1;unsigned char OtherFlags:7;void Kill(bool);};
class CBaseObjectPointer;
// Layout: genuine polymorphic intelligent pointer has PData+4, total8bytes on x86; no active shared wrapper header.
class CBaseIntelligentPointer {public:CBaseIntelligentPointer();virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;CBaseObject* GetPItem()const;void SetPItem(CBaseObject*);};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {public:
 CIntelligentPointer(T* p=0){SetPItem((CBaseObject*)p);}
 T* GetPItem()const{return (T*)CBaseIntelligentPointer::GetPItem();}
 CIntelligentPointer& operator=(const CIntelligentPointer& x){SetPItem((CBaseObject*)x.GetPItem());return *this;}
};
// Layout: no active owner header; PHeadEffect is an8-byte intelligent pointer at 40, proved by retail address and PDB member. Packed layout preserves the original unaligned member where applicable.
#pragma pack(push,1)
class CTCKraken {public:unsigned char Prefix[40];CIntelligentPointer<CThing> PHeadEffect;
public:void EndHeadEffects();friend struct TestAccess;
};
#pragma pack(pop)
void CTCKraken::EndHeadEffects(){if(PHeadEffect.GetPItem() && !PHeadEffect.GetPItem()->Dead){PHeadEffect.GetPItem()->Kill(true);PHeadEffect=CIntelligentPointer<CThing>(0);}}

class CBaseObjectPointer {public:void* Item;};
static CBaseObjectPointer pool[16];static int created,getsCount,setsCount,destructs,kills,bad;static CThing* expected;
CBaseIntelligentPointer::CBaseIntelligentPointer(){PData=&pool[created++];PData->Item=0;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){++destructs;}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{++getsCount;return (CBaseObject*)PData->Item;}
void CBaseIntelligentPointer::SetPItem(CBaseObject* p){++setsCount;PData->Item=p;}
void CThing::Kill(bool force){++kills;if(this!=expected || !force)bad=1;Dead=1;}
struct TestAccess {static void run(CTCKraken& x){x.EndHeadEffects();}};
int main(){CTCKraken owner;CThing thing;memset(&thing,0,sizeof(thing));memset(owner.Prefix,0x5a,sizeof(owner.Prefix));expected=&thing;
 int start=created;getsCount=setsCount=destructs=0;TestAccess::run(owner);
 int ok=getsCount==1 && !setsCount && !destructs && !kills && created==start;
 owner.PHeadEffect.PData->Item=&thing;thing.Dead=1;getsCount=0;TestAccess::run(owner);
 ok=ok && getsCount==2 && !setsCount && !destructs && !kills && owner.PHeadEffect.PData->Item==&thing;
 thing.Dead=0;thing.OtherFlags=63;getsCount=0;TestAccess::run(owner);
 ok=ok && !bad && getsCount==4 && setsCount==2 && destructs==1 && kills==1 && created==start+1 && !owner.PHeadEffect.PData->Item && thing.Dead && thing.OtherFlags==63;
 for(unsigned int i=0;i<sizeof(owner.Prefix);++i)ok=ok && owner.Prefix[i]==0x5a;
 puts(ok?"007fb150_TEST PASS":"007fb150_TEST FAIL");return ok?0:1;
}