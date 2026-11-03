#ifndef FABLE_ENGINE_CTCACTIONUSESCRIPTEDHOOK_H
#define FABLE_ENGINE_CTCACTIONUSESCRIPTEDHOOK_H
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ETCInterfaceType.h"
#include "fable_string.h"
// Original complete data members: both PDB receipts evidence/00501450_*_types.json.
class CTCActionUseScriptedHook : public CTCBase {
private: bool Usable;
private: bool ForceConfirmation;
private: bool WaitingForConfirmation;
private: bool PerformAction;
private: bool TeleportToRegionEntrance;
private: unsigned __int64 EntranceConnectedToUID;
private: CIntelligentPointer<CThing> PEntranceConnectedTo;
private: bool EntranceLocked;
private: unsigned __int64 CameraTrackUID;
private: CIntelligentPointer<CThing> PCameraTrack;
private: CCharString SoundName;
private: CCharString AnimationName;
private: long ReplacementObject;
private: bool ReversedOnMiniMap;
private: bool HiddenOnMiniMap;
public: virtual ETCInterfaceType GetInterfaceType() const;
public: virtual CCharString GetClassTypeName() const;
public: virtual ~CTCActionUseScriptedHook();
public: virtual bool IsSerialiseCreatable() const;
public: virtual bool IsComponentSerialisable() const;
public: virtual void OnInitialActivate();
public: virtual void OnSerialise(CPersistContext &,bool);
public: virtual void GlobalUpdate(bool);
public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return TCI_ACTION_USE_SCRIPTED_HOOK;}
CThing* GetPEntranceConnectedTo() const {return PEntranceConnectedTo.Get();}
};
FABLE_STATIC_ASSERT(sizeof(CTCActionUseScriptedHook)==80);
#endif
