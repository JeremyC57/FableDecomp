// CTCSpecialAbilities::ToggleUsingAbility(long ability) -- for ability 4, cancel the thing's active physical-shield spell move; true when one was cancelled
// Leaf: ToggleUsingAbility
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072AC90, length 93 bytes (`ret 4`; caller 0x005EEAE8).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, the shield component's active test
// @ 0x0079DCB0 (manifest "C3DMeshInfo::HasPhysicsMesh", propagated from the byte-identical getter) and
// CTCPhysicalShieldSpell::CancelMove @ 0x0079E300.
// Layout: the parent thing's interface bitset at +0x20 (type 0x93 -> word 4 = +0x30, bit 19 = 0x80000) and its sorted
// interface map at +0x44 (the landed GetInterface shape of 0x007504E0).
// Shape: the inline out-parameter GetInterface keeps the map and then the component in esi; nested ifs share the
// `xor al,al` exit. GetInterface must be `__forceinline`: as a plain inline template VC7.1 emits it out of line here.
struct CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 std::bitset of thing-component interface types
    CInterfaceMap m_interfaces;                             // +0x44
    template <class T>
    __forceinline bool GetInterface(long type, T** out)     // inline: the landed 0x007504E0 GetInterface shape
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

struct CTCPhysicalShieldSpell {
    bool IsMoveActive();                                    // 0x0079DCB0
    void CancelMove();                                      // 0x0079E300
};

struct CTCSpecialAbilities {
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    bool ToggleUsingAbility(long ability);
};

bool CTCSpecialAbilities::ToggleUsingAbility(long ability)
{
    if (ability == 4) {
        CTCPhysicalShieldSpell* shield;
        if (ParentThing->GetInterface(0x93, &shield)) {
            if (shield->IsMoveActive()) {
                shield->CancelMove();
                return true;
            }
        }
    }
    return false;
}