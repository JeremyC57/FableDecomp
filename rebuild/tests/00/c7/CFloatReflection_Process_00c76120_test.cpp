#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
struct CFloatReflectionParams {unsigned m_auiInputDelay[2];unsigned m_uiFeedbackDelay;float m_fFeedbackGain,m_fOutputGain;};
class CFloatCyclicBuffer {
public:
 float* m_pBase;unsigned length,write;
 unsigned GetWriteCursor()const{return write;}unsigned GetLength()const{return length;}
 void SetWriteCursor(unsigned v){write=v;}
};
class CFloatReflection {
public:
 CFloatCyclicBuffer m_Buffer;
 void Process(CFloatReflectionParams*,const CFloatReflectionParams*,CFloatCyclicBuffer*,float*,float*,unsigned,unsigned);
};
void CFloatReflection::Process(CFloatReflectionParams* pParams0, const CFloatReflectionParams* pParams1,
                              CFloatCyclicBuffer* pInputBuffer, float* pfInternalBuffer, float* pfOutputBuffer,
                              unsigned uiSamples, unsigned uiSamplesToFade)
{
    float* pfBase = m_Buffer.m_pBase;
    unsigned iWrFeedbackCursor = m_Buffer.GetWriteCursor();
    float* pfInputBase = pInputBuffer->m_pBase;
    unsigned iRdFeedbackCursor0 = iWrFeedbackCursor >= pParams0->m_uiFeedbackDelay
        ? iWrFeedbackCursor - pParams0->m_uiFeedbackDelay
        : iWrFeedbackCursor - pParams0->m_uiFeedbackDelay + m_Buffer.GetLength();
    unsigned iRdFeedbackCursor1 = iWrFeedbackCursor >= pParams1->m_uiFeedbackDelay
        ? iWrFeedbackCursor - pParams1->m_uiFeedbackDelay
        : iWrFeedbackCursor - pParams1->m_uiFeedbackDelay + m_Buffer.GetLength();
    unsigned iWrInputCursor = pInputBuffer->GetWriteCursor() >= uiSamples
        ? pInputBuffer->GetWriteCursor() - uiSamples
        : pInputBuffer->GetWriteCursor() - uiSamples + pInputBuffer->GetLength();
    float fFadeOut = (float)uiSamplesToFade / 512.0f;
    float fFadeIn = 1.0f - fFadeOut;
    unsigned uiSamplesToProcess = uiSamples;
    if (uiSamplesToFade < uiSamples)
        uiSamplesToProcess = uiSamplesToFade;
    for (unsigned i = 0; i < uiSamplesToProcess; ++i) {
        unsigned iRdInputCursor0 = iWrInputCursor >= pParams0->m_auiInputDelay[0]
            ? iWrInputCursor - pParams0->m_auiInputDelay[0]
            : iWrInputCursor - pParams0->m_auiInputDelay[0] + pInputBuffer->GetLength();
        unsigned iRdInputCursor1 = iWrInputCursor >= pParams1->m_auiInputDelay[0]
            ? iWrInputCursor - pParams1->m_auiInputDelay[0]
            : iWrInputCursor - pParams1->m_auiInputDelay[0] + pInputBuffer->GetLength();
        float fSample0 = pfInputBase[iRdInputCursor0];
        float fSample1 = pfInputBase[iRdInputCursor1];
        *pfInternalBuffer += fSample0*fFadeOut + fSample1*fFadeIn;
        ++pfInternalBuffer;
        for (unsigned iInput = 1; iInput > 0; --iInput) {
            iRdInputCursor0 = iWrInputCursor >= pParams0->m_auiInputDelay[iInput]
                ? iWrInputCursor - pParams0->m_auiInputDelay[iInput]
                : iWrInputCursor - pParams0->m_auiInputDelay[iInput] + pInputBuffer->GetLength();
            iRdInputCursor1 = iWrInputCursor >= pParams1->m_auiInputDelay[iInput]
                ? iWrInputCursor - pParams1->m_auiInputDelay[iInput]
                : iWrInputCursor - pParams1->m_auiInputDelay[iInput] + pInputBuffer->GetLength();
            fSample0 += pfInputBase[iRdInputCursor0];
            fSample1 += pfInputBase[iRdInputCursor1];
        }
        fSample0 += pfBase[iRdFeedbackCursor0] * pParams0->m_fFeedbackGain;
        fSample1 += pfBase[iRdFeedbackCursor1] * pParams1->m_fFeedbackGain;
        // Bracket: the grouped feedback blend keeps the SDK feedback-store and cursor-update instruction order.
        pfBase[iWrFeedbackCursor] = (fSample0*fFadeOut + fSample1*fFadeIn);
        *pfOutputBuffer += fFadeOut*(fSample0*pParams0->m_fOutputGain)
                         + fFadeIn*(fSample1*pParams1->m_fOutputGain);
        ++pfOutputBuffer;
        iRdFeedbackCursor0 = iRdFeedbackCursor0+1 < m_Buffer.GetLength()
            ? iRdFeedbackCursor0+1 : iRdFeedbackCursor0+1-m_Buffer.GetLength();
        iRdFeedbackCursor1 = iRdFeedbackCursor1+1 < m_Buffer.GetLength()
            ? iRdFeedbackCursor1+1 : iRdFeedbackCursor1+1-m_Buffer.GetLength();
        iWrFeedbackCursor = iWrFeedbackCursor+1 < m_Buffer.GetLength()
            ? iWrFeedbackCursor+1 : iWrFeedbackCursor+1-m_Buffer.GetLength();
        iWrInputCursor = iWrInputCursor+1 < pInputBuffer->GetLength()
            ? iWrInputCursor+1 : iWrInputCursor+1-pInputBuffer->GetLength();
        if (fFadeOut > 1.0f/512.0f) {
            fFadeIn += 1.0f/512.0f;
            fFadeOut -= 1.0f/512.0f;
        }
    }
    m_Buffer.SetWriteCursor(iWrFeedbackCursor);
    if (uiSamples >= uiSamplesToFade)
        *pParams0 = *pParams1;
}
static bool same(float x,float y){return fabs(x-y)<=0.000004*(1+fabs(y));}
static void test(unsigned inputLen,unsigned ringLen,unsigned count,unsigned fade,unsigned read,unsigned write,unsigned variation){
 float input[8],savedInput[8],feedback[8],expectFeedback[8],internal[9],out[9],expectInternal[9],expectOut[9];
 for(int j=0;j<8;++j){input[j]=savedInput[j]=float(j-3)*0.5f;feedback[j]=expectFeedback[j]=float(4-j)*0.25f;}
 for(int k=0;k<9;++k){internal[k]=expectInternal[k]=float(k+2);out[k]=expectOut[k]=float(2-k);}
 CFloatReflection engine;engine.m_Buffer.m_pBase=feedback;engine.m_Buffer.write=write;engine.m_Buffer.length=ringLen;
 CFloatCyclicBuffer in;in.m_pBase=input;in.write=read;in.length=inputLen;
 CFloatReflectionParams a,b;a.m_auiInputDelay[0]=variation%(inputLen+1);a.m_auiInputDelay[1]=inputLen;b.m_auiInputDelay[0]=0;b.m_auiInputDelay[1]=inputLen-1;
 a.m_uiFeedbackDelay=variation%(ringLen+1);b.m_uiFeedbackDelay=ringLen;b.m_fFeedbackGain=-0.25f;a.m_fFeedbackGain=0.5f;
 a.m_fOutputGain=2.0f;b.m_fOutputGain=-1.0f;CFloatReflectionParams initial=a,other=b;
 unsigned limit=count<fade?count:fade;
 for(unsigned i=0;i<limit;++i){
  unsigned pos=(inputLen+read-count+i)%inputLen,ring=(write+i)%ringLen;
  float f0=float(fade-i)/512.0f,f1=1.0f-f0;
  float v0=input[(inputLen+pos-a.m_auiInputDelay[0])%inputLen],v1=input[(inputLen+pos-b.m_auiInputDelay[0])%inputLen];
  expectInternal[1+i]+=v0*f0+v1*f1;
  v0+=input[(inputLen+pos-a.m_auiInputDelay[1])%inputLen];v1+=input[(inputLen+pos-b.m_auiInputDelay[1])%inputLen];
  v0+=expectFeedback[(ringLen+ring-a.m_uiFeedbackDelay)%ringLen]*a.m_fFeedbackGain;
  v1+=expectFeedback[(ringLen+ring-b.m_uiFeedbackDelay)%ringLen]*b.m_fFeedbackGain;
  expectFeedback[ring]=v0*f0+v1*f1;expectOut[1+i]+=v0*a.m_fOutputGain*f0+v1*b.m_fOutputGain*f1;
 }
 engine.Process(&a,&b,&in,internal+1,out+1,count,fade);
 assert(in.GetWriteCursor()==read&&engine.m_Buffer.GetWriteCursor()==(write+limit)%ringLen);
 assert(!memcmp(savedInput,input,sizeof input)&&!memcmp(&other,&b,sizeof b)&&!memcmp(&a,count>=fade?&other:&initial,sizeof a));
 for(unsigned k0=0;k0<8;++k0)assert(same(feedback[k0],expectFeedback[k0]));
 for(unsigned k1=0;k1<9;++k1){assert(same(out[k1],expectOut[k1]));assert(same(internal[k1],expectInternal[k1]));}
}
int main(){
 unsigned fades[]={0,1,2,4,7,256,511,512};unsigned cases=0;
 for(unsigned a=1;a<=7;a+=2)for(unsigned b=1;b<=7;b+=2)for(unsigned c=0;c<=a;++c)
 for(unsigned f=0;f<8;++f)for(unsigned r=0;r<a;++r)for(unsigned w=0;w<b;++w)for(unsigned v=0;v<4;++v){test(a,b,c,fades[f],r,w,v);++cases;}
 assert(cases==51200);puts("00c76120_TEST PASS");return 0;
}