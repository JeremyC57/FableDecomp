#define FABLE_RETAIL_ARENA_FILL_UNINITIALIZED_COPY_PASS29
#define FABLE_RETAIL_STLP_FALSE_TAG_POD
#include <stdio.h>
#include <stdlib.h>
#include <new>
#include "rebuild_abi.h"
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#define FABLE_RETAIL_ARENA_INSERT_ADVANCE_PASS29
#define FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
#include "stlport/fable_containers.h"
struct Item {
    int NumWaveCreatures,Creatures,ShortWave;
    static int live;
    Item(int v=0):NumWaveCreatures(v),Creatures(v*17+3),ShortWave(v*7+1){++live;}
    Item(const Item& r):NumWaveCreatures(r.NumWaveCreatures),Creatures(r.Creatures),ShortWave(r.ShortWave){++live;}
    ~Item(){--live;}
    bool equal(int v) const{return NumWaveCreatures==v && Creatures==v*17+3 && ShortWave==v*7+1;}
};
int Item::live=0;

class FillVector: public std::vector<Item> {
public:
    FillVector(unsigned n,unsigned cap) {
        _M_start=static_cast<Item*>(malloc((cap+1)*sizeof(Item)));
        _M_finish=_M_start;
        _M_end_of_storage._M_data=_M_start+cap;
        for(unsigned i=0;i<n;++i) new(_M_finish++) Item(i+1);
    }
    void write(unsigned pos,unsigned n,const Item& item) {
        _M_fill_insert(begin()+pos,n,item);
    }
};
int main(){
 for(unsigned size=0;size<=8;++size)for(unsigned spare=0;spare<=6;++spare)
 for(unsigned pos=0;pos<=size;++pos)for(unsigned n=0;n<=7;++n)
 for(unsigned alias=0;alias<=size;++alias){
  {
   FillVector v(size,size+spare);
   Item external(73);
   const int value=alias ? alias : 73;
   Item* before=v.begin();
   v.write(pos,n,alias ? v[alias-1] : external);
   if(v.size()!=size+n||(v.begin()!=before)!=(n>spare))return 1;
   for(unsigned j=0;j<size+n;++j)
     if(!v[j].equal(j<pos ? j+1 : j<pos+n ? value : j-n+1))return 2;
   if(Item::live!=int(size+n+1))return 3;
  }
  if(Item::live)return 4;
 }
 puts("00f281e0_TEST PASS");return 0;
}