// Host-only declaration adapter mirrors fable_string.h; target uses its actual header.
#ifndef _MSC_VER
#include <stddef.h>
#define FABLE_WIDE_STRING_HOST_TYPES
#define FABLE_FASTCALL
#define __cdecl
typedef unsigned int fable_u32;
typedef int fable_i32;
typedef unsigned char fable_u8;



struct CWideStringData
{
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};

#pragma pack(push, 1)
struct CCharStringData
{
    char* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_u8 flags0C;
    fable_i32 owners;
};
#pragma pack(pop)

class CCharString;

class CWideString
{
public:
    CWideString();
    CWideString(const CWideString& other);
    explicit CWideString(const wchar_t* text);
    ~CWideString();
    operator const wchar_t*() const;
    long GetLength() const;
    CWideString& operator=(const CWideString& other);
    const CWideString& operator=(const wchar_t* text);
    const CWideString& operator+=(const CWideString& other);
    const CWideString& operator+=(const wchar_t* text);
    bool operator!=(const wchar_t* other) const;

    static CWideString FABLE_FASTCALL CreateFromCharString(
        const CCharString& text);

private:
    explicit CWideString(const CCharString& text);
    void UnassignString();
    CWideStringData* AllocStringData(
        const wchar_t* text,
        long length);

    CWideStringData* storage_;
};

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const CWideString& right);
CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right);
CWideString FABLE_FASTCALL operator+(
    const wchar_t* left,
    const CWideString& right);

class CCharString
{
public:
    CCharString();
    CCharString(const char* text, long length);
    ~CCharString();
    operator const char*() const;
    CCharString& operator=(const CCharString& other);
    const CCharString& operator=(const char* text);
    CWideString ToWideString() const;

private:
    void UnassignString();
    CCharStringData* AllocStringData(
        const char* text,
        long length);

    CCharStringData* storage_;
};

CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    const char* right);

// Retail folds equivalent narrow/wide string lifetime helpers. These counters
// remain separate because their addresses and object families are distinct.
extern fable_i32 g_CWideStringInstanceCount_013BCA20;
extern fable_i32 g_CCharStringInstanceCount_013BD800;
extern wchar_t g_FableEmptyWideString_0129A8E0[1];

#endif
#include <stdio.h>
#ifndef _MSC_VER
#define __declspec(x)
#define __fastcall
#define FABLE_ENGINE_CSCRIPTTHING_H
struct CCPPointerInfo;
struct CScriptThing { void* __vftable; CScriptThing* PImp_Data; CCPPointerInfo* PImp_Info; };
#define FABLE_ENGINE_CGAMESCRIPTINTERFACE_H
struct CGameScriptInterface { void* __vftable; };
#endif
// CV_TalentlessBardScript::CV_TalentlessBardScript(CScriptDataBase*,CGameScriptInterface*) -- construct bard script state, strings and timers
// Retail0x00ED76E0,164 bytes. Historical grade count uncertain: reserve5, leaving at most5 new grades.
// CScriptBase ctor00CB8110; CBaseClass ctor0099A2D0 (RTTI1231710); CScriptThing vtable1238C8C.
// Shared CScriptThing is12 bytes; this native constructor view preserves its PImp member.
// Donor script base84 bytes is64 in retail; SongItems16->12 and SongThingAvailable20->12 explain total260->228.
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
struct CScriptDataBase;
struct CGameScriptTimerMethods {
    void* BeforeRegisterTimer[87];
    long (__fastcall *RegisterTimer)(CGameScriptInterface*); // slot87,0x006E79E0
};
extern CGameScriptInterface* g_Game; // 0x0143E8F8
struct CScriptBaseNative {
    virtual ~CScriptBaseNative();
    unsigned char RetailMembers[0x3c];
    CScriptBaseNative(); // 0x00CB8110
};
struct __declspec(novtable) CQuestScriptObjectNative : CScriptBaseNative {
    CGameScriptInterface* Game;
    CScriptDataBase* Data;
    CQuestScriptObjectNative(CScriptDataBase* data,CGameScriptInterface* game)
        : CScriptBaseNative(),Game(game),Data(data) {}
};
struct CBaseClass {
    virtual ~CBaseClass(); // RTTI1231710 slot0, deleting destructor004371F0
    CBaseClass(); // 0x0099A2D0
};
struct CScriptThingImplementationPointer {
    CScriptThing* Data;
    CCPPointerInfo* Info;
    CScriptThingImplementationPointer() : Data(0),Info(0) {}
};
struct CScriptThingConstructorView : CBaseClass {
    CScriptThingImplementationPointer PImp;
    virtual ~CScriptThingConstructorView(); // CScriptThing RTTI1238C8C slot0 destructor004AB170
    CScriptThingConstructorView() : CBaseClass(),PImp() {}
};
#ifndef FABLE_WIDE_STRING_HOST_TYPES
#include "fable_string.h"
#endif
struct CQuestInfoStrings { CCharString name; CCharString tag; };
struct CScriptTimer {
    long TimerIndex;
    CScriptTimer() : TimerIndex(((CGameScriptTimerMethods*)g_Game->__vftable)->RegisterTimer(g_Game)) {}
};
struct CScriptRetailArray {
    void* Start; void* Finish; void* End;
    CScriptRetailArray() : Start(0),Finish(0),End(0) {}
};
struct CV_TalentlessBardScript : CQuestScriptObjectNative {
    virtual ~CV_TalentlessBardScript();
    bool bHurtBard,bBard_finished;
    unsigned char BeforeSongCount[2];
    unsigned long lNumber_of_things_sung_about;
    bool bRewardGiven;
    unsigned char BeforeBard[3];
    CScriptThingConstructorView stBard;
    bool BardIntroComplete,AttackedByHero,BardFollowing;
    unsigned char BeforeTimer;
    CScriptTimer BardWanderTime;
    CScriptRetailArray SongItems;
    CQuestInfoStrings QuestInfo[12];
    CScriptRetailArray SongThingAvailable;
    CScriptTimer RandomSpeechTimer;
    CV_TalentlessBardScript(CScriptDataBase*,CGameScriptInterface*);
};
CV_TalentlessBardScript::CV_TalentlessBardScript(CScriptDataBase* data,CGameScriptInterface* game)
    : CQuestScriptObjectNative(data,game),stBard() {}

