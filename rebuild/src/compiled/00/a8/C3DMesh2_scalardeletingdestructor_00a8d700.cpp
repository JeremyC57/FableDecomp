// C3DMesh2::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GC3DMesh2@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8D700, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c671): slot 0 of vtable 0x0129DEF0 (RTTI C3DMesh2); scalar-deleting body (`test [esp+8],1` only); the PDB sets list ??_EC3DMesh2@@UAEPAXI@Z (the donor build's vector form) -- confirm the spelling at landing.
// Callees: C3DMesh2::~C3DMesh2 0x00A895F0 (??1C3DMesh2@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the C3DMesh2::ScalarDeletingDestructor (member stand-in)
//   calling C3DMesh2::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class C3DMesh2 {
public:
    C3DMesh2();   // ??0C3DMesh2@@QAE@XZ
    virtual ~C3DMesh2();                                // 0x00A895F0 (??1C3DMesh2@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0C3DMesh2@@QAE@XZ) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
C3DMesh2::C3DMesh2() { }
