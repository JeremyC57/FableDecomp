// Leaf: ?push_back@?$list@U?$pair@VC2DVector@@J@std@@V?$allocator@U?$pair@VC2DVector@@J@std@@@2@@std@@QAEXABU?$pair@VC2DVector@@J@2@@Z
// Flags:
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "stlport/fable_containers.h"
typedef std::pair<C2DVector, long> Pulse;
template void std::list<Pulse>::push_back(const Pulse&);

#include <stdio.h>
#include <string.h>
typedef std::pair<C2DVector,long> Entry;
int main() {
    std::list<Entry> values;
    for (int round=0;round<3;++round) {
        if (!values.empty()) return 1;
        for(int i=0;i<11;++i) {
            Entry v;
            v.first.X=float(i);v.first.Y=float(-i);v.second=9-i;
            values.push_back(v);
        }
        int n=0;
        for(std::list<Entry>::iterator it=values.begin();it!=values.end();++it,++n){
            if(it->first.X!=float(n)||it->first.Y!=float(-n)||it->second!=9-n)return 2;
        }
        if(n!=11)return 3;
        values.clear();
    }
    puts("006a4a70_TEST PASS"); return 0;
}
