#include <cassert>
#include <cstdio>
#include <cstring>
#include <vector>
struct C2DBoxF {float TLX,TLY,BRX,BRY;C2DBoxF(){}C2DBoxF(float x,float y,float r,float b):TLX(x),TLY(y),BRX(r),BRY(b){}};
struct C3DVector {float X,Y,Z;C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
struct CRGBColour {unsigned int IntValue;};
struct CWideString {unsigned int id;CWideString():id(0){}};
struct CTVertexRHWColSpecTex1 {float X,Y,Z,RHW;unsigned int DiffuseColour,SpecularColour;float U1,V1;};
struct IDirect3DTexture9 {int id;};
static IDirect3DTexture9 handles[5];static int balance[5],texture_living;
class CTexture {IDirect3DTexture9* data;public:
 CTexture():data(0){++texture_living;}
 CTexture(IDirect3DTexture9*p):data(0){++texture_living;InitialiseFromPreallocatedTexture(p);}
 CTexture(const CTexture&o):data(o.data){++texture_living;if(data)++balance[data->id];}
 ~CTexture(){Uninitialise();--texture_living;}
 const CTexture&operator=(const CTexture&o){if(this!=&o){Uninitialise();data=o.data;if(data)++balance[data->id];}return *this;}
 bool InitialiseFromPreallocatedTexture(IDirect3DTexture9*p){Uninitialise();data=p;if(data)++balance[data->id];return true;}
 void Uninitialise(){if(data){--balance[data->id];assert(balance[data->id]>=0);data=0;}}
 bool IsValid()const{return data!=0;}IDirect3DTexture9*GetD3DTexture()const{return data;}
};
static bool same_box(const C2DBoxF&a,const C2DBoxF&b){return std::memcmp(&a,&b,sizeof(a))==0;}
static C2DBoxF viewport;static unsigned long active_blend;static int active_texture;static int states[13],expected_z,font_mutation;
struct Draw {int line,count,vertex,texture;unsigned long blend;C2DBoxF box;
 Draw(int l,int c,int v,int t,unsigned long b,const C2DBoxF&w):line(l),count(c),vertex(v),texture(t),blend(b),box(w){}
 bool operator==(const Draw&o)const{return line==o.line&&count==o.count&&vertex==o.vertex&&texture==o.texture&&blend==o.blend&&same_box(box,o.box);}
};
struct Text {unsigned id;float x,y,z,scale;unsigned colour;C2DBoxF box;
 Text(unsigned n,float a,float b,float c,float s,unsigned co,const C2DBoxF&w):id(n),x(a),y(b),z(c),scale(s),colour(co),box(w){}
 bool operator==(const Text&o)const{return id==o.id&&x==o.x&&y==o.y&&z==o.z&&scale==o.scale&&colour==o.colour&&same_box(box,o.box);}
};
static std::vector<Draw>draws;static std::vector<Text>texts;static std::vector<C2DBoxF>windows;static std::vector<unsigned long>blends;
static int captures,restores,realises,shader_apply,shader_prepare,registrations,font_prepare,attach_calls;
class CVertexShader{};class CVertexShaderConstantLayoutEmpty{};static CVertexShaderConstantLayoutEmpty GVertexShaderConstantLayoutEmpty;
class CShaderRenderManager {public:bool vertex,pixel;long layout;
 static CShaderRenderManager&Get(){static CShaderRenderManager s;return s;}
 void SetVertexShadersEnable(bool v,bool*old){if(old)*old=vertex;vertex=v;}
 void SetPixelShadersEnable(bool v,bool*old){if(old)*old=pixel;pixel=v;}
 void RegisterVertexShaderConstantLayout(long n,const CVertexShaderConstantLayoutEmpty*p){assert(n==0&&p==&GVertexShaderConstantLayoutEmpty);++registrations;}
 long PeekVertexShaderConstantLayout(){return layout;}void SetVertexShaderConstantLayout(long n){layout=n;}
 void ApplyVertexShader(const CVertexShader&,bool b){assert(!b&&vertex&&!pixel&&layout==0);++shader_apply;}
 void PrepareForRender(unsigned long f){assert(f==0&&vertex&&!pixel&&layout==0);++shader_prepare;}
};
class CScopedShadersEnable {bool OldVertexEnable,OldPixelEnable;public:
 CScopedShadersEnable(bool v,bool p){CShaderRenderManager::Get().SetVertexShadersEnable(v,&OldVertexEnable);CShaderRenderManager::Get().SetPixelShadersEnable(p,&OldPixelEnable);}
 ~CScopedShadersEnable(){CShaderRenderManager::Get().SetVertexShadersEnable(OldVertexEnable,0);CShaderRenderManager::Get().SetPixelShadersEnable(OldPixelEnable,0);}
};
class CScopedVertexShaderConstantLayout {long OldIndex;public:
 CScopedVertexShaderConstantLayout(long n){CShaderRenderManager&m=CShaderRenderManager::Get();OldIndex=m.PeekVertexShaderConstantLayout();m.SetVertexShaderConstantLayout(n);}
 ~CScopedVertexShaderConstantLayout(){CShaderRenderManager::Get().SetVertexShaderConstantLayout(OldIndex);}
};
class CRenderStateScopedCaptureBlock {int old[13];public:
 CRenderStateScopedCaptureBlock(){++captures;for(int i=0;i<13;++i)old[i]=states[i];}
 ~CRenderStateScopedCaptureBlock(){++restores;for(int i=0;i<13;++i)states[i]=old[i];}
};
enum {POLY_CULL_CCW=0,POLY_FILL_SOLID=0,POLY_SHADE_GOURAUD=1,TEXTURE_FILTER_POINT=0,TEXTURE_MIPMAP_POINT=0,TEXTURE_UV_CLAMP=2,ZBUFFER_FUNC_LESS_EQUAL=3};
enum D3DPRIMITIVETYPE {D3DPT_LINELIST=2,D3DPT_TRIANGLELIST=4};
static const CTVertexRHWColSpecTex1*vertex_origin;
struct IDirect3DDevice9 {long DrawPrimitiveUP(D3DPRIMITIVETYPE type,unsigned int count,const void*data,unsigned int stride){
 assert(stride==32&&count>0&&count<=2001);assert(CShaderRenderManager::Get().vertex&&!CShaderRenderManager::Get().pixel);
 int expected[13]={0,0,1,0,0,0,2,2,0,expected_z,3,expected_z,1};for(int i=0;i<13;++i)assert(states[i]==expected[i]);
 int index=static_cast<int>(static_cast<const CTVertexRHWColSpecTex1*>(data)-vertex_origin);draws.push_back(Draw(type==D3DPT_LINELIST,count,index,active_texture,active_blend,viewport));return 0;
}};
struct CDisplayManager {IDirect3DDevice9 device;IDirect3DDevice9*GetD3DDevice(){return &device;}};
class CFontBank {public:
 void PrepareToRender()const{assert(!CShaderRenderManager::Get().vertex&&!CShaderRenderManager::Get().pixel);++font_prepare;active_blend=42;}
 void Render(const CWideString&s,const C3DVector&p,float scale,const CRGBColour&c,unsigned long flags)const{
  assert(flags==0);texts.push_back(Text(s.id,p.X,p.Y,p.Z,scale,c.IntValue,viewport));
  CShaderRenderManager::Get().vertex=(font_mutation&1)!=0;CShaderRenderManager::Get().pixel=(font_mutation&2)!=0;
 }
};
struct CQuickDrawTriInfo {unsigned char Line:1;unsigned char IsText:1;C2DBoxF Window;CWideString String;CTexture UploadedTexture;union{unsigned long BlendMode;float StringX;};union{float ZAverage;float StringY;};float StringZ,StringScale;unsigned long StringFlags;CRGBColour Colour;const CFontBank*PFont;};
class CStateBlockFunctionBase {public:virtual void Apply()=0;};
class Block:public CStateBlockFunctionBase {public:unsigned long id;void Apply(){active_blend=id;blends.push_back(id);}};
static Block blocks[7];static CFontBank font;
class CRenderManager2D {public:
 CVertexShader VertexShader2D;std::vector<CTVertexRHWColSpecTex1>QuickDraw2DVertices;std::vector<CQuickDrawTriInfo>QuickDraw2DTriInfo;static CStateBlockFunctionBase*LibraryStateBlockFunctions[7];CDisplayManager display;
 void Render2DDrawList(bool);
 CDisplayManager&GetDisplayManager(){return display;}
 void SetAWindow(const C2DBoxF&w){viewport=w;windows.push_back(w);}
 void AttachTextureToStage(long stage,const CTexture*t){assert(stage==0);active_texture=t&&t->IsValid()?t->GetD3DTexture()->id:0;++attach_calls;}
 void RealiseRenderState(){++realises;}
 void SetPolyCullMode(int x){states[0]=x;}void SetPolyFillMode(int x){states[1]=x;}void SetPolyShadeMode(int x){states[2]=x;}
 void SetTextureFilterModeMin(long n,int x){assert(n==0);states[3]=x;}void SetTextureFilterModeMag(long n,int x){assert(n==0);states[4]=x;}
 void SetTextureMipMapMode(long n,int x){assert(n==0);states[5]=x;}void SetTextureUVModeU(long n,int x){assert(n==0);states[6]=x;}void SetTextureUVModeV(long n,int x){assert(n==0);states[7]=x;}
 void SetFogEnable(bool x){states[8]=x;}void SetZBufferEnable(bool x){states[9]=x;}void SetZBufferTestFunc(int x){states[10]=x;}void SetZBufferWriteEnable(bool x){states[11]=x;}void SetClippingEnable(bool x){states[12]=x;}
};
CStateBlockFunctionBase*CRenderManager2D::LibraryStateBlockFunctions[7];

void CRenderManager2D::Render2DDrawList(bool zbuffer)
{
    unsigned long blend_mode=1;
    CTexture texture;
    CTexture current_texture;
    C2DBoxF current_window;
    bool change_window=false;
    bool is_line=false;
    bool toggle_is_line=false;
    bool was_text=false;
    if(QuickDraw2DTriInfo.size()==0)return;
    CScopedShadersEnable enabled(true,false);
    CRenderStateScopedCaptureBlock rsblock;
    SetPolyCullMode(POLY_CULL_CCW);
    SetPolyFillMode(POLY_FILL_SOLID);
    SetPolyShadeMode(POLY_SHADE_GOURAUD);
    SetTextureFilterModeMin(0,TEXTURE_FILTER_POINT);
    SetTextureFilterModeMag(0,TEXTURE_FILTER_POINT);
    SetTextureMipMapMode(0,TEXTURE_MIPMAP_POINT);
    SetTextureUVModeU(0,TEXTURE_UV_CLAMP);
    SetTextureUVModeV(0,TEXTURE_UV_CLAMP);
    SetFogEnable(false);
    SetZBufferEnable(zbuffer);
    SetZBufferTestFunc(ZBUFFER_FUNC_LESS_EQUAL);
    SetZBufferWriteEnable(zbuffer);
    SetClippingEnable(true);
    current_texture=CTexture(0);
    unsigned long current_blend_mode=1;
    LibraryStateBlockFunctions[current_blend_mode]->Apply();
    CShaderRenderManager::Get().RegisterVertexShaderConstantLayout(0,&GVertexShaderConstantLayoutEmpty);
    CScopedVertexShaderConstantLayout constant_layout(0);
    CShaderRenderManager::Get().ApplyVertexShader(VertexShader2D,false);
    CShaderRenderManager::Get().PrepareForRender(0);
    current_window=QuickDraw2DTriInfo[0].Window;
    SetAWindow(current_window);
    long vertices_in=0;
    long c1=0,start_tri=0;
    for(;c1<QuickDraw2DTriInfo.size();++c1) {
        CQuickDrawTriInfo& tri_info=QuickDraw2DTriInfo[c1];
        if(!tri_info.IsText) {
            if(was_text) {
                CShaderRenderManager::Get().ApplyVertexShader(VertexShader2D,false);
                CShaderRenderManager::Get().PrepareForRender(0);
                was_text=false;
            }
            blend_mode=tri_info.BlendMode;
            if(tri_info.UploadedTexture.IsValid()) {
                texture=tri_info.UploadedTexture;
                tri_info.UploadedTexture.Uninitialise();
            } else texture.Uninitialise();
            change_window=memcmp(&current_window,&tri_info.Window,sizeof(C2DBoxF))!=0;
            if(is_line!=tri_info.Line)toggle_is_line=true;
        }
        long pending=c1-start_tri;
        if(pending>2000 || tri_info.IsText || texture.GetD3DTexture()!=current_texture.GetD3DTexture() || current_blend_mode!=blend_mode || change_window || toggle_is_line) {
            if(pending>0) {
                AttachTextureToStage(0,&current_texture);
                if(is_line) {
                    RealiseRenderState();
                    GetDisplayManager().GetD3DDevice()->DrawPrimitiveUP(D3DPT_LINELIST,pending,&QuickDraw2DVertices[vertices_in],sizeof(CTVertexRHWColSpecTex1));
                    vertices_in+=(pending)*2;
                } else {
                    RealiseRenderState();
                    GetDisplayManager().GetD3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,pending,&QuickDraw2DVertices[vertices_in],sizeof(CTVertexRHWColSpecTex1));
                    vertices_in+=(pending)*3;
                }
                start_tri=c1;
            }
            if(tri_info.IsText) {
                float tlx=tri_info.Window.TLX+tri_info.StringX;
                float tly=tri_info.Window.TLY+tri_info.StringY;
                float textz=tri_info.StringZ;
                if(memcmp(&current_window,&tri_info.Window,sizeof(C2DBoxF))!=0) {
                    SetAWindow(tri_info.Window);current_window=tri_info.Window;
                }
                CScopedShadersEnable shader_scope_block(false,false);
                current_blend_mode=9999;
                tri_info.PFont->PrepareToRender();
                tri_info.PFont->Render(tri_info.String,C3DVector(tlx,tly,textz),tri_info.StringScale,tri_info.Colour,0);
                start_tri=c1+1;
                is_line=false;
                current_texture=CTexture(0);
                toggle_is_line=false;
                was_text=true;
            } else {
                if(current_texture.GetD3DTexture()!=texture.GetD3DTexture())current_texture=texture;
                if(blend_mode!=current_blend_mode) {
                    LibraryStateBlockFunctions[blend_mode]->Apply();current_blend_mode=blend_mode;
                }
                if(change_window) {SetAWindow(tri_info.Window);current_window=tri_info.Window;}
                if(toggle_is_line) {is_line=!is_line;toggle_is_line=false;}
            }
        }
    }
    if(c1-start_tri>0) {
        AttachTextureToStage(0,&current_texture);
        if(is_line) {
            RealiseRenderState();
            GetDisplayManager().GetD3DDevice()->DrawPrimitiveUP(D3DPT_LINELIST,c1-start_tri,&QuickDraw2DVertices[vertices_in],sizeof(CTVertexRHWColSpecTex1));
        } else {
            RealiseRenderState();
            GetDisplayManager().GetD3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,c1-start_tri,&QuickDraw2DVertices[vertices_in],sizeof(CTVertexRHWColSpecTex1));
        }
    }
    QuickDraw2DTriInfo.clear();
    QuickDraw2DVertices.clear();
    AttachTextureToStage(0,0);
}


