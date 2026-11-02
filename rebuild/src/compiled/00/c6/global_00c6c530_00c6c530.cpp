// CSalIIRFilter::Process(short* pOutput,const short* pInput) -- Process a 128-sample audio block.
// Leaf: ?Process@CSalIIRFilter@@QAEXPAFPBF@Z
#include "engine/CSalIIRFilter.h"
#include <string.h>
inline short clip16(const int value)
{
    if(value>32767)return 32767;
    if(value<-32768)return -32768;
    return (short)value;
}
void CSalIIRFilter::Process(short* pOutput,const short* pInput)
{
    if(m_iA==0) {
        memcpy(pOutput,pInput,128*sizeof(short));
        return;
    }
    if(m_iB!=0) {
    int iZ=m_iZ;
    for(unsigned int i=0;i<32;++i) {
        iZ=(m_iA*iZ+m_iB*(*pInput++))/32768;
        *pOutput++=clip16(iZ);
        iZ=(m_iA*iZ+m_iB*(*pInput++))/32768;
        *pOutput++=clip16(iZ);
        iZ=(m_iA*iZ+m_iB*(*pInput++))/32768;
        *pOutput++=clip16(iZ);
        iZ=(m_iA*iZ+m_iB*(*pInput++))/32768;
        *pOutput++=clip16(iZ);
    }
    m_iZ=iZ;
    } else {
        memset(pOutput,0,128*sizeof(short));
    }
}