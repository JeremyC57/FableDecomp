// Behaviour test for CTCShop::StockDisplay (0x00774C60): the row body over models of its callees (the two vector producers and
// AddSaleItem). Kills: GetFreeSaleItemPlaces asked for blocked places (true), a producer skipped, the pairs mismatched (item n with
// place n), the count not min(places, items) (off by one, max, either size alone), and a wrong item / place passed.
#include <stdio.h>
#include <stdlib.h>
#include <vector>
namespace std { template<class T> inline const T& min(const T& a, const T& b) { return b < a ? b : a; } }   // STLport's std::min (VC7.1's library lacks it)
static void check(bool v){if(!v){puts("FAIL");exit(1);}}
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up, Forward; };
static int free_calls, items_calls, add_calls; static bool free_arg; static size_t n_places, n_items;
static long added_item[64]; static const void* added_place[64]; static float added_x[64];
class CTCShop {
public:
    struct GoodsPlace { C3DVector pos; CRightHandedSet rhset; };
    std::vector<GoodsPlace> GetFreeSaleItemPlaces(bool allow_blocked)
    { ++free_calls; free_arg = allow_blocked; std::vector<GoodsPlace> v; for (size_t i = 0; i != n_places; ++i) { GoodsPlace g; g.pos.X = 100.0f + i; g.pos.Y = g.pos.Z = 0; v.push_back(g); } return v; }
    std::vector<long> GetSaleItemsToPlace() const
    { ++items_calls; std::vector<long> v; for (size_t i = 0; i != n_items; ++i) v.push_back(1000 + (long)i * 7); return v; }
    void AddSaleItem(long item, const GoodsPlace& place) { added_item[add_calls] = item; added_place[add_calls] = &place; added_x[add_calls] = place.pos.X; ++add_calls; }
    void StockDisplay();
};
void CTCShop::StockDisplay()
{
    std::vector<GoodsPlace> goods_places = GetFreeSaleItemPlaces(false);
    std::vector<long> goods_to_place = GetSaleItemsToPlace();
    for (long n = 0; n < std::min(goods_places.size(), goods_to_place.size()); n++)
        AddSaleItem(goods_to_place[n], goods_places[n]);
}
int main(){
 long cases=0;CTCShop shop;
 for(size_t p=0;p!=7;++p)for(size_t i=0;i!=7;++i){
  n_places=p;n_items=i;free_calls=items_calls=add_calls=0;free_arg=true;
  shop.StockDisplay();
  check(free_calls==1&&!free_arg&&items_calls==1);
  size_t m=p<i?p:i;check(add_calls==(int)m);
  for(size_t k=0;k!=m;++k)check(added_item[k]==1000+(long)k*7&&added_x[k]==100.0f+k);
  ++cases;
 }
 check(cases==49);puts("00774c60_TEST PASS");return 0;
}