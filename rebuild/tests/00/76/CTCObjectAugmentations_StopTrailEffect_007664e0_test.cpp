#include <stdio.h>
#include <string.h>
// CTCObjectAugmentations::StopTrailEffect() -- Kill the live effect and clear its intelligent pointer.
// Leaf: ?StopTrailEffect@CTCObjectAugmentations@@QAEXXZ
// Identity: Own shot callers lookup component0xbf; PDB StopTrailEffect and ProjectileTrailEffect donor+125 adjusted by six shrunk containers to retail+101.
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
// Layout: no active owner header; ProjectileTrailEffect is an8-byte intelligent pointer at 101, proved by retail address and PDB member. Packed layout preserves the original unaligned member where applicable.
#pragma pack(push,1)
class CTCObjectAugmentations {public:unsigned char Prefix[101];CIntelligentPointer<CThing> ProjectileTrailEffect;
public:void StopTrailEffect();friend struct TestAccess;
};
#pragma pack(pop)
void CTCObjectAugmentations::StopTrailEffect(){if(ProjectileTrailEffect.GetPItem() && !ProjectileTrailEffect.GetPItem()->Dead){ProjectileTrailEffect.GetPItem()->Kill(true);ProjectileTrailEffect=CIntelligentPointer<CThing>(0);}}

class CBaseObjectPointer {public:void* Item;};
static CBaseObjectPointer pool[16];static int created,getsCount,setsCount,destructs,kills,bad;static CThing* expected;
CBaseIntelligentPointer::CBaseIntelligentPointer(){PData=&pool[created++];PData->Item=0;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){++destructs;}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{++getsCount;return (CBaseObject*)PData->Item;}
void CBaseIntelligentPointer::SetPItem(CBaseObject* p){++setsCount;PData->Item=p;}
void CThing::Kill(bool force){++kills;if(this!=expected || !force)bad=1;Dead=1;}
struct TestAccess {static void run(CTCObjectAugmentations& x){x.StopTrailEffect();}};
int main(){CTCObjectAugmentations owner;CThing thing;memset(&thing,0,sizeof(thing));memset(owner.Prefix,0x5a,sizeof(owner.Prefix));expected=&thing;
 int start=created;getsCount=setsCount=destructs=0;TestAccess::run(owner);
 int ok=getsCount==1 && !setsCount && !destructs && !kills && created==start;
 owner.ProjectileTrailEffect.PData->Item=&thing;thing.Dead=1;getsCount=0;TestAccess::run(owner);
 ok=ok && getsCount==2 && !setsCount && !destructs && !kills && owner.ProjectileTrailEffect.PData->Item==&thing;
 thing.Dead=0;thing.OtherFlags=63;getsCount=0;TestAccess::run(owner);
 ok=ok && !bad && getsCount==4 && setsCount==2 && destructs==1 && kills==1 && created==start+1 && !owner.ProjectileTrailEffect.PData->Item && thing.Dead && thing.OtherFlags==63;
 for(unsigned int i=0;i<sizeof(owner.Prefix);++i)ok=ok && owner.Prefix[i]==0x5a;
 puts(ok?"007664e0_TEST PASS":"007664e0_TEST FAIL");return ok?0:1;
}