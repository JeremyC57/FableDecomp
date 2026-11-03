#include <vector>
#include <string>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <cstdlib>
static int char_live=0,wide_live=0,file_live=0,parser_live=0;
class CWideString;
class CCharString {
public:
    std::string value;
    CCharString():value() { ++char_live; }
    CCharString(const char* p):value(p) { ++char_live; }
    CCharString(const std::string& p):value(p) { ++char_live; }
    CCharString(const CCharString& p):value(p.value) { ++char_live; }
    ~CCharString() { --char_live; }
    CCharString& operator=(const CCharString& p) { value=p.value;return *this; }
    bool operator==(const char* p) const { return value==p; }
    bool EqualsCaseInsensitive(const CCharString& p) const {
        if(value.size()!=p.value.size())return false;
        for(std::size_t i=0;i<value.size();++i)if(std::tolower((unsigned char)value[i])!=std::tolower((unsigned char)p.value[i]))return false;
        return true;
    }
    CWideString ToWideString() const;
};
class CWideString {
public:
    std::string value;
    CWideString():value() { ++wide_live; }
    CWideString(const char* p):value(p) { ++wide_live; }
    CWideString(const std::string& p):value(p) { ++wide_live; }
    CWideString(const CWideString& p):value(p.value) { ++wide_live; }
    ~CWideString() { --wide_live; }
    CWideString& operator=(const CWideString& p){value=p.value;return *this;}
    CCharString ToCharString() const { return CCharString(value); }
};
CWideString CCharString::ToWideString() const { return CWideString(value); }
CCharString operator+(const CCharString& a,const CCharString& b){return CCharString(a.value+b.value);}
static std::string file_text,file_name,directory;
static int reads=0,progress_count=0,read_length=0,peeked=0,skipped=0,case_compares=0;
static bool progress_valid=true;
enum FileAccessType{FILE_ACCESS_READ=1};
class CDiskFileWin32 {
public:
    CDiskFileWin32(const CWideString& name,FileAccessType a,unsigned long flags){++file_live;file_name=name.value;assert(a==FILE_ACCESS_READ&&flags==2);}
    ~CDiskFileWin32(){--file_live;}
    long GetLength() const{return (long)file_text.size();}
    void Read(void* p,long n,void* process){assert(!process);assert(n==(long)file_text.size());std::memcpy(p,file_text.data(),n);++reads;read_length=(int)n;}
};
enum EParsedItemType{PARSED_ITEM_NULL=0,PARSED_ITEM_SYMBOL=1,PARSED_ITEM_STRING=2,PARSED_ITEM_INTEGER=3};
class CParsedItem{public:CCharString text;};
class CStringParser {
    std::string text;
    std::size_t pos;
    void space(){while(pos<text.size()&&std::isspace((unsigned char)text[pos]))++pos;}
public:
    CStringParser(const char* p):text(p),pos(0){++parser_live;}
    ~CStringParser(){--parser_live;}
    bool NextItemExists(){space();return pos<text.size();}
    EParsedItemType PeekNextItemType(){space();++peeked;if(pos==text.size())return PARSED_ITEM_NULL;unsigned char c=(unsigned char)text[pos];if(std::isalpha(c)||c=='_')return PARSED_ITEM_STRING;if(std::isdigit(c)||c=='-')return PARSED_ITEM_INTEGER;return PARSED_ITEM_SYMBOL;}
    CCharString ReadAsString(){space();std::size_t start=pos;while(pos<text.size()&&(std::isalnum((unsigned char)text[pos])||text[pos]=='_'))++pos;return CCharString(text.substr(start,pos-start));}
    long ReadAsInteger(){space();const char* p=text.c_str()+pos;char* end=0;long n=std::strtol(p,&end,10);pos+=(end-p);return n;}
    CCharString ReadAsSymbol(){space();if(pos==text.size())return CCharString("");return CCharString(text.substr(pos++,1));}
    CCharString ReadAsStringUntilString(const CCharString& term){std::size_t end=text.find(term.value,pos);if(end==std::string::npos)end=text.size();std::string value=text.substr(pos,end-pos);pos=end;return CCharString(value);}
    bool ReadNextItem(CParsedItem& item){space();++skipped;if(pos==text.size())return false;std::size_t start=pos++;if(std::isdigit((unsigned char)text[start]))while(pos<text.size()&&std::isdigit((unsigned char)text[pos]))++pos;item.text=CCharString(text.substr(start,pos-start));return true;}
};
class CExceptionBase{};
class CGenericException:public CExceptionBase{};
namespace NProgressDisplay { void DisplayProgress(const CCharString& text,float value,bool first,bool second){++progress_count;progress_valid=progress_valid&&text.value=="Getting level files from world map"&&value==-1.0f&&!first&&!second;} }
namespace NResourceDirectoryNames { CWideString GetLevelDir(){return CWideString(directory);} }
class CWorldMap {public:void GetLevelFilesFromWorldFile(const CWideString&,std::vector<CWideString>&);};

