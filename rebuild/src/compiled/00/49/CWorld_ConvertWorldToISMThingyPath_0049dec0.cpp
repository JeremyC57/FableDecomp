// CWorld::ConvertWorldToISMThingyPath(const CWideString&) const -- ISM thing-file path for a world: Data\Levels\ + the world's file name + L"_RT.ism" when the runtime-level-files flag is set, else L".ism" (the three-argument overload inlined with xbox = false)
// Leaf: ?ConvertWorldToISMThingyPath@CWorld_Methods@@QBE?AVCWideString@@ABV2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049DEC0, 232 bytes, `ret 8` (__fastcall: hidden result + the world path on
//   the stack; `this` is never read). PDB name ?ConvertWorldToISMThingyPath@CWorld@@QBE?AVCWideString@@ABV2@@Z.
// Callees: CWideString() 0x0099AED0, SplitPath 0x00997620 (`__fastcall`: path ecx, drive edx, directory/name/extension on
//   the stack), the levels-directory getter 0x0041A410 (`__fastcall`, hidden result in ecx; L"Data\\Levels\\"),
//   operator+(const CWideString&, const CWideString&) 0x0099BE70, operator+(const CWideString&, const wchar_t*) 0x0099BF30,
//   CWideString::operator= 0x0099B7D0, copy ctor 0x0099B720, ~CWideString 0x0099B510. Strings L"_RT.ism" 0x01238C18,
//   L".ism" 0x01238BFC. Global 0x013B8616 (the runtime-level-files flag, no PDB name; also passed by
//   CMainGameComponent::PostInit 0x00416953 as the `runtime` argument of the static-map overload; twin of 0x0049DDD0).
// Shape: `return ConvertWorldToISMThingyPath(world, false, g_bRuntimeLevelFiles);` with the landed three-argument
//   overload 0x0049D6B0 defined in the TU and auto-inlined: the flag is read into bl BEFORE SplitPath (it is an argument
//   value), the xbox arms fold away, the two `name = Dir() + name + L"...";` arms keep their own temporaries (the outer +
//   result lands in the dead world-path slot) and cross-jump into the shared last destructor; `return name;` copies
//   into the hidden result (no NRVO). The engine header is a POD snapshot, so the members sit on a CWorld_Methods view.
#include "engine/CWorld.h"

typedef unsigned short wchar16;                    // wchar_t under the era defaults
class CWideString { public:
    void* PStringData;
    CWideString();                                 // 0x0099AED0
    CWideString(const CWideString& other);         // 0x0099B720
    ~CWideString();                                // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70
CWideString __fastcall operator+(const CWideString& left, const wchar16* right);       // 0x0099BF30
class CAFile {
public:
    static void __fastcall SplitPath(const CWideString&, CWideString*, CWideString*, CWideString*, CWideString*);   // 0x00997620
};  // 0x00997620
CWideString __fastcall GetLevelsDirectory_0041a410();       // 0x0041A410 (L"Data\\Levels\\", hidden result in ecx)

extern bool g_bRuntimeLevelFiles;                  // 0x013B8616

struct CWorld_Methods : CWorld {
    CWideString ConvertWorldToISMThingyPath(const CWideString& world, bool xbox, bool runtime) const;   // 0x0049D6B0
    CWideString ConvertWorldToISMThingyPath(const CWideString& world) const;
};

CWideString CWorld_Methods::ConvertWorldToISMThingyPath(const CWideString& world, bool xbox, bool runtime) const
{
    CWideString name;
    CAFile::SplitPath(world, 0, 0, &name, 0);
    if (runtime) {
        if (xbox)
            name = GetLevelsDirectory_0041a410() + name + (const wchar16*)L"_RT_XB.ism";
        else
            name = GetLevelsDirectory_0041a410() + name + (const wchar16*)L"_RT.ism";
    }
    else {
        if (xbox)
            name = GetLevelsDirectory_0041a410() + name + (const wchar16*)L"_XB.ism";
        else
            name = GetLevelsDirectory_0041a410() + name + (const wchar16*)L".ism";
    }
    return name;
}

CWideString CWorld_Methods::ConvertWorldToISMThingyPath(const CWideString& world) const
{
    return ConvertWorldToISMThingyPath(world, false, g_bRuntimeLevelFiles);
}