// CTBaseSingleton<CConsole>::Get() -- create the console singleton on first use, return it
// Leaf: ?Get@?$CTBaseSingleton@VCConsole@@@@SIAAVCConsole@@XZ
// Retail: Fable.exe @ 0x00414C90, 69 bytes, `ret` (static __fastcall, no arguments); many callers (0x00414C49, 0x00414C78,
// 0x004176B0, ...); Ego_r.pdb ?Get@?$CTBaseSingleton@VCConsole@@@@SIAAVCConsole@@XZ.
// Catalog label CTBaseSingleton<CCodeSectionManager>::Get is wrong: the object built is 0x9c bytes by the constructor
// 0x009ECD80, whose vtable 0x0129C600 carries RTTI .?AVCConsole@@ (bases CConsole, CTBaseSingleton<CConsole>).
// Body: if (!Instance.Data) Instance.Reset(new CConsole) (operator new 0x00BFEA1A, null-checked; CCountedPointer::Reset
// 0x00413520 on the static at 0x013CAA40); return *Instance.Data (both paths duplicated by the compiler).
// Shape: /O2 (16-aligned entry).
#include <new>
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    void Reset(T* data);                                 // 0x00413520 for CConsole
};
template <class T> class CTBaseSingleton {
public:
    virtual ~CTBaseSingleton() {}
    static CCountedPointer<T> Instance;                  // 0x013CAA40 for CConsole
    static T& __fastcall Get()
    {
        if (!Instance.Data)
            Instance.Reset(new T);
        return *Instance.Data;
    }
};
class CConsole : public CTBaseSingleton<CConsole> {
public:
    CConsole();                                          // 0x009ECD80
    unsigned char _body[0x9c - 4];
};
template CConsole& __fastcall CTBaseSingleton<CConsole>::Get();