// CAIStateGroup_GazeAtWork::CAIStateGroup_GazeAtWork() -- initialise gaze movement settings and marker definitions.
// Leaf: ??0CAIStateGroup_GazeAtWork@@QAE@XZ
// Identity: proven; own RTTI 0128FEC4 and exact PDB default ctor; actual base CAIStateGroup_Gaze ctor; Chance/MinJourney+28/+32 and Primary/SecondaryMarkerDef+40/+44 from PDB; own primary literal GAZE_AT_WORK_MARKER, secondary GAZE_OUT_OF_BUILDING_MARKER; real CGameDefinitionManager::Get0044C6B0 and CDefinitionManager::GetDefGlobalIndexFromName009AD410 exact PDB const API.
// Signature audit: original CCharString(const char*,long) uses PDB ??0CCharString@@QAE@PBDJ@Z; old int/H alias removed, actual test constructor corrected too.
// Layout: CDefinitionManager shared data header included unchanged under alias; original CGameDefinitionManager primary base+0. No manager fields or size accessed. Generated state-group headers flatten CBaseClass virtual chain and tracked member, so local original Gaze48 inheritance/fields retained. Constructor leaves MaxDistance to base default.
#define CDefinitionManager CDefinitionManager_HeaderFields
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
#ifndef _MSC_VER
#define __fastcall
#endif
class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString(const char*,long=-1);~CCharString();};class CDefinitionManager:public CDefinitionManager_HeaderFields{public:long GetDefGlobalIndexFromName(const CCharString&)const;};class CGameDefinitionManager:public CDefinitionManager{public:static CGameDefinitionManager& __fastcall Get();};
class CBaseClass{public:virtual ~CBaseClass(){} };class CAIStateGroupBase:public CBaseClass{public:unsigned char _unmodeled04[16];};class CBaseIntelligentPointer{public:virtual ~CBaseIntelligentPointer(){}void* PData;};class CThingGameObject;template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{};
class CAIStateGroup_Gaze:public CAIStateGroupBase{public:CIntelligentPointer<CThingGameObject> GazeMarker;float ChanceOfRunning,MinJourneyForRunning,MaxDistanceForCreaturesToLookAt;long PrimaryMarkerDef,SecondaryMarkerDef;CAIStateGroup_Gaze();};class CAIStateGroup_GazeAtWork:public CAIStateGroup_Gaze{public:CAIStateGroup_GazeAtWork();};

CAIStateGroup_GazeAtWork::CAIStateGroup_GazeAtWork(){ChanceOfRunning=0.2f;MinJourneyForRunning=8.0f;PrimaryMarkerDef=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("GAZE_AT_WORK_MARKER");SecondaryMarkerDef=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("GAZE_OUT_OF_BUILDING_MARKER");}