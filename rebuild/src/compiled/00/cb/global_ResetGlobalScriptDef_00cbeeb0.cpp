// ResetGlobalScriptDef() -- release the global script def (the def's count drops; its virtual Destroy runs at zero) and null the pointer
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?ResetGlobalScriptDef@@YIXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBEEB0, 34 bytes, `__fastcall` global with no arguments, plain `ret`
//   (PDB ?ResetGlobalScriptDef@@YIXXZ). No frame: nothing lives on the stack, so the size TU's frame pointer is not built.
// Data: the global script def pointer @ 0x0143E90C (g_pScriptDef in the landed quest scripts).
// Callees: the def's virtual Destroy (slot 1, +4) only.
// Shape: the counted-def release of the landed CDefPointer family (0x0042AEDA): the count lives in a second base at
//   +4 behind the vptr, released through two inline accessors (decrement, then test), so the adjusted base pointer
//   is materialised once (`mov ecx,eax ; add eax,4 ; dec [eax] ; cmp [eax],0`) and the def stays in ecx for the
//   virtual call; the unconditional zero store after the test is the size form `and [g],0`.
struct CRefCounted {                       // second base: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                // slot 1
};

struct CScriptDef : CDefVirtual, CRefCounted {
    void ReleaseReference()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

extern CScriptDef* g_pScriptDef;           // 0x0143E90C

void __fastcall ResetGlobalScriptDef()
{
    if (g_pScriptDef)
        g_pScriptDef->ReleaseReference();
    g_pScriptDef = 0;
}