// CWorld::ConvertWorldToLevelWADPath(const CWideString&) const -- level WAD path for a world: Data\Levels\ + the world's file name + L".wad"
// Leaf: ConvertWorldToLevelWADPath
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049D610, 145 bytes, `ret 8` (__fastcall: hidden result + the world path on
//   the stack; `this` is never read). PDB name ?ConvertWorldToLevelWADPath@CWorld@@QBE?AVCWideString@@ABV2@@Z.
// Callees: CWideString() 0x0099AED0, SplitPath 0x00997620 (`__fastcall`: path ecx, drive edx, directory/name/extension on
//   the stack), NResourceDirectoryNames::GetLevelDir 0x0041A060 (`__fastcall`, hidden result in ecx; L"Data\\Levels\\", the twin of
//   0x0041A410 used by the static-map/ISM paths), operator+(const CWideString&, const CWideString&) 0x0099BE70,
//   operator+(const CWideString&, const wchar_t*) 0x0099BF30, CWideString::operator= 0x0099B7D0, copy ctor 0x0099B720,
//   ~CWideString 0x0099B510. String L".wad" 0x01238BF0.
// Shape: as the landed ConvertWorldToStaticMapPath 0x0049D550: `CWideString name; SplitPath(world, 0, 0, &name, 0);
//   name = Dir() + name + L".wad"; return name;` -- the extension pushed first, &name for the inner +, the outer +
//   result in the dead world-path slot, the temporaries destroyed in reverse; `return name;` copies into the hidden
//   result (no NRVO). The member is declared in engine/CWorld.h (FABLE_RETAIL_CWORLD_LOAD_ORIGINAL).
// Label correction: catalogued as `GetDefaultVal<CFireEffectCreationSet>` (propagated); the PDB name and the L".wad"
//   literal make it CWorld::ConvertWorldToLevelWADPath.
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

CWideString CWorld::ConvertWorldToLevelWADPath(const CWideString& world) const
{
    CWideString name;
    CAFile::SplitPath(world, 0, 0, &name, 0);
    name = NResourceDirectoryNames::GetLevelDir() + name + L".wad";
    return name;
}