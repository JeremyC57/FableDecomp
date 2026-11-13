#ifndef FABLE_ENGINE_CTCINVENTORYSTATS_H
#define FABLE_ENGINE_CTCINVENTORYSTATS_H
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/NHeroInformationScreens_CBase.h"
#include "stlport/fable_map.h"
#ifdef FABLE_RETAIL_CTCINVENTORYSTATS
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/EHeroAbility.h"
#endif
class CTCHeroExperience;class CTCHeroStats;
#ifdef FABLE_RETAIL_CTCINVENTORYSTATS_PERSONALITY
enum EHeroSexuality;                                         // MS forward enum (no enumerator is needed by the rows)
namespace NGameText { class CDataBank; }
#endif
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
#ifdef FABLE_RETAIL_CTCINVENTORYSTATS
// Retail opt-in (chunk 735, row 0x00577AC0): Ego_r.pdb ?StatGetInterfaceType@CTCInventoryStats@@SI?AW4ETCInterfaceType@@XZ
// (inline: TCI_INVENTORY_STATS = 20, the `push 14h` of the inline CThing::GetTC) and ?CreateExperienceSliders@CTCInventoryStats@@
// QAE?AV?$vector@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@PAVCComponent@NUISystem@@J_N@Z
// (out of line at 0x005CDD86, landed).
public: static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_INVENTORY_STATS; }
public: std::vector<NUISystem::CComponent*> CreateExperienceSliders(NUISystem::CComponent *,long,bool);
// Chunk 867 (row 0x005D6748 ConstructExperienceList): Ego_r.pdb / FableWin.pdb ?AddHeroAbility@CTCInventoryStats@@QAEXPAVCComponent@
// NUISystem@@W4EHeroAbility@@AAM@Z, out of line at 0x005D64DD (landed): one ability row on the stats screen, the row position by
// reference (read for the slider's Y, then advanced).
public: void AddHeroAbility(NUISystem::CComponent *,EHeroAbility,float &);
#ifdef FABLE_RETAIL_CTCINVENTORYSTATS_PERSONALITY
// Nested opt-in (chunk 4006, rows 0x005D1F0C ConstructPersonalityListPC / 0x005CE0E6 ConstructPersonalityList): the non-virtual
// helpers the personality page calls, all out of line in retail and all in Ego_r.pdb / FableWin.pdb with these exact manglings:
// ?BuildHeroClass@CTCInventoryStats@@QAE?AVCWideString@@XZ 0x005CD4C4, ?BuildRenownString@..@@QAE?AVCWideString@@XZ 0x005CD433,
// ?ConvertMap@..@@QAEXABV?$map@KJ..@std@@AAV?$list@U?$pair@VCWideString@@J@std@@..@3@@Z 0x005CD6B7,
// ?CreateTitle@..@@QAEXABVCWideString@@PAVCComponent@NUISystem@@_N@Z 0x005CCF0E, ?CreateTitlePC@.. 0x005CCFFC,
// ?GetFatnessAsString@..@@QAE?AVCWideString@@M@Z 0x005CC862, ?GetHeroSexualityAsString@..@@QAE?AVCWideString@@W4EHeroSexuality@@@Z
// 0x005CCBBD, ?GetTimeSpentInGame@..@@SI?AVCWideString@@KPBVCDataBank@NGameText@@@Z 0x005CCC91 (static __fastcall).
// Declarations only: no layout change. The includer defines FABLE_RETAIL_STLP_LIST (std::list) before this header.
public: CWideString BuildHeroClass();
public: CWideString BuildRenownString();
public: void ConvertMap(const std::map<unsigned long,long,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,long> > > &,std::list<std::pair<CWideString,long>,std::allocator<std::pair<CWideString,long> > > &);
public: void CreateTitle(const CWideString &,NUISystem::CComponent *,bool);
public: void CreateTitlePC(const CWideString &,NUISystem::CComponent *,bool);
public: CWideString GetCreatureGroupAsString(unsigned long);   // 0x005CC9AD ?GetCreatureGroupAsString@CTCInventoryStats@@QAE?AVCWideString@@K@Z (landed row)
public: CWideString GetFatnessAsString(float);
public: CWideString GetHeroSexualityAsString(EHeroSexuality);
public: static CWideString __fastcall GetTimeSpentInGame(unsigned long,const NGameText::CDataBank *);
#endif
#endif
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
