// CTCInventoryStats::Copy(const CTCInventoryBase* other) -- copies another stats inventory: the base inventory and screen state unless suppressed, then re-caches the owner's hero-experience (TC 0x68) and hero-stats (TC 4) components
// Leaf: ?Copy@CTCInventoryStats@@
// Retail: Fable.exe @ 0x005CDC1C, 175 bytes, __fastcall `ret 4`. Ego_r.pdb `?Copy@CTCInventoryStats@@UAEXABVCTCBase@@@Z`
// (a pointer in TLC, as in the landed CTCInventoryAbilities::Copy 0x005F3DC6). Virtual (slot 12).
// Callees: NInventory::CTCInventoryBase::Copy(const CTCInventoryBase*) @ 0x005BFB96 and the +0x130 screen base's Copy
// @ 0x0066E7A6 (an empty `ret 4`), both qualified non-virtual calls skipped while the flag @ 0x013B8A88 is set;
// std::bitset::test @ 0x00410DE0 on the owner's UsingComponents (+0x20), the const component-map LowerBound @ 0x004365B0.
// Shape: frame-pointer size TU; the screen base gets `other` converted to that base (null-safe `neg ; sbb ; and`); each
// component is the landed OnCreate (0x005CD33F) GetInterface: the size-optimised `push 68h ; pop edi` constant serves the
// test and the key, the key homed in the dead `other` slot [ebp+8], the owner re-read for the second lookup.
// Layout: retail CTCInventoryBase 0x130 bytes with the screen base at +0x130; CTCInventoryStats pTCHeroExperience +0x14C,
// pTCHeroStats +0x150 (PDB minus 0x1C; the landed OnCreate's model).
// Label: the catalog's CTCInventoryBase_CopyAndSyncInterface is this method.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern bool g_SuppressInventoryBaseCopy;                // 0x013B8A88
class CTCBase;
struct CPresenceBits {
    bool test(long bit) const;                          // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;         // 0x004365B0
};
class CThing { public:
    unsigned char _pad00[0x20];
    CPresenceBits UsingComponents;                      // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap Components;                           // +0x44
    template <class T> __forceinline void GetInterface(long typeId, T** out) const
    {
        if (UsingComponents.test(typeId)) {
            long type = typeId;
            const CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || typeId < it->type) ? map->last : it)->component;
        }
    }
};
namespace NHeroInformationScreens {
class CBase { public:                                          // the screen interface base at +0x130
    virtual void Copy(const CBase* other);              // 0x0066E7A6 (empty)
};
}
struct CTCInventoryBase {                               // retail 0x130 bytes
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Copy(const CTCInventoryBase* other);   // slot 12 (+0x30), base body 0x005BFB96
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x130 - 0x08];
};
class CTCHeroExperience;
class CTCHeroStats;
class CTCInventoryStats : public CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    unsigned char      CreaturesKilled[0xc];            // +0x134 map<unsigned long, long>
    CThing*            pMannequin;                      // +0x140
    bool               UseMannequin;                    // +0x144
    long               lastCategoryIdx;                 // +0x148
    CTCHeroExperience* pTCHeroExperience;               // +0x14c
    CTCHeroStats*      pTCHeroStats;                    // +0x150
    virtual void Copy(const CTCInventoryBase* other);
};
void CTCInventoryStats::Copy(const CTCInventoryBase* other)
{
    const CTCInventoryStats* source = static_cast<const CTCInventoryStats*>(other);
    if (!g_SuppressInventoryBaseCopy) {
        CTCInventoryBase::Copy(source);
        NHeroInformationScreens::CBase::Copy(source);
    }
    ParentThing->GetInterface(0x68, &pTCHeroExperience);
    ParentThing->GetInterface(4, &pTCHeroStats);
}