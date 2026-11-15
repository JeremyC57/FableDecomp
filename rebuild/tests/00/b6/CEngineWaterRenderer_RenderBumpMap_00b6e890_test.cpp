#include <cstdio>
#include <cstdlib>
#include <cstring>
static void Check(bool ok){if(!ok){std::puts("00b6e890_TEST FAIL");std::exit(1);}}
// Target body and PrepareForRender copied verbatim. External rendering calls are instrumented dependencies.
static int events[64],event_count;
static void Event(int n){Check(event_count<64);events[event_count++]=n;}
class C2DVector {public:float X,Y;};
class CRGBFloatColour {public:float R,G,B,A;CRGBFloatColour(float r,float g,float b,float a):R(r),G(g),B(b),A(a){}};
class C4DVector {public:float X,Y,Z,W;C4DVector(){}C4DVector(const CRGBFloatColour& c):X(c.R),Y(c.G),Z(c.B),W(c.A){}};
template<class T>class CArray{public:T* first;T* last;unsigned long size()const{return (unsigned long)(last-first);}T* begin(){return first;}const T* GetAsCArray()const{return size()>0?first:(const T*)this;}};
class CVertexShaderConstant{public:float X,Y,Z,W;};
class CVertexShaderConstantLayout{public:struct CRange{long Offset;} Reserved;CArray<CVertexShaderConstant> ReservedPresets;};
static float expected_offsets[4];static int upload_count,draw_count;static unsigned long original_alpha;
class CShaderRenderManager {public:
 CVertexShaderConstantLayout* CurrentVSConstantLayout;bool WorldMatrixIsIdentity;CArray<C4DVector> PSConstTable;unsigned long UpdateFlags;
 void SetUserVertexShaderConstant(unsigned long i,float x,float y,float z,float w){Check(i==0&&x==expected_offsets[0]&&y==expected_offsets[1]&&z==expected_offsets[2]&&w==expected_offsets[3]);++upload_count;Event(1);}
 void SetPixelShaderConstant(unsigned long index,const C4DVector& value){*(PSConstTable.begin()+index)=value;UpdateFlags|=2;}
     inline void PrepareForRender(unsigned long flags)
    {
        if (UpdateFlags & 4)
            SetVSConstants(CurrentVSConstantLayout->Reserved.Offset, CurrentVSConstantLayout->ReservedPresets.GetAsCArray(),
                           CurrentVSConstantLayout->ReservedPresets.size());
        if (UpdateFlags & 1)
            UpdatePixelShader();
        if (UpdateFlags & 2)
            UpdatePixelShaderConstants();
        if ((flags & 0x400) && (UpdateFlags & 0x20))
            UpdateBoneTransform();
        if (flags & 0x40) {
            if (UpdateFlags & 0x1000)
                UpdateWorldSpaceTextureTransform();
        } else if (flags & 0x80) {
            if (UpdateFlags & 0x800)
                UpdateObjectSpaceTextureTransform();
        }
        if (flags & 0x200) {
            if (UpdateFlags & 0x2000)
                UpdateWorldSpaceFogTransform();
        } else if (flags & 0x100) {
            if (UpdateFlags & 0x4000)
                UpdateObjectSpaceFogTransform();
        }
        if (UpdateFlags & 4)
            UpdateAmbient();
        if (UpdateFlags & 0x20000)
            UpdateFogColour();
        if (UpdateFlags & 0x400)
            UpdateLightGlobals();
        if (flags & 8) {
            if (UpdateFlags & 0x100)
                UpdateObjectSpaceLighting(flags);
        } else if (flags & 0x10) {
            if (UpdateFlags & 0x200)
                UpdateWorldSpaceLighting();
        }
        if ((flags & 0x20) && (UpdateFlags & 0x8000))
            UpdateLocalisedSphereMapping();
        if (!(flags & 1) && !WorldMatrixIsIdentity)
            ResetWorldTransform();
        if (UpdateFlags & 0x80)
            UpdateCombinedProjectionTransform();
        if ((flags & 4) && (UpdateFlags & 8))
            UpdateCameraPos();
    }

