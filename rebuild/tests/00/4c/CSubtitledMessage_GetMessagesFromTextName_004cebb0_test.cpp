#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <wchar.h>

#define FABLE_FASTCALL
#define FABLE_RETAIL_ASSERT(x) ((void)0)
static void require(bool ok, const char* what) {
    if (!ok) { printf("004cebb0_TEST FAIL %s\n", what); exit(1); }
}
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE, GROUP_SELECT_RANDOM, GROUP_SELECT_RANDOM_NO_REPEAT,
    GROUP_SELECT_SEQUENTIAL, GROUP_SELECT_FIRST_ONLY };
class CWideString {
public:
    CWideString(const std::wstring& s = L"") : Value(s) {}
    std::wstring Value;
};
static CWideString operator+(const wchar_t* prefix,const CWideString& suffix) {
    return CWideString(std::wstring(prefix) + suffix.Value);
}
class CCharString {
public:
    CCharString(const char* text="") : Value(text) {}
    bool operator!=(const CCharString& other) const { return Value != other.Value; }
    unsigned long GetCRC() const {
        unsigned long v=0; for(size_t i=0;i<Value.size();++i)v=(v*31+Value[i])&0xffffffffUL;
        return v;
    }
    CWideString ToWideString() const { std::wstring wide; for(size_t i=0;i<Value.size();++i)wide+=(wchar_t)(unsigned char)Value[i]; return CWideString(wide); }
    std::string Value;
};
template<class T> class CArray : public std::vector<T> {};
class CWorld {
public:
    CWorld() : Seed(1729) {}
    unsigned long* GetWorldSeed() const { return &Seed; }
    mutable unsigned long Seed;
};
static unsigned long draws, expectedRange, sampled, groupReads, drops, deletions, typeReads;
static unsigned long* expectedSeed;
static unsigned long GFRandom(unsigned long range,unsigned long* seed) {
    require(range==expectedRange && seed==expectedSeed && sampled<range,"random range, seed and test sample");
    ++draws; return sampled;
}
template<class T> class CIVCountedPointer {
public:
    explicit CIVCountedPointer(T* obj) : Object(obj) { if(Object)++Object->Refs; }
    CIVCountedPointer(const CIVCountedPointer& src) : Object(src.Object) { if(Object)++Object->Refs; }
    ~CIVCountedPointer() { if(Object) { ++drops; if(--Object->Refs==0)++deletions; } }
    T* operator->() const {return Object;}
private:
    T* Object;
};
namespace NGameText {
class CGroupEntry {
public:
    CGroupEntry() : Refs(0) {}
    const CArray<long>& GetTextEntries() const {return Entries;}
    mutable unsigned int Refs;
    CArray<long> Entries;
};
class CDataBank {
public:
    enum EDataType {TYPE_TEXT,TYPE_GROUP,TYPE_NARRATOR_LIST};
    unsigned long FindIndexBySymbolCRC(unsigned long crc) const {
        if(crc==CCharString("text").GetCRC())return 8;
        if(crc==CCharString("group").GetCRC())return 40;
        if(crc==CCharString("other").GetCRC())return 41;
        return 0;
    }
    unsigned long GetEntryDataType(unsigned long id) const {
        ++typeReads; require(id==8 || id==40 || id==41,"bank index"); return id==8?TYPE_TEXT:TYPE_GROUP;
    }
    CIVCountedPointer<const CGroupEntry> GetPGroupEntry(unsigned long id,bool load) const {
        require((id==40 || id==41) && load,"group lookup"); ++groupReads;
        return CIVCountedPointer<const CGroupEntry>(&Group);
    }
    CGroupEntry Group;
};
}
class CMainGameComponent {
public:
    static CMainGameComponent* Get();
    const NGameText::CDataBank& GetTextBank() const{return Bank;}
    const CWorld& PeekWorld() const {return World;}
    NGameText::CDataBank Bank; CWorld World;
};
static CMainGameComponent game;
CMainGameComponent* CMainGameComponent::Get(){return &game;}
class CSubtitledMessage {
public:
    CSubtitledMessage(long i=0,const char* name=0) : Index(i),Narrator(name?name:"") {}
    CSubtitledMessage(const CWideString& text) : Index(-1),Text(text.Value) {}
    static CSubtitledMessage GetMessageFromTextIndex(unsigned long ix,const char* narrator){return CSubtitledMessage(ix,narrator);}
    static void GetMessagesFromTextName(CArray<CSubtitledMessage>&,const char*,const char*,ETextGroupSelectionMethod);
    static CCharString MostRecentGroupQueried; static long LastReturnedIndex;
    long Index; std::string Narrator; std::wstring Text;
};
CCharString CSubtitledMessage::MostRecentGroupQueried;
long CSubtitledMessage::LastReturnedIndex;