struct CScriptDataBase { int Token; };
CGameScriptInterface* g_Game;
static int bad,scriptBaseCalls,thingBaseCalls,stringCalls,timerCalls,eventCount;
static char events[32];
static CCharString* strings[24];
static void record(char event)
{
    if (eventCount>=32) { bad=1;return; }
    events[eventCount++]=event;
}
CScriptBaseNative::CScriptBaseNative() { ++scriptBaseCalls;record('B'); }
CScriptBaseNative::~CScriptBaseNative() {}
CBaseClass::CBaseClass() { ++thingBaseCalls;record('S'); }
CBaseClass::~CBaseClass() {}
CScriptThingConstructorView::~CScriptThingConstructorView() {}
CV_TalentlessBardScript::~CV_TalentlessBardScript() {}
CCharString::CCharString()
{
    if (timerCalls!=1 || stringCalls>=24) { bad=1;return; }
    strings[stringCalls]=this;
    storage_=(CCharStringData*)(size_t)(0x1000+stringCalls);
    ++stringCalls;
    record('C');
}
CCharString::~CCharString() {}
CCharString::operator const char*() const { return (const char*)storage_; }
static long __fastcall registerTimer(CGameScriptInterface* game)
{
    if (game!=g_Game || scriptBaseCalls!=1 || thingBaseCalls!=1 ||
        (timerCalls==0 ? stringCalls!=0 : stringCalls!=24)) bad=1;
    ++timerCalls;record('T');return 100+timerCalls;
}
static int run(CScriptDataBase* data,CGameScriptInterface* suppliedGame)
{
    bad=scriptBaseCalls=thingBaseCalls=stringCalls=timerCalls=eventCount=0;
    CV_TalentlessBardScript script(data,suppliedGame);
    if (bad || scriptBaseCalls!=1 || thingBaseCalls!=1 || stringCalls!=24 || timerCalls!=2 ||
        eventCount!=28 || events[0]!='B' || events[1]!='S' || events[2]!='T' || events[27]!='T' ||
        script.Game!=suppliedGame || script.Data!=data ||
        script.stBard.PImp.Data || script.stBard.PImp.Info ||
        script.BardWanderTime.TimerIndex!=101 || script.RandomSpeechTimer.TimerIndex!=102 ||
        script.SongItems.Start || script.SongItems.Finish || script.SongItems.End ||
        script.SongThingAvailable.Start || script.SongThingAvailable.Finish || script.SongThingAvailable.End) return 0;
    for (int i=0;i<12;++i) {
        if (strings[2*i]!=&script.QuestInfo[i].name || strings[2*i+1]!=&script.QuestInfo[i].tag ||
            events[3+2*i]!='C' || events[4+2*i]!='C' ||
            (const char*)script.QuestInfo[i].name!=(const char*)(size_t)(0x1000+2*i) ||
            (const char*)script.QuestInfo[i].tag!=(const char*)(size_t)(0x1001+2*i)) return 0;
    }
    return !bad;
}
int main()
{
    CGameScriptTimerMethods methods;
    methods.RegisterTimer=registerTimer;
    CGameScriptInterface globalGame,passedGame;
    globalGame.__vftable=&methods;
    passedGame.__vftable=0;
    g_Game=&globalGame;
    CScriptDataBase data;
    if (!run(&data,&passedGame) || !run(0,0)) {
        printf("00ed76e0_TEST FAIL\n");return 1;
    }
    printf("00ed76e0_TEST PASS\n");return 0;
}