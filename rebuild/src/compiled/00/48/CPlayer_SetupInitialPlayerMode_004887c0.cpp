// CPlayer::SetupInitialPlayerMode() -- add the player's initial mode (1 for the main player, 0x18 otherwise), keeping a front-end mode 0x1b on top if one was active, then make this player the master of every co-op spirit in the world
// Leaf: ?SetupInitialPlayerMode@CPlayer_Methods@@QAEXXZ
// (retail ?SetupInitialPlayerMode@CPlayer@@QAEXXZ; defined through the CPlayer_Methods carrier)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004887C0, length 251 bytes, plain `ret`.
// Callees: CPlayerManager::GetMainPlayerID 0x00449700 (returns +0x1c; propagated label "CWorld::DrawGetEnvironment"),
// CPlayer::GetCurrentMode 0x00633BE0, RemoveMode(EPlayerMode) 0x00634190, AddMode(EPlayerMode, bool) 0x006345C0,
// CThingSearchTools::GetAllThings<...> 0x0048D400 (World +0x20; the interface type 0xc7 by reference), CVectorMap<
// ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020, CPlayerManager::GetMainPlayer 0x00449970, CTCCoopSpirit::SetMaster
// 0x0066FA50 (stores +0x24; propagated label "CObservable::AddExclusiveObserver"), free 0x00BFEA14 (the vector).
// Layout: engine/CPlayer.h (PlayerManager +0x10, World +0x1C, Number +0x28) through the CPlayer_Methods carrier; CThing
// TC mask +0x20 (0xc7 = word 6 bit 7, the sign bit of byte +0x38), Components +0x44.
// Shape: `long number = Number;` read before the call and `(number != id) ? 0x18 : 1` for the sete/dec/and/inc select; a local
// STLport vector<CThing*> (zeros through ebx) with the count cached before the loop; GetAllThings has a same-TU stand-in
// that reads and rewrites the vector words by value, so the vector does not escape (start kept in edi, zero stores live); the spirit is the uninitialised
// `T* tc; if (HasTC) tc = GetTC;` local (stale value = the search key's frame word).
#include "engine/CPlayer.h"

typedef long EPlayerMode;
extern "C" void free(void* p);                         // 0x00BFEA14 (MSVCR71)

struct CTCBase;
struct CTCEntry {
    long     Type;
    CTCBase* TC;
};
struct CTCMap {
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const unsigned long& type);   // 0x0040F020
    __forceinline CTCBase* Find(long type)
    {
        CTCEntry* it = LowerBound((unsigned long)type);
        if (it == _M_finish || type < it->Type)
            it = _M_finish;
        return it->TC;
    }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[8];                  // +0x20
    unsigned char _pad40[4];
    CTCMap        Components;                          // +0x44
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type) { return Components.Find(type); }
    template <class T> __forceinline bool GetInterface(T** ppOut, long type)
    {
        if (HasTC(type)) {
            *ppOut = (T*)GetTC(type);
            return true;
        }
        return false;
    }
};

template <class T> struct vector {                     // STLport vector (12 bytes), malloc_alloc storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()
    {
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) { return *(_M_start + n); }
};

struct CThingSearchTools {
    void GetAllThings(const long& interfaceType, vector<CThing*>& things);   // 0x0048D400
};
CThing** SearchFirst_0048d400();
bool SearchThings_0048d400(CThingSearchTools* tools, const long* interfaceType, CThing** first, CThing** last,
                           CThing** end);
CThing** SearchLast_0048d400();
CThing** SearchEnd_0048d400();
// Same-TU stand-in for the CThingSearchTools::GetAllThings instantiation 0x0048D400 (not a reconstruction of it): the
// interface type goes by address, but the result vector's three words are rewritten by value, so VC7.1 knows the vector
// does not escape and keeps its start in a register (the 0x00726BE0 idiom); the words are read first, so the vector's
// zero stores stay live.
#pragma auto_inline(off)
__declspec(noinline) void CThingSearchTools::GetAllThings(const long& interfaceType, vector<CThing*>& things)
{
    SearchThings_0048d400(this, &interfaceType, things._M_start, things._M_finish, things._M_end_of_storage);
    things._M_start = SearchFirst_0048d400();
    things._M_finish = SearchLast_0048d400();
    things._M_end_of_storage = SearchEnd_0048d400();
}
#pragma auto_inline(on)
struct CWorld_Retail {
    unsigned char      _pad00[0x20];
    CThingSearchTools* PThingSearchTools;              // +0x20
};
struct CPlayerManager_Retail {
    long GetMainPlayerID() const;                      // 0x00449700
    CPlayer* GetMainPlayer();                          // 0x00449970
};
struct CTCCoopSpirit {                                 // TC type 0xc7
    void SetMaster(CPlayer* master);                   // 0x0066FA50
};

struct CPlayer_Methods : CPlayer {
    EPlayerMode GetCurrentMode();                      // 0x00633BE0
    void RemoveMode(EPlayerMode mode);                 // 0x00634190
    void AddMode(EPlayerMode mode, bool b);            // 0x006345C0
    void SetupInitialPlayerMode();
};

void CPlayer_Methods::SetupInitialPlayerMode()
{
    long number = Number;
    EPlayerMode mode = (number != ((CPlayerManager_Retail*)PlayerManager)->GetMainPlayerID()) ? 0x18 : 1;
    bool frontEnd = false;
    if (GetCurrentMode() == 0x1b) {
        frontEnd = true;
        RemoveMode(0x1b);
    }
    AddMode(mode, true);
    if (frontEnd)
        AddMode(0x1b, false);
    vector<CThing*> spirits;
    ((CWorld_Retail*)World)->PThingSearchTools->GetAllThings(0xc7, spirits);
    unsigned long count = spirits.size();
    for (unsigned long i = 0; i < count; ++i) {
        CTCCoopSpirit* spirit;
        spirits[i]->GetInterface(&spirit, 0xc7);
        spirit->SetMaster(((CPlayerManager_Retail*)PlayerManager)->GetMainPlayer());
    }
}