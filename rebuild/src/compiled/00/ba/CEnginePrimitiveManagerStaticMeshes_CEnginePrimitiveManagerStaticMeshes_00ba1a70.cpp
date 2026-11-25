// CEnginePrimitiveManagerStaticMeshes::CEnginePrimitiveManagerStaticMeshes(CArray<CEngineScene*>& scenes) -- the static-meshes base, this class's vtable, AddEnableConsoleVariable("EnableStaticMeshes") through a CCharString temporary, StippleAlphaSolidFactor (+0x314) set to 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BA1A70, length 70 bytes.
// Callees: CEnginePrimitiveManagerStaticMeshesBase::CEnginePrimitiveManagerStaticMeshesBase @ 0x00B9ECE0 (the type), CCharString::CCharString(const char*, long)
// @ 0x0099EBF0 (the literal @ 0x012A5148, length -1), the name setter @ 0x00BACD80 (an empty `ret 4` in retail; the manifest label "$E2" is propagated),
// CCharString::~CCharString @ 0x0099EAE0 (the manifest label is a propagated _Cons_val symbol). Vtable 0x012A5114 = CEnginePrimitiveManagerStaticMeshes
// (rtti-vtables.tsv). Shape: the temporary lives in the caller's frame (`lea ecx,[esp+10h]` after the two pushes, `lea ecx,[esp+8]` after), the vtable store is
// scheduled between the pushes and the string constructor, the +0x314 store is an immediate 1.
// Leaf: ??0CEnginePrimitiveManagerStaticMeshes@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_PROTECTED_TAIL
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CPIXELSHADER
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHES_CTOR
#include "engine/CArray.h"
#include "fable_string.h"
#include "engine/CEnginePrimitiveManagerStaticMeshes.h"

CEnginePrimitiveManagerStaticMeshes::CEnginePrimitiveManagerStaticMeshes(CArray<CEngineScene*>& scenes)
    : CEnginePrimitiveManagerStaticMeshesBase(scenes)
{
    {
        CCharString name("EnableStaticMeshes"); // a named local: its address goes to the setter (a bare temporary would push the constructor's eax)
        AddEnableConsoleVariable(name);     // ?AddEnableConsoleVariable@CEnginePrimitiveManager@@QAEXABVCCharString@@@Z 0x00BACD80
    }                                              // destroyed before the count store (the store follows the destructor call)
    StippleAlphaSolidFactor = 1;                   // +0x314
}
