// Behaviour test for 0x007ADAA0 CTCAIScratchpad::~CTCAIScratchpad (compiler-generated): members die in reverse declaration
// order -- the def pointer (Delete on the last reference), SeatName, TableName, TargetName, Action -- then the CTCBase part.
#include <cstdio>
static char g_log[16];
static int g_n = 0;
static void Log(char c) { if (g_n < 15) g_log[g_n++] = c; g_log[g_n] = 0; }

struct CDef { unsigned long Ref; void Delete() { Log('D'); } };
struct CDefPointer {
    CDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) { CDef* o = Object; if (--o->Ref == 0) o->Delete(); Object = 0; } }
};
class CCharString { public: char Tag; CCharString() : Tag('?') {} ~CCharString() { Log(Tag); } };
class CTCBase { public: virtual ~CTCBase() { Log('B'); } };
class CTCAIScratchpad : public CTCBase { public:
    CCharString Action;
    bool FaceHeroForAction;
    CCharString TargetName;
    CCharString TableName;
    CCharString SeatName;
    CDefPointer PDef;
};

int main()
{
    CDef def; def.Ref = 1;
    CTCAIScratchpad* s = new CTCAIScratchpad;
    s->Action.Tag = 'A'; s->TargetName.Tag = 'T'; s->TableName.Tag = 'L'; s->SeatName.Tag = 'S'; s->PDef.Object = &def;
    delete (CTCBase*)s;
    const char* want = "DSLTAB";
    for (int i = 0; want[i] || g_log[i]; ++i)
        if (want[i] != g_log[i]) { std::printf("007adaa0_TEST FAIL\n"); return 1; }
    std::printf("007adaa0_TEST PASS\n");
    return 0;
}