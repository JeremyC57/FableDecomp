// CEnginePrimitiveManagerParticleMeshGroup::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCEnginePrimitiveManagerParticleMeshGroup@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC3C00, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c673): slot 0 of vtable 0x012A63B8; scalar form; FableWin.pdb ??_GCEnginePrimitiveManagerParticleMeshGroup@@UAEPAXI@Z.
// Callees: CEnginePrimitiveManagerParticleMeshGroup::~CEnginePrimitiveManagerParticleMeshGroup 0x00BC3C20 (??1CEnginePrimitiveManagerParticleMeshGroup@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CEnginePrimitiveManagerParticleMeshGroup_scalar_deleting_destructor
//   calling CEnginePrimitiveManagerParticleMeshGroup_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

template <class T> class CArray;
class CEngineScene;

class CEnginePrimitiveManagerParticleMeshGroup {
public:
    CEnginePrimitiveManagerParticleMeshGroup(CArray<CEngineScene*>&);   // ??0CEnginePrimitiveManagerParticleMeshGroup@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z
    virtual ~CEnginePrimitiveManagerParticleMeshGroup(); // 0x00BC3C20 (??1CEnginePrimitiveManagerParticleMeshGroup@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CEnginePrimitiveManagerParticleMeshGroup@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CEnginePrimitiveManagerParticleMeshGroup::CEnginePrimitiveManagerParticleMeshGroup(CArray<CEngineScene*>&) { }
