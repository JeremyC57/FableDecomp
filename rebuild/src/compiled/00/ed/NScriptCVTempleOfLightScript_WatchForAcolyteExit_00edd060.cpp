// CV_TempleOfLightScript::WatchForAcolyteExit() -- script thread: once the hero has attacked, collect the TOL_AcolyteGood things, wait until all are gone and set AllAcolytesEscaped
// Leaf: ?WatchForAcolyteExit@CV_TempleOfLightScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EDD060, length 270 bytes, plain `ret`.
// Label: catalog "CQ_BountyHuntScript::WatchForAllGruntsKilled" is propagated; the body is started as the thread
// "WatchForAcolyteExit" (0x012F3E28) by CV_TempleOfLightScript::Main (0x00EDB190, functor store at 0x00EDB3AD).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 /
// ~CCharString 0x0099EAE0, AreAllThingsInVectorDead (__fastcall, 0x00CBED00), vector<CScriptThing>::~vector
// (out-of-line COMDAT copy 0x008AC970, propagated label "~vector<CConversation>"), CScriptThing's virtual destructor
// (slot 0) and free 0x00BFEA14 in the inlined copy; through the game script interface (+0x40): NewScriptFrame
// (slot 7, +0x1c), GetAllThingsWithScriptName (slot 75, +0x12c). Literal "TOL_AcolyteGood" 0x012F3E54.
// Layout: Ego_r.pdb CV_TempleOfLightScript - 0x14: AttackedByHero +0x5b, AllAcolytesEscaped +0x5d.
// Shape: the inline-once destructor band: retail expands the local vector's destructor at the wait loop's terminating
// return and calls the out-of-line copy at the normal end (the two closing terminating tests jump to that call). The
// lane calls the out-of-line copy at every site, so the vector's destruction is written per exit: a __forceinline copy
// of the STLport ~vector body at the wait-loop return and the out-of-line ~vector (Destroy) once at the merged end.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
extern "C" void free(void* p);                         // 0x00BFEA14 (MSVCR71)

class CCPPointerInfo;

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}

namespace std {
template <class T> class allocator;
template <class T, class A = allocator<T> > class vector {
public:                     // STLport vector (retail 12 bytes), malloc_alloc storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    void Destroy();                                    // ~vector, out of line: 0x008AC970
    __forceinline void DestroyInline()                 // the same body, expanded once (VC's inline-once band)
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
}
using std::vector;
bool __fastcall AreAllThingsInVectorDead(const vector<CScriptThing>& things);   // 0x00CBED00

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

namespace NScript {
class CV_TempleOfLightScript : public CScriptBase {
public:
    CGameScriptInterface* m_pInterface;              // +0x40
    void*                 m_pOwner;                  // +0x44
    long DonationTotal;                              // +0x48
    long DonationThisVisit;                          // +0x4c
    long DonationCounter;                            // +0x50
    bool DonationInvalid;                            // +0x54
    bool DonationActive;                             // +0x55
    bool DisplayQuestInfo;                           // +0x56
    bool WeaponWarningComplete;                      // +0x57
    bool WeaponUnsheathed;                           // +0x58
    bool TempleIntroComplete;                        // +0x59
    bool InsideTemple;                               // +0x5a
    bool AttackedByHero;                             // +0x5b
    bool AcolyteAttackMessageComplete;               // +0x5c
    bool AllAcolytesEscaped;                         // +0x5d
    void WatchForAcolyteExit();
};
} // namespace NScript
using NScript::CV_TempleOfLightScript;

void NScript::CV_TempleOfLightScript::WatchForAcolyteExit()
{
    while (!AttackedByHero) {
        m_pInterface->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    vector<CScriptThing> acolytes;
    m_pInterface->GetAllThingsWithScriptName("TOL_AcolyteGood", acolytes);
    for (;;) {
        if (IsActiveThreadTerminating()) {
            acolytes.DestroyInline();
            return;
        }
        if (AreAllThingsInVectorDead(acolytes))
            break;
        m_pInterface->NewScriptFrame();
    }
    if (!IsActiveThreadTerminating() && !IsActiveThreadTerminating())
        AllAcolytesEscaped = true;
    acolytes.Destroy();
}