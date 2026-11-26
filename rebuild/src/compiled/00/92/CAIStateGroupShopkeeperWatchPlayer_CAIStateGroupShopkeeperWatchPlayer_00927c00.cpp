// CAIStateGroup_ShopkeeperWatchPlayer::CAIStateGroup_ShopkeeperWatchPlayer() -- initialise gaze movement settings and marker definitions.
// Leaf: ??0CAIStateGroup_ShopkeeperWatchPlayer@@QAE@XZ
// Identity: proven; own RTTI 01291784 and exact PDB default ctor; actual base CAIStateGroup_GazeAtWork ctor; Chance/MinJourney+28/+32 and Primary/SecondaryMarkerDef+40/+44 from PDB; own primary literal GAZE_AT_WORK_MARKER, secondary GAZE_OUT_OF_BUILDING_MARKER; real CGameDefinitionManager::Get0044C6B0 and CDefinitionManager::GetDefGlobalIndexFromName009AD410 exact PDB const API.
// Signature audit: original CCharString(const char*,long) uses PDB ??0CCharString@@QAE@PBDJ@Z; old int/H alias removed, actual test constructor corrected too.
// Layout: CDefinitionManager shared data header included unchanged under alias; original CGameDefinitionManager primary base+0. No manager fields or size accessed. Generated state-group headers flatten CBaseClass virtual chain and tracked member, so local original Gaze48 inheritance/fields retained. Constructor leaves MaxDistance to base default.
// NF469 (r467a, ruling r469a): retail's ~CAIStateGroupBase is the implicit one, kept once at 0x008FD288 (Ego_r / FableWin flag it
//   compiler-generated). This local model's implicit copy was not retail's (a lone jmp ~CBaseClass: the model gives Name (+0x0C) no destructor), so the destructor is declared here and the
//   object references 0x008FD288's ??1CAIStateGroupBase@@UAE@XZ like the 640 landed local models; the row's own bytes are unchanged.
#define CDefinitionManager CDefinitionManager_HeaderFields
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
#ifndef _MSC_VER
#define __fastcall
#endif
class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString(const char*,long=-1);~CCharString();};class CDefinitionManager:public CDefinitionManager_HeaderFields{public:long GetDefGlobalIndexFromName(const CCharString&)const;};class CGameDefinitionManager:public CDefinitionManager{public:static CGameDefinitionManager& __fastcall Get();};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CAIStateGroupBase:public CBaseClass{public: virtual ~CAIStateGroupBase();unsigned char _unmodeled04[16];};class CBaseIntelligentPointer{public:virtual ~CBaseIntelligentPointer(){}void* PData;};class CThingGameObject;template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{};
class CAIStateGroup_Gaze:public CAIStateGroupBase{public:CIntelligentPointer<CThingGameObject> GazeMarker;float ChanceOfRunning,MinJourneyForRunning,MaxDistanceForCreaturesToLookAt;long PrimaryMarkerDef,SecondaryMarkerDef;CAIStateGroup_Gaze();};class CAIStateGroup_GazeAtWork:public CAIStateGroup_Gaze{public:CAIStateGroup_GazeAtWork();};
class CAIStateGroup_ShopkeeperWatchPlayer:public CAIStateGroup_GazeAtWork{public:CAIStateGroup_ShopkeeperWatchPlayer();};
CAIStateGroup_ShopkeeperWatchPlayer::CAIStateGroup_ShopkeeperWatchPlayer(){ChanceOfRunning=1.0f;MinJourneyForRunning=14.0f;PrimaryMarkerDef=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("GAZE_AT_WORK_MARKER");SecondaryMarkerDef=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("GAZE_OUT_OF_BUILDING_MARKER");}