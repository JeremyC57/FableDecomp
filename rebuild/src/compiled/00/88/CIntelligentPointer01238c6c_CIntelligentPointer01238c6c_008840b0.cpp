// CIntelligentPointer_01238c6c::CIntelligentPointer_01238c6c() -- default constructor: base CBaseIntelligentPointer, own vtable, SetPItem(0), clear the flag byte at +8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008840B0, length 32 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem
// @ 0x00A01B90 (literal 0). Vtable 0x01238C6C (rtti-vtables.tsv: CIntelligentPointer, a propagated template
// name); the manifest module CCombatAbilityUseProjectileWeaponDef is a propagated label and wrong.
// Same body as the tagged constructor @ 0x00883A30 without the (unused) stack argument: plain `ret`.
struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                       // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    void SetPItem(CBaseObject* pItem);          // 0x00A01B90
};

struct CIntelligentPointer_01238c6c : CBaseIntelligentPointer {
    unsigned char m_locked;                     // +0x08
    virtual void v00();                         // own vtable 0x01238C6C
    CIntelligentPointer_01238c6c();
};

CIntelligentPointer_01238c6c::CIntelligentPointer_01238c6c()
{
    SetPItem(0);
    m_locked = 0;
}