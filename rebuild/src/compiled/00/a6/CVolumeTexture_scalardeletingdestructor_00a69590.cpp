// CVolumeTexture::`scalar deleting destructor'(unsigned int) -- the compiler-generated deleting destructor: the inline user destructor (own vtable store, then Uninitialise()) and the out-of-line ~CBaseClass, then operator delete when bit 0 of the flags is set; returns this
// Leaf: ??_GCVolumeTexture@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A69590, 43 bytes (+ int3 padding), thiscall, `ret 4`; reached only through vtable
//   0x0129DC18 slot 0.
// Identity (proven): CVolumeTexture; evidence: vtable 0x0129DC18 slot 0, RTTI .?AVCVolumeTexture@@ : CBaseClass (directly); 0x00A68060 is the landed CVolumeTexture::Uninitialise; its base destructor 0x0099A2E0 stores the CBaseClass vtable 0x01231710 and is the one a direct CBaseClass child calls (CTCBase and CInputProcessManager, both under CBaseClassNonCopyable, call 0x0099A300 instead), so it is ~CBaseClass; FableWin.pdb names ??1CVolumeTexture@@UAE@XZ.
// Label: the manifest's "`vector_deleting_destructor'" is propagated across family H025 (seven bodies with the same shape: vtable store, one call,
//   the base destructor, the flag test; retail has no identical-code folding, so each address is one class).
// Callees: CVolumeTexture::Uninitialise() 0x00A68060, CBaseClass::~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC.
// Shape: VC7.1 resets the vptr in a user-written destructor, so the store is the class's own vtable; the destructor body is
//   one call (`this` is already in ecx), the base destructor follows. The deleting destructor is emitted with the vtable, in
//   a TU that constructs the class: MakeOne is that construction, nothing more.
class CBaseClass {
public:
    CBaseClass();
    virtual ~CBaseClass();                              // 0x0099A2E0
};
class CVolumeTexture : public CBaseClass {
public:
    CVolumeTexture() { }
    virtual ~CVolumeTexture() { Uninitialise(); }
    void Uninitialise();                                // 0x00A68060
};
void* MakeOne_00a69590()
{
    return new CVolumeTexture;
}