// Complete native Update and GetDef-index bodies with portable callee fixtures.
#include <assert.h>
#include <stdio.h>
#include <string>
#include <vector>
typedef std::string CCharString;
class CDefClassBase {public:int RefCount;CDefClassBase():RefCount(1){}virtual ~CDefClassBase(){}virtual void Delete(){assert(false);}};
template<class T>class CDefPointer {public:CDefClassBase* Object;
 CDefPointer():Object(0){}CDefPointer(CDefClassBase* p):Object(p){if(Object)++Object->RefCount;}
 CDefPointer(const CDefPointer& p):Object(p.Object){if(Object)++Object->RefCount;}
 ~CDefPointer(){Release();}void Release(){if(Object){CDefClassBase* p=Object;if(--p->RefCount==0)p->Delete();Object=0;}}
 bool IsNull()const{return Object==0;}T* operator->()const{return static_cast<T*>(Object);}
 template<class U>void SetDynamicCast(const CDefPointer<U>& p){if(Object!=p.Object){Release();Object=p.Object;if(Object)++Object->RefCount;}}
};
class CSoundThemeDef:public CDefClassBase {public:long MusicSet;};
class CRegionDef:public CDefClassBase {public:long Music;};
static CSoundThemeDef Theme;static CRegionDef RegionDef;static bool LookupSuccess;static int Lookups,SystemReads,PositionReads,RegionCalls;
static std::vector<int> Calls;
class CDefinitionManager {public:
 CDefPointer<const CDefClassBase> GetPDefFromClassIndex(const CCharString& type,long index)const{assert(type=="SOUND_THEME" && index==3);++Lookups;return CDefPointer<const CDefClassBase>(LookupSuccess?&Theme:0);}
 template<class T>bool GetDef(const CCharString& type_name,long index,CDefPointer<const T>* out)const;
};
template<class T>bool CDefinitionManager::GetDef(const CCharString& type_name,long index,CDefPointer<const T>* out)const
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> def = GetPDefFromClassIndex(type_name, index);
    if (!def.IsNull()) {
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
class CGameDefinitionManager:public CDefinitionManager {public:static CGameDefinitionManager& Get(){static CGameDefinitionManager m;return m;}};
class CSystemManager {public:bool ForceMusicPlay;float ForceMusicPlayOffset;
 bool GetForceMusicPlay(float& offset)const{offset=ForceMusicPlayOffset;return ForceMusicPlay;}
 void ResetForceMusicPlay(){ForceMusicPlay=false;ForceMusicPlayOffset=0.0f;Calls.push_back(1);}
};
static CSystemManager System;
CSystemManager* GFGetSystemManager(){++SystemReads;return &System;}
class C2DCoordI {public:long X,Y;C2DCoordI(long x,long y):X(x),Y(y){}};
class C3DVector {public:float X,Y,Z;C2DCoordI ToC2DCoordI()const{return C2DCoordI((long)X,(long)Y);}};
class CThingPlayerCreature {public:bool Dead;C3DVector Position;bool IsDead()const{return Dead;}const C3DVector& GetPos()const{++PositionReads;return Position;}};
class CPlayer {public:CThingPlayerCreature* Creature;CThingPlayerCreature* GetPControlledCreature()const{return Creature;}};
class CPlayerManager {public:CPlayer Player;CPlayer& GetMainPlayer(){return Player;}};
class CRegion {public:CDefPointer<const CRegionDef> Def;CDefPointer<const CRegionDef> GetPRegionDef()const{return Def;}};
class CWorldMap {public:long Block,MapNumber,RegionNumber;CRegion Region;
 long GetBlockValue(const C2DCoordI& point){assert(point.X==-12 && point.Y==9);return Block;}
 long GetMapNumberAt(const C3DVector& p)const{assert(p.X==-12.75f && p.Y==9.875f);return MapNumber;}
 long GetRegionNumberMapIsIn(long n)const{assert(n==MapNumber);return RegionNumber;}
 CRegion& GetRegion(long n){assert(n==RegionNumber);++RegionCalls;return Region;}
};
class CWorld {public:CWorldMap Map;CWorldMap& GetWorldMap(){return Map;}};
class CMainGameComponent {public:CPlayerManager PlayerManager;CWorld World;
 static CMainGameComponent* Get();CPlayerManager& GetPlayerManager(){return PlayerManager;}CWorld& GetWorld(){return World;}
};
static CMainGameComponent Game;CMainGameComponent* CMainGameComponent::Get(){return &Game;}
class CMusicManager {public:
 class CState {public:long RegionSet,OverrideSet,UIOverrideSet,CinematicOverrideSet,MapCellSet;bool Paused,Danger;};
 class CFadeTimes {public:unsigned int FadeOutMS,FadeInMS;};
 CState CurrentState,AppliedState;long PendingMusicID,PendingLoopCount;CFadeTimes* PPendingFadeTimes;bool Paused,AppliedForce;int Updates,OffsetUpdates,Plays;
 void Update();void PlayMusic(long id,const CFadeTimes& fade,long loops){assert(id==83 && &fade==PPendingFadeTimes && loops==-1);++Plays;Calls.push_back(2);}
 void UpdateCurrentMusicOffset(){assert(Updates==0);++OffsetUpdates;Calls.push_back(3);}
 void InternalUpdate(const CState& state,bool force){assert(OffsetUpdates==1);AppliedState=state;AppliedForce=force;++Updates;Calls.push_back(4);}
};
void CMusicManager::Update()
{
    bool forcePlay = false;
    float offset;
    if (GFGetSystemManager()->GetForceMusicPlay(offset)) {
        forcePlay = true;
        GFGetSystemManager()->ResetForceMusicPlay();
    }
    if (PendingMusicID)
        PlayMusic(PendingMusicID, *PPendingFadeTimes, PendingLoopCount);
    CThingPlayerCreature* creature = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (creature && !creature->IsDead()) {
        CWorldMap& map = CMainGameComponent::Get()->GetWorld().GetWorldMap();
        long themeIndex = map.GetBlockValue(creature->GetPos().ToC2DCoordI());
        CDefPointer<const CSoundThemeDef> theme;
        long cellMusic = 0;
        if (themeIndex && CGameDefinitionManager::Get().GetDef("SOUND_THEME", themeIndex, &theme)) {
            if (theme->MusicSet)
                cellMusic = theme->MusicSet;
        }
        long mapNumber = map.GetMapNumberAt(creature->GetPos());
        long regionMusic = 0;
        if (mapNumber) {
            long regionNumber = map.GetRegionNumberMapIsIn(mapNumber);
            if (regionNumber) {
                CDefPointer<const CRegionDef> region = map.GetRegion(regionNumber).GetPRegionDef();
                if (!region.IsNull())
                    regionMusic = region->Music;
            }
        }
        CState newState = CurrentState;
        newState.RegionSet = regionMusic;
        newState.MapCellSet = cellMusic;
        newState.Paused = Paused;
        UpdateCurrentMusicOffset();
        InternalUpdate(newState, forcePlay);
    }
}

int main()
{
 CThingPlayerCreature creature;creature.Position.X=-12.75f;creature.Position.Y=9.875f;creature.Position.Z=123.0f;
 CMusicManager::CFadeTimes fade;fade.FadeOutMS=7;fade.FadeInMS=9;
 for(int flags=0;flags<256;++flags)for(int hero=0;hero<3;++hero)for(int cell=0;cell<3;++cell){
  const bool force=(flags&1)!=0,pending=(flags&2)!=0,block=(flags&4)!=0,success=(flags&8)!=0,mapNumber=(flags&16)!=0,regionNumber=(flags&32)!=0,hasDef=(flags&64)!=0,paused=(flags&128)!=0;
  System.ForceMusicPlay=force;System.ForceMusicPlayOffset=12.25f;Game.PlayerManager.Player.Creature=hero?&creature:0;creature.Dead=hero==2;
  CWorldMap& map=Game.World.Map;map.Block=block?3:0;map.MapNumber=mapNumber?5:0;map.RegionNumber=regionNumber?8:0;
  map.Region.Def.Release();map.Region.Def.Object=hasDef?&RegionDef:0;if(hasDef)++RegionDef.RefCount;
  Theme.MusicSet=cell==0?0:cell==1?17:-23;RegionDef.Music=-31;LookupSuccess=success;
  CMusicManager music;music.PendingMusicID=pending?83:0;music.PendingLoopCount=-1;music.PPendingFadeTimes=&fade;music.Paused=paused;
  music.CurrentState.RegionSet=101;music.CurrentState.OverrideSet=102;music.CurrentState.UIOverrideSet=103;music.CurrentState.CinematicOverrideSet=104;music.CurrentState.MapCellSet=105;music.CurrentState.Paused=!paused;music.CurrentState.Danger=true;
  music.Updates=music.OffsetUpdates=music.Plays=0;Lookups=SystemReads=PositionReads=RegionCalls=0;Calls.clear();
  music.Update();assert(SystemReads==(force?2:1));assert(!System.ForceMusicPlay);assert(System.ForceMusicPlayOffset==(force?0.0f:12.25f));assert(music.Plays==(pending?1:0));
  const bool active=hero==1;assert(music.Updates==(active?1:0) && music.OffsetUpdates==(active?1:0));assert(PositionReads==(active?2:0));assert(Lookups==(active&&block?1:0));assert(RegionCalls==(active&&mapNumber&&regionNumber?1:0));
  if(active){assert(music.AppliedForce==force);assert(music.AppliedState.RegionSet==(mapNumber&&regionNumber&&hasDef?-31:0));assert(music.AppliedState.MapCellSet==(block&&success?Theme.MusicSet:0));assert(music.AppliedState.Paused==paused);assert(music.AppliedState.OverrideSet==102 && music.AppliedState.UIOverrideSet==103 && music.AppliedState.CinematicOverrideSet==104 && music.AppliedState.Danger);}
  unsigned int i=0;if(force)assert(Calls[i++]==1);if(pending)assert(Calls[i++]==2);if(active){assert(Calls[i++]==3);assert(Calls[i++]==4);}assert(i==Calls.size());
  assert(Theme.RefCount==1 && RegionDef.RefCount==(hasDef?2:1));assert(music.CurrentState.RegionSet==101 && music.CurrentState.MapCellSet==105);
 }
 Game.World.Map.Region.Def.Release();assert(RegionDef.RefCount==1);
 CDefPointer<const CSoundThemeDef> out;Lookups=0;assert(!CGameDefinitionManager::Get().GetDef("SOUND_THEME",0,&out));assert(Lookups==0 && out.IsNull());
 puts("006bdc60_TEST PASS");return 0;
}