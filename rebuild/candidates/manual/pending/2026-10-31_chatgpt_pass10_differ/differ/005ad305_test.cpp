// 005ad305 behavior fixture: selected target body pasted unchanged.
// Real objects and owning handles. Fixture-only constructors and reduced unused interfaces.
// Container and engine boundaries are checked separately from native layout and byte parity.
#include <stdio.h>
static int failures=0,liveVectors=0,liveSets=0;static void Check(bool x){if(!x)++failures;}
namespace std{
 template<class T>class vector{T*data;unsigned int used,capacity;public:vector():data(0),used(0),capacity(0){++liveVectors;}~vector(){delete[]data;--liveVectors;}unsigned int size()const{return used;}bool empty()const{return used==0;}T*begin(){return data;}const T*begin()const{return data;}T&operator[](unsigned int i){Check(i<used);return data[i];}const T&operator[](unsigned int i)const{Check(i<used);return data[i];}void push_back(const T&x){if(used==capacity){unsigned int n=capacity?capacity*2:4;T*p=new T[n];for(unsigned int j=0;j<used;++j)p[j]=data[j];delete[]data;data=p;capacity=n;}data[used++]=x;}};
 template<class T>class set{public:T values[8];unsigned int size_;set():size_(0){++liveSets;}~set(){--liveSets;}void insert(const T&x){unsigned int j=0;while(j<size_&&values[j]<x)++j;if(j<size_&&values[j]==x)return;Check(size_<8);for(unsigned int k=size_;k>j;--k)values[k]=values[k-1];values[j]=x;++size_;}};
}
class CDefPointeeBase{public:mutable int refs,adds,releases;CDefPointeeBase():refs(1),adds(0),releases(0){}virtual~CDefPointeeBase(){}void AddRef()const{++refs;++adds;}void Release()const{Check(refs>1);--refs;++releases;}};
template<class T>class CDefPointer{T*Object;public:CDefPointer():Object(0){}CDefPointer(const CDefPointer&o):Object(o.Object){if(Object)Object->AddRef();}~CDefPointer(){if(Object)Object->Release();}CDefPointer&operator=(const CDefPointer&o){Reset(o.Object);return*this;}void Reset(T*p){if(Object!=p){if(Object)Object->Release();Object=p;if(Object)Object->AddRef();}}T*Get()const{return Object;}T*operator->()const{return Object;}};
class CTattooDef:public CDefPointeeBase{public:bool Permanent,RemovesAllTattoos;long ReplacingTextureIndex;CTattooDef():Permanent(false),RemovesAllTattoos(false),ReplacingTextureIndex(0){}};
class CThingBaseDef:public CDefPointeeBase{public:int id;const CTattooDef*tattoo;bool available,availableSecond;mutable int subcalls;CThingBaseDef():id(0),tattoo(0),available(false),availableSecond(false),subcalls(0){}bool GetSubDef(CDefPointer<const CTattooDef>*out)const{++subcalls;bool ok=subcalls==1?available:availableSecond;if(!ok)return false;out->Reset(tattoo);return true;}};
class CIEngine{};static CIEngine engine;
static int events[32],eventCount;static void Event(int n){Check(eventCount<32);events[eventCount++]=n;}
static long uploaded[3][8];static unsigned int uploadSizes[3];static int uploads;
class CTCAppearanceMorphBase{public:virtual~CTCAppearanceMorphBase(){}void UploadTextureMorphsToEngine(CIEngine&e,const std::set<long>*layers,bool b,float f){Check(&e==&engine&&!b&&f==0.0f);Check(liveSets==1);Check(uploads<3);if(!layers){uploadSizes[uploads]=99;Event(400);}else{uploadSizes[uploads]=layers->size_;for(unsigned int j=0;j<layers->size_;++j)uploaded[uploads][j]=layers->values[j];Event(401);}++uploads;}};
static CThingBaseDef*original[3];static unsigned int originalCount;
class CTCHeroMorph:public CTCAppearanceMorphBase{public:mutable int gets;int indexAdds,backupAdds,frames;long expectedIndex;CTCHeroMorph():gets(0),indexAdds(0),backupAdds(0),frames(0),expectedIndex(0){}void GetTattooThings(std::vector<CDefPointer<const CThingBaseDef> >&out)const{++gets;Event(100);for(unsigned int j=0;j<originalCount;++j){CDefPointer<const CThingBaseDef>p;p.Reset(original[j]);out.push_back(p);}}void AddTattoo(long n){Check(n==expectedIndex);++indexAdds;Event(200);}void AddTattoo(CDefPointer<const CThingBaseDef>&p){Check(p.Get()!=0);++backupAdds;Event(210+p->id);}virtual void FrameUpdate(){++frames;Event(300);}};
class CThing{public:CTCHeroMorph&hero;CTCAppearanceMorphBase&appearance;bool hasConst,hasHero,hasAppearance;mutable int peeks;int heroGets,appearanceGets;CThing(CTCHeroMorph&h,CTCAppearanceMorphBase&a):hero(h),appearance(a),hasConst(false),hasHero(false),hasAppearance(false),peeks(0),heroGets(0),appearanceGets(0){}bool PeekTC(const CTCHeroMorph**out)const{++peeks;if(hasConst){*out=&hero;return true;}return false;}bool GetTC(CTCHeroMorph**out){++heroGets;if(hasHero){*out=&hero;return true;}return false;}bool GetTC(CTCAppearanceMorphBase**out){++appearanceGets;if(hasAppearance){*out=&appearance;return true;}return false;}};
class CDisplayEngine{public:CIEngine&Get3DEngine(){return engine;}};class CMainGameComponent{public:CDisplayEngine display;static CMainGameComponent*Get();CDisplayEngine&GetDisplayEngine(){return display;}};static CMainGameComponent mainGame;CMainGameComponent*CMainGameComponent::Get(){return &mainGame;}
static CTattooDef*newTattoo;static long expectedIndex;static int cardReads;
class CTCInventoryItem{public:static CDefPointer<const CTattooDef>GetTattooCardDef(long n){Check(n==expectedIndex);++cardReads;Event(350);CDefPointer<const CTattooDef>p;p.Reset(newTattoo);return p;}};
class CTCInventoryTrade{public:CThing&ParentThing;CTCInventoryTrade(CThing&t):ParentThing(t){}void ApplyTattooCard(long);};
void CTCInventoryTrade::ApplyTattooCard(long defIndex)
{
    std::vector<CDefPointer<const CThingBaseDef> > tattoos;
    std::vector<CDefPointer<const CThingBaseDef> > exclusive;
    {
        const CTCHeroMorph* morph;
        if (static_cast<const CThing&>(ParentThing).PeekTC(&morph))
            morph->GetTattooThings(tattoos);
    }
    for (unsigned int i = 0; i < tattoos.size(); ++i) {
        CDefPointer<const CTattooDef> def;
        if (tattoos[i]->GetSubDef(&def) && def->Permanent)
            exclusive.push_back(tattoos[i]);
    }
    CTCHeroMorph* heroMorph = 0;
    if (ParentThing.GetTC(&heroMorph)) {
        heroMorph->AddTattoo(defIndex);
        heroMorph->FrameUpdate();
    }
    CTCAppearanceMorphBase* morph;
    if (ParentThing.GetTC(&morph)) {
        CDefPointer<const CTattooDef> tattoo = CTCInventoryItem::GetTattooCardDef(defIndex);
        std::set<long> layers;
        if (tattoo->RemovesAllTattoos) {
            morph->UploadTextureMorphsToEngine(CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine(), 0, false, 0.0f);
            unsigned int count = exclusive.size();
            CDefPointer<const CThingBaseDef>* items = exclusive.begin();
            for (unsigned int i = 0; i < count; ++i) {
                if (heroMorph) {
                    heroMorph->AddTattoo(items[i]);
                    heroMorph->FrameUpdate();
                }
                CDefPointer<const CTattooDef> def;
                if (items[i]->GetSubDef(&def))
                    layers.insert(def->ReplacingTextureIndex);
            }
            if (!exclusive.empty())
                morph->UploadTextureMorphsToEngine(CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine(), &layers, false, 0.0f);
        } else {
            layers.insert(tattoo->ReplacingTextureIndex);
            morph->UploadTextureMorphsToEngine(CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine(), &layers, false, 0.0f);
        }
    }
}

