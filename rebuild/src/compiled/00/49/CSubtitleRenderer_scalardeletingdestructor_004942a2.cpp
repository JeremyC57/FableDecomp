#pragma optimize("s",on)   // kept from the landed row: retail built this TU for size (`pop ecx` after operator delete)
// CSubtitleRenderer::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCSubtitleRenderer@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004942A2, 28 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c675): slot 0 of CSubtitleRenderer vtable 0x012384BC (RTTI).
// Callees: CSubtitleRenderer::~CSubtitleRenderer 0x00443E20 (??1CSubtitleRenderer@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 17: landed as the free __fastcall CSubtitleRenderer_scalar_deleting_destructor
//   calling LBP_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CGraphicDataBank;

class CSubtitleRenderer {
public:
    CSubtitleRenderer(const CGraphicDataBank&);   // ??0CSubtitleRenderer@@QAE@ABVCGraphicDataBank@@@Z
    virtual ~CSubtitleRenderer();                       // 0x00443E20 (??1CSubtitleRenderer@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CSubtitleRenderer@@QAE@ABVCGraphicDataBank@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CSubtitleRenderer::CSubtitleRenderer(const CGraphicDataBank&) { }
