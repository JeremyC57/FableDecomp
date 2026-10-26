// CCodeSectionManagerSingleton::Get() -- lazily create the one CCodeSectionManager (0x2C bytes) into its static counted pointer and return the object
// Leaf: ?Get@CCodeSectionManagerSingleton@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00414CE0, 66 bytes. Callees: operator new 0x00BFEA1A,
// CCodeSectionManager::CCodeSectionManager 0x009F0320 (stores RTTI vtable 0x0129C764), the
// counted-pointer Reset 0x00413590 (landed as CCountedPointer<CIPhysicsFacesSegmentBase const>::Reset,
// a propagated label); the static counted pointer sits at 0x013CAA68 (object +0, count block +4).
//
// Retail bytes:  mov eax,[g.object] ; test eax,eax ; jne R ; push 0x2c ; call operator new ;
//   add esp,4 ; test eax,eax ; je Z ; mov ecx,eax ; call CCodeSectionManager ; push eax ;
//   mov ecx,&g ; call Reset ; mov eax,[g.object] ; ret ; Z: xor eax,eax ; push eax ; mov ecx,&g ;
//   call Reset ; mov eax,[g.object] ; ret ; R: ret
// Same singleton shape as CFileInstallerSingleton::Get 0x00404440: `g = new T;` with the
// new-expression's null test duplicating the Reset + reload tail. The manifest's leaf `Get` is
// right; the class is established from the constructor's vtable (CCodeSectionManager) and the
// landed CQuestManager::DeactivateQuest 0x004B3A20, which inlines the same create-on-first-use.
// The only caller is 0x00CB5CC8, inside the manifest's AddScript 0x00CB5C90.
class CCodeSectionManager { public:
    unsigned char m_body[0x2C];
    CCodeSectionManager();                        // 0x009F0320
};

struct SCountBlock;

struct CCountedCodeSectionManagerPointer {
    CCodeSectionManager* m_pObject;               // +0
    SCountBlock* m_pCount;                        // +4
    void Reset(CCodeSectionManager* object);      // 0x00413590
    CCountedCodeSectionManagerPointer& operator=(CCodeSectionManager* object) { Reset(object); return *this; }
};

extern CCountedCodeSectionManagerPointer g_CodeSectionManagerSingleton;   // 0x013CAA68

struct CCodeSectionManagerSingleton {
    static CCodeSectionManager* __fastcall Get();
};

CCodeSectionManager* __fastcall CCodeSectionManagerSingleton::Get()
{
    if (g_CodeSectionManagerSingleton.m_pObject == 0)
        g_CodeSectionManagerSingleton = new CCodeSectionManager;
    return g_CodeSectionManagerSingleton.m_pObject;
}