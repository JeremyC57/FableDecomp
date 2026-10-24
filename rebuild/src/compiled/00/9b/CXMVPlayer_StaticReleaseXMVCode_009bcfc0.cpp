// CXMVPlayer::StaticReleaseXMVCode() -- drop one request for the XMV movie-player code; unload the section when none remain, clamping the count at zero
// Leaf: ?StaticReleaseXMVCode@CXMVPlayer@@SIXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BCFC0, 90 bytes (+ int3 padding), plain `ret`.
// Label: catalogued as `StaticReleaseXMVCode` (module _global); FableWin.pdb `?StaticReleaseXMVCode@CXMVPlayer@@SIXXZ`.
// Callees: CCodeSectionManager::UnloadSection(unsigned long) @ 0x009F02F0 (+ the inlined singleton Get below).
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
// Shape: `if (--count == 0) Get().UnloadSection(handle);` (`dec dword ptr ; jne`), then a signed clamp
// `if (count < 0) count = 0;` re-reading the count (`jge` over an immediate store).

class CCharString {
public:
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
private:
    void* PStringData;
};

class CCPPointerInfo;

template <class T> class CCountedPointer {             // intrusive counted pointer: object + count block
public:
    bool IsNull() const { return Data == 0; }
    T& operator*() const { return *Data; }
    void Reset(T* data);                                // 0x00413590 (folded COMDAT)
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
};

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
    static __forceinline T& __fastcall Get()
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

void __fastcall CXMVPlayer::StaticReleaseXMVCode()
{
    if (--XMVCodeLoadCount == 0)
        CTBaseSingleton<CCodeSectionManager>::Get().UnloadSection(XMVCodeSection);
    if (XMVCodeLoadCount < 0)
        XMVCodeLoadCount = 0;
}