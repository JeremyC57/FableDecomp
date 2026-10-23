// CThingFilter_IsAffectedByTraps::operator()(CThing* thing) -- false only when the thing's interface-0x31 component reports an object of type 0x5F; true otherwise (a frame-pointer, size-optimised twin of 0x0080E480)
// Leaf: ??RCThingFilter_IsAffectedByTraps
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007892EA, length 87 bytes.
// Callees: std::bitset<..>::test @ 0x00410DE0 (out of line in this TU), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x004365B0, the component's object accessor @ 0x0073A6E0 (propagated label "CPixelShader::CountReferences"),
// then that object's virtual slot 1 (a type id).
// Shape: the TU is size-optimised with a frame pointer (`push ebp ; mov ebp,esp` and `push dword` style) =
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)`: the bitset test is not inlined, the key is homed in the
// dead `thing` slot ([ebp+8]), `add esi,44h` re-uses the thing register for the map, and the two exits share one tail.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CTCBase;

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;           // 0x004365B0
};

struct CInterfaceBitset {                           // std::bitset<288>
    unsigned long m_words[9];
    bool test(unsigned long pos) const;             // 0x00410DE0
};

struct CThing {
    unsigned char _pad0[0x20];
    CInterfaceBitset m_presentBits;                 // +0x20
    CInterfaceMap m_interfaces;                     // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits.test(type)) {
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

struct CTypedObject {
    virtual void v00();
    virtual long GetType();                         // slot 1
};

struct CTCInterface31 {                             // interface 0x31 component (class not established)
    CTypedObject* GetObject();                      // 0x0073A6E0
};

struct CThingFilter_IsAffectedByTraps {
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsAffectedByTraps::operator()(CThing* thing) const
{
    CTCInterface31* component;
    if (thing->GetInterface(0x31, &component) && component->GetObject()->GetType() == 0x5F)
        return false;
    return true;
}