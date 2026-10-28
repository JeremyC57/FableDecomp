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