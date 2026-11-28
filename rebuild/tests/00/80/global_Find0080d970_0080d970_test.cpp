// Leaf: ??$__find@PAPBVCThing@@PAVCThingCreatureBase@@@std@@YIPAPBVCThing@@PAPBV1@0ABQAVCThingCreatureBase@@ABUrandom_access_iterator_tag@0@@Z
// Flags: /Gr
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#include "engine/CThingCreatureBase.h"
#include "stlport/fable_algorithm.h"
#include <stdio.h>
union TestStorage { double aligned; unsigned char bytes[0x600]; };
int main(){
    TestStorage objects[4];CThingCreatureBase* values[4];const CThing* list[17];
    for(unsigned k=0;k<4;++k)values[k]=reinterpret_cast<CThingCreatureBase*>(objects[k].bytes);
    for(unsigned n=0;n<=17;++n)for(unsigned pos=0;pos<=n;++pos)for(unsigned repeated=0;repeated<2;++repeated){
        CThingCreatureBase* needle=values[3];
        for(unsigned k=0;k<n;++k)list[k]=(k==pos||(repeated&&k>pos&&!(k%3)))?needle:values[k%3];
        if(std::__find(list,list+n,needle,std::random_access_iterator_tag())!=list+pos)return 1;
    }
    puts("0080d970_TEST PASS");return 0;
}
