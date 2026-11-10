// CTCShop::StockDisplay -- place available sale items into free display positions.
// Grade: DIFFER(188v188) -- this esi vs ebx; counts ebx/ebp vs esi/edi; index edi vs ebp; two loop argument loads exchanged
// Retail: 0x00774C60, 188 bytes; thiscall.
// Leaf: ?StockDisplay@CTCShop@@QAEXXZ
// Flags: none
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSHOP
#include "engine/CTCShop.h"
void CTCShop::StockDisplay()
{
 std::vector<GoodsPlace> goods_places=GetFreeSaleItemPlaces(false);
 std::vector<long> goods_to_place=GetSaleItemsToPlace();
 long n=0;
 unsigned int numItems=goods_to_place.size();
 unsigned int numPlaces=goods_places.size();
 for(std::vector<GoodsPlace>::iterator place=goods_places.begin();n<std::min(numPlaces,numItems);++place){
  const long goods=goods_to_place[n];
  const GoodsPlace& goods_place=*place;
  AddSaleItem(goods,goods_place);
  ++n;
 }
}
