// Alloc_CVtable_012f12d8(void* owner, void* thing) -- script-module __fastcall factory: new CVtable_012f12d8(owner, thing) with the two-argument constructor inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EC3BC0, length 64 bytes.
// Callees: operator new @ 0x00BFEA1A, CScriptBase::CScriptBase @ 0x00CB8110 (script module root
// constructor; the manifest's "CActionEventShot::Alloc" label is propagated). Vtable 0x012F12D8 has
// no RTTI locator (quest-script module), so the class is named CVtable_012f12d8 like the landed
// factory family (see 00d67740, same shape with sizeof 0x84).
// Shape: ecx = owner, edx = thing; 0x84 bytes allocated and null-checked, then the inlined
// constructor: root CScriptBase() on this, thing stored at +0x40 and owner at +0x44, this class's
// vtable. Returns the object (0 when new failed).
class CScriptBase { public:
    virtual void v00();
    CScriptBase();                           // 0x00CB8110
};

struct CVtable_012f12d8 : CScriptBase {      // vtable 0x012F12D8
    unsigned char _pad[0x3c];
    void* m_pThing;                          // +0x40
    void* m_pOwner;                          // +0x44
    unsigned char _tail[0x84 - 0x48];
    virtual void v00();
    CVtable_012f12d8(void* owner, void* thing) : CScriptBase(), m_pThing(thing), m_pOwner(owner) { }
};

CVtable_012f12d8* __fastcall Alloc_CVtable_012f12d8(void* owner, void* thing)
{
    return new CVtable_012f12d8(owner, thing);
}