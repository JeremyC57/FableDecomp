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