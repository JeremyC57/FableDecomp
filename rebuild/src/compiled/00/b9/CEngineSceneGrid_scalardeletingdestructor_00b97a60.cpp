// CEngineSceneGrid::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCEngineSceneGrid@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B97A60, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c670): slot 0 of vtable 0x012A3E58, calls ??1CEngineSceneGrid 0x00B93E40; Ego_r ??_GCEngineSceneGrid@@UAEPAXI@Z.
// Callees: CEngineSceneGrid::~CEngineSceneGrid 0x00B93E40 (??1CEngineSceneGrid@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CEngineSceneGrid_scalar_deleting_destructor
//   calling CEngineSceneGrid_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CEngineSceneGrid {
private:
    CEngineSceneGrid(unsigned long);   // ??0CEngineSceneGrid@@AAE@K@Z
public:
    virtual ~CEngineSceneGrid();                        // 0x00B93E40 (??1CEngineSceneGrid@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CEngineSceneGrid@@AAE@K@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CEngineSceneGrid::CEngineSceneGrid(unsigned long) { }
