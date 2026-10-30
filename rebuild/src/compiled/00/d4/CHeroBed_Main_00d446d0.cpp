// NScript::CQ_GuildTrainingScript::CHeroBed::Main() -- the hero's bed: make Me unusable and persistent, then do the same for every guild floor pallet, apprentice bed and Bowerstone slum brown bed in the level
// Leaf: Main
// Identity (naming wave 11): ?Main@CHeroBed@CQ_GuildTrainingScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012CDF60 slot 1 is stored by the factory 0x00D40B30 registered as "HeroBed"
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
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
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
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_GuildTrainingScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CHeroBed;                        // the entity class below (naming wave 11)
};
}
using NScript::CQ_GuildTrainingScript;
namespace NScript {
class CQ_GuildTrainingScript::CHeroBed : public CEntityScriptBase {
public:
    void* ParentClass;                                  // +0x14
    void* PMasterData;                                  // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CQ_GuildTrainingScript::CHeroBed::Main()
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