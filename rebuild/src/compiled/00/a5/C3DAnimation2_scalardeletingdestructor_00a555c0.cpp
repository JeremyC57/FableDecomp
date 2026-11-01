// C3DAnimation2::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GC3DAnimation2@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A555C0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c672): slot 0 of RTTI vtable 0x0129D548 (rtti-vtables.tsv).
// Callees: C3DAnimation2::~C3DAnimation2 0x00A4EEC0 (??1C3DAnimation2@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the C3DAnimation2::ScalarDeletingDestructor (member stand-in)
//   calling C3DAnimation2::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class C3DAnimation2 {
public:
    C3DAnimation2();   // ??0C3DAnimation2@@QAE@XZ
    virtual ~C3DAnimation2();                           // 0x00A4EEC0 (??1C3DAnimation2@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0C3DAnimation2@@QAE@XZ) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
C3DAnimation2::C3DAnimation2() { }