 void SetVSConstants(long n,const void* data,unsigned long count){Check(n==19&&count==CurrentVSConstantLayout->ReservedPresets.size());Check(data==(count?CurrentVSConstantLayout->ReservedPresets.begin():(const void*)&CurrentVSConstantLayout->ReservedPresets));Event(2);}
 void UpdatePixelShader(){Event(3);UpdateFlags&=~1ul;}
 void UpdatePixelShaderConstants(){Event(4);UpdateFlags&=~2ul;}
 void UpdateAmbient(){Event(5);UpdateFlags&=~4ul;}
 void UpdateFogColour(){Event(6);UpdateFlags&=~0x20000ul;}
 void UpdateLightGlobals(){Event(7);UpdateFlags&=~0x400ul;}
 void ResetWorldTransform(){Event(8);WorldMatrixIsIdentity=true;}
 void UpdateCombinedProjectionTransform(){Event(9);UpdateFlags&=~0x80ul;}
 void UpdateBoneTransform(){Check(false);}void UpdateWorldSpaceTextureTransform(){Check(false);}void UpdateObjectSpaceTextureTransform(){Check(false);}void UpdateWorldSpaceFogTransform(){Check(false);}void UpdateObjectSpaceFogTransform(){Check(false);}void UpdateObjectSpaceLighting(unsigned long){Check(false);}void UpdateWorldSpaceLighting(){Check(false);}void UpdateLocalisedSphereMapping(){Check(false);}void UpdateCameraPos(){Check(false);}
};
enum ALPHA_BLEND_MODE{ALPHA_BLEND_ZERO,ALPHA_BLEND_ONE};
class CRenderStateManager{public:
 struct CStateInfo{unsigned long CurrentState,DesiredState,BookmarkMask,RenderStateParam;bool DirtyListFlag;};
 struct CStateStackElement{CStateInfo* State;unsigned long Value;};
 struct CDeviceState{CStateInfo AlphaBlendEnable,SourceAlphaBlendMode,DestAlphaBlendMode;} DeviceState;
 CStateStackElement StateStack[16];CStateInfo* StateUpdateList[16];long StateStackSize,StateUpdateListSize;unsigned long CurrentBookmarkMask;
 static unsigned long D3DAlphaBlendModeTable[13];
 void SetAlphaBlendEnable(bool enable){SetRenderState(enable?1:0,DeviceState.AlphaBlendEnable);}
 void SetSourceAlphaBlendMode(ALPHA_BLEND_MODE mode){SetRenderState(D3DAlphaBlendModeTable[mode],DeviceState.SourceAlphaBlendMode);}
 void SetDestAlphaBlendMode(ALPHA_BLEND_MODE mode){SetRenderState(D3DAlphaBlendModeTable[mode],DeviceState.DestAlphaBlendMode);}
 void SetRenderState(unsigned long value,CStateInfo& state){
  if(state.DesiredState!=value){
   if((state.BookmarkMask&CurrentBookmarkMask)==0){state.BookmarkMask|=CurrentBookmarkMask;StateStack[StateStackSize].State=&state;StateStack[StateStackSize].Value=state.DesiredState;++StateStackSize;}
   state.DesiredState=value;AddStateToUpdateList(state);
  }
 }
 void AddStateToUpdateList(CStateInfo& state){if(!state.DirtyListFlag){state.DirtyListFlag=true;StateUpdateList[StateUpdateListSize]=&state;++StateUpdateListSize;}}
};
unsigned long CRenderStateManager::D3DAlphaBlendModeTable[13];
class CRenderManagerCore:public CRenderStateManager{public:void DrawCurrentBuffersAsIndexedTris(unsigned long,unsigned long,unsigned long,unsigned long);};
class CRenderManager:public CRenderManagerCore{};
namespace NEngineInternal{CShaderRenderManager* GPShaderRenderManager;CRenderManager* GPRenderManager;}
using namespace NEngineInternal;
void CRenderManagerCore::DrawCurrentBuffersAsIndexedTris(unsigned long a,unsigned long b,unsigned long c,unsigned long d){
 Check(a==0&&b==14&&c==0&&d==2&&draw_count<2&&upload_count==1);Event(10);
 C4DVector* constants=GPShaderRenderManager->PSConstTable.begin();
 for(int i=5;i<7;++i)Check(constants[i].X==(draw_count==0?1.0f:0.0f)&&constants[i].Y==(draw_count==0?0.0f:1.0f)&&constants[i].Z==0&&constants[i].W==0);
 Check(constants[7].X==0&&constants[7].Y==0&&constants[7].Z==1&&constants[7].W==1);
 Check(DeviceState.AlphaBlendEnable.DesiredState==(draw_count==0?original_alpha:1));
 if(draw_count==1)Check(DeviceState.SourceAlphaBlendMode.DesiredState==D3DAlphaBlendModeTable[1]&&DeviceState.DestAlphaBlendMode.DesiredState==D3DAlphaBlendModeTable[1]);
 Check((GPShaderRenderManager->UpdateFlags&(1|2|4|0x80|0x400|0x20000))==0&&GPShaderRenderManager->WorldMatrixIsIdentity);++draw_count;
}
enum EWaterArrayIndexAliases{WATER_LAKE=0,WATER_SEA=1,WATER_ICE=2,WATER_INDEXABLE_WATER_TYPES_NUMBER=3};
class CEngineWaterSettings{public:C2DVector BumpmapGenerationShift[3][2];};
class CEngineWaterRenderer{public:CEngineWaterSettings Settings;void RenderBumpMap(EWaterArrayIndexAliases);};
void CEngineWaterRenderer::RenderBumpMap(EWaterArrayIndexAliases constant_array_index)
{
    GPShaderRenderManager->SetUserVertexShaderConstant(0,
        Settings.BumpmapGenerationShift[constant_array_index][0].X,
        Settings.BumpmapGenerationShift[constant_array_index][0].Y,
        Settings.BumpmapGenerationShift[constant_array_index][1].X,
        Settings.BumpmapGenerationShift[constant_array_index][1].Y);
    GPShaderRenderManager->SetPixelShaderConstant(5, CRGBFloatColour(1.0f,0.0f,0.0f,0.0f));
    GPShaderRenderManager->SetPixelShaderConstant(6, CRGBFloatColour(1.0f,0.0f,0.0f,0.0f));
    GPShaderRenderManager->SetPixelShaderConstant(7, CRGBFloatColour(0.0f,0.0f,1.0f,1.0f));
    GPShaderRenderManager->PrepareForRender(0);
    GPRenderManager->DrawCurrentBuffersAsIndexedTris(0,14,0,2);
    GPRenderManager->SetAlphaBlendEnable(true);
    GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
    GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
    GPShaderRenderManager->SetPixelShaderConstant(5, CRGBFloatColour(0.0f,1.0f,0.0f,0.0f));
    GPShaderRenderManager->SetPixelShaderConstant(6, CRGBFloatColour(0.0f,1.0f,0.0f,0.0f));
    GPShaderRenderManager->SetPixelShaderConstant(7, CRGBFloatColour(0.0f,0.0f,1.0f,1.0f));
    GPShaderRenderManager->PrepareForRender(0);
    GPRenderManager->DrawCurrentBuffersAsIndexedTris(0,14,0,2);
    GPRenderManager->SetAlphaBlendEnable(false);
}

