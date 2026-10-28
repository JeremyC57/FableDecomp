// CWorld::ConvertWorldToQuestPath(const CWideString&) -- quest-file path for a world: Data\Levels\ + the world's file name + L".qst" (static)
// Leaf: ConvertWorldToQuestPath
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049D770, 143 bytes, plain `ret` (static __fastcall: hidden result in ecx ->
//   esi, the world path in edx -> edi). PDB name ?ConvertWorldToQuestPath@CWorld@@SI?AVCWideString@@ABV2@@Z.
// Callees: CWideString() 0x0099AED0, SplitPath 0x00997620 (`__fastcall`: path ecx, drive edx, directory/name/extension on
//   the stack), the levels-directory getter 0x0041A060 (`__fastcall`, hidden result in ecx; L"Data\\Levels\\"),
//   operator+(const CWideString&, const CWideString&) 0x0099BE70, operator+(const CWideString&, const wchar_t*) 0x0099BF30,
//   CWideString::operator= 0x0099B7D0, copy ctor 0x0099B720, ~CWideString 0x0099B510. String L".qst" 0x01238C40.
// Shape: `CWideString name; SplitPath(world, 0, 0, &name, 0); name = Dir() + name + L".qst"; return name;` -- the static
//   fastcall form has no argument slots, so the three temporaries and name take a 16-byte frame; `return name;` copies
//   into the hidden result (no NRVO). The engine header is a POD snapshot, so the member sits on a CWorld_Methods view.
// Label correction: catalogued as `GetDefaultVal<CFireEffectCreationSet>` (propagated); the PDB name and the L".qst"
//   literal make it CWorld::ConvertWorldToQuestPath.
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
    static CWideString __fastcall ConvertWorldToQuestPath(const CWideString& world);
};

CWideString __fastcall CWorld_Methods::ConvertWorldToQuestPath(const CWideString& world)
{
    CWideString name;
    CAFile::SplitPath(world, 0, 0, &name, 0);
    name = GetLevelsDirectory_0041a060() + name + (const wchar16*)L".qst";
    return name;
}