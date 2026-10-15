// CThingObject::vfunc_64() -- when the thing has interface 172 (0xac), asks that component whether its def has property bit 1 (0x0087C420); false without the interface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00703780, length 69 bytes (vtable 0x01264DCC slot 64).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0, the component predicate @ 0x0087C420
// (manifest "CTCSinglePersonOccupiable::IsOfType"; tests a mask against the def word at +0x28, `ret 4`; its bool returned as is).
// Layout (retail CThing): the interface presence bitset at +0x20, the sorted interface map
// {first, last} at +0x44; type 172 is bit 12 of the presence word at +0x34 (`test ah,10h`).
// Shape: the landed 0x007504E0 out-parameter GetInterface template (presence word tested raw,
// the key in the `push ecx` frame slot, the map in esi, end or a larger key -> the end entry,
// the component through the out pointer into eax, then ecx for the call); `this` is not needed
// after the lookup, so no callee-saved register holds it.
struct CTCBase {
    bool HasProperty(unsigned long mask);   // 0x0087C420 (tests the mask against its def's +0x28 word)
};
struct CInterfaceMap {                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x004365B0
};
struct CThing {
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    bool GetInterface(long type, CTCBase** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = it->component;
            return true;
        }
        return false;
    }
};
struct CThingObject : CThing {
    virtual bool vfunc_64();
};

bool CThingObject::vfunc_64()
{
    CTCBase* component;
    if (GetInterface(172, &component))
        return component->HasProperty(1);
    return false;
}