// Full retail body and GetDef helper, with portable counted-definition and sound-manager fixtures.
#include <map>
#include <string>
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <string.h>
typedef std::string CCharString;
class CDefString {public:CCharString Text;CCharString GetString()const{return Text;}};
class CDefClassBase {public:int RefCount;CDefString Name;CDefClassBase():RefCount(1){} virtual ~CDefClassBase(){} virtual void Delete(){assert(false);} CDefString GetInstantiationName()const{return Name;}};
template<class T>class CDefPointer {public:
 CDefClassBase* Object;
 CDefPointer():Object(0){}
 CDefPointer(CDefClassBase* p):Object(p){if(Object)++Object->RefCount;}
 CDefPointer(const CDefPointer& p):Object(p.Object){if(Object)++Object->RefCount;}
 ~CDefPointer(){Release();}
 void Release(){if(Object){CDefClassBase* p=Object;if(--p->RefCount==0)p->Delete();Object=0;}}
 bool IsNull()const{return Object==0;}
 T* operator->()const{return static_cast<T*>(Object);}
 template<class U>void SetDynamicCast(const CDefPointer<U>& p){if(Object!=p.Object){Release();Object=p.Object;if(Object)++Object->RefCount;}}
};
class CSoundThemeDef:public CDefClassBase {public:
 long AtmosIndex,MusicSet;float Gain,Range,Granularity;std::multimap<CCharString,long> AtmosGroupMap;
 bool PlayOnlyWhenInside,PlayQuietlyWhenInside,PlayOnlyWhenOutside,PlayQuietlyWhenOutside;
};
static CSoundThemeDef Themes[4];static int Limit,Queries,Lookups,Deletes,Phase;
class CDefinitionManager {public:
 long GetNoInstantiatedDefsOfType(const CCharString& type)const{assert(type=="SOUND_THEME");++Queries;return Limit;}
 CDefPointer<const CDefClassBase> GetPDefFromClassIndex(const CCharString& type,long index)const{assert(type=="SOUND_THEME" && index>0 && index<Limit);++Lookups;return CDefPointer<const CDefClassBase>(&Themes[index]);}
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
class CGameDefinitionManager:public CDefinitionManager {public:static CGameDefinitionManager& Get(){static CGameDefinitionManager manager;return manager;}};
struct SoundEvent {int Kind;CCharString Theme,Channel;long Flags,Index,Sample;float A,B,C;};
static std::vector<SoundEvent> Events;
class CASoundManager {public:
 void DeleteAllAtmoses(){++Deletes;Events.clear();}
 bool NewAtmos(const CCharString& name,long flags){SoundEvent e; e.Kind=0;e.Theme=name;e.Flags=flags;Events.push_back(e);return false;}
 bool NewChannel(const CCharString& name,const CCharString& group,long sample,long index,float gain){SoundEvent e;e.Kind=1;e.Theme=name;e.Channel=group;e.Sample=sample;e.Index=index;e.A=gain;Events.push_back(e);return false;}
 bool AddAtmos2Map(const CCharString& name,long index,float granularity,float gain,float range){SoundEvent e;e.Kind=2;e.Theme=name;e.Index=index;e.A=granularity;e.B=gain;e.C=range;Events.push_back(e);return false;}
};
static CASoundManager* PSoundManager;
class CAtmosProcess {public:void RegisterAtmosPalette();};
void CAtmosProcess::RegisterAtmosPalette()
{
    if (PSoundManager) {
        PSoundManager->DeleteAllAtmoses();
        for (long i = 1; i < CGameDefinitionManager::Get().GetNoInstantiatedDefsOfType("SOUND_THEME"); ++i) {
            CDefPointer<const CSoundThemeDef> theme;
            CGameDefinitionManager::Get().GetDef("SOUND_THEME", i, &theme);
            long flags = 0;
            if (theme->PlayQuietlyWhenInside)
                flags = 0x2000;
            else if (theme->PlayQuietlyWhenOutside)
                flags = 0x4000;
            if (theme->PlayOnlyWhenInside)
                flags |= 0x800;
            else if (theme->PlayOnlyWhenOutside)
                flags |= 0x1000;
            PSoundManager->NewAtmos(theme->GetInstantiationName().GetString(), flags);
            for (std::multimap<CCharString,long>::const_iterator group = theme->AtmosGroupMap.begin(); group != theme->AtmosGroupMap.end(); ++group) {
                if (theme->AtmosIndex)
                    PSoundManager->NewChannel(theme->GetInstantiationName().GetString(), group->first, theme->AtmosIndex, group->second, theme->Gain);
            }
            PSoundManager->AddAtmos2Map(theme->GetInstantiationName().GetString(), i, theme->Granularity, theme->Gain, theme->Range);
        }
    }
}

int main()
{
    CAtmosProcess atmos;CASoundManager sound;
    PSoundManager=0;Queries=Lookups=Deletes=0;atmos.RegisterAtmosPalette();assert(Queries==0 && Lookups==0 && Deletes==0);
    PSoundManager=&sound;
    for(int mask=0;mask<16;++mask)for(int count=1;count<=4;++count)for(int channels=0;channels<4;++channels)for(int enabled=0;enabled<2;++enabled){
        Limit=count;Queries=Lookups=Deletes=0;
        for(int i=1;i<4;++i){
            CSoundThemeDef& t=Themes[i];char name[16];sprintf(name,"theme%d",i);t.Name.Text=name;
            t.AtmosIndex=enabled?7+i:0;t.Gain=0.25f*i;t.Range=50.0f+i;t.Granularity=2.0f*i;
            t.PlayOnlyWhenInside=(mask&1)!=0;t.PlayQuietlyWhenInside=(mask&2)!=0;t.PlayOnlyWhenOutside=(mask&4)!=0;t.PlayQuietlyWhenOutside=(mask&8)!=0;
            t.AtmosGroupMap.clear();
            if(channels>0)t.AtmosGroupMap.insert(std::make_pair(CCharString("day"),12L));
            if(channels>1)t.AtmosGroupMap.insert(std::make_pair(CCharString("night"),34L));
            if(channels>2)t.AtmosGroupMap.insert(std::make_pair(CCharString("night"),56L));
        }
        atmos.RegisterAtmosPalette();assert(Deletes==1 && Lookups==count-1 && Queries==count);
        unsigned int pos=0;
        for(int j=1;j<count;++j){
            CSoundThemeDef& t=Themes[j];long flags=(mask&2)?0x2000:(mask&8)?0x4000:0;flags|=(mask&1)?0x800:(mask&4)?0x1000:0;
            assert(pos<Events.size());const SoundEvent& start=Events[pos++];assert(start.Kind==0 && start.Theme==t.Name.Text && start.Flags==flags);
            for(std::multimap<CCharString,long>::const_iterator it=t.AtmosGroupMap.begin();enabled && it!=t.AtmosGroupMap.end();++it){
                assert(pos<Events.size());const SoundEvent& e=Events[pos++];assert(e.Kind==1 && e.Theme==t.Name.Text && e.Channel==it->first && e.Sample==t.AtmosIndex && e.Index==it->second && e.A==t.Gain);
            }
            assert(pos<Events.size());const SoundEvent& last=Events[pos++];assert(last.Kind==2 && last.Theme==t.Name.Text && last.Index==j && last.A==t.Granularity && last.B==t.Gain && last.C==t.Range);
            assert(t.RefCount==1);
        }
        assert(pos==Events.size());
    }
    CDefPointer<const CSoundThemeDef> empty;Lookups=0;assert(!CGameDefinitionManager::Get().GetDef("SOUND_THEME",0,&empty));assert(Lookups==0 && empty.IsNull());
    puts("006b16b0_TEST PASS");return 0;
}