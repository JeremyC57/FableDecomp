// Full selected function body, external device/render calls instrumented.
#include <cstdio>
#define FABLE_FORCEINLINE inline
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_BOOKMARK
#include <cmath>
#include <cstring>
#include <cstdlib>
static int failures=0;
static void check(bool v){if(!v)++failures;}
static bool nearf(float a,double b){return std::fabs(double(a)-b)<=0.00001*(1+std::fabs(b));}
class C2DExtentsI { public: long X,Y; C2DExtentsI(long x=0,long y=0):X(x),Y(y){} };
class C2DBoxI { public: long TLX,TLY,BRX,BRY; C2DBoxI(long a=0,long b=0,long c=0,long d=0):TLX(a),TLY(b),BRX(c),BRY(d){} };
static bool samebox(const C2DBoxI&a,const C2DBoxI&b){return a.TLX==b.TLX&&a.TLY==b.TLY&&a.BRX==b.BRX&&a.BRY==b.BRY;}
class CTVertexRHWColTex1 {public:float X,Y,Z,RHW;unsigned long DiffuseColour;float U1,V1;};
class CTexture {public:void BltToRenderTargetWithGPU(bool,bool,bool)const;};
enum {TEXTURE_FILTER_POINT=0,TEXTURE_MIPMAP_DISABLE=2,TEXTURE_ARG_TEXTURE=2,TEXTURE_OP_SELECT_ARG1=1};
struct State {union{unsigned long desired;unsigned long DesiredState;};union{unsigned long bookmark;unsigned long BookmarkMask;};union{bool dirty;bool DirtyListFlag;};};
struct Saved {int index;unsigned long value;};
class CRenderManager {
public:
 typedef ::State CStateInfo;
 struct CStateStackElement{CStateInfo* State;unsigned long Value;};
 State states[11];CStateStackElement StateStack[20];CStateInfo* StateUpdateList[20];
 union{int stacksize;int StateStackSize;};union{int updates;int StateUpdateListSize;};int draws;union{unsigned long mask;unsigned long CurrentBookmarkMask;};bool vertexShader,pixelShader;
 unsigned long fvf;float bias;CTVertexRHWColTex1 output[4];unsigned short ids[6];const CTexture*drawTexture;
 unsigned long drawnStates[11];int phase;int setcalls[11];bool drawnVertex,drawnPixel;
 void set(int n,unsigned long v){++setcalls[n];SetRenderState(v,states[n]);}
    FABLE_FORCEINLINE void SetRenderState(unsigned long value, CStateInfo& state)
    {
        const unsigned long desired_state = state.DesiredState;
        if (desired_state != value) {
#ifdef FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_BOOKMARK
            const unsigned long bookmark_mask = state.BookmarkMask;
            if ((bookmark_mask & CurrentBookmarkMask) == 0) {
#else
            if ((state.BookmarkMask & CurrentBookmarkMask) == 0) {
#endif
                state.BookmarkMask |= CurrentBookmarkMask;
                StateStack[StateStackSize].State = &state;
                StateStack[StateStackSize].Value = state.DesiredState;
                ++StateStackSize;
            }
            state.DesiredState = value;
            AddStateToUpdateList(state);
        }
    }
    FABLE_FORCEINLINE void AddStateToUpdateList(CStateInfo& state)
    {
#ifdef FABLE_RETAIL_CRENDERSTATEMANAGER_ADDSTATE_DIRTY
        // Opt-in (row 0x00B5FAB0 CaptureBackBuffer): the dirty flag read into a named local for the test. Code-identical; one more
        // named symbol per expansion (the record-count tie of the expanded setters, see SETRENDERSTATE_DESIRED). FITTED.
        // Retail bytes that need it: row 0x00B5FAB0 0x00B60665 mov edi,[eax+0x2814] / 0x00B6066B mov edx,[ecx+8] (registers swap
        // without it, see SETRENDERSTATE_BOOKMARK). No PDB local record: `dirty` is a made-up name.
        const bool dirty = state.DirtyListFlag;
        if (!dirty) {
#else
        if (!state.DirtyListFlag) {
#endif
            state.DirtyListFlag = true;
            StateUpdateList[StateUpdateListSize] = &state;
            ++StateUpdateListSize;
        }
    }

 void SetZBufferWriteEnable(bool x){set(0,x?1:0);}void SetAlphaBlendEnable(bool x){set(1,x?1:0);}void SetAlphaTestEnable(bool x){set(2,x?1:0);}void SetDitherEnable(bool x){set(3,x?1:0);}
 void SetTextureFilterModeMag(long stage,int mode){check(stage==0&&mode==0);set(4,1);}void SetTextureFilterModeMin(long stage,int mode){check(stage==0&&mode==0);set(5,1);}void SetTextureMipMapMode(long stage,int mode){check(stage==0&&mode==2);set(6,0);}
 void SetVertexFormat(unsigned long x){fvf=x;}
 void SetTextureArgument1(long stage,int mode){check(stage==0&&mode==2);set(7,2);}void SetTextureOperation(long stage,int mode){check(stage==0&&mode==1);set(8,2);}
 void SetTextureAlphaArgument1(long stage,int mode){check(stage==0&&mode==2);set(9,2);}void SetTextureAlphaOperation(long stage,int mode){check(stage==0&&mode==1);set(10,2);}
 void SetTextureMipmapLODBias(long stage,float x){check(stage==0);bias=x;}
 void DrawAsIndexedTris(const CTexture**t,unsigned long nt,const void*v,long stride,long count,void*indices,long triangles){
  check(phase==1&&nt==1&&stride==sizeof(CTVertexRHWColTex1)&&count==4&&triangles==2);++draws;drawTexture=*t;
  const CTVertexRHWColTex1*p=(const CTVertexRHWColTex1*)v;
  for(int i=0;i<4;++i){output[i].X=p[i].X;output[i].Y=p[i].Y;output[i].Z=p[i].Z;output[i].RHW=p[i].RHW;output[i].U1=p[i].U1;output[i].V1=p[i].V1;}
  std::memcpy(ids,indices,sizeof(ids));for(int i=0;i<11;++i)drawnStates[i]=states[i].desired;drawnVertex=vertexShader;drawnPixel=pixelShader;phase=2;
 }
};
class CDisplayManager {public:CRenderManager rm;C2DBoxI viewport,old,drawViewport;C2DExtentsI dimensions;int sets,gets;
 CRenderManager&GetRenderManager(){return rm;}const C2DBoxI&GetViewport()const{return viewport;}
 C2DExtentsI GetRenderTargetDimensions(){++gets;return dimensions;}
 void SetViewport(const C2DBoxI&b){++sets;viewport=b;if(rm.phase==1)drawViewport=b;else check(rm.phase==2);}
};
class CSystemManager {public:CDisplayManager dm;CDisplayManager*GetDisplayManager(){return &dm;}};
static CSystemManager systemManager;
static CSystemManager*GFGetSystemManager(){return &systemManager;}
class CRenderStateScopedCaptureBlock {public:
 CRenderStateScopedCaptureBlock(){CRenderManager&m=systemManager.dm.rm;check(m.phase==0);m.phase=1;m.mask<<=1;}
 ~CRenderStateScopedCaptureBlock(){CRenderManager&m=systemManager.dm.rm;check(m.phase==3);while(m.stacksize){CRenderManager::CStateStackElement x=m.StateStack[--m.stacksize];x.State->DesiredState=x.Value;x.State->BookmarkMask&=~m.mask;}m.mask>>=1;m.phase=4;}
};
class CScopedShadersEnable {bool v,p;public:
 CScopedShadersEnable(bool a,bool b){CRenderManager&m=systemManager.dm.rm;v=m.vertexShader;p=m.pixelShader;m.vertexShader=a;m.pixelShader=b;}
 ~CScopedShadersEnable(){CRenderManager&m=systemManager.dm.rm;check(m.phase==2);m.vertexShader=v;m.pixelShader=p;m.phase=3;}
};
void CTexture::BltToRenderTargetWithGPU(bool ignore_viewport, bool target_is_mipmap, bool disable_filtering) const
{
    CRenderStateScopedCaptureBlock cb;
    CScopedShadersEnable disable_shaders(false, false);
    CDisplayManager* display_manager = GFGetSystemManager()->GetDisplayManager();
    CRenderManager& render_manager = display_manager->GetRenderManager();
    C2DBoxI old_v = display_manager->GetViewport();
    long width;
    long height;
    if (ignore_viewport || target_is_mipmap) {
        C2DExtentsI dim = display_manager->GetRenderTargetDimensions();
        width = dim.X;
        height = dim.Y;
        C2DBoxI view(0, 0, width, height);
        display_manager->SetViewport(view);
    } else {
        width = old_v.BRX - old_v.TLX;
        height = old_v.BRY - old_v.TLY;
    }
    CTVertexRHWColTex1 verts[4];
    verts[0].X = 0.0f;
    verts[1].X = (float)width;
    verts[2].X = 0.0f;
    verts[3].X = (float)width;
    verts[0].Y = 0.0f;
    verts[1].Y = 0.0f;
    verts[2].Y = (float)height;
    verts[3].Y = (float)height;
    verts[0].U1 = 0.0f;
    verts[1].U1 = 1.0f;
    verts[2].U1 = 0.0f;
    verts[3].U1 = 1.0f;
    verts[0].V1 = 0.0f;
    verts[1].V1 = 0.0f;
    verts[2].V1 = 1.0f;
    verts[3].V1 = 1.0f;
    float tex_offset_u = 0.0f;
    float tex_offset_v = 0.0f;
    if (target_is_mipmap) {
        tex_offset_u = 1.0f / ((float)width * 0.5f);
        tex_offset_v = 1.0f / ((float)height * 0.5f);
    } else if (disable_filtering) {
        tex_offset_u = 1.0f / (float)width / 2.0f;
        tex_offset_v = 1.0f / (float)height / 2.0f;
    }
    for (long n = 0; n < 4; ++n) {
        verts[n].RHW = 1.0f;
        verts[n].Z = 0.0f;
        verts[n].U1 += tex_offset_u;
        verts[n].V1 += tex_offset_v;
    }
    render_manager.SetZBufferWriteEnable(false);
    render_manager.SetAlphaBlendEnable(true);
    render_manager.SetAlphaTestEnable(false);
    render_manager.SetDitherEnable(false);
    if (disable_filtering) {
        render_manager.SetTextureFilterModeMag(0, TEXTURE_FILTER_POINT);
        render_manager.SetTextureFilterModeMin(0, TEXTURE_FILTER_POINT);
        render_manager.SetTextureMipMapMode(0, TEXTURE_MIPMAP_DISABLE);
    }
    render_manager.SetVertexFormat(0x144);
    render_manager.SetTextureArgument1(0, TEXTURE_ARG_TEXTURE);
    render_manager.SetTextureOperation(0, TEXTURE_OP_SELECT_ARG1);
    render_manager.SetTextureAlphaArgument1(0, TEXTURE_ARG_TEXTURE);
    render_manager.SetTextureAlphaOperation(0, TEXTURE_OP_SELECT_ARG1);
    render_manager.SetTextureMipmapLODBias(0, -1.0f);
    unsigned short ids[6] = { 0, 1, 2, 1, 3, 2 };
    const CTexture* texture = this;
    render_manager.DrawAsIndexedTris(&texture, 1, verts, sizeof(CTVertexRHWColTex1), 4, ids, 2);
    if (ignore_viewport || target_is_mipmap)
        display_manager->SetViewport(old_v);
}

int main(){
 const long sizes[4][2]={{1,1},{2,3},{640,480},{1920,1080}};unsigned long cases=0;CTexture texture;
 for(int bits=0;bits<8;++bits)for(int rt=0;rt<4;++rt)for(int vp=0;vp<3;++vp)for(int shaders=0;shaders<4;++shaders)for(int pattern=0;pattern<4;++pattern)for(int bookmark=0;bookmark<3;++bookmark){
  systemManager=CSystemManager();CDisplayManager&d=systemManager.dm;CRenderManager&m=d.rm;std::memset(&m,0,sizeof(m));d.sets=0;d.gets=0;
  d.viewport=C2DBoxI(vp*17-10,5-vp*23,vp*17-10+sizes[vp][0],5-vp*23+sizes[vp][1]);d.old=d.viewport;d.drawViewport=d.viewport;d.dimensions=C2DExtentsI(sizes[rt][0],sizes[rt][1]);
  m.mask=1UL<<bookmark;m.vertexShader=(shaders&1)!=0;m.pixelShader=(shaders&2)!=0;State original[11];
  for(int i=0;i<11;++i){m.states[i].desired=(i+pattern)%4;m.states[i].bookmark=(pattern&1)?m.mask:0;m.states[i].dirty=(pattern&2)!=0;original[i]=m.states[i];}
  texture.BltToRenderTargetWithGPU((bits&1)!=0,(bits&2)!=0,(bits&4)!=0);
  bool reset=(bits&3)!=0;long w=reset?d.dimensions.X:d.old.BRX-d.old.TLX,h=reset?d.dimensions.Y:d.old.BRY-d.old.TLY;
  double u=(bits&2)?2.0/double(w):((bits&4)?0.5/double(w):0),v=(bits&2)?2.0/double(h):((bits&4)?0.5/double(h):0);
  for(int i=0;i<4;++i){check(nearf(m.output[i].X,(i&1)?w:0)&&nearf(m.output[i].Y,(i&2)?h:0));check(m.output[i].RHW==1&&m.output[i].Z==0);check(nearf(m.output[i].U1,((i&1)?1.0:0.0)+u)&&nearf(m.output[i].V1,((i&2)?1.0:0.0)+v));}
  const unsigned short expected[6]={0,1,2,1,3,2};check(std::memcmp(m.ids,expected,sizeof(expected))==0);check(m.draws==1&&m.drawTexture==&texture&&m.fvf==0x144&&m.bias==-1&&m.phase==4);
  check(!m.drawnVertex&&!m.drawnPixel&&m.vertexShader==((shaders&1)!=0)&&m.pixelShader==((shaders&2)!=0));check(d.sets==(reset?2:0)&&d.gets==(reset?1:0)&&samebox(d.viewport,d.old));
  C2DBoxI expectedBox=reset?C2DBoxI(0,0,w,h):d.old;check(samebox(d.drawViewport,expectedBox));
  int updates=0;for(int i=0;i<11;++i){unsigned long want=i==1?1:(i>=7?2:0);bool touched=!(i>=4&&i<=6)||((bits&4)!=0);if(i==4||i==5)want=1;
   check(m.setcalls[i]==(touched?1:0));check(m.drawnStates[i]==(touched?want:original[i].desired));check(m.states[i].desired==original[i].desired&&m.states[i].bookmark==original[i].bookmark);
   if(touched&&want!=original[i].desired&&!original[i].dirty)++updates;
  }check(m.updates==updates&&m.stacksize==0&&m.mask==(1UL<<bookmark));++cases;
 }
 check(cases==4608);std::printf(failures?"009fa780_TEST FAIL\n":"009fa780_TEST PASS\n");return failures?1:0;
}