// CWorld::ConvertWorldToStaticMapPath(const CWideString&) const -- static-map path for a world: Data\Levels\ + the world's file name + L"_RT.stb" when the runtime-level-files flag is set, else L".stb" (the three-argument overload inlined with xbox = false)
// Leaf: ?ConvertWorldToStaticMapPath@CWorld_Methods@@QBE?AVCWideString@@ABV2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049DDD0, 232 bytes, `ret 8` (__fastcall: hidden result + the world path on
//   the stack; `this` is never read). PDB name ?ConvertWorldToStaticMapPath@CWorld@@QBE?AVCWideString@@ABV2@@Z.
// Callees: CWideString() 0x0099AED0, SplitPath 0x00997620 (`__fastcall`: path ecx, drive edx, directory/name/extension on
//   the stack), the levels-directory getter 0x0041A410 (`__fastcall`, hidden result in ecx; L"Data\\Levels\\"),
//   operator+(const CWideString&, const CWideString&) 0x0099BE70, operator+(const CWideString&, const wchar_t*) 0x0099BF30,
//   CWideString::operator= 0x0099B7D0, copy ctor 0x0099B720, ~CWideString 0x0099B510. Strings L"_RT.stb" 0x01238BC8,
//   L".stb" 0x01238BAC. Global 0x013B8616 (the runtime-level-files flag, no PDB name; also passed by
//   CMainGameComponent::PostInit 0x00416953 as the `runtime` argument of the three-argument overload).
// Shape: `return ConvertWorldToStaticMapPath(world, false, g_bRuntimeLevelFiles);` with the landed three-argument
//   overload 0x0049D550 defined in the TU and auto-inlined: the flag is read into bl BEFORE SplitPath (it is an argument
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
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70
CWideString __fastcall operator+(const CWideString& left, const wchar16* right);       // 0x0099BF30
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name,
                          CWideString* extension);  // 0x00997620
CWideString __fastcall GetLevelsDirectory_0041a410();       // 0x0041A410 (L"Data\\Levels\\", hidden result in ecx)

extern bool g_bRuntimeLevelFiles;                  // 0x013B8616

struct CWorld_Methods : CWorld {
    CWideString ConvertWorldToStaticMapPath(const CWideString& world, bool xbox, bool runtime) const;   // 0x0049D550
    CWideString ConvertWorldToStaticMapPath(const CWideString& world) const;
};

CWideString CWorld_Methods::ConvertWorldToStaticMapPath(const CWideString& world, bool xbox, bool runtime) const
{
    CWideString name;
    SplitPath(world, 0, 0, &name, 0);
    if (runtime) {
        if (xbox)
            name = GetLevelsDirectory_0041a410() + name + (const wchar16*)L"_RT_XB.stb";
        else
            name = GetLevelsDirectory_0041a410() + name + (const wchar16*)L"_RT.stb";
    }
    else {
        if (xbox)
            name = GetLevelsDirectory_0041a410() + name + (const wchar16*)L"_XB.stb";
        else
            name = GetLevelsDirectory_0041a410() + name + (const wchar16*)L".stb";
    }
    return name;
}

CWideString CWorld_Methods::ConvertWorldToStaticMapPath(const CWideString& world) const
{
    return ConvertWorldToStaticMapPath(world, false, g_bRuntimeLevelFiles);
}