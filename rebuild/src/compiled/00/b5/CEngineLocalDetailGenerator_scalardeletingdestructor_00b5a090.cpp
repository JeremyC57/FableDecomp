// CEngineLocalDetailGenerator::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCEngineLocalDetailGenerator@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B5A090, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c671): slot 0 of vtable 0x012A2588; Ego_r/FableWin name set.
// Callees: CEngineLocalDetailGenerator::~CEngineLocalDetailGenerator 0x00B59BE0 (??1CEngineLocalDetailGenerator@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CEngineLocalDetailGenerator_scalar_deleting_destructor
//   calling CEngineLocalDetailGenerator_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CEngineLocalDetailGenerator {
public:
    CEngineLocalDetailGenerator(bool);   // ??0CEngineLocalDetailGenerator@@QAE@_N@Z
    virtual ~CEngineLocalDetailGenerator();             // 0x00B59BE0 (??1CEngineLocalDetailGenerator@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CEngineLocalDetailGenerator@@QAE@_N@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CEngineLocalDetailGenerator::CEngineLocalDetailGenerator(bool) { }
