#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <vector>
struct CCharString {char s[64];CCharString(const char* v=""){assert(strlen(v)<64);strcpy(s,v);}operator const char*()const{return s;}};
CCharString GFIntToCharString(long n){char s[12];sprintf(s,"%ld",n);return s;}
CCharString operator+(const char* a,const CCharString& b){char s[64];assert(strlen(a)+strlen(b.s)<64);strcpy(s,a);strcat(s,b.s);return s;}
struct CThingBaseDef {CCharString name;mutable int refs;CThingBaseDef():refs(0){}CCharString GetInstantiationName()const{return name;}};
template<class T> struct CDefPointer {T* p;CDefPointer():p(0){}CDefPointer(const CDefPointer& v):p(v.p){if(p)++p->refs;}
void reset(T* v){if(v)++v->refs;if(p)--p->refs;p=v;}~CDefPointer(){reset(0);}CDefPointer& operator=(const CDefPointer& v){reset(v.p);return *this;}T* operator->()const{return p;}};
static CThingBaseDef defs[4];
static int worldChecks,lookupCount,foundMask;
struct CWorld{bool discard;bool IsSerialisingAboutToLoadHeroState()const{++worldChecks;return discard;}} world;
struct CGameDefinitionManager{bool GetDef(const CCharString& name,CDefPointer<const CThingBaseDef>* dst)const{
 int index=lookupCount++;assert(index>=0&&index<4&&!strcmp(name,defs[index].name));if(!(foundMask&(1<<index)))return false;dst->reset(defs+index);return true;}} manager;
struct CPersistContext{bool save;int n,scalar,names;float vals[7];bool teen;
bool IsSaving()const{return save;}
void Transfer(const char* key,float& v){const char* labels[]={"Strength","Will","Skill","Age","Morality","Fatness","Tan"};assert(scalar<7&&!strcmp(key,labels[scalar]));if(save)vals[scalar]=v;else v=vals[scalar];++scalar;}
void Transfer(const char* key,bool& v){assert(scalar++==7&&!strcmp(key,"Teenager"));if(save)teen=v;else v=teen;}
void Transfer(const char* key,long& v){assert(scalar++==8&&!strcmp(key,"NumberOfTattoos"));if(save)n=v;else v=n;}
void Transfer(const char* key,CCharString& s){char label[32];sprintf(label,"Tattoo%d",names);assert(!strcmp(label,key)&&names<n);if(save)assert(!strcmp(s,defs[names].name));else s=defs[names].name;++names;}
};
struct CTCHeroMorph{float Strength,Berserk,Will,Skill,Age,Morality,Fatness,Tan;bool Teenager;
std::vector<CDefPointer<const CThingBaseDef> > Tattoos;
CWorld& PeekWorld()const{return world;}const CGameDefinitionManager& GetDefinitionManager()const{return manager;}
void OnSerialise(CPersistContext&,bool);
};
void CTCHeroMorph::OnSerialise(CPersistContext& context,bool in_game)
{
    context.Transfer("Strength", Strength);
    context.Transfer("Will", Will);
    context.Transfer("Skill", Skill);
    context.Transfer("Age", Age);
    context.Transfer("Morality", Morality);
    context.Transfer("Fatness", Fatness);
    context.Transfer("Tan", Tan);
    context.Transfer("Teenager", Teenager);
    if(context.IsSaving()) {
        long number_of_tattoos=Tattoos.size();
        context.Transfer("NumberOfTattoos",number_of_tattoos);
        for(long c1=0;c1<number_of_tattoos;++c1) {
            CCharString def_name=Tattoos[c1]->GetInstantiationName();
            context.Transfer("Tattoo"+GFIntToCharString(c1),def_name);
        }
    } else {
        std::vector<CDefPointer<const CThingBaseDef> > temp_tattoos;
        std::vector<CDefPointer<const CThingBaseDef> >& tattoos_ref=PeekWorld().IsSerialisingAboutToLoadHeroState()?temp_tattoos:Tattoos;
        long number_of_tattoos;
        context.Transfer("NumberOfTattoos",number_of_tattoos);
        tattoos_ref.reserve(number_of_tattoos);
        for(long c1=0;c1<number_of_tattoos;++c1) {
            CCharString def_name;
            context.Transfer("Tattoo"+GFIntToCharString(c1),def_name);
            CDefPointer<const CThingBaseDef> pthing_def;
            if(GetDefinitionManager().GetDef(def_name,&pthing_def))tattoos_ref.push_back(pthing_def);
        }
    }
}
int main(){
 for(int i=0;i<4;++i){char s[8];sprintf(s,"Def%d",i);defs[i].name=s;}
 for(int mode=0;mode<8;++mode)for(int n=0;n<=4;++n)for(foundMask=0;foundMask<16;++foundMask){
  {CTCHeroMorph h;float* attrs[]={&h.Strength,&h.Will,&h.Skill,&h.Age,&h.Morality,&h.Fatness,&h.Tan};
   CPersistContext c;c.save=(mode&1)!=0;world.discard=(mode&2)!=0;c.n=n;c.names=c.scalar=0;worldChecks=lookupCount=0;
   for(int j=0;j<7;++j){*attrs[j]=float(12+j);c.vals[j]=float(-6+j);}h.Teenager=true;c.teen=false;h.Berserk=51;
   int pre=c.save?n:1;for(int j=0;j<pre;++j){CDefPointer<const CThingBaseDef> v;v.reset(defs+j);h.Tattoos.push_back(v);}
   h.OnSerialise(c,(mode&4)!=0);
   for(int j=0;j<7;++j){assert(*attrs[j]==(c.save?float(12+j):float(-6+j)));assert(c.vals[j]==*attrs[j]);}
   assert(c.teen==c.save&&h.Teenager==c.save&&h.Berserk==51&&c.n==n&&c.scalar==9&&c.names==n);
   assert(worldChecks==!c.save&&lookupCount==(c.save?0:n));
   int k=pre;if(!c.save&&!world.discard)for(int j=0;j<n;++j)if(foundMask&(1<<j)){assert(h.Tattoos[k++].p==defs+j);}
   assert(int(h.Tattoos.size())==k);for(int j=0;j<4;++j){int expected=(j<pre)+(!c.save&&!world.discard&&j<n&&!!(foundMask&(1<<j)));assert(defs[j].refs==expected);}
  }
  for(int j=0;j<4;++j)assert(defs[j].refs==0);
 }
 puts("0071d020_TEST PASS");return 0;
}