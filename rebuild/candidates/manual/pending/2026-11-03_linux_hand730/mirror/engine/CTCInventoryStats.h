#ifndef FABLE_ENGINE_CTCINVENTORYSTATS_H
#define FABLE_ENGINE_CTCINVENTORYSTATS_H
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/NHeroInformationScreens_CBase.h"
#include "stlport/fable_map.h"
class CTCHeroExperience;class CTCHeroStats;
// Original complete storage; native inventory base130 plus information-screen base4.
class CTCInventoryStats : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public: virtual ~CTCInventoryStats();
public: virtual ETCInterfaceType GetInterfaceType() const;
public: virtual CCharString GetClassTypeName() const;
public: virtual void OnSerialise(CPersistContext &,bool);
public: virtual void OnKill();
public: virtual void Copy(const CTCBase &);
public: virtual void FrameUpdate();
public: virtual bool HasFrameUpdate() const;
public: virtual void OnCreate();
public: virtual void OnItemSelected(long,bool &);
public: virtual void OpenInventory();
public: virtual void CloseInventory();
public: virtual void GlobalUpdate(bool);
public: virtual void OnChangeCategory();
public: virtual CWideString GetScreenName() const;
public: virtual EPlayerMode GetPlayerMode() const;
public: virtual NHeroInformationScreens::EScreenType GetScreenType() const;
public: static CWideString __fastcall GetXString();
public: virtual NUISystem::CComponent * ConstructScreen(NUISystem::CComponent *);
public: virtual NUISystem::CComponent * ConstructPersonalityList(NUISystem::CComponent *);
public: virtual NUISystem::CComponent * ConstructExperienceList(NUISystem::CComponent *);
public: virtual NUISystem::CComponent * ConstructExperienceListPC(NUISystem::CComponent *);
public: virtual NUISystem::CComponent * ConstructPersonalityListPC(NUISystem::CComponent *);
private: std::map<unsigned long,long,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,long> > > CreaturesKilled;
private: CThing * pMannequin;
private: bool UseMannequin;
private: long lastCategoryIdx;
private: const CTCHeroExperience * pTCHeroExperience;
private: const CTCHeroStats * pTCHeroStats;
};
// Native Alloc004d2fdc pushes0x158; original fields plus inherited alignment agree.
FABLE_STATIC_ASSERT(sizeof(CTCInventoryStats)==0x158);
#endif
