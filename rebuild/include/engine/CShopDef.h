#ifndef FABLE_ENGINE_CSHOPDEF_H
#define FABLE_ENGINE_CSHOPDEF_H
#include "engine/CSubDefClassBase.h"
#include "fable_string.h"
#include "engine/CShopItemDef.h"
#include "stlport/fable_map.h"
enum EOpinionAttitudeType;
// Complete original CShopDef data from both PDBs, native STLport containers.
class CShopDef:public CSubDefClassBase {
public:
 virtual void Copy(const CDefClassBase*);
 virtual unsigned long GetSizeofClass();
 static CDefClassBase* FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL StatGetTypeName();
 unsigned long Name;
 unsigned long InitialHawkingPhrase;
 unsigned long ReplyToRequestPhrase;
 unsigned long CustomerFollowPhrase;
 unsigned long ThankingPhrase;
 CCharString ShopkeeperMeshType;
 std::vector<CShopItemDef,std::allocator<CShopItemDef> > DefaultStock;
 std::vector<CShopItemDef,std::allocator<CShopItemDef> > StockCategoryDefaults;
 float BuyPriceMultiplier;
 float SellPriceMultiplier;
 float AttitudeBuyPriceMultiplierDefault;
 std::map<enum EOpinionAttitudeType,float,std::less<enum EOpinionAttitudeType>,std::allocator<std::pair<enum EOpinionAttitudeType const ,float> > > AttitudeBuyPriceMultiplier;
 float AttitudeSellPriceMultiplierDefault;
 std::map<enum EOpinionAttitudeType,float,std::less<enum EOpinionAttitudeType>,std::allocator<std::pair<enum EOpinionAttitudeType const ,float> > > AttitudeSellPriceMultiplier;
 float MaxStockChangePerSell;
 float MaxStockChangePerBuy;
 float MaxStockRevertPerDay;
 float PriceReductionFractionPerItem;
 float MaxPriceReductionFractionDueToStockCount;
 unsigned long PreferredShopkeeper;
 bool IsStockDisplayPermanent;
 bool IsBarberShop;
 bool IsTattooShop;
 bool IsTitleShop;
 bool AlwaysOpen;
 float MinPriceMultiplierForItemToBeWanted;
 float MaxPriceToDisplay;
 float MinTimeToSteal;
 float MaxTimeToSteal;
 long MinGoldForDonation;
 long MaxGoldForDonation;
 float MaxMoralityForDonation;
 float LogarithmicPower;
 long NumDaysToKeepSpecialItem;
 long MinAmountBeforeSpecialItem;
 float ProbabilityOfSpecialItemPerDay;
 float SpecialItemPriceMult;
 virtual void Transfer(CPersistContext&);
 void Test()const;
 CShopDef(const CShopDef&);
 CShopDef();
 virtual ~CShopDef();
 CShopDef& operator=(const CShopDef&);
};
FABLE_STATIC_ASSERT(sizeof(CShopDef)==0xd0);
FABLE_STATIC_ASSERT(offsetof(CShopDef,IsStockDisplayPermanent)==0x98);
#endif
