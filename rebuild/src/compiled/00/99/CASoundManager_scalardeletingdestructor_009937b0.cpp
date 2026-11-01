// CASoundManager::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCASoundManager@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009937B0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c668): slot 0 of vtable 0x0129A574 (RTTI CASoundManager), calls ??1CASoundManager@@UAE@XZ 0x00991880.
// Callees: CASoundManager::~CASoundManager 0x00991880 (??1CASoundManager@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CASoundManager_scalar_deleting_destructor
//   calling CASoundManager_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC
void* __cdecl operator new(unsigned int size);       // 0x00BFEA4A

class CASoundManager {
public:
    CASoundManager() { }
    virtual ~CASoundManager();                          // 0x00991880 (??1CASoundManager@@UAE@XZ)
};

void* MakeOne_009937b0()                                 // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new CASoundManager;
}
