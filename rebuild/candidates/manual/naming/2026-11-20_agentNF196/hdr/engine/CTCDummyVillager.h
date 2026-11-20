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
#ifdef FABLE_RETAIL_CTCDUMMYVILLAGER_ISGUARD_INLINE
// NF196 opt-in (q4522, row 0x0065C45A CTCVillage::EnableGuards): ?IsGuard@CTCDummyVillager@@QAE_NXZ has an out-of-line copy only in
// FableWin.pdb (0x0216A7AF, debug); Ego_r.pdb has none and retail reads Guard (+0x49) in place (0x0065C4C0), so the release body is inline.
public: bool IsGuard() { return Guard; }
#else
public: bool IsGuard();
#endif
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
FABLE_STATIC_ASSERT_AT(45, sizeof(CTCDummyVillager)==0x50);
#endif
