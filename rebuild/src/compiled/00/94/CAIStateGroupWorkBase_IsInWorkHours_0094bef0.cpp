// CAIStateGroup_WorkBase::IsInWorkHours() -- true when the creature's shop (shop keeper interface 0x90 -> shop) reports its keeper available
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094BEF0, length 83 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (the creature's interface lookup, type 0x90 = bit 16 of the dword at +0x30),
// GetShopTC @ 0x00798F50 (__fastcall on the CTCShopKeeper component), CTCShop::IsShopKeeperAvailable @ 0x00773AA0.
// Shape: the GetTC<T>() inline of the landed CAIStateGroup_MinionWatchFires::InitStateGroup @ 0x009365E0 (bitset
// test, LowerBound on a named map pointer kept in esi, the end/greater-key fallback) on PBrain->Creature; the
// looked-up component goes straight to the getter; both nulls fall to the shared `xor al,al` tail and the
// predicate's true result is re-materialised (`mov al,1`), so the body is the early-return form.
// The manifest module (a WorkBase template instantiation) is kept; the class is identified by the component type.
struct CTCBase;
struct CThing;

// A thing's component table: the presence bitset at +0x20 (bit N of dword N/32) and the sorted {type, component}
// vector map at +0x44 searched with LowerBound -- the shape of the landed GetInterface bodies.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCShop {
    bool IsShopKeeperAvailable();                                  // 0x00773AA0
};

struct CTCShopKeeper;
CTCShop* __fastcall GetShopTC(CTCShopKeeper* component);     // 0x00798F50

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_WorkBase : CAIStateGroupBase {
    bool IsInWorkHours();
};

bool CAIStateGroup_WorkBase::IsInWorkHours()
{
    CTCShopKeeper* pComponent;
    if (PBrain->Creature->GetInterface(&pComponent, 0x90)) {
        CTCShop* pOwner = GetShopTC(pComponent);
        if (pOwner != 0) {
            if (pOwner->IsShopKeeperAvailable())
                return true;
        }
    }
    return false;
}