// NScript::CQ_BountyHuntScript::WatchForDeputy1GruntsKilled() -- script thread: collect the BH_GangGruntSpawn things, wait until all are dead (stopping when the thread is terminated) and set AllDeputy1GruntsKilled
// Leaf: ?WatchForDeputy1GruntsKilled@CQ_BountyHuntScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00d18b20, 214 bytes, thiscall, plain `ret`.
// Identity (proven): its only reference is the functor store at 0x00D18906 (thread "WatchForDeputy1GruntsKilled"), the spawned-function registration of that thread name; Ego_r.pdb ?WatchForDeputy1GruntsKilled@CQ_BountyHuntScript@NScript@@QAEXXZ; the flag it sets is Ego_r.pdb CQ_BountyHuntScript::AllDeputy1GruntsKilled (PDB +0x71, retail +0x5d with the script base 0x14 smaller).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0, AreAllThingsInVectorDead (__fastcall) 0x00CBED00, vector<CScriptThing>::~vector 0x008AC970 (the out-of-line
//   copy at the normal end), CScriptThing's virtual destructor (slot 0) and free 0x00BFEA14 in the inlined copy; through the
//   game script interface (+0x40): GetAllThingsWithScriptName (slot 75, +0x12c), NewScriptFrame (slot 7, +0x1c).
//   Literal "BH_GangGruntSpawn" 0x012CA46C.
// Shape (family L140 = the landed WatchForAcolyteExit 0x00EDD060's model): retail expands the local vector's destructor at
//   the wait loop's terminating return and calls the out-of-line copy at the normal end; the lane calls the out-of-line
//   copy at every site, so the destruction is written per exit (a __forceinline copy of the STLport ~vector body at the
//   return, the out-of-line ~vector at the end, written in both arms of the final terminating test so VC7.1 tail-merges
//   the call and hoists its `lea ecx` above the branch), as in the landed twin.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
extern "C" void free(void* p);                         // 0x00BFEA14 (MSVCR71)

class CCPPointerInfo;

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CScriptThing : public CBaseClass { public:                     // 12 bytes, vtable 0x01238C8C (slot 0: virtual dtor)
    CScriptThing* PImp_Data;                           // +0x04
    CCPPointerInfo* PImp_Info;                         // +0x08
};

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
class CQ_BountyHuntScript : public CScriptBase {
public:
    CGameScriptInterface* m_pInterface;              // +0x40
    unsigned char         _pad44[0x5d - 0x44];
    bool                  AllDeputy1GruntsKilled;              // +0x5d
    void WatchForDeputy1GruntsKilled();
};
} // namespace NScript

void NScript::CQ_BountyHuntScript::WatchForDeputy1GruntsKilled()
{
    vector<CScriptThing> things;
    m_pInterface->GetAllThingsWithScriptName("BH_GangGruntSpawn", things);
    while (!AreAllThingsInVectorDead(things)) {
        m_pInterface->NewScriptFrame();
        if (IsActiveThreadTerminating()) {
            things.DestroyInline();
            return;
        }
    }
    if (IsActiveThreadTerminating()) {
        things.Destroy();
        return;
    }
    AllDeputy1GruntsKilled = true;
    things.Destroy();
}