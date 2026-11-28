// Leaf: ?_M_insert_overflow@
// Flags: /Gr
#define FABLE_RETAIL_C3DVECTOR
#include "engine/CCamera.h"
#include "stlport/fable_containers.h"

template void std::vector<CCamera>::_M_insert_overflow(CCamera*, const CCamera&, const std::__false_type&, unsigned int, bool);

#include <cstdio>
#include <cstring>
#include <cassert>
int main(){std::vector<CCamera> cameras;unsigned long raw[14];for(int i=0;i<100;++i){std::memset(raw,0,sizeof(raw));raw[0]=(unsigned)i;raw[13]=i+1;CCamera& x=*(CCamera*)raw;cameras.push_back(x);assert(cameras.size()==unsigned(i+1));for(int j=0;j<=i;++j){const unsigned long* p=(const unsigned long*)&cameras[j];assert(p[0]==unsigned(j)&&p[13]==unsigned(j+1));}}std::puts("00b29840_TEST PASS");return 0;}