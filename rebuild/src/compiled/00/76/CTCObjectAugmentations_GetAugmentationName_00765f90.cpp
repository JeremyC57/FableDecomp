// CTCObjectAugmentations::GetAugmentationName(long defIndex) -- the display name of an augmentation def (its Name text from the game text bank), or "Missing augmentation name!" when the def has no text
// Leaf: ?GetAugmentationName@CTCObjectAugmentations@@SI?AVCWideString@@J@Z
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00765F90, 186 bytes (+int3 pad to 192), plain `ret` (static __fastcall:
//   the hidden CWideString result in ecx, the def index in edx; eax = result).
// Identity: Ego_r.pdb ?GetAugmentationName@CTCObjectAugmentations@@SI?AVCWideString@@J@Z. Callers 0x005C6E55,
//   0x005C736F, 0x005C939D (inventory UI).
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0, CDefinitionManager::GetDef<CThingBaseDef> @ 0x0042AF3C,
//   CDefClassBase::GetSubDef<CAugmentationDef> @ 0x0042B248, NGameText::CDataBank::GetPGroupEntry(unsigned long, bool)
//   const @ 0x009C9070 (the bank at CMainGameComponent::Get() 0x013B86A0 +0x14), the group entry's GetText() @ 0x009C7A40,
//   CWideString::CWideString(const CWideString&) @ 0x0099B720 on MISSING_AUGMENTATION_TEXT (was g_CWideString_013bad98) (L"Missing augmentation name!",
//   dynamic initializer 0x0121EE90 / atexit 0x01229AD0).
// Shape: the def lookups as GetDamageMultiplierFromAugmentationSet 0x007661C0 (GetDef's arguments pushed before Get(),
//   each def-pointer local zeroed after its address is pushed, the thing def read once into edi); `if (Name > 0)`
//   (unsigned, `jbe`) reads the text bank into a block local BEFORE the argument pushes (`mov edx,[g] ; mov ecx,
//   [edx+14h]`; the Name re-read inline, not a named local, keeps it in eax for the push) and returns
//   GetPGroupEntry(Name, true)->GetText() straight into the result (the entry's CIVCountedPointer temporary takes the
//   dead augmentation slot and is released with `Object = 0` through the landed CItem 0x005BD8A0 DecRef/GetRefCount
//   helpers), else copies the missing-name string; both exits share the augmentation release (no null test: it was
//   dereferenced) and the thing def release (null-tested).
// Layout: CAugmentationDef Name (text index) +0x28 (as the landed 0x00766140 model); def reference count +4, release
//   slot 1; CGroupEntry: vptr, IVRefCount +4 (second base), Delete slot 1.
// Name fix (agentNF10 2026-11-11): moved off the local CWideString / CDefClassBase / CDefPointer / CThingBaseDef / CDefinitionManager /
// CIVCountedPointer / NGameText / CMainGameComponent / CTCObjectAugmentations stand-ins (and the same-TU GetDef / GetSubDef stand-in
// bodies) onto the shared headers. 0x0042AF3C binds ??$GetDef@VCThingBaseDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z
// (was the non-template ?GetDef@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z) and 0x0042B248 binds
// GetSubDef<CAugmentationDef> (was ?GetSubDef@CThingBaseDef@@QBE_NPAV?$CDefPointer@$$CBVCAugmentationDef@@@@@Z). The text bank comes
// through the inline CMainGameComponent::GetTextBank() (+0x14). 0x013BAD98 is `anonymous namespace'::MISSING_AUGMENTATION_TEXT
// (agentFAM09: Ego_r.pdb + FableWin.pdb dynamic-initializer names; was g_CWideString_013bad98).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CAUGMENTATIONDEF
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_INNER
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_string.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAugmentationDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CTCObjectAugmentations.h"

namespace { extern CWideString MISSING_AUGMENTATION_TEXT; }       // 0x013BAD98 L"Missing augmentation name!"

CWideString __fastcall CTCObjectAugmentations::GetAugmentationName(long augmentation_def_index)
{
    CDefPointer<const CThingBaseDef> def;
    CGameDefinitionManager::Get().GetDef(augmentation_def_index, &def);
    CDefPointer<const CAugmentationDef> augmentation;
    def->GetSubDef(&augmentation);
    if (augmentation->Name > 0)
        return CMainGameComponent::Get()->GetTextBank().GetPTextEntry(augmentation->Name, true)->GetText();
    return MISSING_AUGMENTATION_TEXT;
}
