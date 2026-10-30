// CQ_HobbeCaveScript::RescuedBandit() -- each frame: wait for the HobbeCaveSideRoom level, gather the "HobbeRoaming" hobbes and, once they are all dead, record KilledSideRoomHobbes and stop
// Levers: ~vector<CScriptThing> __forceinline; for(;;){ if (Term) return; ...} outer loop (keeps three separate inline dtor copies, no cross-jumping); dead block `if (!Term) { if (!Term) KilledSideRoomHobbes = true; }` then one depth-0 `return;`
// Leaf: RescuedBandit
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D91740, 481 bytes (496 with padding), plain `ret`; spawned as "RescuedBandit"
//   (0x012D573C) at 0x00D8E4A0.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("HobbeCaveSideRoom" 0x012D562C,
//   "HobbeRoaming" 0x012D5808), CScriptBase::IsActiveThreadTerminating 0x00CB7940, AreAllThingsInVectorDead 0x00CBED00
//   (__fastcall), the vector<CScriptThing> destructor 0x008AC970 (out of line; manifest "~vector<CConversation>" is a
//   folded twin), free 0x00BFEA14 (inline destructor copies); CScriptThing slot 0 (virtual destructor);
//   CGameScriptInterface (+0x40) slots 7 NewScriptFrame, 11 IsLevelLoaded (+0x2c), 75 GetAllThingsWithScriptName (+0x12c).
// Layout (retail = Ego_r.pdb - 0x14): KilledSideRoomHobbes +0x4e; vector model as the landed CM_TauntingBandit::Main
//   0x00E4B430.
// Shape: the outer loop is `while (!terminating)` (rotated: entry test + bottom test after the frame yield).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
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
};
}
bool __fastcall AreAllThingsInVectorDead(const std::vector<CScriptThing>& things);   // 0x00CBED00
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_HobbeCaveScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    bool                  DoneIntro;                    // +0x48
    bool                  SavedBeforeCaves;             // +0x49
    bool                  BratInCutscene;               // +0x4a
    bool                  MissionSucceeded;             // +0x4b
    bool                  MissionFailed;                // +0x4c
    bool                  AlarmTriggered;               // +0x4d
    bool                  KilledSideRoomHobbes;         // +0x4e
    void RescuedBandit();
};
} // namespace NScript
using NScript::CQ_HobbeCaveScript;

void NScript::CQ_HobbeCaveScript::RescuedBandit()
{
    std::vector<CScriptThing> hobbes;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsLevelLoaded("HobbeCaveSideRoom")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->GetAllThingsWithScriptName("HobbeRoaming", hobbes);
        if (AreAllThingsInVectorDead(hobbes)) {
            if (!IsActiveThreadTerminating()) {
                if (!IsActiveThreadTerminating())
                    KilledSideRoomHobbes = true;
            }
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
        Game->NewScriptFrame();
    }
}