struct Spec {bool line,text;unsigned long blend;int texture;C2DBoxF box;float x,y,z,scale;unsigned id,colour;};
struct Oracle {std::vector<Draw>d;std::vector<Text>t;std::vector<C2DBoxF>w;std::vector<unsigned long>b;int prepares;Oracle():prepares(0){}};
static Oracle expected(const std::vector<Spec>&s){
 Oracle o;if(s.empty())return o;o.b.push_back(1);o.w.push_back(s[0].box);o.prepares=1;
 C2DBoxF window=s[0].box;unsigned long blend=1;bool after_text=false;int vertex=0,batch=0;Spec key=s[0];
 for(unsigned i=0;i<s.size();++i){const Spec&x=s[i];
  bool boundary=x.text||batch==2001||(batch&&(key.line!=x.line||key.texture!=x.texture||key.blend!=x.blend||!same_box(key.box,x.box)));
  if(boundary&&batch){o.d.push_back(Draw(key.line,batch,vertex,key.texture,key.blend,key.box));vertex+=batch*(key.line?2:3);batch=0;}
  if(x.text){
   if(!same_box(window,x.box)){window=x.box;o.w.push_back(window);}
   o.t.push_back(Text(x.id,x.box.TLX+x.x,x.box.TLY+x.y,x.z,x.scale,x.colour,window));blend=9999;after_text=true;
  }else{
   if(after_text){++o.prepares;after_text=false;}
   if(blend!=x.blend){blend=x.blend;o.b.push_back(blend);}
   if(!same_box(window,x.box)){window=x.box;o.w.push_back(window);}
   if(!batch)key=x;++batch;
  }
 }
 if(batch)o.d.push_back(Draw(key.line,batch,vertex,key.texture,key.blend,key.box));return o;
}
static void run(const std::vector<Spec>&s,int z,int flags,int mutation){
 assert(texture_living==0);for(int i=0;i<5;++i){assert(balance[i]==0);handles[i].id=i;}
 CRenderManager2D r;r.QuickDraw2DTriInfo.reserve(s.size());unsigned vertices=0;
 for(unsigned i=0;i<s.size();++i){const Spec&x=s[i];CQuickDrawTriInfo q;q.Line=x.line;q.IsText=x.text;q.Window=x.box;q.String.id=x.id;
  if(x.texture)q.UploadedTexture.InitialiseFromPreallocatedTexture(&handles[x.texture]);
  if(x.text){q.StringX=x.x;q.StringY=x.y;}else{q.BlendMode=x.blend;q.ZAverage=0;vertices+=x.line?2:3;}
  q.StringZ=x.z;q.StringScale=x.scale;q.StringFlags=0x9876;q.Colour.IntValue=x.colour;q.PFont=&font;r.QuickDraw2DTriInfo.push_back(q);
 }
 r.QuickDraw2DVertices.resize(vertices);vertex_origin=vertices?&r.QuickDraw2DVertices[0]:0;
 int original[13];for(int i=0;i<13;++i)states[i]=original[i]=(i*7+flags)%11;
 CShaderRenderManager&m=CShaderRenderManager::Get();m.vertex=(flags&1)!=0;m.pixel=(flags&2)!=0;m.layout=17;
 expected_z=z;font_mutation=mutation;captures=restores=realises=shader_apply=shader_prepare=registrations=font_prepare=attach_calls=0;
 draws.clear();texts.clear();windows.clear();blends.clear();active_texture=3;active_blend=5;
 Oracle o=expected(s);r.Render2DDrawList(z!=0);
 assert(draws==o.d&&texts==o.t&&blends==o.b&&windows.size()==o.w.size());for(unsigned i=0;i<windows.size();++i)assert(same_box(windows[i],o.w[i]));
 assert(shader_apply==o.prepares&&shader_prepare==o.prepares&&font_prepare==static_cast<int>(o.t.size()));assert(realises==static_cast<int>(o.d.size()));
 assert(captures==(!s.empty())&&restores==captures&&registrations==captures);assert(attach_calls==static_cast<int>(o.d.size())+(!s.empty()));
 assert(m.vertex==((flags&1)!=0)&&m.pixel==((flags&2)!=0)&&m.layout==17);for(int i=0;i<13;++i)assert(states[i]==original[i]);
 assert(r.QuickDraw2DTriInfo.empty()&&r.QuickDraw2DVertices.empty());assert(texture_living==0);for(int i=0;i<5;++i)assert(balance[i]==0);
 if(!s.empty())assert(active_texture==0);
}
int main(){
 for(int i=0;i<7;++i){blocks[i].id=i;CRenderManager2D::LibraryStateBlockFunctions[i]=&blocks[i];}
 unsigned runs=0;
 for(unsigned scenario=0;scenario<272;++scenario){std::vector<Spec>s;unsigned n=scenario<256?scenario%26:scenario==256?2000:scenario==257?2001:scenario==258?2002:scenario==259?4003:2050;
  for(unsigned i=0;i<n;++i){Spec x;x.id=i;x.text=scenario<256?((i+scenario)%((scenario%5)+2)==0):(scenario>=260&&(i%509==0));x.line=((scenario>>1)+i/((scenario%5)+1))%2!=0;
   if(scenario>=256&&scenario<260)x.line=(scenario&1)!=0;
   x.blend=scenario>=256&&scenario<260?1:(scenario+i/(1+scenario%7))%7;x.texture=scenario>=256&&scenario<260?2:(scenario/3+i/(1+scenario%3))%5;
   float w=scenario>=256&&scenario<260?0.0f:float((scenario+i/3)%4)*0.25f;x.box=C2DBoxF(w,-w,w+10,w+20);if(scenario%13==0&&i%2)x.box.TLY=-0.0f;
   x.x=float(i%8)*0.5f;x.y=-float(i%4)*0.25f;x.z=float(i%5)*0.125f;x.scale=0.5f+float(i%3)*0.25f;x.colour=(i*77)^scenario;s.push_back(x);
  }
  for(int z=0;z<2;++z)for(int flags=0;flags<4;++flags)for(int mutation=0;mutation<4;++mutation){run(s,z,flags,mutation);++runs;}
 }
 assert(runs==8704);std::puts("009da9f0_TEST PASS");return 0;
}
