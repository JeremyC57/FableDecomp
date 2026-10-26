// CThingFilter_IsNotBeingExclusivelyUsed::operator()(const CThing& thing) const -- the thing has an exclusive-use component and nobody holds it
// Leaf: ??RCThingFilter_IsNotBeingExclusivelyUsed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8AF0, length 78 bytes (+ int3 padding), `ret 4`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound(const ETCInterfaceType&) const @ 0x004365B0,
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Identity: Ego_r.pdb `??RCThingFilter_IsNotBeingExclusivelyUsed@@QBE_NABVCThing@@@Z` -- the PDB lists it next to
//   HasFreeActionPoint (landed 0x006A8A80); the body reads CTCExclusiveUse::User. The catalog's
//   "CTCInterface_FindInterface" is a descriptive auto label. Callers build the empty filter on the stack
//   (`lea ecx,[esp+17h]`) in CAIStateGroup_Gaze / _Drunk / _MinionReactToCriminal / CTCDummyVillager.
// Layout: CTCExclusiveUse (Ego_r.pdb, 20 bytes) CIntelligentPointer<CThing> User at +0x0c; interface type 0x2f =
//   bit 15 of the presence word +0x24 (`test ch,ch ; jns`); the {kind, component} vector map at +0x44.
// Shape: `return PeekTC(&tc) && !tc->User.GetPItem();` -- the logical-and is materialised int-wide (`mov eax,1` /
//   `xor eax,eax`) like the landed 0x006A8B40; the conversion-temporary key sits in the dead thing-argument slot,
//   the component is loaded into eax for the `lea ecx,[eax+0Ch]`.
class CTCBase;
enum ETCInterfaceType { TC_EXCLUSIVE_USE = 0x2f };

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { ETCInterfaceType type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end_of_storage;
    SEntry* LowerBound(const ETCInterfaceType& type) const;     // 0x004365B0
    __forceinline SEntry* Find(long type) const
    {
        SEntry* it = LowerBound((ETCInterfaceType)type);
        return (it == last || type < it->type) ? last : it;
    }
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];               // +0x20 bitset<274>
    CInterfaceMap Components;                       // +0x44
    bool HasTC(ETCInterfaceType type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool PeekTC(const T** pp) const
    {
        if (HasTC(T::StatGetInterfaceType())) {
            *pp = (const T*)Components.Find(T::StatGetInterfaceType())->component;
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

class CTCExclusiveUse {
public:
    static ETCInterfaceType StatGetInterfaceType() { return TC_EXCLUSIVE_USE; }
    unsigned char _pad00[0x0c];                     // +0x00 CTCBase
    CBaseIntelligentPointer User;                   // +0x0c CIntelligentPointer<CThing>
};

class CThingFilter_IsNotBeingExclusivelyUsed {
public:
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsNotBeingExclusivelyUsed::operator()(const CThing& thing) const
{
    const CTCExclusiveUse* exclusive;
    return thing.PeekTC(&exclusive) && !exclusive->User.GetPItem();
}