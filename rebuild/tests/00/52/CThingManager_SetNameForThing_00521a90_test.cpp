// Selected body copied verbatim; actual standard maps and observable string assignments.
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#ifndef _MSC_VER
#define __int64 long long
#endif
#define CHECK(c) do{if(!(c)){printf("FAIL %d\n",__LINE__);abort();}}while(0)
static int narrowAssignments,wideAssignments;
class CCharString {public:std::string Text;CCharString(){}CCharString(const char* t):Text(t){}const CCharString& operator=(const char* t){++narrowAssignments;Text=t;return *this;}};
class CWideString {public:std::basic_string<unsigned short> Text;CWideString(){}CWideString(const unsigned short* t):Text(t){}const CWideString& operator=(const CWideString& t){++wideAssignments;Text=t.Text;return *this;}};
class CThingManager {public:std::map<unsigned __int64,std::pair<CCharString,CWideString> > NPCNameMap;void SetNameForThing(unsigned __int64,const CWideString&);};
void CThingManager::SetNameForThing(unsigned __int64 uid,const CWideString& new_name)
{
 std::map<unsigned __int64,std::pair<CCharString,CWideString> >::const_iterator name_iter=NPCNameMap.find(uid);
 if(name_iter!=NPCNameMap.end()){
  NPCNameMap[uid].second=new_name;
 }else{
  NPCNameMap[uid].first="";
  NPCNameMap[uid].second=new_name;
 }
}

int main(){
 unsigned __int64 keys[]={0,1,((unsigned __int64)1<<32),((unsigned __int64)1<<63),~(unsigned __int64)0};
 unsigned short names[][5]={{0},{65,0},{0x1234,66,0},{0xd83d,0xde00,0},{90,89,88,0}};
 int cases=0;
 for(int k=0;k<5;++k)for(int n=0;n<5;++n)for(int preexisting=0;preexisting<2;++preexisting){
  CThingManager m;unsigned __int64 neighbor=keys[k]^0x12345;
  CWideString old(names[(n+1)%5]),input(names[n]);
  m.NPCNameMap[neighbor]=std::make_pair(CCharString("neighbor_script"),old);
  if(preexisting)m.NPCNameMap[keys[k]]=std::make_pair(CCharString("existing_script"),old);
  narrowAssignments=wideAssignments=0;
  m.SetNameForThing(keys[k],input);++cases;
  CHECK(narrowAssignments==!preexisting && wideAssignments==1);
  CHECK(m.NPCNameMap.size()==2);
  CHECK(m.NPCNameMap[keys[k]].first.Text==(preexisting?"existing_script":""));
  CHECK(m.NPCNameMap[keys[k]].second.Text==input.Text);
  CHECK(m.NPCNameMap[neighbor].first.Text=="neighbor_script");
  CHECK(m.NPCNameMap[neighbor].second.Text==old.Text);
  narrowAssignments=wideAssignments=0;
  m.SetNameForThing(keys[k],old);++cases;
  CHECK(narrowAssignments==0 && wideAssignments==1 && m.NPCNameMap.size()==2);
  CHECK(m.NPCNameMap[keys[k]].second.Text==old.Text);
  narrowAssignments=wideAssignments=0;
  m.SetNameForThing(keys[k],m.NPCNameMap[keys[k]].second);++cases;
  CHECK(narrowAssignments==0 && wideAssignments==1 && m.NPCNameMap[keys[k]].second.Text==old.Text);
 }
 CHECK(cases==150);puts("00521a90_TEST PASS");return 0;
}