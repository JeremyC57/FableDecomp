// CQ_WizardBattleScript::CleanUp() -- kill every spawned screamer, remove the maze thing, stop the override music and take Hook Coast village out of limbo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E1D690, length 286 bytes, plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, KillAllThingsInVector (__fastcall,
// 0x00CBED82), operator delete 0x00BFE9BC, ~CBaseClass 0x0099A2E0, CScriptThing's virtual destructor (slot 0) and free
// 0x00BFEA14 (the local vector); through the game script interface (+0x40, retail vtable 0x01260F0C):
// GetAllThingsWithScriptName (slot 75, +0x12c, 0x008A8570), RemoveThing(const CScriptThing&, bool, bool) (slot 108,
// +0x1b0, 0x008910D0), StopOverrideMusic(bool) (slot 697, +0xae4, 0x008912A0), GetThingWithScriptName (slot 72, +0x120,
// 0x008A7D60), SetVillageLimbo(const CScriptThing&, bool) (slot 440, +0x6e0, 0x00896060).
// Literals "SpawnedScreamer" 0x012E104C, "HookCoastVillage" 0x012DE4A4.
// Layout: Ego_r.pdb CQ_WizardBattleScript - 0x14: Maze (CScriptThing) +0x5c.
// Shape: a local STLport vector (zeroed through edi, the function's zero register) with a __forceinline destructor
// (by-value _Destroy loop + free); the village is a named CScriptThing whose inline destructor runs first.
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
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)

class CCPPointerInfo { public:                                // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);         // destroys the pointee
    void* Data;
};

template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
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

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CScriptThing : public CBaseClass { public:                     // 12 bytes, vtable 0x01238C8C (slot 0: virtual dtor)
    CCountedPointer<CScriptThing> PImp;                // +0x04 Data, +0x08 Info
    ~CScriptThing() {}
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

class CGameScriptInterface;
class CGameScriptInterfaceBase;
void __fastcall KillAllThingsInVector(CGameScriptInterfaceBase& gsi, const vector<CScriptThing>& things, bool b);   // 0x00CBED82

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

namespace NScript {
class CQ_WizardBattleScript : public CScriptBase {
public:
    CGameScriptInterface* m_pInterface;              // +0x40
    void*                 m_pOwner;                  // +0x44
    unsigned char         _pad48[0x14];
    CScriptThing          Maze;                      // +0x5c
    void CleanUp();
};
} // namespace NScript
using NScript::CQ_WizardBattleScript;

void NScript::CQ_WizardBattleScript::CleanUp()
{
    vector<CScriptThing> screamers;
    m_pInterface->GetAllThingsWithScriptName("SpawnedScreamer", screamers);
    KillAllThingsInVector(*m_pInterface, screamers, false);
    m_pInterface->RemoveThing(Maze, false, true);
    m_pInterface->StopOverrideMusic(false);
    CScriptThing village = m_pInterface->GetThingWithScriptName("HookCoastVillage");
    m_pInterface->SetVillageLimbo(village, false);
}