// Standalone behaviour fixture; grading uses the complete shared manager hierarchy.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#define CHECK(c) do{if(!(c)){printf("FAIL %d\n",__LINE__);abort();}}while(0)
class CEngineScene {};
template<class T> class CArray {public: T* First;T* Last;T* End;};
class CGraphicDataBank {};
class C4DVector {public:float X,Y,Z,W;C4DVector(){} C4DVector(float x,float y,float z,float w):X(x),Y(y),Z(z),W(w){}};
class C2DBoxF {public:float TLX,TLY,BRX,BRY;C2DBoxF(){}};
class C2DAlphaMaskInfo {
public:
    C2DAlphaMaskInfo(){Reset();}
    void Reset(){
        AlphaMaskBank=0;
        AlphaMaskBankIndex=0;
        AlphaMaskPositionFactors=C4DVector(0.0f,0.0f,0.0f,0.0f);
        MaskObjectsOutsideAlphaMask=false;
        FilterMask=false;
    }
    const CGraphicDataBank* AlphaMaskBank;
    unsigned long AlphaMaskBankIndex;
    C4DVector AlphaMaskPositionFactors;
    C2DBoxF AlphaMaskClipBox;
    bool MaskObjectsOutsideAlphaMask,FilterMask;
};
static int baseCalls;
static CArray<CEngineScene*>* expectedScenes;
class CEnginePrimitiveManager {
public:
    CEnginePrimitiveManager(CArray<CEngineScene*>& scenes):SceneList(scenes),Enable(true){CHECK(&scenes==expectedScenes);++baseCalls;}
    virtual ~CEnginePrimitiveManager(){}
    CArray<CEngineScene*>& SceneList;
    bool Enable;
};
class CEnginePrimitive2DViewportManager:public CEnginePrimitiveManager {
public:
    CEnginePrimitive2DViewportManager(CArray<CEngineScene*>&);
    C2DAlphaMaskInfo AlphaMaskInfo;
};
CEnginePrimitive2DViewportManager::CEnginePrimitive2DViewportManager(CArray<CEngineScene*>& scenes)
    : CEnginePrimitiveManager(scenes)
{
}
static void CheckReset(const C2DAlphaMaskInfo& v){
 CHECK(v.AlphaMaskBank==0 && v.AlphaMaskBankIndex==0);
 CHECK(v.AlphaMaskPositionFactors.X==0.0f && v.AlphaMaskPositionFactors.Y==0.0f && v.AlphaMaskPositionFactors.Z==0.0f && v.AlphaMaskPositionFactors.W==0.0f);
 CHECK(!v.MaskObjectsOutsideAlphaMask && !v.FilterMask);
}
union Storage {long double alignment;void* pointer;unsigned char bytes[sizeof(CEnginePrimitive2DViewportManager)];};
int main(){
 CArray<CEngineScene*> arrays[2];memset(arrays,0,sizeof arrays);
 const unsigned char fills[]={0,0x33,0x55,0xA5,0xFF};
 for(int a=0;a<2;++a)for(int f=0;f<5;++f){
  Storage raw;memset(raw.bytes,fills[f],sizeof raw.bytes);expectedScenes=&arrays[a];baseCalls=0;
  CEnginePrimitive2DViewportManager* p=new(raw.bytes) CEnginePrimitive2DViewportManager(arrays[a]);
  CHECK(baseCalls==1 && &p->SceneList==&arrays[a] && p->Enable);
  CheckReset(p->AlphaMaskInfo);
  const unsigned char* box=(const unsigned char*)&p->AlphaMaskInfo.AlphaMaskClipBox;
  for(unsigned int i=0;i<sizeof(C2DBoxF);++i)CHECK(box[i]==fills[f]);
  CGraphicDataBank bank; p->AlphaMaskInfo.AlphaMaskBank=&bank;p->AlphaMaskInfo.AlphaMaskBankIndex=27;
  p->AlphaMaskInfo.AlphaMaskPositionFactors=C4DVector(1,2,3,4);
  p->AlphaMaskInfo.MaskObjectsOutsideAlphaMask=true;p->AlphaMaskInfo.FilterMask=true;
  p->AlphaMaskInfo.AlphaMaskClipBox.TLX=-4;p->AlphaMaskInfo.AlphaMaskClipBox.TLY=-2;
  p->AlphaMaskInfo.AlphaMaskClipBox.BRX=8;p->AlphaMaskInfo.AlphaMaskClipBox.BRY=16;
  p->AlphaMaskInfo.Reset();CheckReset(p->AlphaMaskInfo);
  CHECK(p->AlphaMaskInfo.AlphaMaskClipBox.TLX==-4 && p->AlphaMaskInfo.AlphaMaskClipBox.TLY==-2 && p->AlphaMaskInfo.AlphaMaskClipBox.BRX==8 && p->AlphaMaskInfo.AlphaMaskClipBox.BRY==16);
  CHECK(arrays[a].First==0 && arrays[a].Last==0 && arrays[a].End==0);
  p->~CEnginePrimitive2DViewportManager();
 }
 puts("00be1e20_TEST PASS");return 0;
}