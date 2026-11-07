// Self-contained C++03 fixture; selected complete target body pasted unchanged.
#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <cctype>
#ifndef _MSC_VER
#define __fastcall
#endif
static std::vector<std::string> Events;
class CCharString{public:std::string Text;CCharString():Text(){}CCharString(const char*p):Text(p){}CCharString(const std::string&p):Text(p){}bool operator<(const CCharString&v)const{return Text<v.Text;}};
class CDefinitionManager{};class CSymbolMap{};
class CStringParser{public:std::string Text;std::size_t Cursor;
 CStringParser(const CCharString&s,void (__fastcall*callback)(const CCharString&),const CCharString&filename,long offset):Text(s.Text),Cursor(0){assert(!callback&&filename.Text.empty()&&offset==0);Events.push_back("parser");}
 ~CStringParser(){Events.push_back("destroy_parser");}
 long SkipWhitespace(){long count=0;while(Cursor<Text.size()&&std::isspace(static_cast<unsigned char>(Text[Cursor]))){++Cursor;++count;}return count;}
 bool SkipPastString(const CCharString&s){std::size_t found=Text.find(s.Text,Cursor);if(found==std::string::npos)return false;Cursor=found+s.Text.size();return true;}
 bool SkipPastNextItemIfIs(const CCharString&s){SkipWhitespace();if(Text.compare(Cursor,s.Text.size(),s.Text)!=0)return false;Cursor+=s.Text.size();return true;}
 CCharString ReadAsStringUntilString(const CCharString&s){std::size_t found=Text.find(s.Text,Cursor);if(found==std::string::npos)found=Text.size();CCharString result(Text.substr(Cursor,found-Cursor));Cursor=found;return result;}
 CCharString ReadAsStringUntilPastString(const CCharString&s){CCharString result=ReadAsStringUntilString(s);if(Cursor<Text.size())Cursor+=s.Text.size();return result;}
};
class CPersistContext{public:CStringParser&Parser;CPersistContext(CStringParser&p,void (__fastcall*callback)(const CCharString&),bool report,const CDefinitionManager*d,const CSymbolMap*m,CCharString*text):Parser(p){assert(!callback&&report&&!d&&!m&&!text);}};
class CASuspendableProcess{public:unsigned Calls;CASuspendableProcess():Calls(0){}void SuspendSuspendableProcess(){++Calls;Events.push_back("suspend");}};
namespace NProgressDisplay{void __fastcall DisplayProgress(const CCharString&s,float progress,bool a,bool b){assert(s.Text=="Loading entities from script"&&progress==-1.0f&&!a&&!b);Events.push_back("progress");}}
namespace NGlobalConsole{long NoThingsToLoadPerSuspend;}
class CLevelLoader{public:static CCharString __fastcall GetLoadAllSectionName(){return CCharString("XXX_ALL");}};
class CThing{public:std::string ID;};
struct LoadCall{std::string ID,Section;long Level,Version,Inclusions;};
class CThingManager{public:long SerialiseVersionNumber,LevelBeingLoaded;CCharString CurrentSectionName;CThing Things[16];unsigned Used;long ExpectedInclusions,ActivatedLevel;std::vector<LoadCall>Calls;
 CThingManager():SerialiseVersionNumber(-1),LevelBeingLoaded(-1),CurrentSectionName("before"),Used(0),ExpectedInclusions(0),ActivatedLevel(-1){}
 CThing*LoadSingleThing(CPersistContext&context,long inclusions){assert(inclusions==ExpectedInclusions);context.Parser.SkipWhitespace();CCharString id=context.Parser.ReadAsStringUntilPastString(";");LoadCall c;c.ID=id.Text;c.Section=CurrentSectionName.Text;c.Level=LevelBeingLoaded;c.Version=SerialiseVersionNumber;c.Inclusions=inclusions;Calls.push_back(c);Events.push_back("load:"+id.Text);if(id.Text=="drop")return 0;assert(Used<16);Things[Used].ID=id.Text;return &Things[Used++];}
 void ActivateAfterLoading(long level){assert(level==LevelBeingLoaded);ActivatedLevel=level;Events.push_back("activate");}
 void LoadEntitiesFromScript(const CCharString&,long,long,const std::set<CCharString>&,CASuspendableProcess*,long,std::vector<CThing*>&);
};

void CThingManager::LoadEntitiesFromScript(const CCharString& script,long version,long level,const std::set<CCharString>& sections,CASuspendableProcess* psuspend,long inclusions,std::vector<CThing*>& loaded_things)
{
    unsigned long how_many_is_some=NGlobalConsole::NoThingsToLoadPerSuspend;
    SerialiseVersionNumber=version;
    LevelBeingLoaded=level;
    bool loading_with_suspends=psuspend!=0;
    CStringParser parser(script,0,CCharString(""),0);
    long count=0;
    CurrentSectionName="";
    if(version>=2) {
        while(parser.SkipPastString("XXXSectionStart")) {
            parser.SkipWhitespace();
            CCharString name=parser.ReadAsStringUntilString(";");
            parser.SkipPastString(";");
            if(sections.find(name)!=sections.end() || sections.find(CLevelLoader::GetLoadAllSectionName())!=sections.end()) {
                CurrentSectionName=name;
                if(parser.SkipPastNextItemIfIs("XXXSectionEnd;")) {
                    parser.ReadAsStringUntilPastString(";");
                    CurrentSectionName="";
                } else {
                    while(parser.SkipPastNextItemIfIs("NewThing")) {
                        NProgressDisplay::DisplayProgress("Loading entities from script",-1.0f,false,false);
                        CPersistContext thing_context(parser,0,true,0,0,0);
                        CThing* pthing=LoadSingleThing(thing_context,inclusions);
                        if(pthing) loaded_things.push_back(pthing);
                        if(++count>=how_many_is_some) {
                            count=0;
                            if(loading_with_suspends)psuspend->SuspendSuspendableProcess();
                        }
                    }
                    parser.SkipPastString("XXXSectionEnd;");
                }
            } else parser.SkipPastString("XXXSectionEnd;");
        }
    } else {
        CurrentSectionName="NULL";
        while(parser.SkipPastString("NewThing")) {
            NProgressDisplay::DisplayProgress("Loading entities from script",-1.0f,false,false);
            CPersistContext thing_context(parser,0,true,0,0,0);
            CThing* pthing=LoadSingleThing(thing_context,inclusions);
            if(pthing) loaded_things.push_back(pthing);
            ++count;
            if(loading_with_suspends && count>=how_many_is_some) {
                count=0;
                psuspend->SuspendSuspendableProcess();
            }
        }
    }
    if(psuspend)psuspend->SuspendSuspendableProcess();
    ActivateAfterLoading(level);
}


