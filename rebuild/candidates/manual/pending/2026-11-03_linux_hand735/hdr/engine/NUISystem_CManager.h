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
#ifdef FABLE_RETAIL_CMANAGER
// Retail branch (chunk 726): Ego_r.pdb ?GetUIScale@CManager@NUISystem@@QBE?AVC2DVector@@XZ, out of line at 0x0041CF47
// (CList::ComputeChildrenVisibility 0x005379F5 and CScrollingViewport::Update 0x0053F1C0 call it with a hidden result).
public: C2DVector GetUIScale() const;
// Chunk 735 (rows 0x005729D8 / 0x00573792 CTCInventoryExperience::AddStatToList(PC)): Ego_r.pdb / FableWin.pdb
// ?CreateTickSlider@CManager@NUISystem@@QAEPAVCComponent@2@ABVCWideString@@ABVC2DCoordI@@JJABVCRGBFloatColour@@MM_N32M@Z,
// out of line at 0x00427216 (also called by the landed CTCInventoryStats::AddHeroAbility 0x005D64DD). Parameter names unknown.
public: NUISystem::CComponent * CreateTickSlider(const CWideString &,const C2DCoordI &,long,long,const CRGBFloatColour &,float,float,bool,bool,const CRGBFloatColour &,float);
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
#endif
