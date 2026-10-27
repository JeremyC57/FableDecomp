// behaviour test for 0x00893E70 CGameScriptInterface::MsgOnGameSavedManually() (model; the message manager is a recording stub)
#include <cstdio>
class CMessageEvent;
enum EMessageEventType { EMessageEventType_Force32 = 0x7fffffff };   // enumerator names not recovered
class CMessageEventFilter_IsOfType {                     // Ego_r.pdb, 4 bytes
public:
    CMessageEventFilter_IsOfType(EMessageEventType type) : Type(type) { }   // ??0CMessageEventFilter_IsOfType@@QAE@W4EMessageEventType@@@Z
    EMessageEventType Type;                              // +0x00
};
class CMessageEventFilter_OccuredBetween {               // Ego_r.pdb ??0CMessageEventFilter_OccuredBetween@@QAE@JJ@Z, 8 bytes
public:
    CMessageEventFilter_OccuredBetween(long minFrame, long maxFrame) : MinFrame(minFrame), MaxFrame(maxFrame) { }
    long MinFrame;                                       // +0x00
    long MaxFrame;                                       // +0x04
};
template <class TF1, class TF2> class CFilter_Bind2 {    // Ego_r.pdb: F1, F2 (pointers to the bound filters), 8 bytes
public:
    CFilter_Bind2(const TF1& f1, const TF2& f2) : F1(const_cast<TF1*>(&f1)), F2(const_cast<TF2*>(&f2)) { }
    TF1* F1;                                             // +0x00
    TF2* F2;                                             // +0x04
};
class CMessageEventManager {
public:
    template <class TFilter> const CMessageEvent* FindMostRecentMessage(const TFilter& filter) const;
        // <CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_OccuredBetween> > 0x008ABED0
};
class CCPPointerInfo;
template <class T> class CCountedPointer {               // Ego_r.pdb: Data, Info
public:
    T* Data;                                             // +0x00
    CCPPointerInfo* Info;                                // +0x04
    T* operator->() const { return Data; }
};
class CWorld {
public:
    unsigned char _pad_0x00[0x60];                       // vptr .. PScriptInfoManager (not touched here)
    CCountedPointer<CMessageEventManager> PMessageEventManager;   // +0x60 (PDB +96)
};
class CGameScriptInterface {                             // Ego_r.pdb 84 bytes
public:
    virtual ~CGameScriptInterface();
    long GetCurrentWorldFrameForMessages() const;        // 0x006E7510
    long GetMaxWorldFrameForMessages() const;            // 0x006E7530
    virtual bool MsgOnGameSavedManually() const;
    CWorld* World;                                       // +0x04
};

static int g_type = -1; static long g_first = 0, g_last = 0; static const CMessageEvent* g_result = 0;
template <class TFilter> const CMessageEvent* CMessageEventManager::FindMostRecentMessage(const TFilter& filter) const
{ g_type = (int)filter.F1->Type; g_first = filter.F2->MinFrame; g_last = filter.F2->MaxFrame; return g_result; }
CGameScriptInterface::~CGameScriptInterface() { }
long CGameScriptInterface::GetCurrentWorldFrameForMessages() const { return 100; }
long CGameScriptInterface::GetMaxWorldFrameForMessages() const { return 120; }
bool CGameScriptInterface::MsgOnGameSavedManually() const
{
    long first = GetCurrentWorldFrameForMessages();
    long last = GetMaxWorldFrameForMessages();
    CMessageEventManager* manager = World->PMessageEventManager.Data;
    if (manager->FindMostRecentMessage(
            CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_OccuredBetween>(
                CMessageEventFilter_IsOfType((EMessageEventType)0x3E), CMessageEventFilter_OccuredBetween(first, last))))
        return true;
    return false;
}
int main()
{
    static CMessageEventManager mgr; static CWorld world; world.PMessageEventManager.Data = &mgr;
    CGameScriptInterface gsi; gsi.World = &world;
    int ok = !gsi.MsgOnGameSavedManually() && g_type == 0x3E && g_first == 100 && g_last == 120;
    g_result = (const CMessageEvent*)&world;
    ok = ok && gsi.MsgOnGameSavedManually();
    if (!ok) { std::printf("00893e70_TEST FAIL\n"); return 1; }
    std::printf("00893e70_TEST PASS\n");
    return 0;
}