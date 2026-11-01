// CMouseDX::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCMouseDX@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB5FA0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c670): slot 0 of vtable 0x0129EA14, calls ??1CMouseDX 0x00AB5320; Ego_r ??_GCMouseDX@@UAEPAXI@Z.
// Callees: CMouseDX::~CMouseDX 0x00AB5320 (??1CMouseDX@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CMouseDX_scalar_deleting_destructor
//   calling CMouseDX_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CInputManagerDX;

class CMouseDX {
public:
    CMouseDX(CInputManagerDX&, bool);   // 0x00AB5D00
    virtual ~CMouseDX();                                // 0x00AB5320 (??1CMouseDX@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CMouseDX@@QAE@AAVCInputManagerDX@@_N@Z 0x00AB5D00) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CMouseDX::CMouseDX(CInputManagerDX&, bool) { }
