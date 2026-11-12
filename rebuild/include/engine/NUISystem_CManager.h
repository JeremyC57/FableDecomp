#ifndef FABLE_ENGINE_NUISYSTEM_CMANAGER_H
#define FABLE_ENGINE_NUISYSTEM_CMANAGER_H
#include "engine/NUISystem_CObservable.h"
#include "stlport/fable_map.h"
class CGraphicDataBank;class CMeshDataBank;class CInputManager;class CTCHeroExperience;
class CTCInventoryAbilities;class CTCInventoryWeapons;class CTCInventoryMap;class CTCInventoryQuests;
class CTCInventoryExperience;class CTCInventory;class CTCInventoryClothing;class CTCInventoryTrade;
class CThing;class CDefinitionManager;class CWideString;
#ifdef FABLE_RETAIL_CMANAGER
class C2DCoordI;class CRGBFloatColour;
#endif

enum EEngine2DLayer { ENGINE_2D_RANGE_SIZE=16,ENGINE_2D_RANGE_0_START=0,ENGINE_2D_RANGE_1_START=16,ENGINE_2D_RANGE_2_START=32,ENGINE_2D_RANGE_3_START=48,ENGINE_2D_RANGE_4_START=64,ENGINE_2D_LAYER_M3=64,ENGINE_2D_LAYER_M2=65,ENGINE_2D_LAYER_M1=66,ENGINE_2D_LAYER_0=67,ENGINE_2D_LAYER_1=68,ENGINE_2D_LAYER_2=69,ENGINE_2D_LAYER_3=70,ENGINE_2D_LAYER_4=71,ENGINE_2D_MESH_LAYER=72,ENGINE_2D_LAYER_5=73,ENGINE_2D_LAYER_6=74,ENGINE_2D_LAYER_7=75,ENGINE_2D_LAYER_8=76,ENGINE_2D_LAYER_9=77,ENGINE_2D_LAYER_10=78,ENGINE_2D_LAYER_11=79,ENGINE_2D_LAYER_12=80,ENGINE_2D_LAYER_13=81,ENGINE_2D_LAYER_SPELLS=82,ENGINE_NUM_2D_LAYERS=83,ENGINE_2D_NULL_LAYER=-1,ENGINE_2D_LAYER_FIRST=0 };
namespace NUISystem {
class CList;class CText;class CComponent;class CAction;class CFrontEndData;class CUIMiscThingsDef;class CUIIconsDef;
// Original CObservable base and complete PDB fields, with native STLport storage.
class CManager : public CObservable {
protected: CManager(); ~CManager();
public:
enum EChange { UI_CHANGE_ACCEPT=0,UI_CHANGE_REJECT=1 };
enum EInputType { UI_INPUT_KEYBOARD=0,UI_INPUT_XBOX=1 };
enum EUsedKeys { UI_KEY_LEFT=0,UI_KEY_RIGHT=1,UI_KEY_DOWN=2,UI_KEY_UP=3,UI_KEY_RIGHT_STICK_DOWN=4,UI_KEY_RIGHT_STICK_UP=5,UI_KEY_A=6,UI_KEY_B=7,UI_KEY_ZOOM_IN=8,UI_KEY_ZOOM_OUT=9,UI_KEY_SPELL_LEFT=10,UI_KEY_SPELL_RIGHT=11,UI_KEY_SPELL_ASSIGN=12,UI_KEY_SPELL_UNASSIGN=13,UI_KEY_SPELL_LIST_UP=14,UI_KEY_SPELL_LIST_DOWN=15 };
public: static NUISystem::CManager * __fastcall GetInstance();
#ifdef FABLE_RETAIL_CMANAGER_MENUCOUNT
// Retail opt-in (chunk 1005, row 0x0056F30B CTCInventoryExperience::CloseInventory): FableWin.pdb / Ego_r.pdb
// ?OnOpenMenus@CManager@NUISystem@@SIXXZ / ?OnCloseMenus@CManager@NUISystem@@SIXXZ (public static, fastcall), out of line at
// 0x0041D122 / 0x0041D165 (Ego_r bodies 0x0042D956 67 bytes / 0x0042D999 51 bytes, retail 67 / 51). Declarations only.
public: static void __fastcall OnOpenMenus();                                   // 0x0041D122
public: static void __fastcall OnCloseMenus();                                  // 0x0041D165
#endif
#ifdef FABLE_RETAIL_CMANAGER_CLEAR
 static void __fastcall Clear();
#endif
#ifdef FABLE_RETAIL_CMANAGER_ERRORMESSAGE
// Retail opt-in (chunk 1022, row 0x00418289 CMainGameComponent::Update): Ego_r.pdb (0x0042D784) / FableWin.pdb (0x01B0ED72)
// ?ClearErrorMessage@CManager@NUISystem@@QAEXXZ, out of line in retail at 0x0041CE6A, and the HasErrorMessage accessor of the
// FABLE_RETAIL_CMANAGER branch (inline: `cmp byte ptr [eax+9Ch],bl`) for TUs that do not take that branch. Declarations only.
public: void ClearErrorMessage();                                               // 0x0041CE6A
#ifndef FABLE_RETAIL_CMANAGER
public: bool HasErrorMessage() { return m_bHasErrorMessage; }
#endif
#endif
#ifdef FABLE_RETAIL_CMANAGER
// Retail branch (chunk 726): Ego_r.pdb ?GetUIScale@CManager@NUISystem@@QBE?AVC2DVector@@XZ, out of line at 0x0041CF47
// (CList::ComputeChildrenVisibility 0x005379F5 and CScrollingViewport::Update 0x0053F1C0 call it with a hidden result).
public: C2DVector GetUIScale() const;
// Chunk 774 (bake 0x0042DF9E CNewFrontendGameComponent::Draw): Ego_r.pdb ?HasErrorMessage@CManager@NUISystem@@QAE_NXZ, inline in
// retail (Draw tests m_bHasErrorMessage +0x9C in place: `cmp byte ptr [eax+9Ch],bl`), and ?RenderErrorMessage@CManager@NUISystem@@
// QAEXAAVCIEngine@@@Z, out of line at 0x0041D03C.
public: bool HasErrorMessage() { return m_bHasErrorMessage; }
public: void RenderErrorMessage(CIEngine&);                          // 0x0041D03C
// Chunk 752 (row 0x00559A90 CScrollBar::ProcessEvent): Ego_r.pdb ?GetMouseMovement@CManager@NUISystem@@QAEAAVC2DVector@@XZ, inline
// in retail (the row copies MouseMovement +0xB0 in place: `mov edx,[eax+0B0h] ; add eax,0B0h ; ... mov eax,[eax+4]`).
public: C2DVector& GetMouseMovement() { return MouseMovement; }
// Chunk 752 (row 0x00554740 CMorphingSprite::Draw): Ego_r.pdb ?GetGraphicsBank@CManager@NUISystem@@QAE?AV?$CCountedPointer@
// VCGraphicDataBank@@@@XZ, inline in retail (the row copies PGraphicsBank +0x10 / +0x14 in place and bumps the count).
public: CCountedPointer<CGraphicDataBank> GetGraphicsBank() { return PGraphicsBank; }
// Chunk 735 (rows 0x005729D8 / 0x00573792 CTCInventoryExperience::AddStatToList(PC)): Ego_r.pdb / FableWin.pdb
// ?CreateTickSlider@CManager@NUISystem@@QAEPAVCComponent@2@ABVCWideString@@ABVC2DCoordI@@JJABVCRGBFloatColour@@MM_N32M@Z,
// out of line at 0x00427216 (also called by the landed CTCInventoryStats::AddHeroAbility 0x005D64DD). Parameter names unknown.
// Chunk 749 (row 0x005502E0 CMesh::Draw): Ego_r.pdb ?GetMeshBank@CManager@NUISystem@@QAE?AV?$CCountedPointer@VCMeshDataBank@@@@XZ,
// out of line at 0x0041D1B4 (hidden result).
public: CCountedPointer<CMeshDataBank> GetMeshBank();
public: NUISystem::CComponent * CreateTickSlider(const CWideString &,const C2DCoordI &,long,long,const CRGBFloatColour &,float,float,bool,bool,const CRGBFloatColour &,float);
#ifdef FABLE_RETAIL_CMANAGER_ACTION
// Nested opt-in (chunk 886, row 0x0041E6D3 CManager::Action): Ego_r.pdb / FableWin.pdb
// ?GetCorrectComponentInterface@CManager@NUISystem@@QAEPAVCComponent@2@PAV32@@Z, out of line at 0x0041CD93 (28 calls from Action).
// Declaration only: no layout or vtable change.
public: NUISystem::CComponent * GetCorrectComponentInterface(NUISystem::CComponent *);
    // FableWin.pdb ?SetDescriptionScrollingOffset@CManager@NUISystem@@QAEXM@Z / ?SetDescriptionScrollingToMax@CManager@NUISystem@@QAEX_N@Z,
    // out of line (0x0041CF33 float +0xC4, 0x0041CF1F byte +0xC8; the SCROLL_DESCRIPTION_* cases call them).
    public: void SetDescriptionScrollingOffset(float);
    public: void SetDescriptionScrollingToMax(bool);
#endif
#endif
public: virtual bool GetMouseDisabled();
public: NUISystem::CComponent * CreateComponent(const CCharString &,bool);
public: NUISystem::CComponent * CreateComponent(long);
public: virtual void SetKeyPressed(unsigned short);
public: virtual unsigned short GetKeyPressed();
public: virtual void Action(CCountedPointer<NUISystem::CAction> &);
public: virtual void SetWeaponsInventory(CTCInventoryWeapons *);
public: virtual void SetMapInventory(CTCInventoryMap *);
public: virtual void SetQuestInventory(CTCInventoryQuests *);
public: virtual void SetTCHeroExperienceInventory(CTCHeroExperience *);
public: virtual void SetAbilitiesInventory(CTCInventoryAbilities *);
public: virtual void SetExperienceInventory(CTCInventoryExperience *);
public: virtual void SetInventory(CTCInventory *);
public: virtual void SetInventoryClothing(CTCInventoryClothing *);
public: virtual void SetInventoryTrade(CTCInventoryTrade *);
public: virtual void AddThingActionParam(long);
public: virtual void RemoveThingActionParam(long);
public: virtual void SetPlayerThing(CThing *);
public: virtual void AddUIActionParam(NUISystem::CActionParam *);
public: virtual void RemoveUIActionParam(NUISystem::CActionParam *);
public: virtual void ClearActionParams();
public: NUISystem::CComponent * CreateMenuEntryComponent(const CWideString &,float,float,const CCharString &,float,const CCharString,float,CCharString);
public: virtual void SetNumberAugmentations(unsigned long);
public: virtual unsigned long GetNumberAugmentations() const;
public: virtual void SetChange(NUISystem::CManager::EChange);
public: virtual NUISystem::CManager::EChange GetChange();
public: virtual void SetMetaLayer(long);
public: virtual long GetMetaLayer();
public: virtual EEngine2DLayer GetLayer(long);
public: virtual void ClearBastardChild();
public: virtual void SetRightStickForce(C2DVector &);
public: virtual C2DVector & GetRightStickForce();
public: virtual void PlaySoundW(CCharString &,long);
public: virtual unsigned long PlaySample(CCharString &,long);
public: virtual CDefPointer<NUISystem::CUIMiscThingsDef> GetMiscThingsDef();
public: virtual CDefPointer<NUISystem::CUIIconsDef> GetIconsDef();
public: virtual void AddActionsListenIgnoreOnList(NUISystem::CComponent *,NUISystem::CComponent *,NUISystem::CComponent *,NUISystem::EEvent);
#ifdef FABLE_RETAIL_CMANAGER_MOUSECURSOR
// Opt-in (chunk 765, rows 0x0055C8B0 / 0x0055C8C0 CMouseCursor adjustor thunks): Ego_r.pdb ?SetMouseCursor@CManager@NUISystem@@QAEXPAVCComponent@2@@Z,
// inline in retail (~CMouseCursor 0x0055C510 and CMouseCursor(long) 0x0055C650 store PMouseCursor +0xB8 in place). No layout change.
public: void SetMouseCursor(NUISystem::CComponent* cursor) { PMouseCursor = cursor; }
#endif
#ifdef FABLE_RETAIL_CMANAGER_GETMOUSECURSOR
// Opt-in (wf2, row 0x005590C0 CScrollBarOutside::OnLeftClicked): FableWin.pdb ?GetMouseCursor@CManager@NUISystem@@QAEPAVCComponent@2@XZ,
// inline in retail (the row reads PMouseCursor +0xB8 in place: `mov ecx,[eax+0B8h]`; landed 0x0055A5D0 / 0x0055A5B0 spell it the same). No layout change.
public: NUISystem::CComponent* GetMouseCursor() { return PMouseCursor; }
#endif
protected: CCountedPointer<CGraphicDataBank> PGraphicsBank;
protected: CCountedPointer<CMeshDataBank> PMeshBank;
protected: CInputManager * PInputManager;
protected: std::map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys const ,long> > > Keys;
protected: NUISystem::CManager::EInputType InputType;
protected: CTCHeroExperience * PTCHeroExperience;
protected: CTCInventoryAbilities * PAbilitiesInventory;
protected: CTCInventoryWeapons * PWeaponsInventory;
protected: CTCInventoryMap * PMapInventory;
protected: CTCInventoryQuests * PQuestInventory;
protected: CTCInventoryExperience * PExperienceInventory;
protected: CTCInventory * PInventory;
protected: CTCInventoryClothing * PInventoryClothing;
protected: CTCInventoryTrade * PInventoryTrade;
protected: CThing * PPlayer;
protected: std::list<long,std::allocator<long> > ThingActionParams;
protected: std::list<NUISystem::CActionParam *,std::allocator<NUISystem::CActionParam *> > UIActionParams;
protected: CDefinitionManager * m_pDefinitionManager;
protected: unsigned long NumberAugmentations;
protected: NUISystem::CManager::EChange Change;
protected: std::map<long,EEngine2DLayer,std::less<long>,std::allocator<std::pair<long const ,EEngine2DLayer> > > Layers;
protected: long MetaLayer;
protected: CCountedPointer<NUISystem::CComponent> BastardChild;
protected: std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString const ,CCountedPointer<NUISystem::CComponent> > > > BastardChildren;
protected: C2DVector RightStickForce;
protected: bool m_bHasErrorMessage;
protected: NUISystem::CComponent * m_pErrorMessage;
protected: bool Restoring;
protected: double m_fLastRenderTime;
protected: C2DVector MouseMovement;
protected: NUISystem::CComponent * PMouseCursor;
protected: NUISystem::CComponent * PDropped;
protected: unsigned short KeyPressed;
protected: float DescriptionScrollingOffset;
protected: bool DescriptionScrollingToMax;
protected: bool MouseDisabled;
protected: bool AssignmentItemsExpressionsSwapping;
protected: bool AssignmentSpellsSwapping;
};
}
#ifdef FABLE_RETAIL_CMANAGER_ERRORMESSAGE
// Ego_r.pdb ?ShowErrorMessage@@YIXW4ERRORMESSAGE_TYPE@@@Z (0x00438C49, right after ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
// 0x00438C20; retail 0x0041E61B right after GetInstance 0x0041E5F2): a global __fastcall forward to
// CManager::GetInstance()->ShowErrorMessage(type) (Ego_r ?ShowErrorMessage@CManager@NUISystem@@QAEXW4ERRORMESSAGE_TYPE@@@Z, 0x0041DB84).
// ERRORMESSAGE_TYPE is a global enum (W4ERRORMESSAGE_TYPE@@); its enumerators are not in the extracts on disk.
enum ERRORMESSAGE_TYPE { };
void __fastcall ShowErrorMessage(ERRORMESSAGE_TYPE type);                    // 0x0041E61B
#endif
#endif
