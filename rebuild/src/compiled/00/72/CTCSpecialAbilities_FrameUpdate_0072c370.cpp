// CTCSpecialAbilities::FrameUpdate() -- cancel every ability while the thing's interface-0x29 component reports its flag at +0x0d clear
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072C370, length 74 bytes; vtable 0x0126683C (CTCSpecialAbilities) slot 9,
// the CTCBase FrameUpdate slot.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CTCSpecialAbilities::CancelAllAbilities
// @ 0x0072BA40.
// Layout: the parent thing's interface bitset at +0x20 (type 0x29 -> word 1 = +0x24, bit 9: `test ch,2`) and its sorted
// interface map at +0x44 (the landed GetInterface shape of 0x007504E0); the component's flag byte is at +0x0d.
// Shape: inline out-parameter GetInterface (map pointer in esi), then the flag test.
class CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 std::bitset of thing-component interface types
    CInterfaceMap m_interfaces;                             // +0x44
    template <class T>
    bool GetInterface(long type, T** out)                   // inline: the landed 0x007504E0 GetInterface shape
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

struct CTCInterface29 {                                     // the interface-0x29 component (only its flag is read)
    unsigned char _pad[0x0d];
    bool          m_flag0d;                                 // +0x0d
};

class CTCSpecialAbilities { public:
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    void CancelAllAbilities();                              // 0x0072BA40
    void FrameUpdate();
};

void CTCSpecialAbilities::FrameUpdate()
{
    CTCInterface29* component;
    if (ParentThing->GetInterface(0x29, &component)) {
        if (!component->m_flag0d)
            CancelAllAbilities();
    }
}