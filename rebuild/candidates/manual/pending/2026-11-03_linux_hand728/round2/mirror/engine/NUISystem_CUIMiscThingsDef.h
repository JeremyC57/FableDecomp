#ifndef FABLE_ENGINE_NUISYSTEM_CUIMISCTHINGSDEF_H
#define FABLE_ENGINE_NUISYSTEM_CUIMISCTHINGSDEF_H
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#define FABLE_PDCB_TYPED_MEMBERS
#include "engine/CParentDefClassBase.h"
#include "fable_string.h"
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/EMusicSetType.h"
namespace NUISystem {
// Layout: original complete class; native Transfer references every field.
// Original donor0x26c contracts two maps and parent base to retail0x260.
class CUIMiscThingsDef : public CParentDefClassBase {
public:
    static CDefClassBase* __fastcall Alloc();
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    CWideString SpaceSeparator; // +0x3c
    CWideString CommaSeparator; // +0x40
    CWideString ColonSeparator; // +0x44
    CWideString NewLineSeparator; // +0x48
    CWideString OpenBracket; // +0x4c
    CWideString CloseBracket; // +0x50
    CWideString Positive; // +0x54
    CWideString WeaponValueString; // +0x58
    CWideString WeaponAugString; // +0x5c
    CWideString WeaponAugNone; // +0x60
    CWideString WeaponWeightString; // +0x64
    CWideString WeaponLightString; // +0x68
    CWideString WeaponHeavyString; // +0x6c
    CWideString WeaponKillsString; // +0x70
    CWideString WeaponCatMeleeString; // +0x74
    CWideString WeaponCatRangedString; // +0x78
    CWideString WeaponDamageString; // +0x7c
    CWideString TradeCostString; // +0x80
    CWideString TradeProfitString; // +0x84
    CWideString TradeLossString; // +0x88
    CWideString TradeAlreadyOwnsString; // +0x8c
    CWideString TradeNumberInStockString; // +0x90
    CWideString TradeDeliveryString; // +0x94
    CWideString TradeNoDeliveryString; // +0x98
    CWideString TradeDaysString; // +0x9c
    CWideString TradeBuyString; // +0xa0
    CWideString TradeSellString; // +0xa4
    CWideString TradeWantedString; // +0xa8
    CWideString QuestFailedString; // +0xac
    CWideString FailedString; // +0xb0
    CWideString SucceededString; // +0xb4
    CWideString Plus; // +0xb8
    CWideString Minus; // +0xbc
    unsigned long CoreGraphic; // +0xc0
    unsigned long VignetteGraphic; // +0xc4
    unsigned long OptionalGraphic; // +0xc8
    unsigned long FeatGraphic; // +0xcc
    CWideString ObjectsRewardString; // +0xd0
    CWideString NoneString; // +0xd4
    CWideString CheckGuildString; // +0xd8
    CWideString QuestStartingString; // +0xdc
    C2DVector RingCenter; // +0xe0
    C2DVector PCRingCenter; // +0xe8
    C2DVector WorldMapOffset; // +0xf0
    float WorldMapWidth; // +0xf8
    float WorldMapHeight; // +0xfc
    CWideString YouString; // +0x100
    CWideString OwnString; // +0x104
    CWideString NoString; // +0x108
    CWideString HousesString; // +0x10c
    CWideString HouseString; // +0x110
    CWideString InString; // +0x114
    CWideString ShopsString; // +0x118
    CWideString ShopString; // +0x11c
    CWideString ThereString; // +0x120
    CWideString AreString; // +0x124
    CWideString IsString; // +0x128
    CWideString ForString; // +0x12c
    CWideString SaleString; // +0x130
    CWideString GeneralString; // +0x134
    CWideString TitleString; // +0x138
    CWideString BarberString; // +0x13c
    CWideString TatooString; // +0x140
    CWideString LevelString; // +0x144
    unsigned long TotalSpellsInPalettes; // +0x148
    unsigned long TotalSpellsInContainer; // +0x14c
    CWideString LogBookBasicsCategoryString; // +0x150
    CWideString LogBookObjectsCategoryString; // +0x154
    CWideString LogBookTownsCategoryString; // +0x158
    CWideString LogBookHeroCategoryString; // +0x15c
    CWideString LogBookCombatCategoryString; // +0x160
    CWideString LogBookQuestCategoryString; // +0x164
    CWideString LogBookStoryCategoryString; // +0x168
    CWideString LogBookBasicsCategoryNameString; // +0x16c
    CWideString LogBookObjectsCategoryNameString; // +0x170
    CWideString LogBookTownsCategoryNameString; // +0x174
    CWideString LogBookHeroCategoryNameString; // +0x178
    CWideString LogBookCombatCategoryNameString; // +0x17c
    CWideString LogBookQuestCategoryNameString; // +0x180
    CWideString LogBookStoryCategoryNameString; // +0x184
    std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,unsigned long> > > MapPaths; // +0x188
    CCharString SoundUpDown; // +0x194
    CCharString SoundSlider; // +0x198
    CCharString SoundBack; // +0x19c
    CCharString SoundForward; // +0x1a0
    CCharString SoundError; // +0x1a4
    CCharString SoundExit; // +0x1a8
    CCharString CountUpSound; // +0x1ac
    CCharString SoundKeyboardUp; // +0x1b0
    CCharString SoundKeyboardDown; // +0x1b4
    CCharString SoundKeyboardLeft; // +0x1b8
    CCharString SoundKeyboardRight; // +0x1bc
    CCharString SoundKeyboardEnterCharacter; // +0x1c0
    CCharString SoundKeyboardDeleteCharacter; // +0x1c4
    CCharString SoundKeyboardDone; // +0x1c8
    CWideString FrontEndMusic; // +0x1cc
    C2DVector HeroDollTL; // +0x1d0
    C2DVector HeroDollBR; // +0x1d8
    float HeroDollSphereRadius; // +0x1e0
    C2DVector HeroDollTL_PC; // +0x1e4
    C2DVector HeroDollBR_PC; // +0x1ec
    float HeroDollSphereRadius_PC; // +0x1f4
    C2DVector HeroDollFrameTL_PC; // +0x1f8
    float HeroDollFrameEmulateListOffset; // +0x200
    EMusicSetType QuestStartScreenMusic; // +0x204
    EMusicSetType QuestCompleteScreenMusic; // +0x208
    EMusicSetType QuestFailureScreenMusic; // +0x20c
    EMusicSetType DeathScreenMusic; // +0x210
    float DigitCountTime; // +0x214
    unsigned long SaveHeroGraphicIndex; // +0x218
    std::map<CCharString,long,std::less<CCharString>,std::allocator<std::pair<CCharString const ,long> > > MiniMapGraphics; // +0x21c
    long KeyboardSmallKeyGraphic; // +0x228
    long KeyboardLargeKeyGraphic; // +0x22c
    unsigned long TotalAssignableThings; // +0x230
    float TimeInSecsForFade; // +0x234
    float BackBufferFilterSaturation; // +0x238
    float BackBufferFilterContrast; // +0x23c
    float BackBufferFilterBrightness; // +0x240
    float BackBufferFilterTintR; // +0x244
    float BackBufferFilterTintG; // +0x248
    float BackBufferFilterTintB; // +0x24c
    float BackBufferFilterTintScale; // +0x250
    float BackBufferDiffuseScale; // +0x254
    float BackBufferAmbientScale; // +0x258
    float MinimumFilterColor; // +0x25c
};
FABLE_STATIC_ASSERT(sizeof(CUIMiscThingsDef)==0x260);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,SpaceSeparator)==0x3c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,CommaSeparator)==0x40);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,ColonSeparator)==0x44);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,NewLineSeparator)==0x48);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,OpenBracket)==0x4c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,CloseBracket)==0x50);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,Positive)==0x54);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,WeaponValueString)==0x58);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,WeaponAugString)==0x5c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,WeaponAugNone)==0x60);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,WeaponWeightString)==0x64);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,WeaponLightString)==0x68);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,WeaponHeavyString)==0x6c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,WeaponKillsString)==0x70);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,WeaponCatMeleeString)==0x74);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,WeaponCatRangedString)==0x78);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,WeaponDamageString)==0x7c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,TradeCostString)==0x80);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,TradeProfitString)==0x84);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,TradeLossString)==0x88);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,TradeAlreadyOwnsString)==0x8c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,TradeNumberInStockString)==0x90);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,TradeDeliveryString)==0x94);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,TradeNoDeliveryString)==0x98);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,TradeDaysString)==0x9c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,TradeBuyString)==0xa0);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,TradeSellString)==0xa4);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,TradeWantedString)==0xa8);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,QuestFailedString)==0xac);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,FailedString)==0xb0);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,SucceededString)==0xb4);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,Plus)==0xb8);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,Minus)==0xbc);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,CoreGraphic)==0xc0);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,VignetteGraphic)==0xc4);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,OptionalGraphic)==0xc8);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,FeatGraphic)==0xcc);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,ObjectsRewardString)==0xd0);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,NoneString)==0xd4);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,CheckGuildString)==0xd8);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,QuestStartingString)==0xdc);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,RingCenter)==0xe0);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,PCRingCenter)==0xe8);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,WorldMapOffset)==0xf0);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,WorldMapWidth)==0xf8);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,WorldMapHeight)==0xfc);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,YouString)==0x100);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,OwnString)==0x104);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,NoString)==0x108);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,HousesString)==0x10c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,HouseString)==0x110);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,InString)==0x114);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,ShopsString)==0x118);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,ShopString)==0x11c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,ThereString)==0x120);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,AreString)==0x124);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,IsString)==0x128);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,ForString)==0x12c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,SaleString)==0x130);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,GeneralString)==0x134);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,TitleString)==0x138);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,BarberString)==0x13c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,TatooString)==0x140);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,LevelString)==0x144);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,TotalSpellsInPalettes)==0x148);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,TotalSpellsInContainer)==0x14c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,LogBookBasicsCategoryString)==0x150);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,LogBookObjectsCategoryString)==0x154);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,LogBookTownsCategoryString)==0x158);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,LogBookHeroCategoryString)==0x15c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,LogBookCombatCategoryString)==0x160);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,LogBookQuestCategoryString)==0x164);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,LogBookStoryCategoryString)==0x168);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,LogBookBasicsCategoryNameString)==0x16c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,LogBookObjectsCategoryNameString)==0x170);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,LogBookTownsCategoryNameString)==0x174);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,LogBookHeroCategoryNameString)==0x178);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,LogBookCombatCategoryNameString)==0x17c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,LogBookQuestCategoryNameString)==0x180);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,LogBookStoryCategoryNameString)==0x184);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,MapPaths)==0x188);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,SoundUpDown)==0x194);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,SoundSlider)==0x198);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,SoundBack)==0x19c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,SoundForward)==0x1a0);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,SoundError)==0x1a4);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,SoundExit)==0x1a8);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,CountUpSound)==0x1ac);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,SoundKeyboardUp)==0x1b0);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,SoundKeyboardDown)==0x1b4);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,SoundKeyboardLeft)==0x1b8);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,SoundKeyboardRight)==0x1bc);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,SoundKeyboardEnterCharacter)==0x1c0);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,SoundKeyboardDeleteCharacter)==0x1c4);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,SoundKeyboardDone)==0x1c8);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,FrontEndMusic)==0x1cc);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,HeroDollTL)==0x1d0);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,HeroDollBR)==0x1d8);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,HeroDollSphereRadius)==0x1e0);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,HeroDollTL_PC)==0x1e4);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,HeroDollBR_PC)==0x1ec);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,HeroDollSphereRadius_PC)==0x1f4);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,HeroDollFrameTL_PC)==0x1f8);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,HeroDollFrameEmulateListOffset)==0x200);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,QuestStartScreenMusic)==0x204);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,QuestCompleteScreenMusic)==0x208);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,QuestFailureScreenMusic)==0x20c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,DeathScreenMusic)==0x210);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,DigitCountTime)==0x214);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,SaveHeroGraphicIndex)==0x218);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,MiniMapGraphics)==0x21c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,KeyboardSmallKeyGraphic)==0x228);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,KeyboardLargeKeyGraphic)==0x22c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,TotalAssignableThings)==0x230);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,TimeInSecsForFade)==0x234);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,BackBufferFilterSaturation)==0x238);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,BackBufferFilterContrast)==0x23c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,BackBufferFilterBrightness)==0x240);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,BackBufferFilterTintR)==0x244);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,BackBufferFilterTintG)==0x248);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,BackBufferFilterTintB)==0x24c);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,BackBufferFilterTintScale)==0x250);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,BackBufferDiffuseScale)==0x254);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,BackBufferAmbientScale)==0x258);
FABLE_STATIC_ASSERT(offsetof(CUIMiscThingsDef,MinimumFilterColor)==0x25c);
}
#endif
