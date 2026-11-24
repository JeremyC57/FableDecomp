// NUISystem::CManager::CManager() -- constructor of the front-end UI manager: base, empty counted banks, key/layer maps and action lists, cleared component pointers and flags; selects keyboard input, picks the best texture formats into the bank parameters and loads the graphics bank (GBANK_FRONT_END while the new front-end component is active, else GBANK_MAIN), then resets the meta layer
// Leaf: ??0CManager@NUISystem@@IAE@XZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pInfo->Info are the header CCountedPointer's (same offsets).
// Naming wave 18: PDisplayManager is GPDisplayManager (?GPDisplayManager@@3PAVCDisplayManager@@A); ChatGPT pass 10 audit, raw Ego_r public.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041E3F6, length 508 bytes. Frame-pointer size TU: `#pragma optimize("y", off)` + `("s", on)`.
// Callees: base ctor 0x0042BE7B; key map ctor 0x0042BF67; list ctors 0x0042AB58 / 0x0042AB01; layer map ctor 0x0042BF85; children map
//   ctor 0x0042D28B; SetIsUsingRelativeCoords 0x004299A8 (`__fastcall bool`); SetInput 0x0041DF10; CGraphicDataBankInit ctor 0x00415B80;
//   CDisplayManager texture-format pickers 0x009BE830 / 0x009BE870 / 0x009BE8B0 / 0x009BE6C0 / 0x009BE610 / 0x009BE590 (manifest labels
//   are byte-twin propagations); GFGetSystemManager 0x009A4EC0; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0;
//   the bank manager's OpenGraphicBank 0x009F83D0; SetGraphicsBank 0x0042A9B7; SetMetaLayer 0x0041E1CD.
//   Globals: g_pDisplayManager 0x013B8390, CNewFrontendGameComponent::m_pNewFrontendGameComponent 0x013B871C (read through the header's
//   inline Get(); its Running byte +9 through CGameComponent::IsRunning).
// Layout: retail NUISystem::CManager (vtable 0x01230134) as the landed ~CManager 0x0041E641 and SetGraphicsBank 0x0042A9B7: banks +0x10/+0x18,
//   key map +0x24, input type +0x30, ten inventory pointers +0x34..+0x58, lists +0x5c/+0x60, definition manager +0x64, layer map +0x70,
//   meta layer +0x7c, bastard child +0x80, children map +0x88 ... (engine/CManager.h is the PDB view, 0x10 higher from the banks on; engine/NUISystem_CManager.h has the retail offsets and is the model
//   now, NF226).
// Shape: member initialisers for the scalar members in declaration order (fldz hoisted), the ten inventory pointers cleared in the body;
//   the list ctors take an `allocator()` temporary (STLport's list(const allocator&), folded with _List_base's); the bank manager is a NAMED
//   local in each branch -- it is fetched first and spilled to the frame, which also keeps edi out of the function (an inline
//   `GFGetSystemManager()->...` chain loads it last and pulls edi in for constant 0x10 and the argument-slot pointer).
// Names (NF226, 2026-11-22, q12567): protected, ??0CManager@NUISystem@@IAE@XZ in Ego_r.pdb (0x00438A19) and FableWin.pdb (0x01AE4BC6);
//   engine/NUISystem_CManager.h `protected: CManager();`. Was ??0CManager@NUISystem@@QAE@XZ on a row-local NUISystem::CManager model; its only
//   binder 0x0041E5F2 (GetInstance) moves with it. The model is now engine/NUISystem_CManager.h (retail layout, 0xD0; the HEAD local model and
//   its CManagerBase / CKeyMap / CActionParamList / CLayerMap / CChildrenMap / std::list stand-ins and the local CCharString are gone; the
//   local CDisplayManager / CGraphicBankManager / CSystemManager / CNewFrontendGameComponent models are HEAD's, unchanged):
//   - base: NUISystem::CObservable's constructor, out of line at 0x0042BE7B (Ego_r.pdb / FableWin.pdb ??0CObservable@NUISystem@@QAE@XZ):
//     FABLE_RETAIL_COBSERVABLE_CTOR_DECL_ONLY (new, engine/NUISystem_CObservable.h, included first);
//   - Keys / Layers / BastardChildren: STLport 4.6's no-argument map constructor (FABLE_RETAIL_STLP_MAP), called as this size TU's COMDATs
//     0x0042BF67 / 0x0042BF85 / 0x0042D28B; the two lists: the _List_base(const allocator&) COMDATs 0x0042AB58 / 0x0042AB01;
//   - SetInput (FABLE_RETAIL_CMANAGER_WORLDINIT), SetGraphicsBank (FABLE_RETAIL_CMANAGER_SETGRAPHICSBANK, new) and the virtual
//     SetMetaLayer (?SetMetaLayer@CManager@NUISystem@@UAEXJ@Z, both PDBs; vtable 0x01230134 slot 34), called directly as in retail;
//   - FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA: the counted-pointer destructors this TU instantiates are the 5-byte `jmp ReleaseData` owners.
//   Every pick-any copy with a retail owner equals it (34 copies, relocations bound alike) except ~list<CObserver*> (5-byte jmp v 0x0042AC25's
//   23 bytes; unreferenced; NOTES).
// Names (NF228, 2026-11-22, q12598 / q12599): SetIsUsingRelativeCoords is the public static __fastcall
//   ?SetIsUsingRelativeCoords@CUIScreenUnitConverter@NUISystem@@SIX_N@Z (Ego_r.pdb 0x00438D8A / FableWin.pdb 0x01AE527F; the self leaf of
//   0x004299A8), declared by the new opt-in FABLE_RETAIL_CUISCREENUNITCONVERTER_SETISUSINGRELATIVECOORDS (was a local free-function
//   declaration ?SetIsUsingRelativeCoords@@YIX_N@Z). The front-end component check reads the private static
//   ?m_pNewFrontendGameComponent@CNewFrontendGameComponent@@0PAV1@A through engine/CNewFrontendGameComponent.h's inline Get() and its
//   Running byte (+9) through the new inline FABLE_RETAIL_CGAMECOMPONENT_ISRUNNING (FableWin.pdb ?IsRunning@CGameComponent@@QAE_NXZ);
//   the row-local CNewFrontendGameComponent model (pad + a descriptive `Active` byte) and the ?g_pNewFrontendGameComponent stand-in
//   are gone. The two overlaid headers are included first (an opt-in in a header another header includes needs the row's own include).
// Pragmas: `optimize("s", on)` above the includes (the whole size TU); `optimize("y", off)` only around the constructor and
//   `optimize("y", on)` after it (Fidelity: scaffolding, not retail): the template COMDATs VC7.1 instantiates at the end of the TU are then
//   built without a frame pointer, as their retail owners are (with "y" off they differ from 0x0042A237 / 0x0042B76C / 0x0042B8FC ...).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_COBSERVABLE_CTOR_DECL_ONLY
#define FABLE_RETAIL_CMANAGER_WORLDINIT
#define FABLE_RETAIL_CMANAGER_SETGRAPHICSBANK
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_SETISUSINGRELATIVECOORDS
#define FABLE_RETAIL_CGAMECOMPONENT_ISRUNNING
#include "engine/CGameComponent.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/NUISystem_CObservable.h"
class CGraphicDataBank;
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
#define FABLE_RETAIL_CGRAPHICDATABANKINIT_CTOR_DECL_ONLY
#include "engine/CGraphicDataBankInit.h"
#include "engine/CNewFrontendGameComponent.h"
class CDisplayManager { public:
    bool GetBestCompressedOpaqueTextureFormat(unsigned long bits, CPixelFormat& out);    // 0x009BE830
    bool GetBestCompressedAlphaTextureFormat(unsigned long bits, CPixelFormat& out);   // 0x009BE870
    bool GetBestCompressedBooleanAlphaTextureFormat(unsigned long bits, CPixelFormat& out);   // 0x009BE8B0
    bool GetBestAlphaTextureFormat(unsigned long bits, CPixelFormat& out);              // 0x009BE6C0
    bool GetBestOpaqueTextureFormat(unsigned long bits, CPixelFormat& out, bool flag);  // 0x009BE610
    bool GetBestSignedTextureFormat(unsigned long bits, CPixelFormat& out);             // 0x009BE590
};
class CGraphicBankManager { public:
    CCountedPointer<CGraphicDataBank> OpenGraphicBank(const CCharString& name, const CGraphicDataBankInit& init, bool a, bool b);   // 0x009F83D0 (FableWin.pdb / Ego_r.pdb)
};
class CSystemManager { public:
    unsigned char        _pad00[0x6c];
    CGraphicBankManager* GraphicBankManager;    // +0x6c
};
extern CDisplayManager* GPDisplayManager;      // 0x013B8390
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0

