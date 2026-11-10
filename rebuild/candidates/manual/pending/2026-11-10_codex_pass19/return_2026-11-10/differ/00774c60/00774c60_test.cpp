#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#ifdef _MSC_VER
namespace std{template<class T>inline const T& min(const T& a,const T& b){return b<a?b:a;}}
#endif
static void check(bool ok){if(!ok)std::abort();}
struct C3DVector{float X,Y,Z;};
struct CRightHandedSet{C3DVector Up,Forward;};
struct CTCShop{
 struct GoodsPlace{C3DVector pos;CRightHandedSet rhset;};
 std::vector<GoodsPlace> places;
 std::vector<long> goods;
 mutable int phase;
 int calls;
 std::vector<GoodsPlace> GetFreeSaleItemPlaces(bool taken){check(!taken&&phase==0);phase=1;return places;}
 std::vector<long> GetSaleItemsToPlace()const{check(phase==1);phase=2;return goods;}
 void AddSaleItem(long item,const GoodsPlace& place){
  check(phase==2&&calls<(int)places.size()&&calls<(int)goods.size());
  check(item==goods[calls]);const GoodsPlace& expected=places[calls];
  check(place.pos.X==expected.pos.X&&place.pos.Y==expected.pos.Y&&place.pos.Z==expected.pos.Z);
  check(place.rhset.Up.X==expected.rhset.Up.X&&place.rhset.Up.Y==expected.rhset.Up.Y&&place.rhset.Up.Z==expected.rhset.Up.Z);
  check(place.rhset.Forward.X==expected.rhset.Forward.X&&place.rhset.Forward.Y==expected.rhset.Forward.Y&&place.rhset.Forward.Z==expected.rhset.Forward.Z);
  ++calls;
 }
 void StockDisplay();
};
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

int main(){int cases=0;
 for(int pc=0;pc<=16;++pc)for(int gc=0;gc<=16;++gc)for(int seed=-1;seed<=1;++seed){
  CTCShop shop;shop.phase=0;shop.calls=0;
  for(int i=0;i<pc;++i){CTCShop::GoodsPlace place;
   place.pos.X=float(i+seed)*0.25f;place.pos.Y=float(i-seed)*0.5f;place.pos.Z=float(-i);
   place.rhset.Up.X=float(seed);place.rhset.Up.Y=float(i);place.rhset.Up.Z=1.0f;
   place.rhset.Forward.X=-1.0f;place.rhset.Forward.Y=float(-seed);place.rhset.Forward.Z=float(i)*0.125f;
   shop.places.push_back(place);
  }
  for(int j=0;j<gc;++j)shop.goods.push_back((j-8)*(seed+2));
  shop.StockDisplay();check(shop.phase==2&&shop.calls==(pc<gc?pc:gc));
  check(shop.places.size()==(unsigned int)pc&&shop.goods.size()==(unsigned int)gc);++cases;
 }
 check(cases==867);std::puts("00774c60_TEST PASS");return 0;}
