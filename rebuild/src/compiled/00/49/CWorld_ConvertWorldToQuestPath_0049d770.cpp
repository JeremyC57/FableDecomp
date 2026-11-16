// CWorld::ConvertWorldToQuestPath(const CWideString&) -- quest-file path for a world: Data\Levels\ + the world's file name + L".qst" (static)
// Leaf: ConvertWorldToQuestPath
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049D770, 143 bytes, plain `ret` (static __fastcall: hidden result in ecx ->
//   esi, the world path in edx -> edi). PDB name ?ConvertWorldToQuestPath@CWorld@@SI?AVCWideString@@ABV2@@Z.
// Callees: CWideString() 0x0099AED0, SplitPath 0x00997620 (`__fastcall`: path ecx, drive edx, directory/name/extension on
//   the stack), NResourceDirectoryNames::GetLevelDir 0x0041A060 (`__fastcall`, hidden result in ecx; L"Data\\Levels\\"),
//   operator+(const CWideString&, const CWideString&) 0x0099BE70, operator+(const CWideString&, const wchar_t*) 0x0099BF30,
//   CWideString::operator= 0x0099B7D0, copy ctor 0x0099B720, ~CWideString 0x0099B510. String L".qst" 0x01238C40.
// Shape: `CWideString name; SplitPath(world, 0, 0, &name, 0); name = Dir() + name + L".qst"; return name;` -- the static
//   fastcall form has no argument slots, so the three temporaries and name take a 16-byte frame; `return name;` copies
//   into the hidden result (no NRVO). The member is declared in engine/CWorld.h (FABLE_RETAIL_CWORLD_LOAD_ORIGINAL).
// Label correction: catalogued as `GetDefaultVal<CFireEffectCreationSet>` (propagated); the PDB name and the L".qst"
//   literal make it CWorld::ConvertWorldToQuestPath.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SPRINTF
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_TYPED_MEMBERS
#define FABLE_RETAIL_CWORLD_TYPED_SAVE
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CWORLD_LOAD_ORIGINAL
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CDISKFILEWIN32_DEFAULTCTOR
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_WORLDLOAD
#define FABLE_RETAIL_CLEVELLOADER
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_CRITICALDISPLAY
#define FABLE_RETAIL_NRESOURCEDIRECTORYNAMES_LEVELDIR
#include "fable_string.h"
#include "fable_resource_directories.h"
#include "engine/CAFile.h"
#include "engine/CWorld.h"

CWideString __fastcall CWorld::ConvertWorldToQuestPath(const CWideString& world)
{
    CWideString name;
    CAFile::SplitPath(world, 0, 0, &name, 0);
    name = NResourceDirectoryNames::GetLevelDir() + name + L".qst";
    return name;
}