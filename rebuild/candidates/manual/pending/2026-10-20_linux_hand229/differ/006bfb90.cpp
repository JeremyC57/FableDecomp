// CTCBuyableHouse::NeedsMainSignPost() const -- a house wants its main sign post when the building is available for residency, its def does not allow selling without a sign post, it belongs to a residential village, the village is not (flag +0xB3), the house is buyable and not scripted, and it is either owned with no wife living there or not owned and not in use
// Grade: DIFFER(324v311) -- the village def copies: retail copies the counted pointer with `inc` + `dec` through registers (no null re-test) and keeps the residential byte in bl initialised by `xor bl,bl`; ours re-tests the copy (`setne bl`), homes the flag in a frame byte and adds a jmp; plus the lane `(x >> 11) & 0xFFFFFF01` narrowing gap on the IsHouseBeingUsed argument
//   tried: flat early returns (pushes shrink-wrapped, 328 bytes), nested success form (this; shared false exit right) | next: a CCountedPointer whose copy ctor/dtor are the retail CDefPointer forms without the null test; lane gap: `& 0xFFFFFF01` narrowing
// Leaf: NeedsMainSignPost
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BFB90, 311 bytes (+9 int3 padding), `ret`.
// Callees: CThingBuilding::GetIsAvailableForResidency() 0x0082E0E0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
//   0x0040F020 (non-const), CTCVillageMember::GetPVillage() const 0x007EE540 (manifest IS_THING_ALIVE<..> propagated),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCBuyableHouse::IsHouseBeingUsed(bool) 0x006BFB10, the village def's
//   slot 1 on release.
// Shape: the village's counted def pointer (+0x94) is copied by value twice (null test, then the residential byte
//   +0xE5 read in bl); the not-in-use result is a named bool (dead store to the frame); the thing flag +0x34 bit 11 is
//   the bool argument.
// Layout: engine/CTCBuyableHouse.h (PDef +0x0c, IsScripted +0x10, OwnedByPlayer +0x11, WifeLivingHere +0x38);
//   CBuyableHouseDef IsBuyable +0x25, IsBuyableWithoutSignpost +0x49 (retail); CTCVillageMember interface 0x23.
struct CDefPointeeBase {
    virtual void Destroy();
    virtual void Delete();                      // slot 1
    long m_refCount;                            // +0x04
};
template <class T> struct CCountedPointer {
    T* Object;
    CCountedPointer(const CCountedPointer& o) : Object(o.Object) { if (Object) ++Object->m_refCount; }
    ~CCountedPointer() { if (--Object->m_refCount == 0) Object->Delete(); }
    T* operator->() const { return Object; }
    operator bool() const { return Object != 0; }
};
template <class T> struct CDefPointer {
    CDefPointeeBase* Object;
    T* operator->() const { return (T*)Object; }
};
struct CBuyableHouseDef : CDefPointeeBase {
    unsigned char _pad08[0x25 - 0x08];
    bool          IsBuyable;                    // +0x25
    unsigned char _pad26[0x49 - 0x26];
    bool          IsBuyableWithoutSignpost;     // +0x49
};
struct CVillageDef : CDefPointeeBase {
    unsigned char _pad08[0xe5 - 0x08];
    bool          IsResidential;                // +0xe5
};
struct CTCVillage {
    unsigned char _pad00[0x94];
    CCountedPointer<CVillageDef> PDef;          // +0x94
    unsigned char _pad98[0xb3 - 0x98];
    bool          Flag_b3;                      // +0xb3
    CCountedPointer<CVillageDef> GetPDef() const { return PDef; }
};
struct CTCVillageMember {                       // interface 0x23
    CTCVillage* GetPVillage() const;            // 0x007EE540
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[5];            // +0x20
    unsigned long  m_flags34;                   // +0x34 (bit 11 passed to IsHouseBeingUsed)
    unsigned char  _pad38[0x44 - 0x38];
    CInterfaceMap  m_interfaces;                // +0x44
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
class CThingBuilding : public CThing {
public:
    bool GetIsAvailableForResidency() const;    // 0x0082E0E0
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
class CTCBuyableHouse {
public:
    void*         __vftable;                    // +0x00
    CThingBuilding* ParentThing;                // +0x04
    unsigned char _pad08[4];
    CDefPointer<const CBuyableHouseDef> PDef;   // +0x0c
    bool          IsScripted;                   // +0x10
    bool          OwnedByPlayer;                // +0x11
    unsigned char _pad12[0x38 - 0x12];
    CBaseIntelligentPointer WifeLivingHere;     // +0x38
    bool IsHouseBeingUsed(bool flag) const;     // 0x006BFB10
    bool NeedsMainSignPost() const;
};

bool CTCBuyableHouse::NeedsMainSignPost() const
{
    if (!ParentThing->GetIsAvailableForResidency() || (PDef.Object && PDef->IsBuyableWithoutSignpost))
        return false;
    bool residential = false;
    if (ParentThing->HasTC(0x23)) {
        CTCVillageMember* member = ParentThing->GetTC<CTCVillageMember>(0x23);
        if (member->GetPVillage()) {
            if (member->GetPVillage()->GetPDef())
                residential = member->GetPVillage()->GetPDef()->IsResidential;
            if (!member->GetPVillage()->Flag_b3 && residential && PDef.Object
                && (PDef->IsBuyable || PDef->IsBuyableWithoutSignpost) && !IsScripted) {
                if (OwnedByPlayer) {
                    if (!WifeLivingHere.GetPItem())
                        return true;
                } else {
                    bool free = !IsHouseBeingUsed(((ParentThing->m_flags34 >> 11) & 1) != 0);
                    if (free)
                        return true;
                }
            }
        }
    }
    return false;
}
