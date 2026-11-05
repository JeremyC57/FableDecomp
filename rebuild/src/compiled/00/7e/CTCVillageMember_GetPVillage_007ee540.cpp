// CTCVillageMember::GetPVillage() const -- the village component of the member's village thing (null when the member has no village thing)
// Leaf: ?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's pointer members m_pFirst/m_pLast are the header CArray base's begin() / end() (STLport vector start +0 / finish +4).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EE540, 84 bytes (+ int3 padding). FableWin.pdb
//   `?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ` (public thiscall const); 269 call sites. The catalog label
//   `IS_THING_ALIVE<CIntelligentPointer<CThingPhysical const>>` is propagated.
// Layout: CTCVillageMember (Ego_r.pdb, 0x20): CTCBase (0xc), PDef +0x0c, PVillage +0x10 (CIntelligentPointer<CThing>),
//   VillageUID +0x18.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
//   (non-const) @ 0x0040F020; interface type 0x22 = CTCVillage (presence word 1 bit 2: `test byte [eax+24h],4`).
// Shape: the PDB's out-parameter `CThing::GetTC<CTCVillage>(CTCVillage**)` with its bool result ignored, so a thing
//   without the component returns the never-written local (retail reads the shared key/out slot `mov eax,[esp+4]`); the
//   null return is written last (an early `return 0` reuses the zero eax inline instead of retail's trailing `xor eax,eax`).
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry;

typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (HasTC(T::INTERFACE_TYPE)) {
            CTCMap* map = &m_tcs;
            STCEntry* it = map->LowerBound((ETCInterfaceType)T::INTERFACE_TYPE);
            if (it == map->end() || T::INTERFACE_TYPE < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CTCVillage {
public:
    enum { INTERFACE_TYPE = 0x22 };
};

class CTCVillageMember {
public:
    virtual void v00();
    CThing*                 ParentThing;                        // +0x04
    unsigned char           _pad08[4];                          // CTCBase flags
    void*                   PDef;                               // +0x0c CDefPointer<const CVillageMemberDef>
    CIntelligentPointer<CThing> PVillage;                           // +0x10 CIntelligentPointer<CThing>
    unsigned long           VillageUID[2];                      // +0x18
    CTCVillage* GetPVillage() const;
};

CTCVillage* CTCVillageMember::GetPVillage() const
{
    if (PVillage.GetPItem() != 0) {
        CTCVillage* village;
        PVillage.GetPItem()->GetTC(&village);
        return village;
    }
    return 0;
}