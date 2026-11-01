// CEngine::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCEngine@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2AB10, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c673): slot 0 of vtable 0x012A0F3C (rtti-vtables.tsv CEngine); scalar form; FableWin.pdb ??_GCEngine@@UAEPAXI@Z.
// Callees: CEngine::~CEngine 0x00B261D0 (??1CEngine@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the CEngine::ScalarDeletingDestructor (member stand-in)
//   calling CEngine::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CEngine {
public:
    CEngine();   // 0x00B260B0
    virtual ~CEngine();                                 // 0x00B261D0 (??1CEngine@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CEngine@@QAE@XZ 0x00B260B0) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CEngine::CEngine() { }
