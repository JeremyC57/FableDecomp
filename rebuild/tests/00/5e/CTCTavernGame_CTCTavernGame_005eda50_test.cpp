// Behaviour test for CTCTavernGame::~CTCTavernGame @ 0x005EDA50 (implicit): the members are destroyed in reverse declaration
// order -- the game master pointer, the stats / target / score font counted pointers (each info block released at count zero),
// the winnings / money / bet / box / handle primitive vectors, the UI component vector -- and then ~CTCBase runs.
#include <stdio.h>
#include <string.h>
static char g_log[512];
static void logc(const char* s) { strcat(g_log, s); strcat(g_log, ","); }
class CCPPointerInfo { public: unsigned int RefCount; void (*DeleteFunc)(void* data); void* Data; };
static void deleteFont(void* data) { logc((const char*)data); }
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
};
class Named { public: const char* Name; ~Named() { logc(Name); } };
class CFontBank;
class CTCBase { public: virtual ~CTCBase() { logc("base"); } void* ParentThing; };
class CTCTavernGame : public CTCBase { public:
    Named                       UIComponents;
    int                         NumRounds;
    Named                       Handles;
    Named                       BoxHandles;
    Named                       BetHandles;
    Named                       MoneyHandles;
    Named                       WinningsHandles;
    CCountedPointer<CFontBank>  PScoreFont;
    CCountedPointer<CFontBank>  PTargetFont;
    CCountedPointer<CFontBank>  PStatsFont;
    float                       StateTimer;
    Named                       PGameMaster;
    bool                        GamePlayed;
};
void EmitImplicitDestructor(CTCTavernGame* game) { game->CTCTavernGame::~CTCTavernGame(); }
static int fail(int line) { printf("005eda50_TEST FAIL\nline %d\n%s\n", line, g_log); return 1; }
static CCPPointerInfo* info(unsigned int count, const char* name)
{ CCPPointerInfo* i = new CCPPointerInfo; i->RefCount = count; i->DeleteFunc = deleteFont; i->Data = (void*)name; return i; }
int main()
{
    CTCTavernGame* game = (CTCTavernGame*)operator new(sizeof(CTCTavernGame));
    game->UIComponents.Name = "ui"; game->Handles.Name = "h"; game->BoxHandles.Name = "box"; game->BetHandles.Name = "bet";
    game->MoneyHandles.Name = "money"; game->WinningsHandles.Name = "win"; game->PGameMaster.Name = "master";
    game->PScoreFont.Info = info(1, "score"); game->PTargetFont.Info = info(2, "target"); game->PStatsFont.Info = 0;
    CCPPointerInfo* shared = game->PTargetFont.Info;
    EmitImplicitDestructor(game);
    if (strcmp(g_log, "master,score,win,money,bet,box,h,ui,base,") != 0) return fail(__LINE__);
    if (shared->RefCount != 1 || game->PScoreFont.Info != 0 || game->PTargetFont.Data != 0) return fail(__LINE__);
    delete shared;
    operator delete(game);
    printf("005eda50_TEST PASS\n");
    return 0;
}