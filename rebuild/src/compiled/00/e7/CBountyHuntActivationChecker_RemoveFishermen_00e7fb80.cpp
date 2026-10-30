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
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
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