void FABLE_FASTCALL CSubtitledMessage::GetMessagesFromTextName(
    CArray<CSubtitledMessage>& messages, const char* ptext_name,
    const char* pnarrator_name, ETextGroupSelectionMethod selection_method)
{
    const NGameText::CDataBank& text_bank = CMainGameComponent::Get()->GetTextBank();
    unsigned long text_index = text_bank.FindIndexBySymbolCRC(CCharString(ptext_name).GetCRC());
    if (text_index > 0) {
        if (text_bank.GetEntryDataType(text_index) == NGameText::CDataBank::TYPE_TEXT) {
            FABLE_RETAIL_ASSERT(pnarrator_name == 0);
            messages.push_back(GetMessageFromTextIndex(text_index, pnarrator_name));
        } else {
            FABLE_RETAIL_ASSERT(text_bank.GetEntryDataType(text_index) == NGameText::CDataBank::TYPE_GROUP);
            const CArray<long>& group_entries = text_bank.GetPGroupEntry(text_index, true)->GetTextEntries();
            if (selection_method == GROUP_SELECT_NONE) {
                for (long c1 = 0; c1 < group_entries.size(); ++c1)
                    messages.push_back(GetMessageFromTextIndex(group_entries[c1], pnarrator_name));
            } else if (selection_method == GROUP_SELECT_FIRST_ONLY) {
                if (!group_entries.empty())
                    messages.push_back(GetMessageFromTextIndex(group_entries.front(), pnarrator_name));
            } else {
                const CWorld& world = CMainGameComponent::Get()->PeekWorld();
                if (CCharString(ptext_name) != MostRecentGroupQueried) {
                    MostRecentGroupQueried = CCharString(ptext_name);
                    LastReturnedIndex = -1;
                }
                long random_index = 0;
                switch (selection_method) {
                case GROUP_SELECT_RANDOM:
                    random_index = GFRandom(group_entries.size(), world.GetWorldSeed());
                    break;
                case GROUP_SELECT_RANDOM_NO_REPEAT:
                    if (group_entries.size() > 1 && LastReturnedIndex != -1) {
                        random_index = GFRandom(group_entries.size() - 1, world.GetWorldSeed());
                        if (random_index >= LastReturnedIndex)
                            ++random_index;
                    } else {
                        random_index = GFRandom(group_entries.size(), world.GetWorldSeed());
                    }
                    break;
                case GROUP_SELECT_SEQUENTIAL:
                    random_index = LastReturnedIndex + 1;
                    if (random_index >= group_entries.size())
                        random_index = 0;
                    break;
                default:
                    FABLE_RETAIL_ASSERT(false);
                    break;
                }
                LastReturnedIndex = random_index;
                messages.push_back(GetMessageFromTextIndex(group_entries[random_index], pnarrator_name));
            }
        }
    } else {
        CWideString text_string = L"(HC)" + CCharString(ptext_name).ToWideString();
        messages.push_back(CSubtitledMessage(text_string));
    }
}

static unsigned int cases;
static void run(const char* name,int mode,int count,const char* oldGroup,long oldIndex,
                unsigned long pick,unsigned long range,long finalIndex,
                const long* want,int n,bool remember,bool useGroup,unsigned int retained) {
    const char* narrator=std::string(name)=="text"?0:"GUIDE";
    CArray<CSubtitledMessage> messages;
    messages.push_back(CSubtitledMessage(101));
    game.Bank.Group.Entries.clear();
    for(int i=0;i<count;++i)game.Bank.Group.Entries.push_back(23+i*9);
    game.Bank.Group.Refs=retained;
    CSubtitledMessage::MostRecentGroupQueried=oldGroup; CSubtitledMessage::LastReturnedIndex=oldIndex;
    draws=typeReads=groupReads=drops=deletions=0;
    expectedSeed=&game.World.Seed;sampled=pick;expectedRange=range;
    CSubtitledMessage::GetMessagesFromTextName(messages,name,narrator,(ETextGroupSelectionMethod)mode);
    require(messages.size()==(size_t)n+1 && messages[0].Index==101,"append preserves earlier message");
    for(int k=0;k<n;++k) {
        require(messages[k+1].Index==want[k],"selected entry index/order");
        if(want[k]==-1)require(messages[k+1].Text==std::wstring(L"(HC)")+CCharString(name).ToWideString().Value,"fallback wide prefix and key");
        else require(messages[k+1].Narrator==(narrator?narrator:""),"narrator forwarding");
    }
    require(typeReads==(unsigned)(std::string(name)!="missing") && groupReads==(unsigned)useGroup,"bank dispatch");
    require(game.Bank.Group.Refs==retained && (drops>0)==useGroup && deletions==(unsigned)(useGroup&&!retained),"temporary group reference");
    require(CSubtitledMessage::MostRecentGroupQueried.Value==(remember?name:oldGroup) &&
            CSubtitledMessage::LastReturnedIndex==finalIndex,"remembered name and index");
    require(draws==(range?1UL:0UL),"random draw count");
    ++cases;
}
int main() {
    long fallback=-1,single=8,all[]={23,32,41,50};
    for(unsigned int held=0;held<2;++held) {
        run("missing",2,4,"group",2,0,0,2,&fallback,1,false,false,held);
        run("text",3,4,"group",2,0,0,2,&single,1,false,false,held);
        for(int n=0;n<=4;++n) {
            run("group",0,n,"other",2,0,0,2,all,n,false,true,held);
            run("group",4,n,"other",2,0,0,2,all,n?1:0,false,true,held);
        }
        for(int n=1;n<=4;++n)for(int prev=-1;prev<n;++prev)for(int changed=0;changed<2;++changed) {
            const char* old=changed?"other":"group";long last=changed?-1:prev;
            for(int p=0;p<n;++p)run("group",1,n,old,prev,p,n,p,all+p,1,true,true,held);
            int range=(n>1&&last!=-1)?n-1:n;
            for(int q=0;q<range;++q) {
                int result=(range<n&&q>=last)?q+1:q;
                run("group",2,n,old,prev,q,range,result,all+result,1,true,true,held);
            }
            long seq=(last+1>=n)?0:last+1;
            run("group",3,n,old,prev,0,0,seq,all+seq,1,true,true,held);
            run("group",5,n,old,prev,0,0,0,all,1,true,true,held);
        }
    }
    require(cases==438,"scenario coverage");puts("004cebb0_TEST PASS");return 0;
}