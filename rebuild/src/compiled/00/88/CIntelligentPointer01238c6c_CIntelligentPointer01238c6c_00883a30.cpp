// CIntelligentPointer_01238c6c::CIntelligentPointer_01238c6c(long tag) -- tagged constructor: base CBaseIntelligentPointer, own vtable, SetPItem(0), clear the flag byte at +8
// Leaf: ??0CIntelligentPointer_01238c6c@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00883A30, length 33 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem
// @ 0x00A01B90 (pushed literal 0; the stack argument is never read, it only selects the overload: ret 4).
// The vtable 0x01238C6C is labelled CIntelligentPointer in rtti-vtables.tsv (propagated across the template
// instantiations); the manifest module CCombatAbilityUseProjectileWeaponDef is a propagated label and wrong
// (this body is a pointer-wrapper constructor, not a def). Sibling default constructor @ 0x008840B0.
// Shape: base ctor call, then `push 0 ; mov ecx,esi ; mov [esi],vtable ; call SetPItem ; mov byte [esi+8],0`
// (the vtable store is scheduled between the argument push and the call), `mov eax,esi` returns this.
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
    CIntelligentPointer_01238c6c(long tag);
};

CIntelligentPointer_01238c6c::CIntelligentPointer_01238c6c(long tag)
{
    SetPItem(0);
    m_locked = 0;
}