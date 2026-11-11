#include <map>
#include <utility>
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#define __int64 long long
#endif
static void check(bool b){if(!b){std::puts("FAIL");std::exit(1);}}
class CThingAICreature{ public:bool dead;unsigned __int64 uid;mutable int refs,uidReads;CThingAICreature():dead(false),uid(0),refs(0),uidReads(0){}bool IsDead()const{return dead;}unsigned __int64 GetUniqueID()const{++uidReads;return uid;}};
template<class T>class CIntelligentPointer{ public:T*p;CIntelligentPointer(T*q=0):p(q){if(p)++p->refs;}CIntelligentPointer(const CIntelligentPointer&o):p(o.p){if(p)++p->refs;}~CIntelligentPointer(){if(p)--p->refs;}T*Get()const{return p;}};
class CGroupMemberData{ public:CGroupMemberData(const CGroupMemberData&o):PMember(o.PMember),IsActive(o.IsActive),JoinedFrame(o.JoinedFrame),DeactivatedFrame(o.DeactivatedFrame){}CGroupMemberData(const CThingAICreature*m,bool a,unsigned long j,unsigned long d):PMember(m),IsActive(a),JoinedFrame(j),DeactivatedFrame(d){}CIntelligentPointer<const CThingAICreature>PMember;bool IsActive;unsigned long JoinedFrame,DeactivatedFrame;~CGroupMemberData(){}};
class CWorld{ public:static long frame;static int frameReads;static long GetFrame(){++frameReads;return frame;}};long CWorld::frame;int CWorld::frameReads;
class CTCCreatureGroup{ public:unsigned int MemberCount,ActiveMemberCount;bool ShutdownOnEmpty;std::map<unsigned __int64,CGroupMemberData>Members;CWorld*world;mutable int worldReads;const CWorld&PeekWorld()const{++worldReads;return *world;}bool AddNewMember(const CThingAICreature*);};
template<class T>bool IS_THING_DEAD(const T&p){return p==0||p->IsDead();}
bool CTCCreatureGroup::AddNewMember(const CThingAICreature* member)
{
 if(IS_THING_DEAD(member))return false;
 std::map<unsigned __int64,CGroupMemberData>::iterator it_member=Members.find(member->GetUniqueID());
 if(it_member==Members.end()) {
  Members.insert(std::make_pair(member->GetUniqueID(),CGroupMemberData(member,false,PeekWorld().GetFrame(),0)));
  ++MemberCount;
 }
 return true;
}

int main(){unsigned long cases=0;for(int state=0;state<3;++state)for(int initial=0;initial<4;++initial)for(int uidCase=0;uidCase<6;++uidCase)for(int collision=0;collision<4;++collision)for(int frameCase=0;frameCase<3;++frameCase)for(int countCase=0;countCase<3;++countCase)for(int shutdown=0;shutdown<2;++shutdown){
 CThingAICreature objects[4],member;for(int i=0;i<4;++i)objects[i].uid=(unsigned __int64)(i+1);const unsigned __int64 uids[]={0,1,4,0xffffffffUL,((unsigned __int64)1<<32),~(unsigned __int64)0};member.uid=collision<initial?objects[collision].uid:uids[uidCase];member.dead=state==1;CWorld world;const long frames[]={0,16,0x7fffffffL};CWorld::frame=frames[frameCase];CWorld::frameReads=0;unsigned int start=countCase==0?0U:countCase==1?7U:0xffffffffU;
 {CTCCreatureGroup g;g.MemberCount=start;g.ActiveMemberCount=19;g.ShutdownOnEmpty=shutdown!=0;g.world=&world;g.worldReads=0;for(int i=0;i<initial;++i)g.Members.insert(std::make_pair(objects[i].uid,CGroupMemberData(&objects[i],true,8,16)));unsigned before=unsigned(g.Members.size());bool present=g.Members.find(member.uid)!=g.Members.end();const CThingAICreature*arg=state==0?0:&member;bool result=g.AddNewMember(arg);bool added=state==2&&!present;
 check(result==(state==2));check(g.MemberCount==start+unsigned(added)&&g.ActiveMemberCount==19&&g.ShutdownOnEmpty==(shutdown!=0));check(g.Members.size()==before+unsigned(added));check(g.worldReads==int(added)&&CWorld::frameReads==int(added));check(member.uidReads==(state==2?(added?2:1):0));check(member.refs==int(added));
 for(int i=0;i<initial;++i){std::map<unsigned __int64,CGroupMemberData>::const_iterator it=g.Members.find(objects[i].uid);check(it!=g.Members.end()&&it->second.PMember.Get()==&objects[i]&&it->second.IsActive&&it->second.JoinedFrame==8&&it->second.DeactivatedFrame==16&&objects[i].refs==1);}
 if(added){std::map<unsigned __int64,CGroupMemberData>::const_iterator it=g.Members.find(member.uid);check(it!=g.Members.end()&&it->second.PMember.Get()==&member&&!it->second.IsActive&&it->second.JoinedFrame==static_cast<unsigned long>(CWorld::frame)&&it->second.DeactivatedFrame==0);unsigned int count=g.MemberCount;check(g.AddNewMember(&member)&&g.MemberCount==count&&member.refs==1&&g.Members.size()==before+1);}
 }
 for(int i=0;i<4;++i)check(objects[i].refs==0);check(member.refs==0);++cases;
 }check(cases==5184);std::puts("00813010_TEST PASS");return 0;}