void CWorldMap::GetLevelFilesFromWorldFile(const CWideString& world_filename,std::vector<CWideString>& level_files)
{
    CDiskFileWin32 file(world_filename,FILE_ACCESS_READ,2);
    long file_length=file.GetLength();
    std::vector<char> text(file_length+1);
    file.Read(&text[0],file_length,0);
    text[file.GetLength()]=0;
    CParsedItem parsed_item;
    CStringParser parser(&text[0]);
    CCharString level_name;
    long map_id=0;
    while(parser.NextItemExists()) {
        if(parser.PeekNextItemType()!=PARSED_ITEM_STRING) { parser.ReadNextItem(parsed_item); continue; }
        {
            CCharString name=parser.ReadAsString();
            if(name=="NewMap") {
                NProgressDisplay::DisplayProgress("Getting level files from world map",-1.0f,false,false);
                map_id=parser.ReadAsInteger();
            } else if(name=="EndMap") {
                if(map_id) {
                    if(level_files.size()<(unsigned long)(map_id+1)) level_files.resize(map_id+1);
                    level_files[map_id]=level_name.ToWideString();
                }
            } else if(name=="LoadedOnPlayerProximity") {
                CCharString t=parser.ReadAsString();
                t.EqualsCaseInsensitive("TRUE");
            } else if(name=="LevelName") {
                if(!(parser.ReadAsSymbol()=="\"")) throw CGenericException();
                level_name=NResourceDirectoryNames::GetLevelDir().ToCharString()+parser.ReadAsStringUntilString("\"");
            }
        }
    }
}

static int scenarios=0;
static void run(const std::string& input,const std::vector<std::string>& initial,const std::vector<std::string>& expected,int expected_progress,bool throws=false)
{
    assert(char_live==0&&wide_live==0&&file_live==0&&parser_live==0);
    file_text=input;reads=progress_count=peeked=skipped=0;progress_valid=true;directory="levels/";
    {
        CWorldMap world;CWideString filename("world.wld");std::vector<CWideString> result;
        for(std::size_t i=0;i<initial.size();++i)result.push_back(CWideString(initial[i]));
        bool caught=false;try{world.GetLevelFilesFromWorldFile(filename,result);}catch(const CGenericException&){caught=true;}
        assert(caught==throws);assert(result.size()==expected.size());
        for(std::size_t i=0;i<expected.size();++i)assert(result[i].value==expected[i]);
        assert(reads==1&&read_length==(int)input.size()&&file_name=="world.wld");
        assert(progress_count==expected_progress&&progress_valid);assert(file_live==0&&parser_live==0&&char_live==0);
    }
    assert(char_live==0&&wide_live==0&&file_live==0&&parser_live==0);++scenarios;
}
int main()
{
    std::vector<std::string> none,initial,expected;
    run("",none,none,0);
    run(" \n 123 ; @ UnknownThing EndMap LoadedOnPlayerProximity TRUE",none,none,0);
    run("NewMap 0 LevelName \"ignored\" EndMap",none,none,1);
    expected.resize(2);expected[1]="levels/oakvale";
    run("NewMap 1 LevelName \"oakvale\" EndMap",none,expected,1);
    run("NewMap 1 LevelName \"oakvale\" LoadedOnPlayerProximity TrUe EndMap",none,expected,1);
    run("NewMap 1 LevelName \"oakvale\" LoadedOnPlayerProximity FALSE EndMap",none,expected,1);
    run("NewMap 1 LevelName !",none,none,1,true);
    run("LevelName !",none,none,0,true);
    initial.push_back("zero");initial.push_back("old1");initial.push_back("old2");
    expected=initial;expected[1]="levels/a";
    run("NewMap 1 LevelName \"a\" EndMap",initial,expected,1);
    expected=initial;expected[1]="levels/b";
    run("NewMap 1 LevelName \"a\" LevelName \"b\" EndMap",initial,expected,1);
    expected.resize(4);expected[3]="levels/reused";expected[1]="levels/reused";
    run("NewMap 3 LevelName \"reused\" EndMap NewMap 1 EndMap",initial,expected,2);
    for(int size=0;size<7;++size)for(int map=1;map<8;++map)for(int repeats=1;repeats<4;++repeats) {
        initial.clear();for(int i=0;i<size;++i){char b[32];std::sprintf(b,"keep%d",i);initial.push_back(b);}
        expected=initial;if(expected.size()<(unsigned int)(map+1))expected.resize(map+1);
        char b[128];std::sprintf(b,"NewMap %d LevelName \"test_%d_%d\" LoadedOnPlayerProximity TRUE EndMap ",map,size,map);
        std::string input;for(int i=0;i<repeats;++i)input+=b;
        char name[64];std::sprintf(name,"levels/test_%d_%d",size,map);expected[map]=name;
        run(input,initial,expected,repeats);
    }
    assert(scenarios==158);
    std::puts("00501d30_TEST PASS");return 0;
}
