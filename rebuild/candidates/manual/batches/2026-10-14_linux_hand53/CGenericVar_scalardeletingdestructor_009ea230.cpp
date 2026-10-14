// CGenericVar::`scalar deleting destructor'(unsigned int flags) -- this class's vtable (0x0129C3E4), the CCharString at +8 destroyed (0x0099EAE0), then free for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009EA230, length 39 bytes.
// Leaf: ??_GCGenericVar@@UAEPAXI@Z
// The chunk label is propagated; the vtable (0x0129C3E4) names the class. Shape: the vtable store, the members, `test byte ptr [esp+8],1`, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CCharString {
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};
struct CGenericVarRoot_009ea230 {
    virtual void v00();
};
struct CGenericVar : CGenericVarRoot_009ea230 {              // vtable 0x0129C3E4
    long        m_4;                               // +0x04
    CCharString m_name;                            // +0x08
    __forceinline virtual ~CGenericVar() { }
};

void* MakeOne_009ea230()                                 // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new CGenericVar;
}
