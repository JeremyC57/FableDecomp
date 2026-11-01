// CEngineShadowRenderer::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCEngineShadowRenderer@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B50B80, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c671): slot 0 of vtable 0x012A2374; Ego_r.pdb name set; calls ~CEngineShadowRenderer 0x00B4D210.
// Callees: CEngineShadowRenderer::~CEngineShadowRenderer 0x00B4D210 (??1CEngineShadowRenderer@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the CEngineShadowRenderer::ScalarDeletingDestructor (member stand-in)
//   calling CEngineShadowRenderer::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CEngineShadowRenderer {
public:
    CEngineShadowRenderer();   // ??0CEngineShadowRenderer@@QAE@XZ
    virtual ~CEngineShadowRenderer();                   // 0x00B4D210 (??1CEngineShadowRenderer@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CEngineShadowRenderer@@QAE@XZ) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CEngineShadowRenderer::CEngineShadowRenderer() { }
