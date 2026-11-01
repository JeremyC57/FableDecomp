// CEngineBillboardGenerator::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCEngineBillboardGenerator@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8C7A0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c670): slot 0 of vtable 0x012A3BF4, calls ??1CEngineBillboardGenerator 0x00B8C7C0; Ego_r ??_GCEngineBillboardGenerator@@UAEPAXI@Z.
// Callees: CEngineBillboardGenerator::~CEngineBillboardGenerator 0x00B8C7C0 (??1CEngineBillboardGenerator@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CEngineBillboardGenerator_scalar_deleting_destructor
//   calling CEngineBillboardGenerator_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CEngineBillboardGenerator {
public:
    CEngineBillboardGenerator();   // ??0CEngineBillboardGenerator@@QAE@XZ
    virtual ~CEngineBillboardGenerator();               // 0x00B8C7C0 (??1CEngineBillboardGenerator@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CEngineBillboardGenerator@@QAE@XZ) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CEngineBillboardGenerator::CEngineBillboardGenerator() { }
