// CPlayerGui::~CPlayerGui() -- release the cached PLAYER_GUI_PC definition, then destroy the player GUI's members (hero information screens manager, screen message / script text lists, primitive handles, stat bars, illustrators, debug texture, button vectors, the draw-component and font counted pointers, the subtitle renderer)
// Leaf: ??1CPlayerGui@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043A570, 2770 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven, was the placeholder "NHeroInformationScreens_Destructor"): it stores the CPlayerGui vtable 0x0123177C first
//   (rtti-vtables.tsv: CPlayerGui slot 0 = the scalar deleting dtor 0x00444F20), and its sole caller is that ??_G (0x00444F23: call,
//   then `test [esp+8],1` / operator delete); Ego_r.pdb / FableWin.pdb ??1CPlayerGui@@UAE@XZ + ??_GCPlayerGui@@UAEPAXI@Z. It ends
//   with the base CTBaseSingletonNoDefaultAlloc<CPlayerGui> vtable 0x01231720 (Ego_r ??1?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@UAE@XZ,
//   inline, empty).
// Body: the user part is the static PGuiDef's release (0x013B878C; the def pointer every CPlayerGui method loads on first use), written
//   as CDefPointer::Release() + `Object = 0` (Reset(NULL)'s inline body: Release's own store folds into the unconditional one at the
//   join, 0x0043A59D); everything else is the implicit member destruction in reverse declaration order, from the members' Ego_r.pdb
//   types at the retail offsets (engine/CPlayerGui.h opt-in FABLE_RETAIL_CPLAYERGUI_MEMBERS, chunk 853).
// Callees: _List_base<NPlayerGui::CScreenMessage>::clear 0x0043FE40 / <CScriptScreenText>::clear 0x0043FDF0 (out of line; the head
//   node then freed), free 0x00BFEA14, operator delete 0x00BFE9BC, CTexture::Uninitialise 0x009F9F70, CBaseClass::~CBaseClass
//   0x0099A2E0 (end of the inline ~CEnginePrimitiveHandle), ~CSubtitleRenderer 0x00443E20; virtual: CDefPointeeBase::Delete (slot 1),
//   CEnginePersistentPrimitiveBase::NotifyReleased (slot 3), the handle elements' scalar deleting dtor (slot 0, flag 0).
// Header opt-ins (chunk 853, hdr/README.md): FABLE_RETAIL_CPLAYERGUI_MEMBERS (CPlayerGui.h), FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
//   (the inline ~CManager frees three vectors), FABLE_RETAIL_CSUBTITLERENDERER_DTOR (the vptr as the virtual destructor),
//   FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF (the count read through a reference, `add eax,4`, as the static's own atexit destructor
//   0x01228B70 does; the in-place form gives `dec [ecx+4]`).
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CSUBTITLERENDERER_DTOR
#include "stlport/fable_containers.h"
#include "engine/CPlayerGui.h"

// Ego_r.pdb ??1?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@UAE@XZ: inline in retail (only its vtable store remains, 0x0043B038).
template <class T> CTBaseSingletonNoDefaultAlloc<T>::~CTBaseSingletonNoDefaultAlloc() { }

CPlayerGui::~CPlayerGui()
{
    PGuiDef.Release();
    PGuiDef.Object = 0;
}