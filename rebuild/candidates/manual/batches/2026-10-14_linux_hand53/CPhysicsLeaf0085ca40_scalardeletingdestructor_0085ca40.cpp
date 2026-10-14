// CPhysicsLeaf_0085ca40::`scalar deleting destructor'(unsigned int flags) -- this class's vtable (0x01276AEC), the raw buffer at +0x10 freed (`operator delete`), the async data member at +0xc discarded (0x00850530, `__fastcall`), the NCameraModes::CSubComponentBase base's inline destructor stores its vtable (0x012758F4), then free for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085CA40, length 64 bytes.
// Leaf: ??_GCPhysicsLeaf_0085ca40@@UAEPAXI@Z
// The chunk label is propagated; the vtable (0x01276AEC) names the class. Shape: the vtable store, the members, `test byte ptr [esp+8],1`, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CAsyncData_00850530;
void __fastcall DiscardAsyncData_00850530(CAsyncData_00850530* data);   // 0x00850530
struct CAsyncData_00850530 {
    void* m_pData;
    __forceinline ~CAsyncData_00850530() { DiscardAsyncData_00850530(this); }
};
namespace NCameraModes {
struct CSubComponentBase {                        // vtable 0x012758F4
    virtual void v00();
    ~CSubComponentBase() { }                       // inline: the vtable store
};
}
struct CPhysicsLeaf_0085ca40 : NCameraModes::CSubComponentBase {              // vtable 0x01276AEC
    long                 m_4;
    long                 m_8;
    CAsyncData_00850530  m_asyncData;              // +0x0c
    char*                m_pBuffer;                // +0x10
    __forceinline virtual ~CPhysicsLeaf_0085ca40()
    {
        delete m_pBuffer;
    }
};

void* MakeOne_0085ca40()                                 // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new CPhysicsLeaf_0085ca40;
}
