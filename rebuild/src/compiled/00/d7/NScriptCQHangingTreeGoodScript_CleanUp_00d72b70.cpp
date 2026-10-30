// CQ_HangingTreeGoodScript::CleanUp() -- remove every HTG_FreedomFighter and HTG_Audience thing, then (unless the thread is ending) the prisoner and the executioner if they still live
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D72B70, length 1575 bytes; called from 0x00D6FB73.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//   0x00CB7940, free 0x00BFEA14 (inline vector destructor copies), operator delete 0x00BFE9BC / ~CBaseClass 0x0099A2E0
//   (inline ~CScriptThing copies); CScriptThing slot 0 (virtual destructor) and IsAlive (slot 75, inline); through
//   the interface (+0x40): GetThingWithScriptName (72), GetAllThingsWithScriptName (75), RemoveThing (108).
// Strings: "HTG_Prisoner" 0x012D270C, "HTG_Executioner" 0x012D2698, "HTG_FreedomFighter" 0x012D2668, "HTG_Audience" 0x012D2688.
// Shape: the landed sibling CQ_HangingTreeEvilScript::CleanUp 0x00D6B720 (same CScriptThing / vector model) with a second
//   vector loop; every early return expands the whole inline cleanup. Both vectors are declared first (their zero stores
//   precede the first GetThingWithScriptName call in retail).
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
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
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
class CQ_HangingTreeGoodScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    void CleanUp();
};
void CQ_HangingTreeGoodScript::CleanUp()
{
    std::vector<CScriptThing> freedomFighters;
    std::vector<CScriptThing> audience;
    CScriptThing prisoner = Game->GetThingWithScriptName("HTG_Prisoner");
    CScriptThing executioner = Game->GetThingWithScriptName("HTG_Executioner");
    Game->GetAllThingsWithScriptName("HTG_FreedomFighter", freedomFighters);
    Game->GetAllThingsWithScriptName("HTG_Audience", audience);
    for (unsigned int i = 0; i < freedomFighters.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(freedomFighters[i], false, true);
    }
    if (IsActiveThreadTerminating())
        return;
    for (unsigned int j = 0; j < audience.size(); ++j) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(audience[j], false, true);
    }
    if (IsActiveThreadTerminating())
        return;
    if (prisoner.IsAlive())
        Game->RemoveThing(prisoner, false, true);
    if (executioner.IsAlive())
        Game->RemoveThing(executioner, false, true);
}
}