// CEngineSurfaceCompositionManager::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCEngineSurfaceCompositionManager@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2F6E0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c681): slot 0 of RTTI vtable 0x012A1238 (rtti-vtables.tsv); calls ??1 0x00B2D1B0.
// Callees: CEngineSurfaceCompositionManager::~CEngineSurfaceCompositionManager 0x00B2D1B0 (??1CEngineSurfaceCompositionManager@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 17: landed as the CEngineSurfaceCompositionManager::ScalarDeletingDestructor (member stand-in)
//   calling CEngineSurfaceCompositionManager::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CEngineSurfaceCompositionManager {
public:
    CEngineSurfaceCompositionManager();   // ??0CEngineSurfaceCompositionManager@@QAE@XZ
    virtual ~CEngineSurfaceCompositionManager();        // 0x00B2D1B0 (??1CEngineSurfaceCompositionManager@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CEngineSurfaceCompositionManager@@QAE@XZ) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CEngineSurfaceCompositionManager::CEngineSurfaceCompositionManager() { }
