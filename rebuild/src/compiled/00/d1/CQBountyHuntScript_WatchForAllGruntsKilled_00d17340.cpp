// CQ_BountyHuntScript::WatchForAllGruntsKilled() -- script thread: collect the BH_GangGruntSpawn things, wait until all of them are dead, then set AllGruntsKilled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D17340, length 294 bytes, plain `ret`; started as a thread at 0x00D14D61.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, AreAllThingsInVectorDead
// (__fastcall, 0x00CBED00), CScriptBase::IsActiveThreadTerminating 0x00CB7940, CScriptThing's virtual destructor
// (slot 0, flag 0) per element and free 0x00BFEA14 (the vector's storage); through the game script interface (+0x40,
// retail vtable 0x01260F0C): GetAllThingsWithScriptName(const CCharString&, vector<CScriptThing>&) (slot 75, +0x12c,
// 0x008A8570), NewScriptFrame (slot 7, +0x1c, 0x006E7410). Literal "BH_GangGruntSpawn" 0x012CA46C.
// Layout: Ego_r.pdb CQ_BountyHuntScript - 0x14: AllGruntsKilled +0x61.
// Shape: a local STLport vector (three immediate zero stores) whose __forceinline destructor (by-value _Destroy loop of
// virtual destructor calls, then free) is expanded at all four returns with the free tail shared; the wait loop
// is `for (;;) { if (term) return; if (AllDead) break; yield; }`, followed by two terminating re-tests (a plain
// `inline` destructor is emitted out of line and called once).
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

template <class T> __forceinline void _Destroy(T* first, T* last)
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
    __forceinline ~vector()
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

class CScriptBase { public:
    virtual void v00();
    unsigned char _body[0x3c];                       // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                // 0x00CB7940
};

struct CQ_BountyHuntScript : CScriptBase {
    CGameScriptInterface* m_pInterface;              // +0x40
    void*                 m_pOwner;                  // +0x44
    unsigned char         _pad48[0x19];
    bool                  AllGruntsKilled;           // +0x61
    void WatchForAllGruntsKilled();
};

void CQ_BountyHuntScript::WatchForAllGruntsKilled()
{
    vector<CScriptThing> grunts;
    m_pInterface->GetAllThingsWithScriptName("BH_GangGruntSpawn", grunts);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (AreAllThingsInVectorDead(grunts))
            break;
        m_pInterface->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        return;
    if (IsActiveThreadTerminating())
        return;
    AllGruntsKilled = true;
}