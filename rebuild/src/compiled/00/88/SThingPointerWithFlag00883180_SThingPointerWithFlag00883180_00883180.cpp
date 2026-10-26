// SThingPointerWithFlag_00883180::SThingPointerWithFlag_00883180() -- a {CIntelligentPointer<const CThing> Thing; bool Flag} aggregate: the member pointer is built null, the flag cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00883180, length 31 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem
// @ 0x00A01B90 (called with a null pointer).
// The single vtable store is 0x01238C6C = CIntelligentPointer<CThing const> (rtti-vtables.tsv), and
// no second vtable follows, so the 12-byte object is NOT derived from the pointer: it holds the
// pointer as its first member (its inline constructor is CBaseIntelligentPointer() + vtable +
// SetPItem(0), the same body as the out-of-line 0x004A96F0) and a bool at +8. The manifest's
// "CCombatAbilityUseProjectileWeaponDef" label is propagated; the struct is not in Ego_r.pdb, so
// it is named by its shape and address.
class CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* PData;                                            // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
    void SetPItem(const CThing* item);                      // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {      // CIntelligentPointer<CThing const>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

struct SThingPointerWithFlag_00883180 {
    CIntelligentPointer Thing;                              // +0
    bool                Flag;                               // +8
    SThingPointerWithFlag_00883180();
};

SThingPointerWithFlag_00883180::SThingPointerWithFlag_00883180()
    : Thing(0)
    , Flag(false)
{
}