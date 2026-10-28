// NCreatureMode::CModeDragonStrafeBurninate::EndBeamEffect() -- if the dragon's beam-weapon effect thing is still alive, kill it and clear the pointer
// Leaf: EndBeamEffect
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D7BB0, length 112 bytes. Catalog label "CTCKraken::EndHeadEffects" is a
// propagated byte-twin label: CTCKraken is only 0x78 bytes (PDB) and this body works on +0x98; the only callers are
// CModeDragonStrafeBurninate vfunc_6 @ 0x008DCB4F and CModeDragonStrafeBurninate::FrameUpdate @ 0x008DE132 (vtable 0x01268718).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x4), CThing::Kill(bool) @ 0x004C9B80, CBaseIntelligentPointer ctor
// @ 0x00A01B10 / SetPItem @ 0x00A01B90 (x2) / dtor @ 0x00A01C10; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Layout: PDB CModeDragonStrafeBurninate (0xCC) has PBeamWeaponEffect at +0x9C; retail is +0x98 (4 bytes lower).
// Shape: IS_THING_ALIVE inlined (pointer set and CThing +0x91 bit 0 "dead" clear), Kill(true), then the landed "clear through a
// default-constructed stack CIntelligentPointer" form (0x008C99C0): ctor, vtable store, SetPItem(0), SetPItem(temp), dtor.
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    void Kill(bool immediately);            // 0x004C9B80
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();             // 0x00A01C10
    CThing* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
namespace NCreatureMode {
struct CModeDragonStrafeBurninate {
    virtual void v00();
    unsigned char _pad04[0x98 - 4];
    CBaseIntelligentPointer PBeamWeaponEffect;   // +0x98 (PDB +0x9c)
    void EndBeamEffect();
};

void CModeDragonStrafeBurninate::EndBeamEffect()
{
    if (IS_THING_ALIVE(PBeamWeaponEffect)) {
        PBeamWeaponEffect.GetPItem()->Kill(true);
        CIntelligentPointer none;
        none.SetPItem(0);
        PBeamWeaponEffect.SetPItem(none.GetPItem());
    }
}
}