// CXMVPlayer::GlobalRegisterPlayer() -- register the XMV movie-player code section ("XMV" / "Xbox Movie Player Code") with the code-section manager and remember its handle
// Leaf: ?GlobalRegisterPlayer@CXMVPlayer@@SIXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BD020, 107 bytes (+ int3 padding), plain `ret`.
// Label: catalogued as `GlobalRegisterPlayer` (module _global); FableWin.pdb `?GlobalRegisterPlayer@CXMVPlayer@@SIXXZ`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("XMV" @ 0x0129B394, "Xbox Movie Player Code"
// @ 0x0129B398), CCodeSectionManager::RegisterSection(CCharString, CCharString, bool, bool) @ 0x009F05A0 (FableWin
// `QAEKVCCharString@@0_N1@Z`) (+ the inlined singleton Get below).
// Identity: FableWin.pdb names the three statics CXMVPlayer::StaticLoadXMVCode / StaticReleaseXMVCode /
// GlobalRegisterPlayer (`SIXXZ`: static __fastcall void()); RegisterSection("XMV", "Xbox Movie Player Code") @ 0x009BD020
// stores the handle these two load and unload.
// Singleton: CTBaseSingleton<CCodeSectionManager>::Get() (FableWin `SIAAVCCodeSectionManager@@XZ`) is inlined: if the
// static CCountedPointer @ 0x013CAA68 is null, Reset(new CCodeSectionManager) (operator new 0x00BFEA1A, 0x2C bytes,
// ctor 0x009F0320, Reset 0x00413590 -- the landed CCodeSectionManagerSingleton::Get 0x00414CE0 is the same body);
// the new-expression's null test gives `xor eax,eax` on the failed path. Get must be `__forceinline` (plain inline
// stays an out-of-line call in the load/release pair).
// Globals: 0x013CA7E0 section handle, 0x013CA7E4 load count, 0x013CA7E8 registered flag (no PDB names: descriptive
// static members).
// Shape: the inlined Get() result (the manager) is loaded into esi before the arguments; the two by-value CCharString
// arguments are built in their stack slots right to left (`push ecx ; mov ecx,esp ; push -1 ; push lit`) after the
// two bools (false, true); the handle and the flag are stored after the call.

#include "fable_compiler.h"
class CCharString {
public:
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
private:
    void* PStringData;
};

#include "engine/CCountedPointer.h"


class CCodeSectionManager {                             // retail 0x2C bytes (vtable + three STLport maps + flag)
public:
    CCodeSectionManager();                              // 0x009F0320
    unsigned long RegisterSection(CCharString shortName, CCharString longName, bool loadNow, bool canUnload);   // 0x009F05A0
    void LoadSection(unsigned long handle);             // 0x009F02C0
    void UnloadSection(unsigned long handle);           // 0x009F02F0
private:
    unsigned long _body[0x2C / 4];
};

template <class T> class CTBaseSingleton {
public:
    static FABLE_FORCEINLINE T& __fastcall Get()
    {
        if (Instance.IsNull())
            Instance.Reset(new T);
        return *Instance;
    }
    static CCountedPointer<T> Instance;                 // 0x013CAA68 for CCodeSectionManager
};

class CXMVPlayer {
public:
    static void __fastcall StaticLoadXMVCode();
    static void __fastcall StaticReleaseXMVCode();
    static void __fastcall GlobalRegisterPlayer();
    static unsigned long XMVCodeSection;                // 0x013CA7E0 (handle from RegisterSection)
    static long          XMVCodeLoadCount;              // 0x013CA7E4
    static bool          PlayerRegistered;              // 0x013CA7E8
};

unsigned long CXMVPlayer::XMVCodeSection;
bool CXMVPlayer::PlayerRegistered;

void __fastcall CXMVPlayer::GlobalRegisterPlayer()
{
    XMVCodeSection = CTBaseSingleton<CCodeSectionManager>::Get().RegisterSection("XMV", "Xbox Movie Player Code", false, true);
    PlayerRegistered = true;
}