// CThingFilter_IsTargetable::operator()(CThing* thing) -- true when the thing has a targeted component (kind 9) that is targetable and, when ZTargeting, is Z-targetable or UsingLightning is set
// Leaf: ??RCThingFilter_IsTargetable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A84F0, length 103 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (inlined GetTC of kind 9 = bit 9 of CThing+0x20),
// CTCTargeted::IsTargetable @ 0x007EA280 (manifest "CScriptBase::IsActiveThreadTerminating" is a propagated label; body
// returns +0x10 when +0x0c is set), CTCTargeted::IsZTargetable @ 0x007EA290.
// Layout: PDB CThingFilter_IsTargetable { bool ZTargeting (+0); bool UsingLightning (+1); }; CThing component bits +0x20,
// interface map +0x44 (local model, as in the landed CThingFilter rows).
// Shape: HasTC + GetTC with the component a named local (`mov esi,[eax+4]`); one if with an && / || condition and
// byte-constant returns.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
struct CTCTargeted {
    bool IsTargetable() const;                  // 0x007EA280
    bool IsZTargetable() const;                 // 0x007EA290
};
struct CThingFilter_IsTargetable {
    bool ZTargeting;                            // +0x00
    bool UsingLightning;                        // +0x01
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsTargetable::operator()(CThing* thing) const
{
    if (thing->HasTC(9)) {
        CTCTargeted* targeted = (CTCTargeted*)thing->GetTC(9);
        if (targeted->IsTargetable() && (!ZTargeting || targeted->IsZTargetable() || UsingLightning))
            return true;
    }
    return false;
}