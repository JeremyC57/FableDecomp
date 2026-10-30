// CV_KnotholeGladeGatesScript::EndMission() -- remove every WB_Villager, then the guard runner, the gate guard (if alive) and the horn blower, and flag the mission as succeeded
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA1360, 0x2c2 bytes, plain `ret`. PDB ?EndMission@CV_KnotholeGladeGatesScript@NScript@@QAEXXZ.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940,
//   free 0x00BFEA14 (inline vector destructor copies), operator delete 0x00BFE9BC / ~CBaseClass 0x0099A2E0 (inline
//   ~CScriptThing copies); CScriptThing slot 0 (virtual destructor, pop_back and the vector destructor) and IsAlive (slot 75,
//   inline); CGameScriptInterface (+0x40) slots 72 GetThingWithScriptName, 75 GetAllThingsWithScriptName, 108 RemoveThing.
// Strings: "WB_Villager" 0x012E09A8, "KGG_GuardRunner" 0x012EDBE8, "KG_GateGuard" 0x012EDBD8, "WB_HornBlower" 0x012EDBC8.
// Shape: the villager loop removes `things.back()` and pops it while an index counts up against the shrinking size
//   (size() recomputed each pass: the /12 imul); each terminate exit expands its own inline vector destructor.
//   CScriptThing / vector model of the landed CQ_HangingTreeEvilScript::CleanUp 0x00D6B720.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, MissionSucceeded +0x48.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
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
    T& back() { return *(_M_finish - 1); }
    void pop_back() { --_M_finish; _M_finish->~T(); }
};
}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CV_KnotholeGladeGatesScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                          // +0x40
    void* Data;                                          // +0x44
    bool MissionSucceeded;                               // +0x48
    void EndMission();
};
} // namespace NScript
using NScript::CV_KnotholeGladeGatesScript;

void NScript::CV_KnotholeGladeGatesScript::EndMission()
{
    std::vector<CScriptThing> villagers;
    Game->GetAllThingsWithScriptName("WB_Villager", villagers);
    for (unsigned int i = 0; i < villagers.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(villagers.back(), false, true);
        villagers.pop_back();
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing runner = Game->GetThingWithScriptName("KGG_GuardRunner");
    Game->RemoveThing(runner, false, true);
    CScriptThing guard = Game->GetThingWithScriptName("KG_GateGuard");
    if (guard.IsAlive())
        Game->RemoveThing(guard, false, true);
    CScriptThing hornBlower = Game->GetThingWithScriptName("WB_HornBlower");
    Game->RemoveThing(hornBlower, false, true);
    MissionSucceeded = true;
}