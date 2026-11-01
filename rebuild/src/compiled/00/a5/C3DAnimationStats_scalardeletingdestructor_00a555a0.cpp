// C3DAnimationStats::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GC3DAnimationStats@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A555A0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c672): slot 0 of RTTI vtable 0x0129D514; Ego_r lists this symbol.
// Callees: C3DAnimationStats::~C3DAnimationStats 0x00A4EB50 (??1C3DAnimationStats@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall C3DAnimationStats_scalar_deleting_destructor
//   calling C3DAnimationStats_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class C3DAnimationStats {
public:
    C3DAnimationStats();   // 0x00A4EB00
    virtual ~C3DAnimationStats();                       // 0x00A4EB50 (??1C3DAnimationStats@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0C3DAnimationStats@@QAE@XZ 0x00A4EB00) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
C3DAnimationStats::C3DAnimationStats() { }
