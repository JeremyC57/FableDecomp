namespace std {
template<class T> class vector {
    T values[8];
    unsigned count;
public:
    vector():count(0){}
    unsigned size()const{return count;}
    bool empty()const{return count==0;}
    T& operator[](unsigned i){return values[i];}
    const T& operator[](unsigned i)const{return values[i];}
    void push_back(const T& v){values[count++]=v;}
};
}
#include <stdio.h>
#include <math.h>
struct C3DVector {};
struct CFishDef { long Level; };
struct CRegionDef { float FishWeightMult; };
struct CDefClassBase {
    CFishDef* fish;
    template<class P> bool GetSubDef(P* p) const { p->p=fish;return fish!=0; }
};
template<class T> struct CDefPointer {
    T* p;
    CDefPointer(T* x=0):p(x){}
    T* operator->() const {return p;}
    bool IsNull() const {return p==0;}
};
struct CFishingDef {std::vector<long> FishTypes;float FishWeightRainMult,FishWeightSnowMult,FishWeightMistMult;};
struct CTCHeroExperience {long level;long GetFishingLevel() const{return level;}};
struct CTCFish {
    CDefPointer<const CFishDef> def;float length,weight;
    CDefPointer<const CFishDef> GetPDef()const{return def;}
    float GetLength()const{return length;} float GetWeight()const{return weight;}
    void SetLength(float l){length=l;}void SetWeight(float w){weight=w;}
};
struct CThing {
    CTCHeroExperience* exp;CTCFish* fish;bool limbo;C3DVector pos;
    bool GetTC(CTCHeroExperience** p){*p=exp;return exp!=0;}
    bool GetTC(CTCFish** p){*p=fish;return fish!=0;}
    void SetInLimbo(bool b){limbo=b;}
    const C3DVector& GetPos()const{return pos;}
    long GetPlayerNumber()const{return 17;}
};
struct CThingObject: CThing {
    static CThingObject* Create(long,const C3DVector&,long,const void*,unsigned long,const char*);
};
struct CIntelligentFish {
    CThing* p;CThing* Get()const{return p;}CThing* operator->()const{return p;}
    void operator=(CThing* t){p=t;}
};
struct DefManager {
    CDefClassBase defs[5];
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long i){return &defs[i];}
};
struct Region {CDefPointer<const CRegionDef> d;CDefPointer<const CRegionDef> GetPRegionDef()const{return d;}};
struct WorldMap {Region r;long GetLoadedRegion()const{return 13;} const Region& GetRegion(long)const{return r;}};
struct Environment {
    float rain,snow,mist;
    float GetRainStrength()const{return rain;}float GetSnowStrength()const{return snow;}float GetMistStrength()const{return mist;}
};
struct World {Environment env;float theme;float GetEnvironmentFishWeightMult(const C3DVector&){return theme;} Environment& GetEnvironment(){return env;}};
static unsigned long pick, draws;static bool createOK;static long created;static CThingObject obj;static CFishDef defs[5];static CTCFish component;
CThingObject* CThingObject::Create(long i,const C3DVector&,long player,const void*,unsigned long,const char*) {
    created=player==17?i:-1;component.def=&defs[i];return createOK?&obj:0;
}
unsigned long GFRandom(unsigned long n,unsigned long*) {++draws;return pick%n;}
struct CTCFishing {
    CThing parent;CIntelligentFish PFish;CDefPointer<const CFishingDef> PDef;DefManager mgr;WorldMap map;World world;
    long CurrentFishWeight;unsigned long seed;
    CThing& GetParentThing(){return parent;} DefManager& GetDefinitionManager(){return mgr;}
    unsigned long* GetPWorldSeed(){return &seed;}WorldMap& PeekWorldMap(){return map;}World& GetWorld(){return world;}
    bool CreateFish(bool);
};
bool CTCFishing::CreateFish(bool using_fishing_spot)
{
    CTCHeroExperience* ptc_experience;
    if (GetParentThing().GetTC(&ptc_experience)) {
        long fishing_level = using_fishing_spot ? 1 : ptc_experience->GetFishingLevel();
        std::vector<long> fish_lower;
        std::vector<long> fish_level;
        if (false) {} else for (long i = 0; i < PDef->FishTypes.size(); ++i) {
            CDefPointer<const CDefClassBase> pbase_def = GetDefinitionManager().GetPDefFromGlobalIndex(PDef->FishTypes[i]);
            CDefPointer<const CFishDef> pfish_def;
            if (pbase_def->GetSubDef(&pfish_def)) {
                if (pfish_def->Level == fishing_level)
                    fish_level.push_back(PDef->FishTypes[i]);
                else if (pfish_def->Level < fishing_level) {
                    // Retail expands the subscript and calls the frame-pointer push_back owner at 007b1b57.
                    const long& value=PDef->FishTypes[i];
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    fish_lower.push_back(value);
#ifdef _MSC_VER
#pragma inline_depth(8)
#endif
                }
            }
        }
        long fish_def_index = 0;
        if (!fish_level.empty())
            fish_def_index = fish_level[GFRandom(fish_level.size(), GetPWorldSeed())];
        else if (!fish_lower.empty())
            fish_def_index = fish_lower[GFRandom(fish_lower.size(), GetPWorldSeed())];
        if (fish_def_index > 0) {
            PFish = CThingObject::Create(fish_def_index, GetParentThing().GetPos(), GetParentThing().GetPlayerNumber(), 0, 0, "");
            if (PFish.Get()) {
                PFish->SetInLimbo(true);
                CTCFish* ptc_fish = 0;
                PFish->GetTC(&ptc_fish);
                long level_difference = fishing_level - ptc_fish->GetPDef()->Level;
                if (fishing_level > 0) {
                    ptc_fish->SetLength(ptc_fish->GetLength() * (1.0f + 0.2f * level_difference));
                    float weight_mult = 1.0f + 0.2f * level_difference;
                    weight_mult = weight_mult * weight_mult * weight_mult;
                    ptc_fish->SetWeight(ptc_fish->GetWeight() * weight_mult);
                }
                CDefPointer<const CRegionDef> pregion_def = PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).GetPRegionDef();
                if (!pregion_def.IsNull()) {
                    float region_mult = pregion_def->FishWeightMult;
                    float theme_mult = GetWorld().GetEnvironmentFishWeightMult(GetParentThing().GetPos());
                    float rain_mult = 1.0f + GetWorld().GetEnvironment().GetRainStrength() * (PDef->FishWeightRainMult - 1.0f);
                    float snow_mult = 1.0f + GetWorld().GetEnvironment().GetSnowStrength() * (PDef->FishWeightSnowMult - 1.0f);
                    float mist_mult = 1.0f + GetWorld().GetEnvironment().GetMistStrength() * (PDef->FishWeightMistMult - 1.0f);
                    ptc_fish->SetWeight(ptc_fish->GetWeight() * region_mult * theme_mult * rain_mult * snow_mult * mist_mult);
                }
                CurrentFishWeight = (long)ptc_fish->GetWeight();
                return true;
            }
        }
    }
    return false;
}
int main()
{
    for(unsigned n=0;n<3072;++n) {
        CTCFishing g;CFishingDef f;CTCHeroExperience exp;CRegionDef region;
        exp.level=(n/3)%5-1;g.parent.exp=(n%11)?&exp:0;g.PDef=&f;g.PFish.p=0;g.CurrentFishWeight=-99;
        createOK=n%7!=0;created=-1;pick=n/9;draws=0;obj.fish=&component;obj.limbo=false;
        float startWeight=11.625f+(n%7)*2,startLength=4.5f+(n%11);
        component.weight=startWeight;component.length=startLength;
        f.FishWeightRainMult=1.5f;f.FishWeightSnowMult=.75f;f.FishWeightMistMult=2.0f;
        g.world.env.rain=.25f*(n%4);g.world.env.snow=.25f*((n/4)%4);g.world.env.mist=.25f*((n/16)%4);
        g.world.theme=.75f;region.FishWeightMult=1.25f;g.map.r.d=n%3?&region:0;
        bool spot=n%4==0;int level=spot?1:exp.level, best[5],lower[5],nb=0,nl=0;
        for(int j=1;j<=4;++j) {
            defs[j].Level=((n/(j+2))+j)%5-1;
            bool valid=(n+j)%6!=0;g.mgr.defs[j].fish=valid?&defs[j]:0;
            if(((n>>j)&1)==0)continue;
            f.FishTypes.push_back(j);
            if(!valid)continue;
            if(defs[j].Level==level)best[nb++]=j;
            else if(defs[j].Level<level)lower[nl++]=j;
        }
        int expected=g.parent.exp?(nb?best[pick%nb]:nl?lower[pick%nl]:-1):-1;
        bool ok=expected>0 && createOK;
        bool result=g.CreateFish(spot);
        if(result!=ok||created!=expected||draws!=(expected>0?1u:0u)||obj.limbo!=ok)return 1;
        if(!ok){if(g.CurrentFishWeight!=-99)return 2;continue;}
        double scale=level>0?1.0+.2*(level-defs[expected].Level):1.;
        double expectedWeight=startWeight*scale*scale*scale;
        if(g.map.r.d.p)expectedWeight*=1.25*.75*(1+.5*g.world.env.rain)*(1-.25*g.world.env.snow)*(1+g.world.env.mist);
        if(fabs(component.length-startLength*scale)>0.0001||fabs(component.weight-expectedWeight)>0.001)return 3;
        if(g.CurrentFishWeight!=(long)component.weight)return 4;
    }
    puts("007b1a30_TEST PASS");return 0;
}