// CLoadedMesh_00ad88c0::`scalar deleting destructor'(unsigned int flags) -- destroy the member at +4 (0x00AD83A0, labelled "CActiveFile::OnReadFinished"), then the C3DOptimisedMeshBase base's inline destructor (its vtable 0x0129F048, CBaseClassNonCopyable::~ @ 0x0099A300) and free for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD88C0, length 46 bytes.
// Leaf: ??_GCLoadedMesh_00ad88c0@@QAEPAXI@Z
// The chunk label is propagated; named by address. Shape: the derived class stores no vtable of its own (its vtable folds into the base's), the base's inline destructor stores the base vtable, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CMember_00ad83a0 {
    unsigned char _body[4];
    ~CMember_00ad83a0();                           // 0x00AD83A0
};
struct CBaseClassNonCopyable {
    virtual void v00();
    ~CBaseClassNonCopyable();                      // 0x0099A300
};
struct C3DOptimisedMeshBase : CBaseClassNonCopyable {   // vtable 0x0129F048
    virtual void v00();
    ~C3DOptimisedMeshBase() { }                    // inline: the vtable store then the base destructor
};
struct CLoadedMesh_00ad88c0 {                      // non-polymorphic: the mesh base is its first member (its inline destructor stores the base vtable)
    C3DOptimisedMeshBase m_base;                   // +0x00
    CMember_00ad83a0     m_member4;                // +0x04
    ~CLoadedMesh_00ad88c0() { }
};

void Kill_00ad88c0(CLoadedMesh_00ad88c0* object)   // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}