struct Entry{std::string ID,Section;Entry(const char*i,const char*s):ID(i),Section(s){}};
int main(){unsigned cases=0;const long intervals[]={0,1,2,3,100,-1};
 for(long version=0;version<4;++version)for(unsigned process=0;process<2;++process)for(unsigned ii=0;ii<6;++ii)for(unsigned filter=0;filter<6;++filter)for(unsigned scenario=0;scenario<3;++scenario){
 Events.clear();NGlobalConsole::NoThingsToLoadPerSuspend=intervals[ii];CASuspendableProcess p;CThingManager manager;manager.ExpectedInclusions=long(scenario)+1;std::set<CCharString>sections;if(filter==1)sections.insert(CCharString("A"));if(filter==2)sections.insert(CCharString("B"));if(filter==3)sections.insert(CCharString("XXX_ALL"));if(filter==4)sections.insert(CCharString("C"));if(filter==5){sections.insert(CCharString("A"));sections.insert(CCharString("C"));}
 std::string script;std::vector<Entry>entries;if(scenario==0)script="no entity declarations";else if(scenario==1){script="prefix XXXSectionStart A; NewThing a1; NewThing drop; NewThing a2; XXXSectionEnd; A; XXXSectionStart B; NewThing b1; NewThing b2; XXXSectionEnd; B; XXXSectionStart A; XXXSectionEnd; A; XXXSectionStart C; NewThing c1; NewThing c2; XXXSectionEnd; C;";entries.push_back(Entry("a1","A"));entries.push_back(Entry("drop","A"));entries.push_back(Entry("a2","A"));entries.push_back(Entry("b1","B"));entries.push_back(Entry("b2","B"));entries.push_back(Entry("c1","C"));entries.push_back(Entry("c2","C"));}else {script="XXXSectionStart A; XXXSectionEnd; A; XXXSectionStart C; NewThing c3; XXXSectionEnd; C;";entries.push_back(Entry("c3","C"));}
 std::vector<Entry>wanted;for(unsigned i=0;i<entries.size();++i)if(version<2||sections.count(CCharString(entries[i].Section))||sections.count(CCharString("XXX_ALL")))wanted.push_back(entries[i]);
 CThing existing;existing.ID="existing";std::vector<CThing*>loaded;loaded.push_back(&existing);manager.LoadEntitiesFromScript(CCharString(script),version,17,sections,process?&p:0,manager.ExpectedInclusions,loaded);
 assert(manager.SerialiseVersionNumber==version&&manager.LevelBeingLoaded==17&&manager.ActivatedLevel==17&&manager.Calls.size()==wanted.size()&&loaded[0]==&existing);
 unsigned loadedCount=1;for(unsigned i=0;i<wanted.size();++i){const LoadCall&c=manager.Calls[i];assert(c.ID==wanted[i].ID&&c.Section==(version<2?"NULL":wanted[i].Section)&&c.Version==version&&c.Level==17&&c.Inclusions==manager.ExpectedInclusions);if(wanted[i].ID!="drop"){assert(loadedCount<loaded.size()&&loaded[loadedCount]->ID==wanted[i].ID);++loadedCount;}}
 assert(loaded.size()==loadedCount);std::vector<std::string>expected;expected.push_back("parser");unsigned count=0,suspends=0;for(unsigned i=0;i<wanted.size();++i){expected.push_back("progress");expected.push_back("load:"+wanted[i].ID);++count;bool threshold=intervals[ii]>=0&&(intervals[ii]==0||count>=unsigned(intervals[ii]));if(version>=2){if(threshold){count=0;if(process){expected.push_back("suspend");++suspends;}}}else if(process&&threshold){count=0;expected.push_back("suspend");++suspends;}}
 if(process){expected.push_back("suspend");++suspends;}expected.push_back("activate");expected.push_back("destroy_parser");assert(Events==expected&&p.Calls==suspends);
 if(version<2)assert(manager.CurrentSectionName.Text=="NULL");else if(scenario==0||filter==0||filter==1)assert(manager.CurrentSectionName.Text.empty());else if(scenario==1&&filter==2)assert(manager.CurrentSectionName.Text=="B");else if(filter==3||filter==4||filter==5)assert(manager.CurrentSectionName.Text=="C");
 ++cases;}
 assert(cases==864);std::puts("00520d00_TEST PASS");return 0;}