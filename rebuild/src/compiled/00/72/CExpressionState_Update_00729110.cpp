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