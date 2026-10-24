// _global::GFUninitialise() -- engine shutdown: tear the global services down in reverse start-up order, ending with a tail call into the engine-side manager's Uninitialise
// Leaf: GFUninitialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00401B80, 64 bytes. Callees: CInitBaseClass::Uninitialise
// 0x00412F60 (on global 0x013B83D0), file-installer singleton Release 0x00406DF0,
// CBankFileManager::Close 0x009A7990 (global 0x013CA79C), GFGetSystemManager 0x009A4EC0 ->
// CSystemManager::Uninitialise 0x009A60D0, CThreadedFileManager::Uninitialise 0x0098E0D0 (global
// 0x013BC870), UninitializeStreamingFontTextureCache 0x009A1660, the static-instance accessor
// 0x00404A80 (`mov eax,0x013B7CD8 ; ret`) -> its Uninitialise 0x004051C0 (tail `jmp`).
//
// Retail bytes:  mov ecx,g_init ; call ; call Release ; mov ecx,g_banks ; call Close ;
//   call GFGetSystemManager ; mov ecx,eax ; call Uninitialise ; mov ecx,g_threadedFiles ; call ;
//   call UninitializeStreamingFontTextureCache ; call Get ; mov ecx,eax ; jmp Uninitialise
// Every callee is a plain call on a global object or an accessor result; the last void member
// call is sibling-call optimised to `jmp`. The manifest labels 0x00404A80 as
// CEngineInternalPrimitiveBase::AddChildPrimitive (propagated): its body only returns the
// address of the static object at 0x013B7CD8. An earlier lane landed this body as
// Global_GFUninitialise_00401b80.cpp (fable_gfuninitialise.h); this TU is self-contained.
struct CInitBaseClass {
    void Uninitialise();                          // 0x00412F60
};

struct CBankFileManager {
    void Close();                                 // 0x009A7990
};

struct CSystemManager {
    void Uninitialise();                          // 0x009A60D0
};

struct CThreadedFileManager {
    void Uninitialise();                          // 0x0098E0D0
};

struct CEnginePrimitiveManager {
    void Uninitialise();                          // 0x004051C0
};

extern CInitBaseClass g_InitBase;                 // 0x013B83D0
extern CBankFileManager g_BankFileManager;        // 0x013CA79C
extern CThreadedFileManager g_ThreadedFileManager;   // 0x013BC870

void __fastcall FileInstallerSingletonRelease();              // 0x00406DF0
CSystemManager* __fastcall GFGetSystemManager();             // 0x009A4EC0
void __fastcall UninitializeStreamingFontTextureCache();     // 0x009A1660
CEnginePrimitiveManager* __fastcall GetEnginePrimitiveManager();   // 0x00404A80

void __cdecl GFUninitialise()
{
    g_InitBase.Uninitialise();
    FileInstallerSingletonRelease();
    g_BankFileManager.Close();
    GFGetSystemManager()->Uninitialise();
    g_ThreadedFileManager.Uninitialise();
    UninitializeStreamingFontTextureCache();
    GetEnginePrimitiveManager()->Uninitialise();
}