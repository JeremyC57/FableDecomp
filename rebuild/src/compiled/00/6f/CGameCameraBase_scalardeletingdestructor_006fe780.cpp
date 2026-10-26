// CGameCameraBase::`scalar deleting destructor'(unsigned int flags) -- this class's vtable (0x01264A8C), the two list members at +0xac / +0xa8 cleared (0x0048CA60, labelled "SortTreeRecursively": std::list::clear), the CAIGameCameraBase base's inline destructor stores its vtable (0x0125C1F4), then free for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FE780, length 64 bytes.
// Leaf: ??_GCGameCameraBase@@UAEPAXI@Z
// The chunk label is propagated; the vtable (0x01264A8C) names the class. Shape: the vtable store, the members, `test byte ptr [esp+8],1`, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CList_0048ca60 {                            // a 4-byte list (one sentinel pointer)
    void* m_pNode;
    ~CList_0048ca60();                             // 0x0048CA60
};
class CAIGameCameraBase { public:                         // vtable 0x0125C1F4
    virtual void v00();
    unsigned char _body[0xa4];
    ~CAIGameCameraBase() { }                       // inline: the vtable store
};
struct CGameCameraBase : CAIGameCameraBase {              // vtable 0x01264A8C
    CList_0048ca60 m_lista8;                       // +0xa8
    CList_0048ca60 m_listac;                       // +0xac
    __forceinline virtual ~CGameCameraBase() { }
};

void* MakeOne_006fe780()                                 // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new CGameCameraBase;
}