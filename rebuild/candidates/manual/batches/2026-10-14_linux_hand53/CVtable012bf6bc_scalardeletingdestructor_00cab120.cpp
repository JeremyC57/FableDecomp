// CVtable_012bf6bc::`scalar deleting destructor'(unsigned int flags) -- this class's vtable (0x012BF6BC, no RTTI), the owner's (+8) embedded interface at +0xc released through its COM-style virtual slot 2 (`__stdcall`, the interface pushed), then free for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAB120, length 43 bytes.
// Leaf: ??_GCVtable_012bf6bc@@UAEPAXI@Z
// The chunk label is propagated; the vtable (0x012BF6BC) names the class. Shape: the vtable store, the members, `test byte ptr [esp+8],1`, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct IReleasable_00cab120 {                     // COM-style: __stdcall slots with `this` pushed
    virtual void __stdcall v000(); virtual void __stdcall v001();
    virtual void __stdcall Release();              // slot 2 (+8)
};
struct COwner_00cab120 {
    unsigned char       _pad[0xc];
    IReleasable_00cab120 m_interface;              // +0x0c
};
struct CReleasingRoot_00cab120 {
    virtual void v00();
};
struct CVtable_012bf6bc : CReleasingRoot_00cab120 {              // vtable 0x012BF6BC
    long              m_4;
    COwner_00cab120*  m_pOwner;                    // +0x08
    __forceinline virtual ~CVtable_012bf6bc()
    {
        m_pOwner->m_interface.Release();
    }
};

void* MakeOne_00cab120()                                 // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new CVtable_012bf6bc;
}
