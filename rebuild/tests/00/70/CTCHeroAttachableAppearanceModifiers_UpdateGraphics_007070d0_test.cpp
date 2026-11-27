#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cassert>

static int warnings;
#define FABLE_RETAIL_ASSERT(c) do { if (!(c)) ++warnings; } while (false)
static void require(bool b) { if (!b) std::exit(1); }

enum EHeroMorphType { HERO_MORPH_NONE, HERO_MORPH_TAN };
struct CTextureReplacementDef { mutable int refs; CTextureReplacementDef():refs(0){} };
struct CAppearanceModifierGraphics {
    struct CEntry { EHeroMorphType MorphType; long BankIndex; float RenderSizeX,StartMorphStrength,EndMorphStrength; };
    std::vector<CEntry> Graphics;
    long GetNoGraphics() const { return (long)Graphics.size(); }
    const CEntry& GetGraphic(long n) const { return Graphics[n]; }
};
struct CAppearanceModifierDef { mutable int refs; CAppearanceModifierGraphics MorphGraphics; CAppearanceModifierDef():refs(0){} };
template<class T> struct CDefPointer {
    T* value;
    CDefPointer():value(0){}
    CDefPointer(const CDefPointer& r):value(r.value){if(value) ++value->refs;}
    ~CDefPointer(){if(value) --value->refs;}
    CDefPointer& operator=(const CDefPointer& r){set(r.value);return *this;}
    void set(T* p){if(p) ++p->refs;if(value)--value->refs;value=p;}
    T* operator->()const{return value;}
};
struct CThingObjectDef {
    mutable int refs, appearance_gets, texture_gets;
    const CAppearanceModifierDef* mod;
    const CTextureReplacementDef* tex;
    CThingObjectDef():refs(0),appearance_gets(0),texture_gets(0),mod(0),tex(0){}
    bool GetSubDef(CDefPointer<const CAppearanceModifierDef>* r)const{++appearance_gets;r->set(mod);return mod!=0;}
    bool GetSubDef(CDefPointer<const CTextureReplacementDef>* r)const{++texture_gets;r->set(tex);return tex!=0;}
};
struct CGameDefinitionManager {
    const CThingObjectDef* definitions[2]; mutable long queries[2];
    bool GetDef(long i,CDefPointer<const CThingObjectDef>* r)const{require(i==0||i==1);++queries[i];r->set(definitions[i]);return true;}
};
struct CTCHeroMorph {
    mutable int reads;float value;
    float GetMorphStrength(EHeroMorphType m)const{require(m==HERO_MORPH_TAN);++reads;return value;}
};
struct CThing {
    CTCHeroMorph* morph;mutable int calls;
    bool GetTC(CTCHeroMorph** r)const{++calls;if(!morph)return false;*r=morph;return true;}
};
struct CTCHeroAttachableAppearanceModifiers {
    struct CModifierEntry {long DefIndex;bool Rendered;};
    struct CModifierGraphic {
        CDefPointer<const CTextureReplacementDef> PTextureReplacement;
        long BankIndex;float RenderSizeX;
        CModifierGraphic(long bank_index,float render_sizex,CDefPointer<const CTextureReplacementDef> ptexture_replacement)
          :PTextureReplacement(ptexture_replacement),BankIndex(bank_index),RenderSizeX(render_sizex){}
    };
    std::vector<CModifierGraphic> Graphics;
    std::vector<std::vector<CModifierEntry> > AppearanceModifiers;
    CThing thing;CGameDefinitionManager manager;bool RecreatePrimitives;
    const CGameDefinitionManager& GetDefinitionManager(){return manager;}
    CThing& GetParentThing(){return thing;}
    void AddGraphic(const CModifierGraphic& graphic){Graphics.push_back(graphic);}
    void UpdateGraphics();
};
void CTCHeroAttachableAppearanceModifiers::UpdateGraphics()
{
    RecreatePrimitives = true;
    Graphics.clear();
    CTCHeroMorph* ptc_morph = 0;
    if (!GetParentThing().GetTC(&ptc_morph)) {
        FABLE_RETAIL_ASSERT(false);
    }
    for (long appearance_type = 0; appearance_type < AppearanceModifiers.size(); ++appearance_type) {
        std::vector<CModifierEntry>& object_set = AppearanceModifiers[appearance_type];
        for (std::vector<CModifierEntry>::iterator it_modifier = object_set.begin();
             it_modifier != object_set.end(); ++it_modifier) {
            if (it_modifier->Rendered) {
                CDefPointer<const CThingObjectDef> pobject_def;
                GetDefinitionManager().GetDef(it_modifier->DefIndex, &pobject_def);
                CDefPointer<const CAppearanceModifierDef> pappearance_modifier_def;
                if (!pobject_def->GetSubDef(&pappearance_modifier_def)) {
                    FABLE_RETAIL_ASSERT(false);
                }
                CDefPointer<const CTextureReplacementDef> ptexture_replacement_def;
                pobject_def->GetSubDef(&ptexture_replacement_def);
                for (long morph_graphic_index = 0;
                     morph_graphic_index < pappearance_modifier_def->MorphGraphics.GetNoGraphics(); ++morph_graphic_index) {
                    const CAppearanceModifierGraphics::CEntry& graphic =
                        pappearance_modifier_def->MorphGraphics.GetGraphic(morph_graphic_index);
                    if (graphic.MorphType == HERO_MORPH_NONE) {
                        AddGraphic(CModifierGraphic(graphic.BankIndex, graphic.RenderSizeX, ptexture_replacement_def));
                    } else {
                        float morph_strength = ptc_morph->GetMorphStrength(graphic.MorphType);
                        if (morph_strength >= graphic.StartMorphStrength && morph_strength < graphic.EndMorphStrength)
                            AddGraphic(CModifierGraphic(graphic.BankIndex, graphic.RenderSizeX, ptexture_replacement_def));
                    }
                }
            }
        }
    }
}

