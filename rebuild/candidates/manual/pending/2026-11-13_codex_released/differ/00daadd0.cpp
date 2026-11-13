// NScript::CQ_NewOakValeIntroScript::Init() -- reset the Oakvale-intro quest state (flags, deed counters, the talk timer, the bully health readout) and fill the eight villager-speech vectors with their TEXT_QST_048 identifiers
// Grade: DIFFER(3406v3306) -- md=350: the first 609 instructions match; residue is VC7.1 inline-budget only: retail expands std::_Construct<CCharString> (placement copy) at the first 22 of 42 push_back sites and calls the out-of-line __fastcall 0x0044B110 at the last 20; ours expands all 42
// Next idea: no plain spelling, flag or TU-level filler moves the cut (ATTEMPTS.log); /d2inl* knobs are all-or-nothing here; ask the owner whether a per-TU inline knob is acceptable, or find what retail's Init TU had that spends the budget
// Leaf: ?Init@CQ_NewOakValeIntroScript@NScript@@UAEXXZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DAADD0, 3306 bytes, plain `ret`.
// Identity: slot 3 of the CQ_NewOakValeIntroScript vtable 0x012D7A28 (the landed ctor 0x00DAAC00 stores it; slot 1 RegisterMain
//   0x00DAACE0, slot 2 Main 0x00DABAC0, slot 4 OnPersist 0x00DAADA0), and Ego_r.pdb / FableWin.pdb both list
//   ?Init@CQ_NewOakValeIntroScript@NScript@@UAEXXZ. Catalog "Script_Villager_Reputation_Dialog" was a placeholder.
// Callees: pDSTGame (0x0143E8F8) slot 89 SetTimer through the inline CTimer::operator=(long); CCharString::CCharString(const char*,
//   long) 0x0099EBF0, ~CCharString 0x0099EAE0, the copy ctor 0x0099EC30 (inside the expanded _Construct), the out-of-line
//   std::_Construct<CCharString,CCharString> 0x0044B110 (__fastcall) and vector<CCharString>::_M_insert_overflow 0x00433530.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CQ_NewOakValeIntroScript.h"
#include "engine/CGameScriptInterfaceBase.h"
extern CGameScriptInterfaceBase* pDSTGame;                            // 0x0143E8F8
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }

void NScript::CQ_NewOakValeIntroScript::Init()
{
    VictimShake = false;
    DadFound = false;
    AttackOver = false;
    DadFinishedIntro = false;
    BullySubdued = false;
    BullyRanOff = false;
    GivenHeroTeddy = false;
    HeroAttackedVictim = false;
    GoodDeedsPerformed = 0;
    BadDeedsPerformed = 0;
    GuardsDealtWithBadDeeds = 0;
    HeroDiscoveredInfidelity = false;
    BarrelManLeftHeroInCharge = false;
    BarrelManSpokenToHeroOnReturn = false;
    InstructionGiven_Barrels = false;
    ReceiveKiss = false;
    ReceiveHug = false;
    BarrelBrokenPersistent = false;
    TalkIntermittentTimer = 0;
    GUIBullyHealthCounter = -999;
    TeddyRuined = false;
    GuardsSpokenOnce = false;
    TalkingToWoman = false;
    GivenSweets = false;
    GivenTheresaChocs = false;
    VictimComplainsAboutLosingTeddy = false;
    for (long i = 0; i < 5; ++i)
        WhichBadDeedsPerformed[i] = false;

    VillagerSpeechFemaleBad.push_back("TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_10");
    VillagerSpeechFemaleBad.push_back("TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_20");
    VillagerSpeechFemaleBad.push_back("TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_30");
    VillagerSpeechFemaleBad.push_back("TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_40");
    VillagerSpeechFemaleBad.push_back("TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_50");
    VillagerSpeechFemaleBad.push_back("TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_60");

    VillagerSpeechMaleBad.push_back("TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_10");
    VillagerSpeechMaleBad.push_back("TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_20");
    VillagerSpeechMaleBad.push_back("TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_30");
    VillagerSpeechMaleBad.push_back("TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_40");
    VillagerSpeechMaleBad.push_back("TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_50");
    VillagerSpeechMaleBad.push_back("TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_60");

    VillagerSpeechFemaleBoth.push_back("TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_10");
    VillagerSpeechFemaleBoth.push_back("TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_20");
    VillagerSpeechFemaleBoth.push_back("TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_30");
    VillagerSpeechFemaleBoth.push_back("TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_40");

    VillagerSpeechMaleBoth.push_back("TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_10");
    VillagerSpeechMaleBoth.push_back("TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_20");
    VillagerSpeechMaleBoth.push_back("TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_30");
    VillagerSpeechMaleBoth.push_back("TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_40");

    VillagerSpeechFemaleGood.push_back("TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_10");
    VillagerSpeechFemaleGood.push_back("TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_20");
    VillagerSpeechFemaleGood.push_back("TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_30");
    VillagerSpeechFemaleGood.push_back("TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_40");
    VillagerSpeechFemaleGood.push_back("TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_50");
    VillagerSpeechFemaleGood.push_back("TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_60");

    VillagerSpeechMaleGood.push_back("TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_10");
    VillagerSpeechMaleGood.push_back("TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_20");
    VillagerSpeechMaleGood.push_back("TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_30");
    VillagerSpeechMaleGood.push_back("TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_40");
    VillagerSpeechMaleGood.push_back("TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_50");
    VillagerSpeechMaleGood.push_back("TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_60");

    VillagerSpeechFemaleNeither.push_back("TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_10");
    VillagerSpeechFemaleNeither.push_back("TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_20");
    VillagerSpeechFemaleNeither.push_back("TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_30");
    VillagerSpeechFemaleNeither.push_back("TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_40");
    VillagerSpeechFemaleNeither.push_back("TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_50");

    VillagerSpeechMaleNeither.push_back("TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_10");
    VillagerSpeechMaleNeither.push_back("TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_20");
    VillagerSpeechMaleNeither.push_back("TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_30");
    VillagerSpeechMaleNeither.push_back("TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_40");
    VillagerSpeechMaleNeither.push_back("TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_50");
}
