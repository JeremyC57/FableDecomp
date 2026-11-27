#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

static void require(bool ok) { if(!ok) exit(3); }
typedef std::wstring CWideString;
enum FileAccessType { FILE_ACCESS_READ=1 };
enum EParsedItemType { PARSED_ITEM_OTHER=1, PARSED_ITEM_STRING=2 };
struct Plan { bool known; int events, tag[3], index[3]; float time[3]; };
static Plan blocks[3];
static int blockCount, block, item, consumed, skips, meshes, opened, peeked, readTag;
static int clears, trimmed, lookedUp, headerEnds, paths, loads, saves, compareCount, added;
static bool filePresent, hasHeader, GUseCompiledAnimationEvents, GAllowDataGeneration;
static CWideString binaryName;
struct CAFile {
    static bool PathExists(const CWideString& p){require(p==L"D:/data/events.txt");++paths;return filePresent;}
    static void SplitPath(const CWideString& p,CWideString* drive,CWideString* dir,CWideString* stem,CWideString* ext){
        require(p==L"D:/data/events.txt" && ext==0);*drive=L"D:";*dir=L"/data/";*stem=L"events";
    }
};
struct CDiskFileWin32:CAFile {
    CDiskFileWin32(const CWideString& p,FileAccessType access,unsigned long flags){
        require(p==L"D:/data/events.txt" && access==FILE_ACCESS_READ && flags==2);++opened;
    }
};
struct CCharString : std::string {
    CCharString(const char* p=""):std::string(p){}
    CCharString(const CDiskFileWin32&,long n):std::string("script"){require(n==-1);}
};
namespace NBankFileManager { static CWideString GetHeaderDir(){return L"D:/headers/";} }
struct CSymbolMap {
    bool LoadFromFile(const CWideString& p){require(p==L"D:/headers/meshdata.h");++meshes;return true;}
    bool FindSymbol(const CCharString& n,long& index)const{
        require(n=="AnimA"||n=="Unknown");++lookedUp;if(n=="Unknown")return false; index=7;return true;
    }
    long GetSymbolSLONG(const CCharString& n)const{require(n=="One"||n=="Two");return n=="One"?1:2;}
};
struct CParsedItem {
    EParsedItemType type; CCharString text;
    EParsedItemType GetType()const{return type;}
    const CCharString& GetString()const{return text;}
};
struct CStringParser {
    bool readingAnim;
    CStringParser(const CCharString& n):readingAnim(false){require(n=="script");}
    bool SkipPastString(const char* n){
        if(!strcmp(n,"BEGIN_ANIMATION_EVENTS"))return hasHeader;
        if(!strcmp(n,"END_EVENTS")){require(block<blockCount&&!blocks[block].known);++skips;++block;return true;}
        require(!strcmp(n,"END_ANIMATION_EVENTS")&&block==blockCount);++headerEnds;return true;
    }
    bool SkipPastNextItemIfIs(const char* n){
        if(!strcmp(n,"BEGIN_EVENTS:")){if(block==blockCount)return false;item=0;readingAnim=true;return true;}
        require(!strcmp(n,"END_EVENTS") && block<blockCount);if(item<blocks[block].events)return false;++block;return true;
    }
    void SkipWhitespace(){}
    CCharString ReadAsString(){
        if(readingAnim){readingAnim=false;return blocks[block].known?"AnimA":"Unknown";}
        return blocks[block].index[item]==1?"One":"Two";
    }
    float ReadAsFloat(){return blocks[block].time[item++];}
    bool PeekNextItem(CParsedItem& p){
        ++peeked;int tag=blocks[block].tag[item-1];p.type=tag==1?PARSED_ITEM_OTHER:PARSED_ITEM_STRING;
        p.text=tag==2?"STOP":tag==3?"START":"IGNORE";return tag!=0;
    }
    bool ReadNextItem(CParsedItem& p){require(p.text=="STOP"||p.text=="START");++readTag;return true;}
};
namespace NAnimationEvents {
struct CEvent{
    unsigned long Index;float Time;bool ExternallyImported,StopEvent;
    CEvent(unsigned long id,float time,bool ext,bool stop):Index(id),Time(time),ExternallyImported(ext),StopEvent(stop){}
    bool operator==(const CEvent& e)const{return Index==e.Index&&Time==e.Time&&ExternallyImported==e.ExternallyImported&&StopEvent==e.StopEvent;}
};
template<class T,int N> struct CSmallVector {};
template<class K,class V> struct CVectorMap {
    typedef const int* iterator; std::vector<CEvent> items;
    void clear(){items.clear();++clears;}
    void DeallocateUnusedMemory(){++trimmed;}
    iterator Find(unsigned long id){require(id==7);++consumed;return 0;}
};
struct CManager {
    CVectorMap<unsigned long,CSmallVector<CEvent,32> > AnimToExtraEventsMap;
    CSymbolMap EventSymbolMap;bool DifferentToSavedCopy;
    void LoadBinary(const CWideString& p){++loads;binaryName=p;}
    void SaveBinary(const CWideString& p){++saves;binaryName=p;}
    bool EventExists(long anim,const CEvent& ev)const{
        require(anim==7&&!ev.ExternallyImported);++compareCount;
        const std::vector<CEvent>& es=AnimToExtraEventsMap.items;
        for(size_t i=0;i<es.size();++i)if(es[i]==ev)return true;
        return false;
    }
    void AddEventToAnim(unsigned long anim,const CEvent& e){require(anim==7);AnimToExtraEventsMap.items.push_back(e);++added;}
    bool Load(const CWideString&);
};
}
bool NAnimationEvents::CManager::Load(const CWideString& animation_events_save_path)
{
    if (GUseCompiledAnimationEvents) {
        CWideString bin_drive, bin_dir, bin_filename;
        CAFile::SplitPath(animation_events_save_path,&bin_drive,&bin_dir,&bin_filename,0);
        CWideString bin_pathname = bin_drive + bin_dir + bin_filename + L".bin";
        LoadBinary(bin_pathname);
        DifferentToSavedCopy=false;
        return true;
    }
    bool file_exists=CAFile::PathExists(animation_events_save_path);
    if(file_exists) {
        AnimToExtraEventsMap.clear();
        CDiskFileWin32 game_file(animation_events_save_path,FILE_ACCESS_READ,2);
        CCharString script(game_file,-1);
        CStringParser parser(script);
        if(parser.SkipPastString("BEGIN_ANIMATION_EVENTS")) {
            CSymbolMap mesh_symbol_map;
            mesh_symbol_map.LoadFromFile(NBankFileManager::GetHeaderDir()+L"meshdata.h");
            while(parser.SkipPastNextItemIfIs("BEGIN_EVENTS:")) {
                parser.SkipWhitespace();
                CCharString anim_name=parser.ReadAsString();
                long anim_index;
                if(mesh_symbol_map.FindSymbol(anim_name,anim_index)) {
                    while(!parser.SkipPastNextItemIfIs("END_EVENTS")) {
                        CCharString event_name=parser.ReadAsString();
                        long event_index=EventSymbolMap.GetSymbolSLONG(event_name);
                        float event_time=parser.ReadAsFloat();
                        bool stop_event=false;
                        CParsedItem parsed_item;
                        if(parser.PeekNextItem(parsed_item) && parsed_item.GetType()==PARSED_ITEM_STRING) {
                            if(parsed_item.GetString()=="STOP") {
                                stop_event=true;
                                parser.ReadNextItem(parsed_item);
                            } else if(parsed_item.GetString()=="START") {
                                parser.ReadNextItem(parsed_item);
                            }
                        }
                        CEvent event(event_index,event_time,false,stop_event);
                        CVectorMap<unsigned long,CSmallVector<CEvent,32> >::iterator anim_pos=AnimToExtraEventsMap.Find(anim_index);
                        if(!EventExists(anim_index,event))AddEventToAnim(anim_index,event);
                    }
                    DifferentToSavedCopy=false;
                    parser.SkipWhitespace();
                } else {
                    parser.SkipPastString("END_EVENTS");
                }
            }
            parser.SkipPastString("END_ANIMATION_EVENTS");
        }
    }
    AnimToExtraEventsMap.DeallocateUnusedMemory();
    if(GAllowDataGeneration) {
        CWideString bin_drive,bin_dir,bin_filename;
        CAFile::SplitPath(animation_events_save_path,&bin_drive,&bin_dir,&bin_filename,0);
        CWideString bin_pathname=bin_drive+bin_dir+bin_filename+L".bin";
        SaveBinary(bin_pathname);
    }
    return file_exists;
}
int main(){
    using namespace NAnimationEvents;
    for(unsigned mask=0;mask<256;++mask)for(int flavor=0;flavor<25;++flavor){
        GUseCompiledAnimationEvents=(mask&1)!=0;GAllowDataGeneration=(mask&2)!=0;
        filePresent=(mask&4)!=0;hasHeader=(mask&8)!=0;blockCount=(mask>>4)&3;
        bool dirty=(mask&64)!=0;
        CManager m;m.DifferentToSavedCopy=dirty;
        m.AnimToExtraEventsMap.items.push_back(CEvent(8,4.0f,true,false));
        std::vector<CEvent> want=m.AnimToExtraEventsMap.items;
        block=item=consumed=skips=meshes=opened=peeked=readTag=clears=trimmed=lookedUp=headerEnds=0;
        paths=loads=saves=compareCount=added=0;binaryName=L"";
        bool parse=!GUseCompiledAnimationEvents&&filePresent&&hasHeader;
        if(!GUseCompiledAnimationEvents&&filePresent)want.clear();
        int unknown=0,total=0,markers=0,unique=0,valid=0;
        for(int b=0;b<blockCount;++b){
            Plan& p=blocks[b];p.known=((b+flavor)%3)!=0;
            p.events=(mask&128)?3:(b+flavor)%3;
            if(parse && !p.known)++unknown;
            if(parse && p.known)++valid;
            for(int i=0;i<p.events;++i){
                p.tag[i]=(i+(b?flavor/5:flavor%5))%5;
                p.index[i]=1+((i+flavor/5)%2);
                p.time[i]=(i+flavor)%2?0.5f:0.25f;
                if(!parse || !p.known)continue;
                ++total;if(p.tag[i]==2||p.tag[i]==3)++markers;
                CEvent e(p.index[i],p.time[i],false,p.tag[i]==2);bool seen=false;
                for(size_t j=0;j<want.size();++j)if(want[j].Index==e.Index&&want[j].Time==e.Time&&want[j].StopEvent==e.StopEvent)seen=true;
                if(!seen){want.push_back(e);++unique;}
            }
        }
        require(m.Load(L"D:/data/events.txt")==bool(GUseCompiledAnimationEvents||filePresent));
        require(m.DifferentToSavedCopy==(GUseCompiledAnimationEvents||valid?false:dirty));
        require(m.AnimToExtraEventsMap.items==want);
        require(loads==int(GUseCompiledAnimationEvents)&&saves==int(!GUseCompiledAnimationEvents&&GAllowDataGeneration));
        require((loads||saves)?binaryName==L"D:/data/events.bin":binaryName.empty());
        require(paths==int(!GUseCompiledAnimationEvents)&&trimmed==paths&&clears==int(!GUseCompiledAnimationEvents&&filePresent)&&opened==clears);
        require(meshes==int(parse)&&headerEnds==meshes&&lookedUp==(parse?blockCount:0)&&skips==unknown);
        require(peeked==total&&consumed==total&&compareCount==total&&readTag==markers&&added==unique);
    }
    puts("006fa4e0_TEST PASS");return 0;
}