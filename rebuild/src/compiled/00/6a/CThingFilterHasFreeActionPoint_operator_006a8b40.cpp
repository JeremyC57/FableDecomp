// CThingFilter_HasFreeActionPoint::operator()(CThing* thing) -- true when the thing has a user-action-point component (interface 0x2D) whose action point is free
// Leaf: ??RCThingFilter_HasFreeActionPoint
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8B40, length 76 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (the thing's interface map at +0x44),
// CTCUserActionPointBase::IsActionPointFree @ 0x007FEBF0.
// Layout: CThing keeps the interface-present bitset at +0x20 (type 0x2D -> word 1 at +0x24, bit 13: `test ch,20h`)
// and the sorted interface map at +0x44 (landed GetInterface family, 0x007504E0 / 0x0075D8F0 shape).
// Shape: `return GetInterface(...) && component->IsActionPointFree();` -- the logical-and result is materialised as an
// int (`mov eax,1` / `xor eax,eax`) even though the operator returns bool; the key lives in the dead `thing` slot.
// The filter has no data members (PDB: no layout recorded); `this` is unused.
struct CTCBase;

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;           // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCUserActionPointBase {
    bool IsActionPointFree();                       // 0x007FEBF0
};

struct CThingFilter_HasFreeActionPoint {
    bool operator()(CThing* thing) const;
};

bool CThingFilter_HasFreeActionPoint::operator()(CThing* thing) const
{
    CTCUserActionPointBase* actionPoint;
    return thing->GetInterface(0x2D, &actionPoint) && actionPoint->IsActionPointFree();
}