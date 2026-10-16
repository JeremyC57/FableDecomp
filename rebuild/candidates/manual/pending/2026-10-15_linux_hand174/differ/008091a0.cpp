// CThingFilter_IsTargetableBehind::operator()(const CIntelligentPointer<CThing>& pthing) -- true when the thing is melee-vulnerable from Targeter, is PTargetedThing, or has a targeted component (kind 9) with its low flag bit set
// Grade: DIFFER(104v101) -- the flag test compiles to `mov ecx,[eax+4] ; mov dl,[ecx+0Ch] ; mov al,1 ; test al,dl` with a separate true exit where retail has `mov eax,[eax+4] ; test byte ptr [eax+0Ch],1 ; je`; tried dword & 1, nested if, unsigned long bit field
// Leaf: ??RCThingFilter_IsTargetableBehind
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008091A0, length 101 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, IsThingVulnerableToMeleeStrikesFromThing @ 0x007F59F0 (__fastcall
// (thing, Targeter)), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (inlined GetTC of kind 9).
// Layout: PDB CThingFilter_IsTargetableBehind { CThing* Targeter (+0); CThing* PTargetedThing (+4); }; the CTCTargeted flag
// byte at +0x0c (bit 0; the same word IsZTargetable 0x007EA290 tests); CThing component bits +0x20, interface map +0x44.
// Shape: the item is a named local (esi); the __forceinline GetTC (emitted out of line otherwise) result is used directly (`mov eax,[eax+4]`), the key
// homed in the dead argument slot; byte-constant returns.
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
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
struct CBaseIntelligentPointer {
    void* m_vfptr;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {};
bool __fastcall IsThingVulnerableToMeleeStrikesFromThing(const CThing* thing, const CThing* attacker);   // 0x007F59F0
struct CTCTargeted {
    unsigned char _pad00[0x0c];
    unsigned long m_flags;                      // +0x0c
};
struct CThingFilter_IsTargetableBehind {
    CThing* Targeter;                           // +0x00
    CThing* PTargetedThing;                     // +0x04
    bool operator()(const CIntelligentPointer<CThing>& pthing) const;
};

bool CThingFilter_IsTargetableBehind::operator()(const CIntelligentPointer<CThing>& pthing) const
{
    CThing* thing = pthing.GetPItem();
    if (IsThingVulnerableToMeleeStrikesFromThing(thing, Targeter) || PTargetedThing == thing)
        return true;
    if (thing->HasTC(9)) {
        if (((CTCTargeted*)thing->GetTC(9))->m_flags & 1)
            return true;
    }
    return false;
}
