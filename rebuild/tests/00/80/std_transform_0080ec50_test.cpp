// Leaf: ??$transform@PAV?$CIntelligentPointer@VCThing@@@@V?$back_insert_iterator@V?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@@std@@VCGetThingFromIntelligentPointer@?A0x
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
#define FABLE_RETAIL_EXPLOSION_HITS_PASS29
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_BACK_INSERTER
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_TRANSFORM_PASS30
#include "engine/CThingCreatureBase.h"
#include "engine/tcd_explosion_filters.h"
#include "stlport/fable_algorithm.h"
#include <stdio.h>
union TestStorage { double aligned; unsigned char bytes[0x600]; };
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{return reinterpret_cast<CBaseObject*>(PData);}
void CBaseIntelligentPointer::SetPItem(CBaseObject* p){PData=reinterpret_cast<CBaseObjectPointer*>(p);}
int main(){
    TestStorage objects[7];CThing* things[7];CIntelligentPointer<CThing> values[12];
    for(unsigned i=0;i<7;++i)things[i]=reinterpret_cast<CThing*>(objects[i].bytes);
    for(unsigned n=0;n<=12;++n)for(unsigned mask=0;mask<16;++mask){
        std::vector<const CThing*> out;out.push_back(things[2]);
        for(unsigned k=0;k<n;++k)values[k].Set(mask&(1u<<(k%4))?things[k%7]:0);
        std::back_insert_iterator<std::vector<const CThing*> > at=std::transform(values,values+n,std::back_inserter(out),CGetThingFromIntelligentPointer());
        *at=things[1];
        if(out.size()!=n+2||out[0]!=things[2]||out[n+1]!=things[1])return 1;
        for(unsigned i=0;i<n;++i)if(out[i+1]!=values[i].Get())return 1;
    }
    puts("0080ec50_TEST PASS");return 0;
}
