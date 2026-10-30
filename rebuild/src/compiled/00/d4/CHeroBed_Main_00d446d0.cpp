// CHeroBed::Main() -- the hero's bed: make Me unusable and persistent, then do the same for every guild floor pallet, apprentice bed and Bowerstone slum brown bed in the level
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D446D0, 1415 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: IsActiveThreadTerminating 0x00F35B30, free 0x00BFEA14 (inline vector destructor copies), CCharString 0x0099EBF0 /
//   0x0099EAE0; CScriptThing slot 0 (virtual destructor); GameCalls slots 80 GetAllThingsWithDefName, 447 SetThingAsUsable,
//   477 SetThingPersistent.
// Layout: Ego_r.pdb CHeroBed: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18.
// Shape: vector model of the landed CQ_HangingTreeGoodScript::WatchForHeroNearExit 0x00D76490 (`__forceinline` ~vector).
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo;
class CScriptThing { public:                                   // vtable 0x01238C8C, 12 bytes
    virtual ~CScriptThing();                            // slot 0; complete destructor 0x004AA840
    void*           PImp_Data;                          // +0x04
    CCPPointerInfo* PImp_Info;                          // +0x08
};
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
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
struct CHeroBed : CEntityScriptBase {
    void* ParentClass;                                  // +0x14
    void* PMasterData;                                  // +0x18
    void Main();
};

void CHeroBed::Main()
{
    Game->SetThingAsUsable(Me, false);
    Game->SetThingPersistent(Me, true);
    std::vector<CScriptThing> pallets;
    std::vector<CScriptThing> slumBeds;
    std::vector<CScriptThing> apprenticeBeds;
    long count = Game->GetAllThingsWithDefName("OBJECT_GUILD_BED_FLOOR_PALLET_01", pallets);
    for (long i = 0; i < count; ++i) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetThingAsUsable(pallets[i], false);
        Game->SetThingPersistent(pallets[i], true);
    }
    if (IsActiveThreadTerminating())
        return;
    count = Game->GetAllThingsWithDefName("OBJECT_GUILD_BED_APPRENTICE_01", apprenticeBeds);
    for (long j = 0; j < count; ++j) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetThingAsUsable(apprenticeBeds[j], false);
        Game->SetThingPersistent(apprenticeBeds[j], true);
    }
    if (IsActiveThreadTerminating())
        return;
    count = Game->GetAllThingsWithDefName("OBJECT_BS_SLUM_BED_BROWN_01", slumBeds);
    for (long k = 0; k < count; ++k) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetThingAsUsable(slumBeds[k], false);
        Game->SetThingPersistent(slumBeds[k], true);
    }
    if (IsActiveThreadTerminating())
        return;
}