int main() {
    const float strengths[]={-1.0f,0.0f,0.25f,0.5f,0.75f,1.0f,1.25f,2.0f};
    typedef CTCHeroAttachableAppearanceModifiers Renderer;
    for(unsigned int mode=0;mode<16384;++mode) {
        Renderer r;CTextureReplacementDef textures[2];CAppearanceModifierDef mods[2];CThingObjectDef defs[2];
        CTCHeroMorph morph;morph.value=strengths[mode%8];morph.reads=0;r.thing.morph=(mode&8)?&morph:0;r.thing.calls=0;
        r.RecreatePrimitives=(mode&16)!=0;warnings=0;
        for(int k=0;k<2;++k){
            r.manager.definitions[k]=&defs[k];r.manager.queries[k]=0;defs[k].mod=&mods[k];defs[k].tex=(mode&(32u<<k))?&textures[k]:0;
            int count=(mode & (128u<<k))? k+2:0;
            for(int n=0;n<count;++n){
                CAppearanceModifierGraphics::CEntry e;
                e.MorphType=(r.thing.morph && (mode&512) && ((n+k)&1))?HERO_MORPH_TAN:HERO_MORPH_NONE;
                e.BankIndex=100*k+n;e.RenderSizeX=(float)(1+k+n)*0.25f;
                e.StartMorphStrength=k?0.5f:0.0f;e.EndMorphStrength=k?1.0f:0.75f;
                mods[k].MorphGraphics.Graphics.push_back(e);
            }
        }
        const unsigned int layers=(mode>>10)&3u;
        int expected_queries[2]={0,0},reads=0;std::vector<int> banks,owners;std::vector<float> sizes;
        for(unsigned int m=0;m<layers;++m){
            r.AppearanceModifiers.push_back(std::vector<Renderer::CModifierEntry>());
            for(unsigned int k=0;k<=m;++k){
                Renderer::CModifierEntry e;e.DefIndex=(m+k)&1;e.Rendered=(mode&(4096u<<((m+k)&1)))!=0;
                r.AppearanceModifiers.back().push_back(e);
                if(!e.Rendered)continue;
                int ix=(int)e.DefIndex;++expected_queries[ix];
                const std::vector<CAppearanceModifierGraphics::CEntry>& gs=mods[ix].MorphGraphics.Graphics;
                for(unsigned int i=0;i<gs.size();++i){
                    if(gs[i].MorphType!=HERO_MORPH_NONE){
                        ++reads;
                        if(morph.value<gs[i].StartMorphStrength||morph.value>=gs[i].EndMorphStrength)continue;
                    }
                    owners.push_back(ix);banks.push_back((int)gs[i].BankIndex);sizes.push_back(gs[i].RenderSizeX);
                }
            }
        }
        { CDefPointer<const CTextureReplacementDef> old;old.set(&textures[0]);r.Graphics.push_back(Renderer::CModifierGraphic(-1,4.0f,old)); }
        for(int pass=1;pass<=2;++pass) {
            r.UpdateGraphics();require(r.RecreatePrimitives && r.Graphics.size()==banks.size() && morph.reads==reads*pass);
            require(r.thing.calls==pass && warnings==(r.thing.morph?0:pass));
            int refs[]={0,0};
            for(unsigned int i=0;i<banks.size();++i){
                const Renderer::CModifierGraphic& g=r.Graphics[i];int ix=owners[i];
                require(g.BankIndex==banks[i] && g.RenderSizeX==sizes[i] && g.PTextureReplacement.value==defs[ix].tex);
                if(defs[ix].tex)++refs[ix];
            }
            for(int k=0;k<2;++k) require(r.manager.queries[k]==expected_queries[k]*pass && defs[k].appearance_gets==expected_queries[k]*pass && defs[k].texture_gets==expected_queries[k]*pass && defs[k].refs==0 && mods[k].refs==0 && textures[k].refs==refs[k]);
        }
        r.Graphics.clear();require(!textures[0].refs&&!textures[1].refs);
    }
    std::puts("007070d0_TEST PASS");return 0;
}