int main(){unsigned int cases=0;
 for(unsigned int count=0;count<=3;++count)for(int pattern=0;pattern<27;++pattern)for(int components=0;components<8;++components)for(int removes=0;removes<2;++removes)for(int repeat=0;repeat<2;++repeat){
  Check(liveVectors==0&&liveSets==0);CTattooDef defs[3];CThingBaseDef things[3];int digits=pattern;int preserved[3],preservedCount=0;for(int j=0;j<3;++j){int state=digits%3;digits/=3;defs[j].Permanent=state==2;defs[j].ReplacingTextureIndex=repeat?7:j+5;things[j].id=j;things[j].tattoo=&defs[j];things[j].available=state!=0;things[j].availableSecond=(j!=1||!repeat);original[j]=&things[j];if((unsigned int)j<count&&state==2)preserved[preservedCount++]=j;}
  CTattooDef card;card.RemovesAllTattoos=removes!=0;card.ReplacingTextureIndex=9;newTattoo=&card;expectedIndex=123;originalCount=count;
  CTCHeroMorph hero;hero.expectedIndex=expectedIndex;CTCAppearanceMorphBase appearance;CThing parent(hero,appearance);parent.hasConst=(components&1)!=0;parent.hasHero=(components&2)!=0;parent.hasAppearance=(components&4)!=0;CTCInventoryTrade inventory(parent);
  if(!parent.hasConst)preservedCount=0;eventCount=uploads=cardReads=0;inventory.ApplyTattooCard(expectedIndex);
  int expectedEvents[32],n=0;if(parent.hasConst)expectedEvents[n++]=100;if(parent.hasHero){expectedEvents[n++]=200;expectedEvents[n++]=300;}if(parent.hasAppearance){expectedEvents[n++]=350;if(removes){expectedEvents[n++]=400;for(int j=0;j<preservedCount;++j)if(parent.hasHero){expectedEvents[n++]=210+preserved[j];expectedEvents[n++]=300;}if(preservedCount)expectedEvents[n++]=401;}else expectedEvents[n++]=401;}
  Check(n==eventCount);for(int j=0;j<n&&j<eventCount;++j)Check(events[j]==expectedEvents[j]);Check(parent.peeks==1&&parent.heroGets==1&&parent.appearanceGets==1);Check(hero.gets==(parent.hasConst?1:0));Check(hero.indexAdds==(parent.hasHero?1:0));int expectedBackups=parent.hasHero&&parent.hasAppearance&&removes?preservedCount:0;Check(hero.backupAdds==expectedBackups&&hero.frames==(parent.hasHero?1:0)+expectedBackups);Check(cardReads==(parent.hasAppearance?1:0));
  if(parent.hasAppearance){if(removes){Check(uploadSizes[0]==99);Check(uploads==1+(preservedCount?1:0));if(preservedCount){long wanted[3];int wn=0;for(int j=0;j<preservedCount;++j){int id=preserved[j];if(!things[id].availableSecond)continue;long layer=defs[id].ReplacingTextureIndex;bool seen=false;for(int k=0;k<wn;++k)if(wanted[k]==layer)seen=true;if(!seen)wanted[wn++]=layer;}Check(uploadSizes[1]==(unsigned int)wn);for(int j=0;j<wn;++j)Check(uploaded[1][j]==wanted[j]);}}else{Check(uploads==1&&uploadSizes[0]==1&&uploaded[0][0]==9);}}else Check(uploads==0);
  for(int j=0;j<3;++j){int queries=parent.hasConst&&(unsigned int)j<count?1:0;if(parent.hasAppearance&&removes&&parent.hasConst&&(unsigned int)j<count&&defs[j].Permanent)++queries;Check(things[j].subcalls==queries);Check(things[j].refs==1&&things[j].adds==things[j].releases);Check(defs[j].refs==1&&defs[j].adds==defs[j].releases);}Check(card.refs==1&&card.adds==card.releases);Check(liveVectors==0&&liveSets==0);++cases;
 }
 Check(cases==3456);if(failures){fprintf(stderr,"failures=%d cases=%u\n",failures,cases);return 1;}puts("005ad305_TEST PASS");return 0;
}
