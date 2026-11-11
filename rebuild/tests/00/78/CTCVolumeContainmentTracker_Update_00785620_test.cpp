// Self-contained C++03 dependency fixture; exact selected function body follows.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#define CHECK(x) do{if(!(x)){std::fprintf(stderr,"check failed line %d case %ld: %s\n",__LINE__,cases,#x);std::exit(1);}}while(0)
static long cases=0;
class C3DVector{ public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}};
class C2DCoordI{ public:long X,Y;C2DCoordI(long x,long y):X(x),Y(y){}};
class C2DBoxI{ public:long TLX,TLY,BRX,BRY;C2DBoxI(const C2DCoordI& p,long r):TLX(p.X-r),TLY(p.Y-r),BRX(p.X+r),BRY(p.Y+r){}};
class CEnginePrimitiveThemeChannelDesc{ public:unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad;CEnginePrimitiveThemeChannelDesc(int a=0,int b=0,int c=0,int p=0):ChannelIndex0(a),ChannelIndex1(b),ChannelBlendFactor(c),Pad(p){}};
static bool equal(const CEnginePrimitiveThemeChannelDesc&a,const CEnginePrimitiveThemeChannelDesc&b){return a.ChannelIndex0==b.ChannelIndex0&&a.ChannelIndex1==b.ChannelIndex1&&a.ChannelBlendFactor==b.ChannelBlendFactor&&a.Pad==b.Pad;}
struct Ref{mutable long refs;Ref():refs(100){}};
template<class T>class CDefPointer{ public:T* p;CDefPointer(T*q=0):p(q){if(p)++p->refs;}CDefPointer(const CDefPointer&o):p(o.p){if(p)++p->refs;}~CDefPointer(){if(p)--p->refs;}CDefPointer&operator=(const CDefPointer&o){if(o.p)++o.p->refs;if(p)--p->refs;p=o.p;return *this;}T*operator->()const{return p;}};
template<class T>class CIntelligentPointer{ public:T*p;CIntelligentPointer(T*q=0):p(q){}T*Get()const{return p;}T*operator->()const{return p;}CIntelligentPointer&operator=(T*q){p=q;return*this;}};
template<class T>bool operator!=(const CIntelligentPointer<T>&p,int){return p.Get()!=0;}
template<class T>bool IS_THING_ALIVE(const T&p){return p!=0&&!p->IsDead();}
class CCharString{ public:const char*p;CCharString(const char*q):p(q){}};
static const CCharString BUILDING_BOUNDRY("BUILDING_BOUNDRY");
enum ETCInterfaceType{TCI_DOOR=38,TCI_SHOP=83,TCI_TAVERN=171};
class CShopDef:public Ref{ public:bool IsStockDisplayPermanent;CShopDef():IsStockDisplayPermanent(false){}};
class CThingBaseDef:public Ref{ public:CShopDef*shop;bool found;CThingBaseDef():shop(0),found(false){}template<class T>bool GetSubDef(CDefPointer<const T>*out)const{if(!found)return false;*out=CDefPointer<const T>(shop);return true;}};
namespace NGraphicAppearance{class CTCGraphicAppearance{ public:CEnginePrimitiveThemeChannelDesc channel;int writes,changed;CTCGraphicAppearance():writes(0),changed(0){}const CEnginePrimitiveThemeChannelDesc&PeekLightingChannel()const{return channel;}void SetLightingChannel(const CEnginePrimitiveThemeChannelDesc&v,bool a,bool b){CHECK(a&&b);channel=v;++writes;}void SetAsChanged(unsigned long f){CHECK(f==4);++changed;}};}
class CThing;
class CTCCarrying{ public:std::vector<CThing*>things;int reads;CTCCarrying():reads(0){}void GetAllCarriedThings(std::vector<CThing*>&out){++reads;out=things;}};
class CSoundPair{ public:long FirstSound,LastSound;CSoundPair(long a,long b):FirstSound(a),LastSound(b){}};
class CSoundInit{ public:float Volume,Pitch,Pan;long NoPlays,PlaybackType;unsigned long GainFaderBits,PitchFaderBits;long OverrideID;CSoundInit():Volume(1),Pitch(1),Pan(0),NoPlays(1),PlaybackType(1),GainFaderBits(0),PitchFaderBits(0),OverrideID(-1){}};
class CTCSoundPlayer{ public:int plays;long sample;CTCSoundPlayer():plays(0),sample(0){}unsigned long PlayAttachedSample(const CSoundPair&p,const CSoundInit&i,bool a,bool b,bool c){CHECK(p.FirstSound==p.LastSound);CHECK(i.Volume==1&&i.Pitch==1&&i.Pan==0&&i.NoPlays==1&&i.PlaybackType==1&&i.GainFaderBits==0x8010&&i.PitchFaderBits==0&&i.OverrideID==-1);CHECK(a&&!b&&!c);++plays;sample=p.FirstSound;return 1;}};
class CThing{ public:bool dead,shop,tavern;unsigned long type;C3DVector pos;NGraphicAppearance::CTCGraphicAppearance*app;CTCCarrying*carry;CTCSoundPlayer*sound;CThingBaseDef*base;CThing():dead(false),shop(false),tavern(false),type(1),app(0),carry(0),sound(0),base(0){}bool IsDead()const{return dead;}unsigned long GetType()const{return type;}bool HasTC(ETCInterfaceType t)const{return t==TCI_SHOP?shop:t==TCI_TAVERN?tavern:false;}CDefPointer<const CThingBaseDef>GetPBaseDef()const{return CDefPointer<const CThingBaseDef>(base);}const C3DVector&GetPos()const{return pos;}NGraphicAppearance::CTCGraphicAppearance*GetTCGraphicAppearance(){return app;}bool GetTC(NGraphicAppearance::CTCGraphicAppearance**p){if(!app)return false;*p=app;return true;}bool GetTC(CTCCarrying**p){if(!carry)return false;*p=carry;return true;}bool GetTC(CTCSoundPlayer**p){if(!sound)return false;*p=sound;return true;}};
class CThingBuilding:public CThing{ public:};
class CThingFilter_OverlappingSphere{ public:C3DVector pos;float radius;CThingFilter_OverlappingSphere(const C3DVector&p,float r):pos(p),radius(r){}};
class CThingFilter_PosInContainmentVolume{ public:C3DVector pos;CCharString volume;CThingFilter_PosInContainmentVolume(const C3DVector&p,const CCharString&v):pos(p),volume(v){}};
template<class A,class B>class CFilter_Bind2{ public:const A&a;const B&b;CFilter_Bind2(const A&x,const B&y):a(x),b(y){}};
template<class A,class B>CFilter_Bind2<A,B>GFFilter_Bind2(const A&a,const B&b){return CFilter_Bind2<A,B>(a,b);}
class CThingFilter_HasTC{ public:ETCInterfaceType type;CThingFilter_HasTC(ETCInterfaceType t):type(t){}};
class CThingCompare_Nearest{ public:C3DVector pos;CThingCompare_Nearest(const C3DVector&p):pos(p){}};
static C3DVector expected_pos;
static bool pos_equal(const C3DVector&a,const C3DVector&b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
class CThingSearchTools{ public:CThingBuilding*inside;CThing*door;int containment_calls,door_calls;CThingSearchTools():inside(0),door(0),containment_calls(0),door_calls(0){}template<class F>bool GetPFirstLeafThing(const F&f,CThingBuilding**out){++containment_calls;CHECK(pos_equal(f.a.pos,expected_pos)&&f.a.radius==0);CHECK(pos_equal(f.b.pos,expected_pos)&&std::strcmp(f.b.volume.p,"BUILDING_BOUNDRY")==0);if(!inside)return false;*out=inside;return true;}bool GetPBestThingInRoughArea(const C2DBoxI&b,const CThingFilter_HasTC&f,const CThingCompare_Nearest&c,CThing**out)const{CThingSearchTools*self=const_cast<CThingSearchTools*>(this);++self->door_calls;CHECK(b.TLX==(long)expected_pos.X-3&&b.TLY==(long)expected_pos.Y-3&&b.BRX==(long)expected_pos.X+3&&b.BRY==(long)expected_pos.Y+3);CHECK(f.type==TCI_DOOR&&pos_equal(c.pos,expected_pos));if(!door)return false;*out=door;return true;}};
class CWorld{ public:CThingSearchTools tools;CThingSearchTools&PeekThingSearchTools(){return tools;}};
struct CASoundManager{bool SoundEnabled;};static CASoundManager*PSoundManager=0;
class CASoundBank{ public:long index;int calls;CASoundBank():index(0),calls(0){}long GetSoundIndexFromSymbol(const CCharString&s){CHECK(std::strcmp(s.p,"SND_SHOPBELL_01")==0);++calls;return index;}};
class CMainGameComponent{ public:CASoundBank bank;static CMainGameComponent*current;static CMainGameComponent*Get(){return current;}CASoundBank&GetSampleBank(){return bank;}};CMainGameComponent*CMainGameComponent::current=0;
class CTCVolumeContainmentTrackerDef:public Ref{ public:bool CalculateBlendedLightingChannel;CTCVolumeContainmentTrackerDef():CalculateBlendedLightingChannel(false){}};
class CTCVolumeContainmentTracker{ public:CThing&parent;CWorld&world;CIntelligentPointer<CThing>PThingInside;CEnginePrimitiveThemeChannelDesc CurrentThemeChannel;bool PreviouslyUpdated;CDefPointer<const CTCVolumeContainmentTrackerDef>PDef;bool UpdateLightingChannels;int blends;CTCVolumeContainmentTracker(CThing&p,CWorld&w,CTCVolumeContainmentTrackerDef&d):parent(p),world(w),PreviouslyUpdated(false),PDef(&d),UpdateLightingChannels(false),blends(0){}CThing&GetParentThing(){return parent;}CWorld&GetWorld(){return world;}const CThingSearchTools&PeekThingSearchTools(){return world.tools;}void CalculateBlendedLightingChannel(CEnginePrimitiveThemeChannelDesc&c){++blends;c=CEnginePrimitiveThemeChannelDesc(5,6,7,8);}void Update();};

void CTCVolumeContainmentTracker::Update()
{
 if(UpdateLightingChannels) {
  CurrentThemeChannel.ChannelIndex0=0;
  CurrentThemeChannel.ChannelIndex1=0;
  CurrentThemeChannel.ChannelBlendFactor=0;
 }
 CThing* last_thing_inside=PThingInside.Get();
 C3DVector parent_pos=GetParentThing().GetPos();
 CThingBuilding* pthing;
 if(GetWorld().PeekThingSearchTools().GetPFirstLeafThing(GFFilter_Bind2(CThingFilter_OverlappingSphere(parent_pos,0),CThingFilter_PosInContainmentVolume(parent_pos,BUILDING_BOUNDRY)),&pthing)) {
  PThingInside=pthing;
  if(UpdateLightingChannels&&!PDef->CalculateBlendedLightingChannel) {
   NGraphicAppearance::CTCGraphicAppearance* graphic_app=0;
   if(pthing->GetTC(&graphic_app))CurrentThemeChannel=graphic_app->PeekLightingChannel();
  }
 } else PThingInside=(CThing*)0;
 if(UpdateLightingChannels) {
  if(PDef->CalculateBlendedLightingChannel)CalculateBlendedLightingChannel(CurrentThemeChannel);
  NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance=GetParentThing().GetTCGraphicAppearance();
  if(ptc_graphic_appearance) {
   const CEnginePrimitiveThemeChannelDesc& existing_channel=ptc_graphic_appearance->PeekLightingChannel();
   if(existing_channel.ChannelIndex0!=CurrentThemeChannel.ChannelIndex0||existing_channel.ChannelIndex1!=CurrentThemeChannel.ChannelIndex1||existing_channel.ChannelBlendFactor!=CurrentThemeChannel.ChannelBlendFactor) {
    ptc_graphic_appearance->SetLightingChannel(CurrentThemeChannel,true,true);
    ptc_graphic_appearance->SetAsChanged(4);
    CTCCarrying* ptc_carrying;
    if(GetParentThing().GetTC(&ptc_carrying)) {
     std::vector<CThing*> carried_things;
     ptc_carrying->GetAllCarriedThings(carried_things);
     for(long c1=0;c1<carried_things.size();++c1) {
      CThing* pcarried_thing=carried_things[c1];
      if(IS_THING_ALIVE(pcarried_thing)&&pcarried_thing->GetTCGraphicAppearance()) {
       pcarried_thing->GetTCGraphicAppearance()->SetLightingChannel(CurrentThemeChannel,true,true);
       pcarried_thing->GetTCGraphicAppearance()->SetAsChanged(4);
      }
     }
    }
   }
  }
 }
 if(PSoundManager&&PSoundManager->SoundEnabled&&PreviouslyUpdated&&last_thing_inside!=PThingInside.Get()&&(GetParentThing().GetType()==1||GetParentThing().GetType()==2)) {
  CDefPointer<const CShopDef> shop_def;
  if((IS_THING_ALIVE(PThingInside)&&PThingInside->HasTC(TCI_SHOP)&&!PThingInside->HasTC(TCI_TAVERN)&&PThingInside->GetPBaseDef()->GetSubDef(&shop_def)&&shop_def->IsStockDisplayPermanent)||
     (IS_THING_ALIVE(last_thing_inside)&&last_thing_inside->HasTC(TCI_SHOP)&&!last_thing_inside->HasTC(TCI_TAVERN)&&last_thing_inside->GetPBaseDef()->GetSubDef(&shop_def)&&shop_def->IsStockDisplayPermanent)) {
   CThing* sound_thing=0;
   const C3DVector& pos=GetParentThing().GetPos();
   C2DBoxI box(C2DCoordI((long)pos.X,(long)pos.Y),3);
   if(PeekThingSearchTools().GetPBestThingInRoughArea(box,CThingFilter_HasTC(TCI_DOOR),CThingCompare_Nearest(GetParentThing().GetPos()),&sound_thing)) {
    CTCSoundPlayer* sound_player;
    if(sound_thing->GetTC(&sound_player)) {
     long sound;
     {
      CCharString symbol("SND_SHOPBELL_01");
      sound=CMainGameComponent::Get()->GetSampleBank().GetSoundIndexFromSymbol(symbol);
     }
     if(sound) {
      CSoundInit init;
      init.GainFaderBits=0x8010;
      sound_player->PlayAttachedSample(CSoundPair(sound,sound),init,true,false,false);
     }
    }
   }
  }
 }
 PreviouslyUpdated=true;
}
static void configure(CThingBuilding&thing,CThingBaseDef&base,CShopDef&shop,int profile,NGraphicAppearance::CTCGraphicAppearance&app){thing.base=&base;base.shop=&shop;thing.dead=profile==7;thing.shop=profile>=2;thing.tavern=profile==6;base.found=profile>=4;shop.IsStockDisplayPermanent=profile>=5;thing.app=(profile&1)?&app:0;app.channel=CEnginePrimitiveThemeChannelDesc(11,12,13,14);}
static bool bell_shop(const CThingBuilding*p){if(!p||p->dead||!p->shop||p->tavern)return false;return p->base->found&&p->base->shop->IsStockDisplayPermanent;}
int main(){
 for(int mode=0;mode<16;++mode)for(int cm=0;cm<4;++cm)for(int oldp=0;oldp<8;++oldp)for(int newp=0;newp<9;++newp)for(int sm=0;sm<3;++sm)for(int prev=0;prev<2;++prev)for(int ty=1;ty<=3;++ty)for(int door_mode=0;door_mode<5;++door_mode){
  ++cases;
  CWorld world;CThing parent;CThingBuilding old_building,new_building;CThingBaseDef old_base,new_base;CShopDef old_shop,new_shop;NGraphicAppearance::CTCGraphicAppearance old_app,new_app,parent_app,carry_app,dead_app;CTCCarrying carrying;CTCSoundPlayer sound_player;CThing door,live_carried,dead_carried,no_app;
  configure(old_building,old_base,old_shop,oldp,old_app);configure(new_building,new_base,new_shop,newp,new_app);
  CThingBuilding*old_inside=oldp?&old_building:0;CThingBuilding*new_inside=newp==8?old_inside:newp?&new_building:0;
  world.tools.inside=new_inside;world.tools.door=door_mode?&door:0;
  parent.type=ty;parent.app=(mode&4)?&parent_app:0;parent.carry=(mode&8)?&carrying:0;
  expected_pos=C3DVector(cases%3==0?-1.75f:cases%3==1?1.75f:128.0f,-2.5f,4.0f);parent.pos=expected_pos;
  door.sound=door_mode>=2?&sound_player:0;
  CMainGameComponent main_component;CMainGameComponent::current=&main_component;main_component.bank.index=door_mode==3?32:door_mode==4?-7:0;
  CASoundManager manager;manager.SoundEnabled=sm==2;PSoundManager=sm?&manager:0;
  CTCVolumeContainmentTrackerDef definition;definition.CalculateBlendedLightingChannel=(mode&2)!=0;
  CTCVolumeContainmentTracker tracker(parent,world,definition);tracker.PThingInside=old_inside;tracker.PreviouslyUpdated=prev!=0;tracker.UpdateLightingChannels=(mode&1)!=0;tracker.CurrentThemeChannel=CEnginePrimitiveThemeChannelDesc(1,2,3,99);
  CEnginePrimitiveThemeChannelDesc expected=tracker.CurrentThemeChannel;
  if(mode&1){expected.ChannelIndex0=expected.ChannelIndex1=expected.ChannelBlendFactor=0;if(!(mode&2)&&new_inside&&new_inside->app)expected=new_inside->app->channel;if(mode&2)expected=CEnginePrimitiveThemeChannelDesc(5,6,7,8);}
  parent_app.channel=expected;if(cm==1)parent_app.channel.Pad=77;else if(cm==2)++parent_app.channel.ChannelIndex0;else if(cm==3)++parent_app.channel.ChannelBlendFactor;
  CEnginePrimitiveThemeChannelDesc before=parent_app.channel;
  live_carried.app=&carry_app;dead_carried.app=&dead_app;dead_carried.dead=true;
  carrying.things.push_back(0);carrying.things.push_back(&dead_carried);carrying.things.push_back(&no_app);carrying.things.push_back(&live_carried);carrying.things.push_back(&parent);carrying.things.push_back(&live_carried);
  bool changed=(mode&1)&&(mode&4)&&cm>=2;
  bool bell_search=sm==2&&prev&&old_inside!=new_inside&&ty<=2&&(bell_shop(new_inside)||bell_shop(old_inside));
  bool lookup=bell_search&&door_mode>=2;bool play=lookup&&door_mode>=3;
  tracker.Update();
  CHECK(tracker.PThingInside.Get()==new_inside&&tracker.PreviouslyUpdated);CHECK(equal(tracker.CurrentThemeChannel,expected));CHECK(tracker.blends==(((mode&1)&&(mode&2))?1:0));CHECK(world.tools.containment_calls==1&&world.tools.door_calls==(bell_search?1:0));
  CHECK(parent_app.writes==(changed?1+((mode&8)?1:0):0));CHECK(parent_app.changed==parent_app.writes);CHECK(equal(parent_app.channel,changed?expected:before));
  CHECK(carrying.reads==((changed&&(mode&8))?1:0));CHECK(carry_app.writes==((changed&&(mode&8))?2:0));CHECK(carry_app.changed==carry_app.writes);if(carry_app.writes)CHECK(equal(carry_app.channel,expected));CHECK(dead_app.writes==0);
  CHECK(old_app.writes==0&&new_app.writes==0);CHECK(main_component.bank.calls==(lookup?1:0));CHECK(sound_player.plays==(play?1:0));if(play)CHECK(sound_player.sample==main_component.bank.index);
  CHECK(old_base.refs==100&&new_base.refs==100&&old_shop.refs==100&&new_shop.refs==100&&definition.refs==101);
 }
 CHECK(cases==414720);
 std::puts("00785620_TEST PASS");return 0;
}