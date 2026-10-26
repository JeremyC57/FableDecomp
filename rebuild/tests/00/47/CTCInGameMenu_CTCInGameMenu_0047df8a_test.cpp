// Behaviour test for CTCInGameMenu::CTCInGameMenu(CThing*) (retail 0x0047DF8A): model copy + stub callees.
#include <cstdio>

class CThing;
static int g_globalUpdates = 0;
static unsigned long g_vectorCount = 0;
class CTCBase {
public:
    CTCBase(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) {}
    CThing* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
    void AddToGlobalUpdateTCs() { IsInGlobalUpdateTCs = true; ++g_globalUpdates; }
};
template <class T> struct CRetailVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { } };
class CEnginePrimitiveHandle;
struct CEnginePrimitiveHandleVector { unsigned long count; CEnginePrimitiveHandleVector(unsigned long n) : count(n) { g_vectorCount = n; } };
struct CSignatureMap { int built; CSignatureMap() : built(1) {} };
class CWideString; class CCharString; class CDateAndTime; class CDefPointeeBase;
namespace NUISystem { class CComponent; class CYesNo; }
class CTCInGameMenu : public CTCBase {
public:
    bool             InGameMenuOpen;
    CDefPointeeBase* PDef;
    long             GlobalDefIndex;
    long             ActiveScreen;
    long             ItemCount;
    long             ItemHighlightIndex;
    long             ItemTopIndex;
    CRetailVector<CWideString>   FileNames;
    CRetailVector<CDateAndTime>  FileTimes;
    CEnginePrimitiveHandleVector PrimitiveHandles;
    CRetailVector<CEnginePrimitiveHandle> MenuPrimitiveHandles;
    CRetailVector<NUISystem::CComponent*> Components;
    long             LoadGameIndex;
    double           CurStartPauseTime;
    double           AccumulativePausedTime;
    CRetailVector<CCharString>   FileRegionNames;
    CRetailVector<CCharString>   FileRegionMiniMapNames;
    CRetailVector<long>          FilePlayTimes;
    double           LastRenderTime;
    bool             OpenInventory;
    NUISystem::CYesNo* m_pOKDialog;
    NUISystem::CYesNo* PVideoOptionsNotAcceptedDialog;
    NUISystem::CYesNo* PVideoOptionsChangedDialog;
    long             SaveGameToLoad;
    double           VideoChangesTimer;
    long             SaveGameIndex;
    long             FramesToWaitToSave;
    NUISystem::CComponent* PSaveInProgress;
    double           SaveInProgressTime;
    CSignatureMap    Signatures;
    NUISystem::CComponent* PMousePointer;
    NUISystem::CYesNo* PYesNo;
    CTCInGameMenu(CThing* thing);
};
CTCInGameMenu::CTCInGameMenu(CThing* thing)
    : CTCBase(thing), InGameMenuOpen(false), PDef(0), ActiveScreen(0), ItemCount(0), ItemHighlightIndex(0), ItemTopIndex(0),
      PrimitiveHandles(5), LoadGameIndex(-1), CurStartPauseTime(0.0), AccumulativePausedTime(0.0), LastRenderTime(0.0),
      OpenInventory(false), m_pOKDialog(0), PVideoOptionsNotAcceptedDialog(0), PVideoOptionsChangedDialog(0),
      SaveGameToLoad(-1), VideoChangesTimer(-1.0), SaveGameIndex(-1), FramesToWaitToSave(-1), PSaveInProgress(0),
      PMousePointer(0), PYesNo(0)
{
    AddToGlobalUpdateTCs();
}
int main()
{
    int fail = 0;
    CThing* thing = (CThing*)0x1234;
    CTCInGameMenu menu(thing);
    if (menu.ParentThing != thing || !menu.IsInGlobalUpdateTCs || g_globalUpdates != 1) fail = 1;
    if (menu.InGameMenuOpen || menu.PDef || menu.ActiveScreen || menu.ItemCount || menu.ItemHighlightIndex || menu.ItemTopIndex) fail = 2;
    if (g_vectorCount != 5 || menu.PrimitiveHandles.count != 5 || menu.FileNames._M_start || menu.Components._M_end_of_storage) fail = 3;
    if (menu.LoadGameIndex != -1 || menu.SaveGameToLoad != -1 || menu.SaveGameIndex != -1 || menu.FramesToWaitToSave != -1) fail = 4;
    if (menu.CurStartPauseTime != 0.0 || menu.AccumulativePausedTime != 0.0 || menu.LastRenderTime != 0.0 || menu.VideoChangesTimer != -1.0) fail = 5;
    if (menu.OpenInventory || menu.m_pOKDialog || menu.PVideoOptionsNotAcceptedDialog || menu.PVideoOptionsChangedDialog || menu.PSaveInProgress || menu.PMousePointer || menu.PYesNo || !menu.Signatures.built) fail = 6;
    if (fail) { std::printf("0047df8a_TEST FAIL\n"); return 1; }
    std::printf("0047df8a_TEST PASS\n");
    return 0;
}