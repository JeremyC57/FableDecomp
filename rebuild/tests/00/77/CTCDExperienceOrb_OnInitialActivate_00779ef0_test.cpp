// Full native OnInitialActivate and GetSubDef bodies with portable fixtures.
// Tree header has real value storage for the observed rend() boundary read; no fixture pointer leaves its backing nodes.
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
class CCharString {public:const char* Text;CCharString(const char* text,long=-1):Text(text){}};
static unsigned int FloatBits(float f){unsigned int u;memcpy(&u,&f,4);return u;}
static long X87FistpNear(double v){if(!(v>=-2147483648.5 && v<2147483647.5))return -2147483647L-1L;long t=(long)v;double r=v-(double)t;if(r>0.5)++t;else if(r<-.5)--t;else if((r==.5||r==-.5)&&(t&1L)!=0)t+=(r>0?1L:-1L);return t;}
static long GFFloatToLongCeil(float value){long result=X87FistpNear((double)value+.5);float back=(float)((double)result-1.0);if(FloatBits(value)==FloatBits(back))result=(long)(int)((unsigned int)result-1u);return result;}
namespace std {
template<class K,class V>class map {
public:
 struct value_type {K first;V second;};
 struct Node {bool Colour;Node* Parent;Node* Left;Node* Right;value_type Value;};
 Node Header,Nodes[3];int Count;
 class const_iterator {public:Node* Current;const_iterator(Node* p):Current(p){}const value_type* operator->()const{return &Current->Value;}
  const_iterator& operator--(){Node* n=Current;if(!n->Colour && n->Parent->Parent==n)n=n->Right;else if(n->Left){Node* y=n->Left;while(y->Right)y=y->Right;n=y;}else{Node* y=n->Parent;while(n==y->Left){n=y;y=y->Parent;}n=y;}Current=n;return *this;}
  const_iterator& operator++(){Node* n=Current;if(n->Right){n=n->Right;while(n->Left)n=n->Left;}else{Node* y=n->Parent;while(n==y->Right){n=y;y=y->Parent;}if(n->Right!=y)n=y;}Current=n;return *this;}
  bool operator!=(const const_iterator& p)const{return Current!=p.Current;}
 };
 class const_reverse_iterator {const_iterator Current;public:const_reverse_iterator(const_iterator i):Current(i){}const value_type* operator->()const{const_iterator tmp=Current;--tmp;return tmp.operator->();}};
 const_iterator begin()const{return const_iterator(Header.Left);}const_iterator end()const{return const_iterator(const_cast<Node*>(&Header));}
 const_reverse_iterator rend()const{return const_reverse_iterator(begin());}
 void Build(int count){Count=count;Header.Colour=false;Header.Parent=&Nodes[count==3?1:0];Header.Left=&Nodes[0];Header.Right=&Nodes[count-1];Header.Value.first=-991;Header.Value.second=(V)12.5f;
  for(int i=0;i<3;++i){Nodes[i].Colour=true;Nodes[i].Parent=&Header;Nodes[i].Left=Nodes[i].Right=0;Nodes[i].Value.first=(K)(i==0?10:i==1?30:100);Nodes[i].Value.second=(V)(i==0?1.25f:i==1?2.5f:4.0f);}
  if(count==2){Nodes[0].Right=&Nodes[1];Nodes[1].Parent=&Nodes[0];}
  if(count==3){Nodes[1].Left=&Nodes[0];Nodes[1].Right=&Nodes[2];Nodes[0].Parent=Nodes[2].Parent=&Nodes[1];}
 }
};
}
class CDefPointeeBase {public:int RefCount;CDefPointeeBase():RefCount(1){}virtual ~CDefPointeeBase(){}virtual void Delete(){assert(false);}};
class CDefClassBase;
template<class T>class CDefPointer {public:CDefPointeeBase* Object;
 CDefPointer():Object(0){}CDefPointer(CDefPointeeBase* p):Object(p){if(Object)++Object->RefCount;}
 CDefPointer(const CDefPointer& p):Object(p.Object){if(Object)++Object->RefCount;}
 template<class U>CDefPointer(const CDefPointer<U>& p):Object(p.Object){if(Object)++Object->RefCount;}
 ~CDefPointer(){Release();}void Release(){if(Object){CDefPointeeBase* p=Object;if(--p->RefCount==0)p->Delete();Object=0;}}
 T* operator->()const{return static_cast<T*>(Object);}T& operator*()const{return *static_cast<T*>(Object);}
 template<class U>void SetDynamicCast(const CDefPointer<U>& source){CDefPointeeBase* p=source.Object;if(Object!=p){Release();Object=p;if(Object)++Object->RefCount;}}
};
class CDefinitionManager {public:CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long);};
class CDefClassBase:public CDefPointeeBase {public:struct CSubDefInfo{long DefIndex;};CDefinitionManager* PDefManager;
 const CSubDefInfo* GetPSubDefInfo(const CCharString&)const;
 template<class T>bool GetSubDef(CDefPointer<const T>* out)const;
};
template<class T>bool CDefClassBase::GetSubDef(CDefPointer<const T>* out)const
{
 const CSubDefInfo* info=GetPSubDefInfo(T::StatGetTypeName());
 if(info){
  CDefPointer<const CDefClassBase> def=PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
  out->SetDynamicCast(def);
  return true;
 }
 return false;
}
class CExperienceDef:public CDefClassBase {public:static CCharString StatGetTypeName(){return CCharString("CExperienceDef",-1);}std::map<long,float> LifeTimeForDistanceThresholds;};
class CSoundPair {public:long FirstSound,LastSound;};
static CSoundPair Pair;static bool PairExists,SoundDefExists;static int SoundQueries,SubDefQueries;
class CSoundMap {public:const CSoundPair* GetPSoundPair(const CCharString& name)const{assert(strcmp(name.Text,"AMBIENT_SOUND")==0);++SoundQueries;return PairExists?&Pair:0;}};
class CEntitySoundDef:public CDefClassBase {public:static CCharString StatGetTypeName(){return CCharString("CEntitySoundDef",-1);}CSoundMap SoundMap;};
static CExperienceDef Experience;static CEntitySoundDef SoundDef;static CDefinitionManager Definitions;
static CDefClassBase::CSubDefInfo ExperienceInfo={1},SoundInfo={2};
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString& type)const{++SubDefQueries;if(strcmp(type.Text,"CExperienceDef")==0)return &ExperienceInfo;assert(strcmp(type.Text,"CEntitySoundDef")==0);return SoundDefExists?&SoundInfo:0;}
CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index){return CDefPointer<CDefClassBase>(index==1?(CDefClassBase*)&Experience:(CDefClassBase*)&SoundDef);}
enum ESoundPlaybackType{PLAY_NORMAL=1};
class CSoundInit {public:float Volume,Pitch,Pan;long NoPlays;ESoundPlaybackType PlaybackType;unsigned int GainFaderBits,PitchFaderBits;long OverrideID;
 CSoundInit():Volume(1.0f),Pitch(1.0f),Pan(0.0f),NoPlays(1),GainFaderBits(0),PitchFaderBits(0),OverrideID(-1){PlaybackType=(ESoundPlaybackType)1;}
};
static int SoundPlays,BaseCalls,ParentPosReads,HeroPosReads,PlayerQueries,TCQueries;
class CTCSoundPlayer {public:unsigned long PlayAttachedSample(const CSoundPair& pair,const CSoundInit& init,bool a,bool b,bool c){assert(&pair==&Pair);assert(init.Volume==140.0f && init.Pitch==1.0f && init.Pan==0.0f && init.NoPlays==1 && init.PlaybackType==(ESoundPlaybackType)1 && init.GainFaderBits==0x8010 && init.PitchFaderBits==0 && init.OverrideID==-1 && !a && !b && !c);++SoundPlays;return 0;}};
class C3DVector {public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector operator-(const C3DVector& p)const{return C3DVector(X-p.X,Y-p.Y,Z-p.Z);}float GetAccurateMagnitude()const{float magnitude=X*X+Y*Y+Z*Z;return(float)sqrt(magnitude);}};
class CThing {public:CDefClassBase Def;C3DVector Pos;bool Dead,Hero;CTCSoundPlayer* SoundPlayer;
 CThing():Dead(false),Hero(false),SoundPlayer(0){Def.PDefManager=&Definitions;}
 const CDefClassBase& PeekBaseDef()const{return Def;}const C3DVector& GetPos()const{if(Hero)++HeroPosReads;else ++ParentPosReads;return Pos;}
 bool IsDead()const{return Dead;}bool GetTC(CTCSoundPlayer** out){++TCQueries;if(SoundPlayer){*out=SoundPlayer;return true;}return false;}
};
class CThingPlayerCreature:public CThing{};
class CPlayer {public:CThingPlayerCreature* Creature;CThingPlayerCreature* GetPControlledCreature()const{return Creature;}};
class CPlayerManager {public:CPlayer Player;CPlayer& GetMainPlayer(){++PlayerQueries;return Player;}};
static CPlayerManager Players;
class CTCBase {public:CThing* ParentThing;virtual void OnInitialActivate(){++BaseCalls;}CThing& GetParentThing(){return *ParentThing;}CPlayerManager& GetPlayerManager(){return Players;}};
class CMainGameComponent {public:static int FPS;static long GetConstantFPS(){return FPS;}};int CMainGameComponent::FPS=15;
class CTCDExperienceOrb:public CTCBase {public:CDefPointer<const CExperienceDef> PDef;long ExperiencePoints,CombinationImmunityTimer,LifeTimer;bool DecayOverTime,PickedUp;
 virtual void OnInitialActivate();private:const CExperienceDef& GetDef()const{return *PDef;}
};
void CTCDExperienceOrb::OnInitialActivate()
{
    CTCBase::OnInitialActivate();
    GetParentThing().PeekBaseDef().GetSubDef(&PDef);
    CombinationImmunityTimer = GFFloatToLongCeil(CMainGameComponent::GetConstantFPS() * 0.6f);
    CThingPlayerCreature* creature = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    // Original FableWin01F57870 calls rend(), as does retail; preserve that boundary read.
    LifeTimer = (long)(CMainGameComponent::GetConstantFPS() * GetDef().LifeTimeForDistanceThresholds.rend()->second);
    if (creature && !creature->IsDead()) {
        long distance = (long)(creature->GetPos() - GetParentThing().GetPos()).GetAccurateMagnitude();
        for (std::map<long,float>::const_iterator it = GetDef().LifeTimeForDistanceThresholds.begin(); it != GetDef().LifeTimeForDistanceThresholds.end(); ++it) {
            long threshold = it->first;
            float lifetime = it->second;
            if (distance <= threshold) {
                LifeTimer = (long)(CMainGameComponent::GetConstantFPS() * lifetime);
                break;
            }
        }
    }
    CDefPointer<const CEntitySoundDef> soundDef;
    CTCSoundPlayer* player;
    if (GetParentThing().GetTC(&player)) {
        if (GetParentThing().PeekBaseDef().GetSubDef(&soundDef)) {
            const CSoundPair* sound = soundDef->SoundMap.GetPSoundPair("AMBIENT_SOUND");
            if (sound) {
                CSoundInit init;
                init.GainFaderBits = 0x8010;
                init.Volume = 140.0f;
                player->PlayAttachedSample(*sound, init, false, false, false);
            }
        }
    }
}

