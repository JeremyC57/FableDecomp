// CThingFilter_IsAffectedByObstructions::operator()(CThing* thing) -- false only when the thing's interface-0x31 component reports an object of type 0x5F (obstruction-immune); true otherwise (also when the component is absent)
// Leaf: ??RCThingFilter_IsAffectedByObstructions
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080E480, length 82 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0, the component's object accessor @ 0x0073A6E0
// (manifest "CPixelShader::CountReferences" is a propagated label; it returns a polymorphic object), then that object's
// virtual slot 1 (`call [edx+4]`, a type id).
// Layout: interface 0x31 -> bitset word 1 (+0x24) bit 17 (`test dword ptr [eax+24h],20000h`).
// Shape: `if (GetInterface && obj->Type() == 0x5F) return false; return true;` -- both exits are byte constants.
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

struct CTypedObject {
    virtual void v00();
    virtual long GetType();                         // slot 1
};

struct CTCInterface31 {                             // interface 0x31 component (class not established)
    CTypedObject* GetObject();                      // 0x0073A6E0
};

struct CThingFilter_IsAffectedByObstructions {
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsAffectedByObstructions::operator()(CThing* thing) const
{
    CTCInterface31* component;
    if (thing->GetInterface(0x31, &component) && component->GetObject()->GetType() == 0x5F)
        return false;
    return true;
}