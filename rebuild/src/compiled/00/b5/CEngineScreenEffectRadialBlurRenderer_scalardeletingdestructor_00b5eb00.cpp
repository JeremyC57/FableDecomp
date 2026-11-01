// CEngineScreenEffectRadialBlurRenderer::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCEngineScreenEffectRadialBlurRenderer@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B5EB00, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c672): slot 0 of RTTI vtable 0x012A2658.
// Callees: CEngineScreenEffectRadialBlurRenderer::~CEngineScreenEffectRadialBlurRenderer 0x00B5C050 (??1CEngineScreenEffectRadialBlurRenderer@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the CEngineScreenEffectRadialBlurRenderer::ScalarDeletingDestructor (member stand-in)
//   calling CEngineScreenEffectRadialBlurRenderer::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CEngineInit;

class CEngineScreenEffectRadialBlurRenderer {
public:
    CEngineScreenEffectRadialBlurRenderer(const CEngineInit&);   // ??0CEngineScreenEffectRadialBlurRenderer@@QAE@ABVCEngineInit@@@Z
    virtual ~CEngineScreenEffectRadialBlurRenderer();   // 0x00B5C050 (??1CEngineScreenEffectRadialBlurRenderer@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CEngineScreenEffectRadialBlurRenderer@@QAE@ABVCEngineInit@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CEngineScreenEffectRadialBlurRenderer::CEngineScreenEffectRadialBlurRenderer(const CEngineInit&) { }
