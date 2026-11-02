#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef FABLE_ENGINE_CSALIIRFILTER_H
#define FABLE_ENGINE_CSALIIRFILTER_H
// GameCODA debug CodeView: class V, size12, private int members +0/+4/+8.
// Named release object Process overloads are exact retail byte twins.
class CSalIIRFilter {
public:
    CSalIIRFilter();
    void SetLowPass(int iLevelHF,float fFrequencyRef);
    void Clear();
    bool IsSilent() const;
    void Process(short* pOutput,const short* pInput);
    void Process(short* pBuffer);
private:
    int m_iA;
    int m_iB;
    int m_iZ;
};
#endif

// CSalIIRFilter::Process(short* pBuffer) -- Process a 128-sample audio block.
// Leaf: ?Process@CSalIIRFilter@@QAEXPAF@Z

#include <string.h>
inline short clip16(const int value)
{
    if(value>32767)return 32767;
    if(value<-32768)return -32768;
    return (short)value;
}
void CSalIIRFilter::Process(short* pBuffer)
{
    if(m_iA==0) {
        
        return;
    }
    if(m_iB!=0) {
    int iZ=m_iZ;
    for(unsigned int i=0;i<32;++i) {
        iZ=(m_iA*iZ+m_iB*(*pBuffer))/32768;
        *pBuffer++=clip16(iZ);
        iZ=(m_iA*iZ+m_iB*(*pBuffer))/32768;
        *pBuffer++=clip16(iZ);
        iZ=(m_iA*iZ+m_iB*(*pBuffer))/32768;
        *pBuffer++=clip16(iZ);
        iZ=(m_iA*iZ+m_iB*(*pBuffer))/32768;
        *pBuffer++=clip16(iZ);
    }
    m_iZ=iZ;
    } else {
        memset(pBuffer,0,128*sizeof(short));
    }
}

CSalIIRFilter::CSalIIRFilter():m_iA(0),m_iZ(0){}
static unsigned int randomState=0x12345678U;
static int sample() { randomState=randomState*1664525U+1013904223U; return (int)(randomState>>16)-32768; }
static short clipped(int value) { if(value>32767)value=32767; if(value<-32768)value=-32768; return (short)value; }
static void die(int reason) { fprintf(stderr,"audio check %d\n",reason); exit(reason); }
int main()
{
    if(sizeof(CSalIIRFilter)!=12||sizeof(int)!=4||sizeof(short)!=2)die(1);
    const int coefficients[8][2]={{0,0},{0,32768},{32768,0},{16384,16384},{24576,8192},{8192,24576},{1,32768},{-8192,16384}};
    const int histories[7]={0,1,-1,32767,-32768,65535,-65535};
    int c,h,pattern,block,i;
    for(c=0;c<8;++c)for(h=0;h<7;++h)for(pattern=0;pattern<4;++pattern) {
        CSalIIRFilter filter; int state[3]={coefficients[c][0],coefficients[c][1],histories[h]};
        memcpy(&filter,state,sizeof(state)); int expectedState=state[2];
        for(block=0;block<3;++block) {
            short input[130],output[130],expected[128];
            input[0]=output[0]=12345;input[129]=output[129]=-12345;
            for(i=0;i<128;++i) {
                input[i+1]=(short)(pattern==0?0:pattern==1?((i&1)?32767:-32768):pattern==2?(i==0?32767:0):sample());
                output[i+1]=-23456;
                if(state[0]==0)expected[i]=input[i+1];
                else if(state[1]==0)expected[i]=0;
                else {
                    double sum=(double)state[0]*expectedState+(double)state[1]*input[i+1];
                    if(sum>2147483647.0||sum< -2147483648.0)die(2);
                    expectedState=(int)(sum/32768.0); expected[i]=clipped(expectedState);
                }
            }
            memcpy(output+1,input+1,128*sizeof(short));
            filter.Process(output+1);

            for(i=0;i<128;++i)if(output[i+1]!=expected[i])die(3);
            int observed[3];memcpy(observed,&filter,sizeof(observed));
            if(observed[0]!=state[0]||observed[1]!=state[1]||observed[2]!=expectedState)die(4);
            if(input[0]!=12345||input[129]!=-12345||output[0]!=12345||output[129]!=-12345)die(5);
        }
    }
    puts("00c6c0f0_TEST PASS");return 0;
}