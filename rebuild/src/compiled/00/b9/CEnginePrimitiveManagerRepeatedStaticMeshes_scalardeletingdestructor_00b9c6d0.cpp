// CEnginePrimitiveManagerRepeatedStaticMeshes::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCEnginePrimitiveManagerRepeatedStaticMeshes@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B9C6D0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c681): slot 0 of RTTI vtable 0x012A3E9C (rtti-vtables.tsv); calls ??1 0x00B999F0.
// Callees: CEnginePrimitiveManagerRepeatedStaticMeshes::~CEnginePrimitiveManagerRepeatedStaticMeshes 0x00B999F0 (??1CEnginePrimitiveManagerRepeatedStaticMeshes@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 17: landed as the free __fastcall CEnginePrimitiveManagerRepeatedStaticMeshes_scalar_deleting_destructor
//   calling CEnginePrimitiveManagerRepeatedStaticMeshes_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

template <class T> class CArray;
class CEngineScene;

class CEnginePrimitiveManagerRepeatedStaticMeshes {
public:
    CEnginePrimitiveManagerRepeatedStaticMeshes(CArray<CEngineScene*>&);   // ??0CEnginePrimitiveManagerRepeatedStaticMeshes@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z
    virtual ~CEnginePrimitiveManagerRepeatedStaticMeshes(); // 0x00B999F0 (??1CEnginePrimitiveManagerRepeatedStaticMeshes@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CEnginePrimitiveManagerRepeatedStaticMeshes@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CEnginePrimitiveManagerRepeatedStaticMeshes::CEnginePrimitiveManagerRepeatedStaticMeshes(CArray<CEngineScene*>&) { }