int main(){
 CEngineWaterRenderer water;CShaderRenderManager shader;CRenderManager manager;CVertexShaderConstantLayout layout;C4DVector table[10];CVertexShaderConstant presets[2];
 GPShaderRenderManager=&shader;GPRenderManager=&manager;shader.CurrentVSConstantLayout=&layout;shader.PSConstTable.first=table;shader.PSConstTable.last=table+10;layout.Reserved.Offset=19;layout.ReservedPresets.first=presets;
 const unsigned long flagbits[8]={1,2,4,0x80,0x400,0x20000,0x100,0x8000};
 unsigned cases=0;
 for(unsigned pattern=0;pattern<256;++pattern)for(unsigned identity=0;identity<2;++identity)for(unsigned maskcase=0;maskcase<4;++maskcase)for(unsigned index=0;index<3;++index){
  unsigned long flags=0;for(unsigned k=0;k<8;++k)if(pattern&(1u<<k))flags|=flagbits[k];shader.UpdateFlags=flags;shader.WorldMatrixIsIdentity=identity!=0;
  layout.ReservedPresets.last=presets+(pattern%3);
  for(unsigned k=0;k<10;++k)table[k]=C4DVector(CRGBFloatColour(-2,-4,-8,-16));
  for(unsigned k=0;k<3;++k)for(unsigned j=0;j<2;++j){water.Settings.BumpmapGenerationShift[k][j].X=(float)(k*8+j*2);water.Settings.BumpmapGenerationShift[k][j].Y=(float)(k*8+j*2+1);}
  expected_offsets[0]=(float)(index*8);expected_offsets[1]=(float)(index*8+1);expected_offsets[2]=(float)(index*8+2);expected_offsets[3]=(float)(index*8+3);
  for(unsigned k=0;k<13;++k)manager.D3DAlphaBlendModeTable[k]=1000+pattern+k;
  CRenderStateManager::CStateInfo* states[3]={&manager.DeviceState.AlphaBlendEnable,&manager.DeviceState.SourceAlphaBlendMode,&manager.DeviceState.DestAlphaBlendMode};
  unsigned long initial[3],oldmask[3];bool dirty[3];
  for(unsigned k=0;k<3;++k){states[k]->DesiredState=k==0?(pattern%3):((pattern>>(k+2))&1?manager.D3DAlphaBlendModeTable[1]:77+k);states[k]->BookmarkMask=(pattern+k)%4;states[k]->DirtyListFlag=((pattern>>k)&1)!=0;states[k]->CurrentState=800+k;states[k]->RenderStateParam=900+k;initial[k]=states[k]->DesiredState;oldmask[k]=states[k]->BookmarkMask;dirty[k]=states[k]->DirtyListFlag;}
  original_alpha=initial[0];const unsigned long masks[4]={0,1,2,8};manager.CurrentBookmarkMask=masks[maskcase];manager.StateStackSize=1;manager.StateUpdateListSize=1;manager.StateStack[0].State=states[0];manager.StateStack[0].Value=1234;manager.StateUpdateList[0]=states[0];
  int capture_index[4],queue_index[3],nc=0,nq=0;unsigned long captured_value[4];unsigned long finalmask[3]={oldmask[0],oldmask[1],oldmask[2]};bool finaldirty[3]={dirty[0],dirty[1],dirty[2]};
  // Expected writes in draw-pass order: alpha-on, both blend factors, alpha-off.
  const int order[4]={0,1,2,0};const unsigned long wanted[4]={1,manager.D3DAlphaBlendModeTable[1],manager.D3DAlphaBlendModeTable[1],0};unsigned long running[3]={initial[0],initial[1],initial[2]};
  for(int k=0;k<4;++k){int ix=order[k];if(running[ix]==wanted[k])continue;if(!(finalmask[ix]&masks[maskcase])){capture_index[nc]=ix;captured_value[nc++]=running[ix];finalmask[ix]|=masks[maskcase];}if(!finaldirty[ix]){queue_index[nq++]=ix;finaldirty[ix]=true;}running[ix]=wanted[k];}
  event_count=upload_count=draw_count=0;water.RenderBumpMap((EWaterArrayIndexAliases)index);
  int expected[16],ne=0;expected[ne++]=1;if(flags&4)expected[ne++]=2;if(flags&1)expected[ne++]=3;expected[ne++]=4;if(flags&4)expected[ne++]=5;if(flags&0x20000)expected[ne++]=6;if(flags&0x400)expected[ne++]=7;if(!identity)expected[ne++]=8;if(flags&0x80)expected[ne++]=9;expected[ne++]=10;expected[ne++]=4;expected[ne++]=10;
  Check(event_count==ne&&upload_count==1&&draw_count==2);for(int k=0;k<ne;++k)Check(events[k]==expected[k]);
  Check(shader.UpdateFlags==(flags&(0x100|0x8000)));Check(manager.StateStackSize==1+nc&&manager.StateUpdateListSize==1+nq&&manager.CurrentBookmarkMask==masks[maskcase]);
  Check(manager.StateStack[0].State==states[0]&&manager.StateStack[0].Value==1234&&manager.StateUpdateList[0]==states[0]);
  for(int k=0;k<nc;++k)Check(manager.StateStack[k+1].State==states[capture_index[k]]&&manager.StateStack[k+1].Value==captured_value[k]);
  for(int k=0;k<nq;++k)Check(manager.StateUpdateList[k+1]==states[queue_index[k]]);
  for(unsigned k=0;k<3;++k){Check(states[k]->DesiredState==running[k]&&states[k]->BookmarkMask==finalmask[k]&&states[k]->DirtyListFlag==finaldirty[k]);Check(states[k]->CurrentState==800+k&&states[k]->RenderStateParam==900+k);}
  for(unsigned k=0;k<10;++k)if(k<5||k>7)Check(table[k].X==-2&&table[k].Y==-4&&table[k].Z==-8&&table[k].W==-16);
  for(unsigned k=0;k<3;++k)for(unsigned j=0;j<2;++j)Check(water.Settings.BumpmapGenerationShift[k][j].X==(float)(k*8+j*2)&&water.Settings.BumpmapGenerationShift[k][j].Y==(float)(k*8+j*2+1));
  ++cases;
 }
 Check(cases==6144);std::puts("00b6e890_TEST PASS");return 0;
}