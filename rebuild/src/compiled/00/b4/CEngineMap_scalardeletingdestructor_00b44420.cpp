// CEngineMap::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCEngineMap@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B44420, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c668): calls ~CEngineMap 0x00B42A90 then operator delete on bit 0; FableWin.pdb ??_GCEngineMap@@QAEPAXI@Z (non-virtual: CEngineMap has no vtable).
// Callees: CEngineMap::~CEngineMap 0x00B42A90 (??1CEngineMap@@QAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00b42a90; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CEngineMap {
public:
    ~CEngineMap();                                      // 0x00B42A90 (??1CEngineMap@@QAE@XZ)
};

void Kill_00b44420(CEngineMap* object)                     // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}
