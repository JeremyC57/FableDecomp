// CQ_HangingTreeEvilScript::CleanUp() -- remove the HTE_FreedomFighter things, then (unless the thread is ending) the prisoner and the two guards if they still live
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6B720, length 922 bytes; called from 0x00D69AD0 and 0x00D69B8B.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//   0x00CB7940, free 0x00BFEA14 (inline vector destructor copies), operator delete 0x00BFE9BC / ~CBaseClass 0x0099A2E0
//   (inline ~CScriptThing copies); CScriptThing slot 0 (virtual destructor) and IsAlive (slot 75, inline); through
//   the interface (+0x40): GetThingWithScriptName (72), GetAllThingsWithScriptName (75), RemoveThing (108).
// Strings: "GUARD1" 0x012D2018, "GUARD2" 0x012D2010, "HTE_Prisoner" 0x012D2030, "HTE_FreedomFighter" 0x012D2050.
// Layout (retail = Ego_r.pdb - 0x14): Prisoner +0x80.
// Shape: CScriptThing model of the landed CQ_HangingTreeGoodScript::ReassignLeader 0x00D74F80 (inline IsAlive, empty
//   inline destructor); vector model of the landed CQ_HobbeCaveScript::RescuedBandit 0x00D91740.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
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
};
}
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
class CQ_HangingTreeEvilScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x80 - 0x48];
    CScriptThing          Prisoner;                     // +0x80
    void CleanUp();
};
void CQ_HangingTreeEvilScript::CleanUp()
{
    CScriptThing guard1 = Game->GetThingWithScriptName("GUARD1");
    CScriptThing guard2 = Game->GetThingWithScriptName("GUARD2");
    CScriptThing prisoner = Game->GetThingWithScriptName("HTE_Prisoner");
    std::vector<CScriptThing> freedomFighters;
    Game->GetAllThingsWithScriptName("HTE_FreedomFighter", freedomFighters);
    for (unsigned int i = 0; i < freedomFighters.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(freedomFighters[i], false, true);
    }
    if (IsActiveThreadTerminating())
        return;
    if (Prisoner.IsAlive())
        Game->RemoveThing(Prisoner, false, true);
    if (guard1.IsAlive())
        Game->RemoveThing(guard1, false, true);
    if (guard2.IsAlive())
        Game->RemoveThing(guard2, false, true);
}
}