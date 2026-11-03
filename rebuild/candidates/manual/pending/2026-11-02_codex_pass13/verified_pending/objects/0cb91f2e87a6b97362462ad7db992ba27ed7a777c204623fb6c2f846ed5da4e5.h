#ifndef FABLE_ENGINE_CTCDREGIONEXIT_H
#define FABLE_ENGINE_CTCDREGIONEXIT_H
#include "rebuild_abi.h"
#include "engine/CTCDriverBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ETCInterfaceType.h"
#include "fable_string.h"
// Original complete data members: both PDB receipts evidence/00501450_*_types.json.
class CTCDRegionExit : public CTCDriverBase {
private: bool Active;
private: bool InLetterBoxMode;
private: bool WaitingForConfirmation;
private: bool WaitingForActivationOrPlayerToLeave;
private: bool WaitingForPlayerToLeaveMessageArea;
private: float Radius;
private: float MessageRadius;
private: unsigned __int64 EntranceConnectedToUID;
private: bool ReversedOnMiniMap;
private: bool HiddenOnMiniMap;
private: CIntelligentPointer<CThing> PEntranceConnectedTo;
public: virtual ETCInterfaceType GetInterfaceType() const;
public: virtual CCharString GetClassTypeName() const;
public: virtual bool IsDriverType(long) const;
public: virtual ~CTCDRegionExit();
public: virtual void CreateAdditionalTCs();
public: virtual bool IsComponentSerialisable() const;
public: virtual void OnSerialise(CPersistContext &,bool);
public: virtual void OnInitialActivate();
public: virtual bool HasFrameUpdate() const;
public: virtual void FrameUpdate();
public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return TCI_REGION_EXIT;}
const CCharString& GetNameOfRegionConnectingTo() const;
};
FABLE_STATIC_ASSERT(sizeof(CTCDRegionExit)==56);
#endif
