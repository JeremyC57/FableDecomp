// Leaf: ?push_back@?$list@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAEXABVC3DVector@@@Z
// Flags:
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "stlport/fable_containers.h"
template void std::list<C3DVector>::push_back(const C3DVector&);

#include <stdio.h>
#include <string.h>
typedef C3DVector Entry;
int main() {
    std::list<Entry> values;
    for (int round=0;round<3;++round) {
        if (!values.empty()) return 1;
        for(int i=0;i<11;++i) {
            Entry v;
            v.X=float(i);v.Y=float(-i);v.Z=float(9-i);
            values.push_back(v);
        }
        int n=0;
        for(std::list<Entry>::iterator it=values.begin();it!=values.end();++it,++n){
            if(it->X!=float(n)||it->Y!=float(-n)||it->Z!=float(9-n))return 2;
        }
        if(n!=11)return 3;
        values.clear();
    }
    puts("006a4ab0_TEST PASS"); return 0;
}
