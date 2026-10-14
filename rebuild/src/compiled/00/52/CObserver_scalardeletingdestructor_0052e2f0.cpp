// CObserver::`scalar deleting destructor'(unsigned int flags) -- this class's vtable (0x01246020), the member at +4 destroyed (0x0052E1E0, labelled "LTextTreeWalkThrough::Dtor"), then free for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052E2F0, length 39 bytes.
// Leaf: ??_GCObserver@NUISystem@@UAEPAXI@Z
// The chunk label is propagated; the vtable (0x01246020) names the class. Shape: the vtable store, the members, `test byte ptr [esp+8],1`, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CMember_0052e1e0 {
    unsigned char _body[4];
    ~CMember_0052e1e0();                           // 0x0052E1E0
};
struct CObserverBase_0052e2f0 {
    virtual void v00();
};
namespace NUISystem {
struct CObserver : CObserverBase_0052e2f0 {              // vtable 0x01246020
    CMember_0052e1e0 m_member4;                    // +0x04
    __forceinline virtual ~CObserver() { }
};

}
void* MakeOne_0052e2f0()                                 // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new NUISystem::CObserver;
}