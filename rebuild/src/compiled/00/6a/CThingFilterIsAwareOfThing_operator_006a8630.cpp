// CThingFilter_IsAwareOfThing::operator()(CThing* thing) -- true when the thing has a perception component (kind 0x34) that is perceiving PThingToBeAwareOf with a non-zero awareness value
// Leaf: ??RCThingFilter_IsAwareOfThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8630, length 109 bytes.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (inlined GetTC of kind 0x34 = bit 20 of CThing+0x24),
// the CTCPerceiveThing perceived-thing getter @ 0x007446A0 (`add ecx,0Ch ; jmp CBaseIntelligentPointer::GetPItem`).
// Globals: float 0.0f @ 0x0122DEDC.
// Layout: PDB CThingFilter_IsAwareOfThing { CThing* PThingToBeAwareOf; }; retail CThing component bits at +0x20, the
// {kind, component} vector map at +0x44; the perception component's awareness float at +0x18 (local model).
// Shape: HasTC + GetTC (component a named local: `mov esi,[eax+4]`, the lookup key homed in the dead thing slot);
// the `&&` return materialises an int, the absent component returns the byte `xor al,al`; `!= 0.0f` gives
// `fucompp ; test ah,44h ; jnp`.
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
struct CTCPerceiveThing {
    unsigned char _pad00[0x18];
    float         m_awareness;                  // +0x18
    CThing* GetPerceivedThing() const;          // 0x007446A0
};
struct CThingFilter_IsAwareOfThing {
    CThing* PThingToBeAwareOf;                  // +0x00
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsAwareOfThing::operator()(CThing* thing) const
{
    if (thing->HasTC(0x34)) {
        CTCPerceiveThing* perceive = (CTCPerceiveThing*)thing->GetTC(0x34);
        return perceive->GetPerceivedThing() == PThingToBeAwareOf && perceive->m_awareness != 0.0f;
    }
    return false;
}