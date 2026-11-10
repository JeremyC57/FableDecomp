#ifndef FABLE_CTCDUMMYVILLAGER_H
#define FABLE_CTCDUMMYVILLAGER_H
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
class CTCVillage; class CThingBuilding; class CThingAICreature;
class CTCDummyVillager : public CTCBase {
public: static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType();
public: virtual ETCInterfaceType GetInterfaceType() const;
public: static CTCBase* FABLE_FASTCALL Alloc(CThing&);
public: static CCharString FABLE_FASTCALL StatGetClassTypeName();
public: virtual CCharString GetClassTypeName() const;
public: CTCDummyVillager(const CTCDummyVillager&);
public: CTCDummyVillager(CThing&);
public: virtual void OnCreate();
public: virtual void OnInitialActivate();
public: virtual void OnSerialise(CPersistContext&,bool);
public: virtual bool IsComponentSerialisable() const;
public: virtual bool HasFrameUpdate() const;
public: virtual void FrameUpdate();
public: void SetupFromVillager(CThingAICreature&,bool);
public: void SetupFromData(unsigned long,CThingBuilding*,CThingBuilding*,bool);
public: CThingAICreature* CreateVillager(CTCVillage&,const C3DVector&,bool);
public: bool CanBeRecreated();
public: CThingBuilding* GetHomeBuilding() const;
public: CThingBuilding* GetWorkBuilding() const;
public: CThingAICreature* GetCreature() const { return Creature.Get(); }
public: void Evict();
public: bool IsGuard();
public: void SetRespawnable(bool);
public: unsigned long GetDefType() { return DefType; }
private: unsigned long DefType;
private: CIntelligentPointer<CThingBuilding> HomeBuilding;
private: unsigned __int64 HomeBuildingUID;
private: CIntelligentPointer<CThingBuilding> WorkBuilding;
private: unsigned __int64 WorkBuildingUID;
private: CIntelligentPointer<CThingAICreature> Creature;
private: unsigned __int64 CreatureUID;
private: long DayToBeRecreated;
private: long FrameToBeRecreated;
private: bool Respawnable;
private: bool Guard;
public: virtual ~CTCDummyVillager();
public: CTCDummyVillager& operator=(const CTCDummyVillager&);
};
FABLE_STATIC_ASSERT(sizeof(CTCDummyVillager)==0x50);
#endif
