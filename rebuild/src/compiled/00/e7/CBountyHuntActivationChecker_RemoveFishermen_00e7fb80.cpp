// CBountyHuntActivationChecker::RemoveFishermen() -- remove the fishing competition's organiser if he is alive, then every living competitor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E7FB80, 456 bytes (464 with padding), plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, operator delete 0x00BFE9BC (the counted
//   pointer's info block), ~CBaseClass 0x0099A2E0, CScriptThing's virtual destructor (slot 0) and free 0x00BFEA14 (the
//   local vector); CScriptThing::IsAlive slot 75 (+0x12c); through the game script interface (+0x04):
//   GetThingWithScriptName 72 (+0x120, by value), GetAllThingsWithScriptName 75 (+0x12c), RemoveThing 108 (+0x1b0).
// Literals "FishCompOrganiser" 0x012EA554, "FishCompCompetitor" 0x012EA568.
// Layout: Ego_r.pdb CBountyHuntActivationChecker (28 bytes): entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14.
// Shape: the landed CQ_WizardBattleScript::CleanUp (0x00E1D690) model: by-value CScriptThing temporaries with the inline
//   counted-pointer destructor, a local STLport vector with a __forceinline destructor (free); the competitor count is
//   read once.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
extern "C" void free(void* p);                         // 0x00BFEA14 (MSVCR71)
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)

#include "engine/CCountedPointer.h"


#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}

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
    long size() const { return _M_finish - _M_start; }
    T& operator[](long n) { return _M_start[n]; }
    __forceinline ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
}
using std::vector;

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CBountyHuntActivationChecker {
    void*                 __vftable;               // +0x00
    CGameScriptInterface* Game;                    // +0x04
    CScriptThing          Me;                      // +0x08
    void*                 ParentClass;             // +0x14 CV_FishingCompetitionScript*
    void*                 PMasterData;             // +0x18
    void RemoveFishermen();
};

void CBountyHuntActivationChecker::RemoveFishermen()
{
    if (Game->GetThingWithScriptName("FishCompOrganiser").IsAlive())
        Game->RemoveThing(Game->GetThingWithScriptName("FishCompOrganiser"), false, true);
    vector<CScriptThing> competitors;
    Game->GetAllThingsWithScriptName("FishCompCompetitor", competitors);
    long count = competitors.size();
    if (count != 0) {
        for (long i = 0; i < count; ++i) {
            if (competitors[i].IsAlive())
                Game->RemoveThing(competitors[i], false, true);
        }
    }
}