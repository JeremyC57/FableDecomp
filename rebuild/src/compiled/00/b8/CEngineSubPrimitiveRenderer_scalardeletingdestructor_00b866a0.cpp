// CEngineSubPrimitiveRenderer::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCEngineSubPrimitiveRenderer@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B866A0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c670): slot 0 of vtable 0x012A3A60 (RTTI CEngineSubPrimitiveRenderer), calls ??1CEngineSubPrimitiveRenderer 0x00B84390 (Ego_r has ??_E for the class).
// Callees: CEngineSubPrimitiveRenderer::~CEngineSubPrimitiveRenderer 0x00B84390 (??1CEngineSubPrimitiveRenderer@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the CEngineSubPrimitiveRenderer::ScalarDeletingDestructor (member stand-in)
//   calling CEngineSubPrimitiveRenderer::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CEngineSubPrimitiveRenderer {
public:
    CEngineSubPrimitiveRenderer();   // ??0CEngineSubPrimitiveRenderer@@QAE@XZ
    virtual ~CEngineSubPrimitiveRenderer();             // 0x00B84390 (??1CEngineSubPrimitiveRenderer@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CEngineSubPrimitiveRenderer@@QAE@XZ) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CEngineSubPrimitiveRenderer::CEngineSubPrimitiveRenderer() { }
