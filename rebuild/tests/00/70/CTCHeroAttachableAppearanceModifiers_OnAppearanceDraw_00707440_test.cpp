#include <vector>
#include <set>
#include <stdio.h>
#include <assert.h>
struct CRGBColour { unsigned char r,g,b,a; CRGBColour(int R=0,int G=0,int B=0,int A=255):r(R),g(G),b(B),a(A){} };
enum EEngineInterpolationSet { normal };
enum { COMPOSITE_BLEND_SOLID=3 };
typedef long CCharString;
struct CCompositeBlendDesc { int type,texture; CRGBColour left,right; CCompositeBlendDesc(int t,int tex,CRGBColour l,CRGBColour r): type(t),texture(tex),left(l),right(r){} };
struct CEngineTextureMorph { long OriginalTextureIndex; std::vector<CCompositeBlendDesc> Blends; };
struct Blend { int source; CCompositeBlendDesc desc; Blend(int s,CCompositeBlendDesc d):source(s),desc(d){} };
struct CEngineTextureMorphSet { std::vector<Blend> v; void AddTextureMorph(long s,CCompositeBlendDesc d){v.push_back(Blend(s,d));} const std::vector<Blend>& PeekEngineMorphs()const{return v;} };
struct CTextureReplacementDef { struct CEntry { long OriginalTexture,NewTexture; }; std::vector<CEntry> v; long GetNoEntries()const{return v.size();} const CEntry& GetEntry(long i)const{return v.at(i);} };
struct Definition { CTextureReplacementDef* p; CTextureReplacementDef* Get()const{return p;} CTextureReplacementDef* operator->()const{return p;} };
struct Thing { int GetUniqueID()const{return 41;} };
static int anim,pos,orientation;
struct CEnginePrimitiveMorphedAnimatingMesh {
 int bank; float scale; std::vector<Blend> blends; int MeshEffect;
 CEnginePrimitiveMorphedAnimatingMesh(int id,int b,const std::vector<Blend>& v,const std::set<CCharString>& hide,int& a,int& p,int& o,float s,bool lit,int alpha,CRGBColour tint,float fade,EEngineInterpolationSet interp,int chan): bank(b),scale(s),blends(v),MeshEffect(0) { assert(id==41 && hide.empty() && &a==&anim && &p==&pos && &o==&orientation && lit && alpha==128 && tint.r==9 && fade==77 && interp==normal && chan==13); }
};
namespace NGraphicAppearance {
enum { GAPT_ANIMATING_MESH=2 };
struct CPrimitive { int type,bank; CPrimitive(int t,int b):type(t),bank(b){} };
struct CGraphicAppearanceDrawInfo { unsigned long ReasonsForDraw; int *Anim,*Pos,*Orientation; const std::vector<CEngineTextureMorph>* PTextureMorphs; };
struct CTCGraphicAppearance {
 std::vector<int> events; std::vector<CEnginePrimitiveMorphedAnimatingMesh> meshes;
 float GetScale(){return 2;} float GetFadeDistance(){return 77;} int PeekLightingChannel(){return 13;} int GetAlpha(){return 128;} CRGBColour GetTintColour(){return CRGBColour(9);} int GetPMeshEffect(){return 6;}
 void RemovePrimitive(CPrimitive* p){events.push_back(-p->bank);delete p;}
 CPrimitive* AddPersistentPrimitive(const CPrimitive& p){assert(p.type==2);events.push_back(100+p.bank);return new CPrimitive(p);}
 void SetPrimitiveGraphics(CPrimitive* p,const CEnginePrimitiveMorphedAnimatingMesh& m,int flags){assert(!flags && m.MeshEffect==6);events.push_back(200+p->bank);meshes.push_back(m);}
 void RefreshPrimitiveAnimationAndPos(CPrimitive* p,int& a,int& b,int& c){assert(&a==&anim && &b==&pos && &c==&orientation);events.push_back(300+p->bank);}
};
}
struct CTCHeroAttachableAppearanceModifiers {
 struct CModifierGraphic { Definition PTextureReplacement; long BankIndex; float RenderSizeX; };
 std::vector<CModifierGraphic> Graphics; std::vector<NGraphicAppearance::CPrimitive*> PrimHandles; bool RecreatePrimitives; Thing* ParentThing;
 bool OnAppearanceDraw(NGraphicAppearance::CTCGraphicAppearance&,const NGraphicAppearance::CGraphicAppearanceDrawInfo&);
};
bool CTCHeroAttachableAppearanceModifiers::OnAppearanceDraw(NGraphicAppearance::CTCGraphicAppearance& tc_appearance,const NGraphicAppearance::CGraphicAppearanceDrawInfo& draw_info)
{
    float scale=tc_appearance.GetScale();
    float fade_dist=tc_appearance.GetFadeDistance();
    if(RecreatePrimitives){
        while(!PrimHandles.empty()){
            tc_appearance.RemovePrimitive(PrimHandles.back());
            PrimHandles.pop_back();
        }
        PrimHandles.reserve(Graphics.size());
    }
    long handle_no=0;
    for(std::vector<CModifierGraphic>::const_iterator it_graphic=Graphics.begin();it_graphic!=Graphics.end();++it_graphic){
        if(!RecreatePrimitives && (draw_info.ReasonsForDraw==2 || draw_info.ReasonsForDraw==1 || draw_info.ReasonsForDraw==3)) {
            tc_appearance.RefreshPrimitiveAnimationAndPos(PrimHandles[handle_no],*draw_info.Anim,*draw_info.Pos,*draw_info.Orientation);
            ++handle_no;
        }else{
            CEngineTextureMorphSet morph_set;
            if(it_graphic->PTextureReplacement.Get()){
                for(long c1=0;c1<it_graphic->PTextureReplacement->GetNoEntries();++c1){
                    const CTextureReplacementDef::CEntry& entry=it_graphic->PTextureReplacement->GetEntry(c1);
                    morph_set.AddTextureMorph(entry.OriginalTexture,CCompositeBlendDesc(COMPOSITE_BLEND_SOLID,entry.NewTexture,CRGBColour(0,0,0),CRGBColour(0,0,0,0)));
                }
            }
            if(const std::vector<CEngineTextureMorph>* vect=draw_info.PTextureMorphs){
                for(long c1=0;c1<vect->size();++c1)
                    for(long c2=0;c2<(*vect)[c1].Blends.size();++c2)
                        morph_set.AddTextureMorph((*vect)[c1].OriginalTextureIndex,(*vect)[c1].Blends[c2]);
            }
            if(RecreatePrimitives)
                PrimHandles.push_back(tc_appearance.AddPersistentPrimitive(NGraphicAppearance::CPrimitive(NGraphicAppearance::GAPT_ANIMATING_MESH,it_graphic->BankIndex)));
            std::set<CCharString> hide_materials;
            CEnginePrimitiveMorphedAnimatingMesh mesh_prim(ParentThing->GetUniqueID(),it_graphic->BankIndex,morph_set.PeekEngineMorphs(),hide_materials,*draw_info.Anim,*draw_info.Pos,*draw_info.Orientation,it_graphic->RenderSizeX*scale,true,tc_appearance.GetAlpha(),tc_appearance.GetTintColour(),fade_dist,(EEngineInterpolationSet)0,tc_appearance.PeekLightingChannel());
            mesh_prim.MeshEffect=tc_appearance.GetPMeshEffect();
            tc_appearance.SetPrimitiveGraphics(PrimHandles[handle_no],mesh_prim,0);
            ++handle_no;
        }

    }
    RecreatePrimitives=false;
    return true;
}
int main(){
 using namespace NGraphicAppearance;
 for(int recreate=0;recreate<2;++recreate) for(int reason=0;reason<6;++reason)
 for(int n=0;n<4;++n) for(int old=0;old<4;++old) for(int add=0;add<3;++add){
  if(!recreate && n!=old)continue;
  CTCHeroAttachableAppearanceModifiers obj;Thing thing;obj.ParentThing=&thing;obj.RecreatePrimitives=recreate!=0;
  CTCGraphicAppearance tc; CGraphicAppearanceDrawInfo draw; draw.Anim=&anim;draw.Pos=&pos;draw.Orientation=&orientation;draw.ReasonsForDraw=reason;
  std::vector<CEngineTextureMorph> ext;draw.PTextureMorphs=add?&ext:0;
  if(add==2)for(int k=0;k<3;++k){CEngineTextureMorph m;m.OriginalTextureIndex=10+k;for(int j=0;j<k;++j)m.Blends.push_back(CCompositeBlendDesc(6,20+j,CRGBColour(1),CRGBColour(2)));ext.push_back(m);}
  CTextureReplacementDef def;for(int j=0;j<2;++j){CTextureReplacementDef::CEntry e={50+j,60+j};def.v.push_back(e);}
  for(int i=0;i<n;++i){CTCHeroAttachableAppearanceModifiers::CModifierGraphic g;g.BankIndex=30+i;g.PTextureReplacement.p=i%2?&def:0;g.RenderSizeX=3.5f+i;obj.Graphics.push_back(g);}
  for(int i=0;i<old;++i)obj.PrimHandles.push_back(new CPrimitive(2,70+i));
  assert(obj.OnAppearanceDraw(tc,draw) && !obj.RecreatePrimitives);unsigned int event=0,mesh=0;
  if(recreate)for(int j=old;j>0;--j)assert(tc.events.at(event++)==-(69+j));
  for(int j=0;j<n;++j){int b=recreate?30+j:70+j;assert(obj.PrimHandles.at(j)->bank==b);
   if(!recreate && reason>=1 && reason<=3){assert(tc.events.at(event++)==300+b);continue;}
   if(recreate)assert(tc.events.at(event++)==100+b);assert(tc.events.at(event++)==200+b);
   const CEnginePrimitiveMorphedAnimatingMesh& m=tc.meshes.at(mesh++);assert(m.bank==30+j && m.scale==(3.5f+j)*2);
   unsigned int ix=0;if(j%2)for(int q=0;q<2;++q){const Blend& v=m.blends.at(ix++);assert(v.source==50+q && v.desc.texture==60+q && v.desc.type==3 && v.desc.left.a==255 && v.desc.right.a==0);}
   if(add==2)for(int k=0;k<3;++k)for(int q=0;q<k;++q){const Blend& v=m.blends.at(ix++);assert(v.source==10+k && v.desc.texture==20+q && v.desc.type==6);}
   assert(ix==m.blends.size());
  }
  assert(event==tc.events.size() && mesh==tc.meshes.size());for(unsigned int i=0;i<obj.PrimHandles.size();++i)delete obj.PrimHandles[i];
 }
 puts("00707440_TEST PASS");return 0;
}