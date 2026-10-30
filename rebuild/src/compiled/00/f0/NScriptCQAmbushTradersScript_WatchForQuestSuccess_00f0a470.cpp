// CQ_AmbushTradersScript::WatchForQuestSuccess() -- wait for Deserted Farm, for the spy contact and for Bowerstone Jail; then, while the quest is not finished and Bowerstone Posh is loaded, finish the quest once the convoy spy (AT_ConvoySpy) follows the hero; when the quest is finished and the success speech is done, mark the quest succeeded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0A470, length 1127 bytes.
// Callees: CBaseClass ctor/dtor 0x0099A2D0 / 0x0099A2E0, CCharString() 0x0099E4B0 / CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, Compare 0x00411570 (the inlined CCharString == const char*), operator delete 0x00BFE9BC, free
//   0x00BFEA14, ~CScriptThing 0x004AA840, vector<CScriptThing>::~vector 0x008AC970 (COMDAT-folded, propagated label
//   "~vector<CConversation>"), CScriptBase::IsActiveThreadTerminating 0x00CB7940; CScriptThing slots 0 (virtual dtor),
//   1 GetName, 75 IsAlive; the game script interface (+0x40) slots 7, 12, 70, 72, 231.
// Strings: "DesertedFarm" 0x012C7778, "BowerstoneJail" 0x012C7788, "AT_ConvoySpy" 0x012F7CE8, "BowerstonePosh" 0x012C6C6C.
// Layout (retail = Ego_r.pdb - 0x14): QuestFinished +0x49, QuestSucceeded +0x4a, SpyFollowing +0x5b, SpyFollowingHero +0x5c,
//   SpyContactMade +0x5f, QuestSuccessSpeechComplete +0x65.
// Shape: generated script code (`if (IsActiveThreadTerminating()) return;` after every statement); inline CCharString == of
//   the landed CQ_BountyHuntScript 0x00D151D0; destructors as the landed CMeleeThunder::Main 0x00D58080 (inline only at the
//   first wait loop's return, every later exit under in-body inline_depth(0) calls the out-of-line copies). ~vector and the
//   CCharString == are __forceinline: without it the first exit calls ~vector and the == is called out of line.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
#include <string.h>
#pragma intrinsic(strcmp)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
#pragma pack(push, 1)
struct CBasicStringChar {                               // CBasicString<char>, 13 bytes
    char*         PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    bool          UseFastExtend : 1;                    // +0x0c
    bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
class CCharStringData { public:                                // 17 bytes
    CBasicStringChar Data;                              // +0x00
    long             NoRefs;                            // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;                                      // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4
    void* Data;                                         // +8
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImpl;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
namespace std {
template <class T> class allocator;
template <class T, class A = allocator<T> > class vector {
public:                      // STLport vector (retail 12 bytes), malloc_alloc storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~vector()                             // out-of-line copy 0x008AC970
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) { return _M_start[i]; }
    void pop_back() { --_M_finish; _M_finish->~T(); }
};
}
using std::vector;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_AmbushTradersScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    bool QuestFailed;                                   // +0x48
    bool QuestFinished;                                 // +0x49
    bool QuestSucceeded;                                // +0x4a
    bool QuestOver;                                     // +0x4b
    bool RegionInvalid;                                 // +0x4c
    bool BanditIntroDone;                               // +0x4d
    bool TraderEscaped;                                 // +0x4e
    bool NoEnemiesLeft;                                 // +0x4f
    bool PhaseComplete;                                 // +0x50
    bool ConvoyTrigger2;                                // +0x51
    bool AllBanditsDead;                                // +0x52
    bool AllTradersDead;                                // +0x53
    bool AllGuardsDead;                                 // +0x54
    bool ContactDead;                                   // +0x55
    bool ContactAttackedByHero;                         // +0x56
    bool HeroDead;                                      // +0x57
    bool SpyDead;                                       // +0x58
    bool SentryAttackedByHero;                          // +0x59
    bool SpySeenBySentry;                               // +0x5a
    bool SpyFollowing;                                  // +0x5b
    bool SpyFollowingHero;                              // +0x5c
    bool SpyHeroProximity;                              // +0x5d
    bool SpyWanderReset;                                // +0x5e
    bool SpyContactMade;                                // +0x5f
    bool SpyEscaped;                                    // +0x60
    bool SpyExited;                                     // +0x61
    bool SpyReachedNoEntryMarker;                       // +0x62
    bool AnyTraderEscaped;                              // +0x63
    bool RegionClear;                                   // +0x64
    bool QuestSuccessSpeechComplete;                    // +0x65
    void WatchForQuestSuccess();
};
void CQ_AmbushTradersScript::WatchForQuestSuccess()
{
    CScriptThing thing;                                 // declared but never used: retail constructs and destroys it
    vector<CScriptThing> followers;
    CCharString name;                                   // likewise unused
    while (!Game->IsRegionLoaded("DesertedFarm")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    while (!SpyContactMade) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    while (!Game->IsRegionLoaded("BowerstoneJail")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    CScriptThing spy = Game->GetThingWithScriptName("AT_ConvoySpy");
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    for (;;) {
        if (Game->IsRegionLoaded("BowerstonePosh") && !QuestFinished) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            while (followers.size() != 0)
                followers.pop_back();
            Game->GetFollowingEntityList(Game->GetHero(), followers);
            long count = followers.size();
            if (count > 0) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                SpyFollowing = false;
                for (long i = 0; i < count; ++i) {
                    if (followers[i].IsAlive() && followers[i].GetName() == "AT_ConvoySpy") {
                        if (IsActiveThreadTerminating())
                            {
#pragma inline_depth(0)
                                return;
#pragma inline_depth()
                            }
                        SpyFollowing = true;
                        break;
                    }
                }
            }
            if (SpyFollowingHero || SpyFollowing) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                QuestFinished = true;
            }
        }
        if (QuestFinished && QuestSuccessSpeechComplete)
            break;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    QuestSucceeded = true;
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    {
#pragma inline_depth(0)
        return;
#pragma inline_depth()
    }
}
}