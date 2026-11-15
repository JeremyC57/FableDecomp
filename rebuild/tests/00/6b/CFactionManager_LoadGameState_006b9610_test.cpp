// Full target and original faction accessor/copy bodies, with observable persistence inputs.
#include <assert.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
unsigned long g_CRCTable[256];
class CCRC {public:static unsigned long Calc(unsigned long seed,const void* data,unsigned long byteCount){
 if(byteCount==0)return seed;
 const unsigned char* bytes=static_cast<const unsigned char*>(data);
 for(unsigned long index=0;index<byteCount;++index)seed=g_CRCTable[bytes[index]^(seed&0xff)]^(seed>>8);
 return seed;
}};
class CCharString {public:std::string Text;CCharString(){}CCharString(const char*s):Text(s){}operator const char*()const{return Text.c_str();}bool operator<(const CCharString& other)const{return Text<other.Text;}bool operator==(const CCharString& other)const{return Text==other.Text;}
 unsigned long GetCRC()const{return CCRC::Calc(0,Text.c_str(),(unsigned long)Text.size());}
};
CCharString GFIntToCharString(long value){char buffer[32];sprintf(buffer,"%ld",value);return CCharString(buffer);}
CCharString operator+(const char* prefix,const CCharString& value){std::string result(prefix);result+=value.Text;return CCharString(result.c_str());}
class CFaction {public:
 CCharString Name;typedef std::set<CCharString> TStringSet;TStringSet AlliedFactions,EnemyFactions;
 CFaction(){}
 CCharString GetName()const{return Name;}
 void InitialiseFromFaction(const CFaction& faction){Name=faction.Name;EnemyFactions=faction.EnemyFactions;AlliedFactions=faction.AlliedFactions;}
};
template<class T>class CCountedPointer {public:T* Data;void* Info;CCountedPointer(T*p=0):Data(p),Info(0){}T* operator->()const{return Data;}};
template<class K,class V>class CKeyPairCompareLess {public:bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b)const{return a.first<b.first;}};
template<class K,class V,class Compare>class CVectorMap {public:typedef std::pair<K,V> value_type;typedef value_type* iterator;std::vector<value_type> Data;
 iterator begin(){return Data.empty()?0:&Data[0];}iterator end(){return Data.empty()?0:&Data[0]+Data.size();}
 iterator Find(const K& key){iterator first=begin();long count=(long)Data.size();while(count>0){long half=count/2;iterator middle=first+half;if(middle->first<key){first=middle+1;count-=half+1;}else count=half;}return first==end()||key<first->first?end():first;}
};
class CPersistContext {public:long Count;std::vector<CFaction> Records;int CountCalls,RecordCalls;
 CPersistContext():Count(0),CountCalls(0),RecordCalls(0){}
 void Transfer(const char* name,long& count,const long& fallback){assert(std::string(name)=="NumberOfFactions" && fallback==0);++CountCalls;count=Count;}
 void Transfer(const char* name,CFaction& faction){char label[32];sprintf(label,"Faction%d",RecordCalls);assert(std::string(name)==label);assert(faction.Name.Text.empty()&&faction.AlliedFactions.empty()&&faction.EnemyFactions.empty());assert(RecordCalls<(int)Records.size());faction=Records[RecordCalls++];}
};
class CFactionManager {public:void LoadGameState(CPersistContext& context);typedef CVectorMap<unsigned long,CCountedPointer<CFaction>,CKeyPairCompareLess<unsigned long,CCountedPointer<CFaction> > > TFactionMap;TFactionMap Factions;};
void CFactionManager::LoadGameState(CPersistContext& context)
{
    long count;
    context.Transfer("NumberOfFactions",count,0L);
    for(long index=0;index<count;++index) {
        CFaction faction;
        context.Transfer("Faction"+GFIntToCharString(index),faction);
        TFactionMap::iterator found=Factions.Find(faction.GetName().GetCRC());
        if(found!=Factions.end())
            found->second->InitialiseFromFaction(faction);
    }
}

static CFaction makeFaction(const char* name,const char* ally,const char* enemy){CFaction f;f.Name=CCharString(name);if(*ally)f.AlliedFactions.insert(CCharString(ally));if(*enemy)f.EnemyFactions.insert(CCharString(enemy));return f;}
static void same(const CFaction& a,const CFaction& b){assert(a.Name==b.Name&&a.AlliedFactions==b.AlliedFactions&&a.EnemyFactions==b.EnemyFactions);}
static void run(long count,bool emptyMap,bool includeUnknown,bool duplicate){
 CFaction actual[3],expected[3];const char* names[]={"Guild","Bandits",""};CFactionManager manager;CPersistContext context;context.Count=count;
 for(int i=0;i<3;++i){actual[i]=makeFaction(names[i],"old-ally","old-enemy");expected[i]=actual[i];if(!emptyMap)manager.Factions.Data.push_back(CFactionManager::TFactionMap::value_type(actual[i].GetName().GetCRC(),CCountedPointer<CFaction>(&actual[i])));}
 std::sort(manager.Factions.Data.begin(),manager.Factions.Data.end(),CKeyPairCompareLess<unsigned long,CCountedPointer<CFaction> >());
 for(long j=0;j<count;++j){const char* name=includeUnknown&&j%2?"NotPresent":names[duplicate?0:j%3];CFaction record=makeFaction(name,j%2?"new-ally":"",j%3?"":"new-enemy");context.Records.push_back(record);if(!emptyMap)for(int k=0;k<3;++k)if(expected[k].Name.Text==name)expected[k]=record;}
 const unsigned int size=(unsigned int)manager.Factions.Data.size();manager.LoadGameState(context);assert(context.CountCalls==1&&context.RecordCalls==(count>0?count:0));assert(manager.Factions.Data.size()==size);
 for(int i=0;i<3;++i)same(actual[i],expected[i]);
}
int main(){
 for(unsigned long n=0;n<256;++n){unsigned long c=n;for(int bit=0;bit<8;++bit)c=(c&1)?0xedb88320UL^(c>>1):c>>1;g_CRCTable[n]=c;}
 assert(CCRC::Calc(0,"123456789",9)==0x2dfd2d88UL);
 const long counts[]={-1,0,1,3,6};
 for(int c=0;c<5;++c)for(int empty=0;empty<2;++empty)for(int unknown=0;unknown<2;++unknown)for(int duplicate=0;duplicate<2;++duplicate)run(counts[c],empty!=0,unknown!=0,duplicate!=0);
 puts("006b9610_TEST PASS");return 0;
}