int main()
{
 CThing parent;CThingPlayerCreature hero;CTCSoundPlayer player;hero.Hero=true;
 const float distances[]={0.0f,9.75f,10.25f,30.75f,100.75f,101.0f};
 for(int count=1;count<=3;++count)for(int who=0;who<3;++who)for(int d=0;d<6;++d)for(int flags=0;flags<16;++flags){
  Experience.LifeTimeForDistanceThresholds.Build(count);CMainGameComponent::FPS=(flags&8)?23:15;
  SoundDefExists=(flags&2)!=0;PairExists=(flags&4)!=0;parent.SoundPlayer=(flags&1)?&player:0;
  Players.Player.Creature=who?&hero:0;hero.Dead=who==2;hero.Pos=C3DVector(distances[d],0,0);parent.Pos=C3DVector();
  BaseCalls=ParentPosReads=HeroPosReads=PlayerQueries=TCQueries=SoundQueries=SubDefQueries=SoundPlays=0;
  {
   CTCDExperienceOrb orb;orb.ParentThing=&parent;orb.ExperiencePoints=11;orb.DecayOverTime=true;orb.PickedUp=false;
   orb.OnInitialActivate();
   assert(BaseCalls==1 && PlayerQueries==1 && TCQueries==1 && SubDefQueries==((flags&1)?2:1));
   assert(ParentPosReads==(who==1?1:0) && HeroPosReads==(who==1?1:0));
   float seconds=count<3?1.25f:12.5f;
   if(who==1){long distance=(long)distances[d];for(int i=0;i<count;++i){long threshold=i==0?10:i==1?30:100;if(distance<=threshold){seconds=i==0?1.25f:i==1?2.5f:4.0f;break;}}}
   assert(orb.LifeTimer==(long)(CMainGameComponent::FPS*seconds));
   assert(orb.CombinationImmunityTimer==(CMainGameComponent::FPS==15?9:14));
   assert(orb.ExperiencePoints==11 && orb.DecayOverTime && !orb.PickedUp);
   assert(Experience.RefCount==2 && SoundDef.RefCount==1);
   const int expectsQuery=(flags&1)&&SoundDefExists?1:0;assert(SoundQueries==expectsQuery);assert(SoundPlays==(expectsQuery&&PairExists?1:0));
  }
  assert(Experience.RefCount==1 && SoundDef.RefCount==1);
 }
 puts("00779ef0_TEST PASS");return 0;
}