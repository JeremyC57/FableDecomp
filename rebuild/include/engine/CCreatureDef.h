#ifndef FABLE_ENGINE_CCREATUREDEF_H
#define FABLE_ENGINE_CCREATUREDEF_H
#include "engine/CSubDefClassBase.h"
// Raw TPI0x14be3 public CSubDefClassBase base. Native00667df1 reads FlashOnHit+c9.
// Original PWoundMorphs through RespawnWaitInFramesMax and RadiusWeight/Spread tail
// remain opaque at complete retail offsets, after the four STLport container reductions.
#ifdef FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES
#include "engine/CDefString.h"
#include "stlport/fable_containers.h"
#endif
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
#include "engine/CCountedPointer.h"
class CCreatureStatsDef;
#endif
class CCreatureDef : public CSubDefClassBase {
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
public:const CCreatureStatsDef& GetStatsDef()const{return *PStats;}
#endif
public:
#ifdef FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES
    // Nested opt-in (chunk 760, row 0x006E2C00 CTCTalk::OnCreate): Ego_r.pdb CCreatureDef DialogueVoices
    // (vector<CDefString>, Ego_r +0x78 behind InitialActions / InitialAppearanceModifiers / PhonemeAnim, each 4 bytes wider
    // in Ego_r's STLport) at retail +0x6C (`mov eax,[edi+70h]; sub eax,[edi+6Ch]; sar eax,2` at 0x006E2C3B); the inline
    // ?StatGetTypeName@CCreatureDef@@SI?AVCCharString@@XZ (GetSubDef<CCreatureDef> 0x00485815 builds "CCreatureDef" in place).
    static CCharString __fastcall StatGetTypeName() { return CCharString("CCreatureDef", -1); }
#ifdef FABLE_RETAIL_CCREATUREDEF_INITIALAPPEARANCEMODIFIERS
    // Nested opt-in (chunk 1003, row 0x005B3AEF CTCInventoryClothing::OnInitialActivate): Ego_r.pdb InitialActions / InitialAppearanceModifiers
    // (vector<long>, Ego_r +72 / +88 with the 16-byte debug vectors) at retail +0x48 / +0x54 (the 12-byte STLport vector; OnInitialActivate
    // walks `mov eax,[edi+58h] ; sub eax,[edi+54h] ; sar eax,2` at 0x005B3B43), then PhonemeAnim (map<CCharString,long>, 12 bytes) at +0x60,
    // which puts DialogueVoices at the +0x6C above. No layout change.
    unsigned char _pad_0x25[0x48 - 0x25];
    std::vector<long> InitialActions;                                    // +0x48
    std::vector<long> InitialAppearanceModifiers;                        // +0x54
    unsigned char PhonemeAnim[0xc];                                      // +0x60 map<CCharString,long>
#else
    unsigned char _pad_0x25[0x6c - 0x25];
#endif
    std::vector<CDefString> DialogueVoices;                              // +0x6c
#ifdef FABLE_RETAIL_CCREATUREDEF_SCREENINVENTORIES
    // Nested opt-in (chunk 1023, row 0x0061A6A0 CTCInventoryQuests::OnCreate): Ego_r.pdb CCreatureDef Inventory .. InGameMenuScreen
    // (thirteen longs, Ego_r +0x88..+0xB8) at retail +0x78..+0xA8, right behind DialogueVoices (0x10 lower, as DialogueVoices).
    // Retail reads QuestsScreenInventory +0xA4 (0x0061A705), WeaponsInventory +0x80 (CTCInventoryWeapons::OnCreate 0x005C3057) and
    // MapScreenInventory +0x8C (CTCInventoryMap::OnCreate 0x00602E30) off GetSubDef<CCreatureDef>. No layout change.
    long Inventory;                                                      // +0x78
    long ClothingInventory;                                              // +0x7c
    long WeaponsInventory;                                               // +0x80
    long HeroAbilitiesScreenInventory;                                   // +0x84
    long QuestCardScreenInventory;                                       // +0x88
    long MapScreenInventory;                                             // +0x8c
    long StatsScreenInventory;                                           // +0x90
    long MagicScreenInventory;                                           // +0x94
    long ExperienceScreenInventory;                                      // +0x98
    long TradeScreenInventory;                                           // +0x9c
    long AugmentScreenInventory;                                         // +0xa0
    long QuestsScreenInventory;                                          // +0xa4
    long InGameMenuScreen;                                               // +0xa8
#ifdef FABLE_RETAIL_CCREATUREDEF_RESPAWNWAIT
    // Nested opt-in (agentNF10, row 0x0072F170 CTCDummyVillager::SetupFromData): Ego_r.pdb CCreatureDef CreatureGroup .. RespawnWaitInFramesMax
    // (Ego_r +0xBC..+0xD4) at retail +0xAC..+0xC4, 0x10 lower as the screen inventories above. Retail reads RespawnWaitInDaysMin
    // +0xB8 / RespawnWaitInDaysMax +0xBC off GetSubDef<CCreatureDef> 0x00485815 (0x0072F23E..). No layout change.
    unsigned long CreatureGroup;                                         // +0xac
    CDefString BattleCrySound;                                           // +0xb0
    long OpinionSourceDef;                                               // +0xb4
    long RespawnWaitInDaysMin;                                           // +0xb8
    long RespawnWaitInDaysMax;                                           // +0xbc
    long RespawnWaitInFramesMin;                                         // +0xc0
    long RespawnWaitInFramesMax;                                         // +0xc4
#else
    unsigned char _pad_0xac[0xc8 - 0xac];
#endif
#else
    unsigned char _pad_0x78[0xc8 - 0x78];
#endif
#else
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
protected:
    unsigned char _pad_0x25[0x40-0x25];
    CCountedPointer<CCreatureStatsDef> PStats;
public:
    unsigned char _pad_0x48[0xc8-0x48];
#else
    unsigned char _pad_0x25[0xa3];
#endif
#endif
    bool Short,FlashOnHit,PickPocketable,ChildThatCanRegionFollow;
    bool AllowedInProtectedTowns,UseActualHitPosForHitEffects;
    unsigned char _pad_0xce[0x1a];
};
FABLE_STATIC_ASSERT(sizeof(CCreatureDef)==0xe8);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef,FlashOnHit)==0xc9);

#endif
