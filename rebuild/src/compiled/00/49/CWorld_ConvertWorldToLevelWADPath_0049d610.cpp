// CWorld::ConvertWorldToLevelWADPath(const CWideString&) const -- level WAD path for a world: Data\Levels\ + the world's file name + L".wad"
// Leaf: ConvertWorldToLevelWADPath
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049D610, 145 bytes, `ret 8` (__fastcall: hidden result + the world path on
//   the stack; `this` is never read). PDB name ?ConvertWorldToLevelWADPath@CWorld@@QBE?AVCWideString@@ABV2@@Z.
// Callees: CWideString() 0x0099AED0, SplitPath 0x00997620 (`__fastcall`: path ecx, drive edx, directory/name/extension on
//   the stack), the levels-directory getter 0x0041A060 (`__fastcall`, hidden result in ecx; L"Data\\Levels\\", the twin of
//   0x0041A410 used by the static-map/ISM paths), operator+(const CWideString&, const CWideString&) 0x0099BE70,
//   operator+(const CWideString&, const wchar_t*) 0x0099BF30, CWideString::operator= 0x0099B7D0, copy ctor 0x0099B720,
//   ~CWideString 0x0099B510. String L".wad" 0x01238BF0.
// Shape: as the landed ConvertWorldToStaticMapPath 0x0049D550: `CWideString name; SplitPath(world, 0, 0, &name, 0);
//   name = Dir() + name + L".wad"; return name;` -- the extension pushed first, &name for the inner +, the outer +
//   result in the dead world-path slot, the temporaries destroyed in reverse; `return name;` copies into the hidden
//   result (no NRVO). The engine header is a POD snapshot, so the member sits on a CWorld_Methods view.
// Label correction: catalogued as `GetDefaultVal<CFireEffectCreationSet>` (propagated); the PDB name and the L".wad"
//   literal make it CWorld::ConvertWorldToLevelWADPath.
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
CWideString __fastcall GetLevelsDirectory_0041a060();       // 0x0041A060 (L"Data\\Levels\\", hidden result in ecx)

struct CWorld_Methods : CWorld {
    CWideString ConvertWorldToLevelWADPath(const CWideString& world) const;
};

CWideString CWorld_Methods::ConvertWorldToLevelWADPath(const CWideString& world) const
{
    CWideString name;
    CAFile::SplitPath(world, 0, 0, &name, 0);
    name = GetLevelsDirectory_0041a060() + name + (const wchar16*)L".wad";
    return name;
}