// CQ_HangingTreeGoodScript::WatchForHeroNearExit() -- gather the REGION_EXIT_POINT things, then each frame check whether the hero is within 8 of any exit while more than 3 from the prisoner; the first time he is, have the leader tell him not to leave, and stop
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D76490, length 555 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//   0x00CB7940, IsDistanceBetweenThingsUnder 0x00CBE2FF / IsDistanceBetweenThingsOver 0x00CBE3EA (__fastcall), free
//   0x00BFEA14 (inline vector destructor copies); CScriptThing slot 0 (virtual destructor); through the interface (+0x40):
//   NewScriptFrame (7), GetHero (70), GetAllThingsWithDefName (80), AddNewConversation (364), AddLineToConversation (366).
// Strings: "REGION_EXIT_POINT" 0x012CA510, "TEXT_QST_030_GUARD_DONT_EXIT" 0x012D2B6C.
// Layout: Leader +0x68, Prisoner +0x74 (the landed CQ_HangingTreeGoodScript::ReassignLeader 0x00D74F80).
// Shape: vector model of the landed CQ_HobbeCaveScript::RescuedBandit 0x00D91740 (`__forceinline` ~vector).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
namespace std {
template<class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA
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
class CQ_HangingTreeGoodScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x68 - 0x48];
    CScriptThing          Leader;                       // +0x68
    CScriptThing          Prisoner;                     // +0x74
    void WatchForHeroNearExit();
};
void CQ_HangingTreeGoodScript::WatchForHeroNearExit()
{
    bool warned = false;
    std::vector<CScriptThing> exits;
    Game->GetAllThingsWithDefName("REGION_EXIT_POINT", exits);
    while (!warned) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        for (unsigned int i = 0; i < exits.size(); ++i) {
            if (IsActiveThreadTerminating())
                return;
            if (IsDistanceBetweenThingsUnder(Game->GetHero(), exits[i], 8.0f)
                && IsDistanceBetweenThingsOver(Game->GetHero(), Prisoner, 3.0f)) {
                long conversation = Game->AddNewConversation(Leader, false, false);
                Game->AddLineToConversation(conversation, "TEXT_QST_030_GUARD_DONT_EXIT", false, Leader, Game->GetHero());
                warned = true;
            }
        }
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
}
}