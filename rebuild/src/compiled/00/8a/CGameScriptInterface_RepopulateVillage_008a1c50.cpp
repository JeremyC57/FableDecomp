// CGameScriptInterface::RepopulateVillage(const CScriptThing& thing) -- for a thing holding a village component (interface 0x22), have the world map repopulate its region with villagers
// Leaf: ?RepopulateVillage@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
// Retail: Fable.exe @ 0x008A1C50, 104 bytes, `ret 4`; Ego_r.pdb same symbol.
// Body: if thing.IsAlive() (slot 75, +0x12c): t = thing.GetPThing() (slot 11); if t holds interface 0x22 (bit 2 of
// UsingComponents word 1, +0x24) and the component from the non-const Components map (LowerBound 0x0040F020, key in
// the dead `thing` slot) is non-null: World (+0x04) slot 12 (+0x30) -> PopulateRegionWithVillagers(*t) (0x00502E90,
// ?PopulateRegionWithVillagers@CWorldMap@@QAEXAAVCThing@@@Z per engine_api.tsv). No null test on t.
// Layout: CThing as the matched 0x0089A330 / 0x0089B0B0 rows. Interface 0x22 is modelled as CTCVillage and World slot 12
// as GetWorldMap(); neither name is confirmed.
// Shape: /O2 (16-aligned entry); the GetTC out-pointer form.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                    // CThing::Components, CVectorMap<ETCInterfaceType, CTCBase*> (retail 8 bytes)
public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry*       LowerBound(const ETCInterfaceType& type);          // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;    // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char Type;                                  // +0x10 (1 = a creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)               // ??$GetTC@V...@@@CThing@@QAE_NPAPAV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || T::INTERFACE_TYPE < it->first)
            it = map->last;
        *out = (T*)it->second;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const  // ??$PeekTC@$$CBV...@@@CThing@@QBE_NPAPBV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || T::INTERFACE_TYPE < it->first)
            it = map->last;
        *out = (const T*)it->second;
        return true;
    }
};
class CTCVillage {                                      // interface 0x22 (name not confirmed)
public:
    enum { INTERFACE_TYPE = 0x22 };
};
class CWorldMap {
public:
    void PopulateRegionWithVillagers(CThing& village);   // 0x00502E90
};
class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();                    // slot 12 (+0x30, name not confirmed)
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::RepopulateVillage(const CScriptThing& thing) const
{
    if (thing.IsAlive())
    {
        CThing* t = thing.GetPThing();
        CTCVillage* village;
        if (t->GetTC(&village) && village)
            World->GetWorldMap()->PopulateRegionWithVillagers(*t);
    }
}