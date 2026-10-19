#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstddef>
#ifndef _MSC_VER
#define __forceinline inline
#endif
// CTCCreatureExpression::CExpressionState::Update(float) -- approach primary weight or fade nonprimary expression.
// Native 00729110,229 bytes. No callees. Pool122DED4=3,122DED8=1,122DEDC=0 independently read.
// PDB CExpressionState size20 and fields from struct_layouts_egor.tsv80410; no engine header exists.
__forceinline void ClampWeight(float& value,const float& lower,const float& upper) {if(value<lower)value=lower;else if(value>upper)value=upper;}
struct CExpressionState {
 long AnimIndex; float MaxWeight,Weight,OldWeight; bool Primary; unsigned char Stage;
 void Update(float dt);
};
void CExpressionState::Update(float dt) {
 OldWeight=Weight;
 if (Primary) {
  if (Weight < MaxWeight) {
   Weight += dt*3.0f;
   ClampWeight(Weight,0.0f,MaxWeight);
  } else if (Weight > MaxWeight) {
   Weight -= dt*3.0f;
   ClampWeight(Weight,MaxWeight,1.0f);
  }
 } else Weight-=dt*3.0f;
 ClampWeight(Weight,0.0f,1.0f);
}


#ifdef _MSC_VER
typedef char StateSize[sizeof(CExpressionState)==20?1:-1];
typedef char MaxOffset[offsetof(CExpressionState,MaxWeight)==4?1:-1];
typedef char WeightOffset[offsetof(CExpressionState,Weight)==8?1:-1];
typedef char OldWeightOffset[offsetof(CExpressionState,OldWeight)==12?1:-1];
typedef char PrimaryOffset[offsetof(CExpressionState,Primary)==16?1:-1];
typedef char StageOffset[offsetof(CExpressionState,Stage)==17?1:-1];
#endif
int main(){
 struct Case {float w,max,dt,expect;bool primary;};
 Case cases[]={{.2f,.8f,.1f,.5f,true},{.2f,.4f,.2f,.4f,true},{.8f,.2f,.1f,.5f,true},{.8f,.6f,.2f,.6f,true},{.4f,.4f,1,.4f,true},{.2f,.8f,.1f,0,false},{.9f,.8f,-.1f,1,false},{.2f,.8f,-1,0,true},{.8f,.2f,-1,1,true},{-.2f,.1f,0,0,false},{1.2f,1,0,1,false}};
 for(unsigned i=0;i<sizeof(cases)/sizeof(cases[0]);++i){CExpressionState x;x.AnimIndex=71;x.Weight=cases[i].w;x.OldWeight=4;x.MaxWeight=cases[i].max;x.Primary=cases[i].primary;x.Stage=9;x.Update(cases[i].dt);
 if(std::fabs(x.Weight-cases[i].expect)>1e-6f||x.OldWeight!=cases[i].w||x.AnimIndex!=71||x.MaxWeight!=cases[i].max||x.Stage!=9){std::puts("00729110_TEST FAIL");return 1;}}
 union {unsigned int u;float f;} nan;nan.u=0x7fc00001;
 CExpressionState x;x.Weight=nan.f;x.MaxWeight=.5f;x.Primary=true;x.Update(1);if(x.Weight==x.Weight||x.OldWeight==x.OldWeight)return 1;
 x.Weight=.2f;x.MaxWeight=nan.f;x.Primary=true;x.Update(1);if(x.Weight!=.2f)return 1;
 x.Weight=.2f;x.MaxWeight=.8f;x.Primary=true;x.Update(nan.f);if(x.Weight==x.Weight)return 1;
 std::puts("00729110_TEST PASS");return 0;
}