#pragma optimize("y", off)
namespace NUISystem {
CManager::CManager()
    : m_pDefinitionManager(0), m_bHasErrorMessage(false), m_pErrorMessage(0), Restoring(false), m_fLastRenderTime(0.0),
      PMouseCursor(0), PDropped(0), DescriptionScrollingOffset(-1.0f), DescriptionScrollingToMax(false), MouseDisabled(false),
      AssignmentItemsExpressionsSwapping(false), AssignmentSpellsSwapping(false)
{
    PTCHeroExperience = 0;
    PAbilitiesInventory = 0;
    PWeaponsInventory = 0;
    PMapInventory = 0;
    PQuestInventory = 0;
    PExperienceInventory = 0;
    PInventory = 0;
    PInventoryClothing = 0;
    PInventoryTrade = 0;
    PPlayer = 0;
    CUIScreenUnitConverter::SetIsUsingRelativeCoords(true);
    SetInput(UI_INPUT_KEYBOARD);
    PWeaponsInventory = 0;
    CGraphicDataBankInit params;
    GPDisplayManager->GetBestCompressedOpaqueTextureFormat(0x10, params.NonAlphaFormat);
    GPDisplayManager->GetBestCompressedAlphaTextureFormat(0x10, params.AlphaFormat);
    GPDisplayManager->GetBestCompressedBooleanAlphaTextureFormat(0x10, params.BooleanAlphaFormat);
    GPDisplayManager->GetBestAlphaTextureFormat(0x20, params.UncompressedAlphaFormat);
    GPDisplayManager->GetBestOpaqueTextureFormat(0x20, params.UncompressedNonAlphaFormat, false);
    GPDisplayManager->GetBestSignedTextureFormat(0x10, params.SignedFormat);
    params.MaxGraphicWidth = (unsigned long)-1;
    params.MaxGraphicHeight = (unsigned long)-1;
    params.GenerateMipmaps = false;
    params.AllowDither = false;
    if (CNewFrontendGameComponent::Get() && CNewFrontendGameComponent::Get()->IsRunning()) {
        CGraphicBankManager* manager = GFGetSystemManager()->GraphicBankManager;
        SetGraphicsBank(manager->OpenGraphicBank(CCharString("GBANK_FRONT_END"), params, true, true));
    } else {
        CGraphicBankManager* manager = GFGetSystemManager()->GraphicBankManager;
        SetGraphicsBank(manager->OpenGraphicBank(CCharString("GBANK_MAIN"), params, true, true));
    }
    SetMetaLayer(0);
}

}
#pragma optimize("y", on)
