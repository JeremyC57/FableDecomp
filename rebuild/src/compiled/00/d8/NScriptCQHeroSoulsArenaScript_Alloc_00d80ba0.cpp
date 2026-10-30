// NScript::CQ_HeroSoulsArenaScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- script factory: `new CQ_HeroSoulsArenaScript(data, game)` with the constructor inlined
// Leaf: ?Alloc@CQ_HeroSoulsArenaScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D80BA0, 92 bytes, static __fastcall (data in ecx, game in edx). PDB:
// ?Alloc@CQ_HeroSoulsArenaScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z.
// Callees: operator new 0x00BFEA1A (0x68 bytes), CScriptBase::CScriptBase 0x00CB8110, g_pGameScriptInterface (0x0143E8F8)
// ->RegisterTimer (slot 87, +0x15C) for the GuardChatTimer member. Vtable 0x012D3994.
// Inline constructor: CLevelScriptBase<T> (PDB: CScriptBase + { CGameScriptInterfaceBase* Game; T* MasterScriptData; })
// stores Game +0x40 and the data +0x44 before the class's own vtable store (a novtable intermediate base, as in the
// landed quest-script ctors, e.g. CQ_HangingTreeEvilScript 0x00D68F00), then GuardChatTimer (+0x58) and the empty
// Rounds vector (+0x5C..+0x64). CreatureCount, the seven bools and ArenaLevelReached stay uninitialised.
// Layout: PDB (Ego_r) offsets minus 0x14 (script base; CreatureCount PDB +0x5C -> retail +0x48) and a 12-byte STLport
// vector (PDB 16): retail sizeof 0x68 (`push 68h`), PDB 128.
// Shape: the Rounds vector must be the real STLport _Vector_base (_M_start, _M_finish) + _STLP_alloc_proxy end-of-storage
// built from an allocator() temporary: with a flat three-pointer vector VC pops edi before the three zero stores,
// retail pops it between the second and third (92 v 92, scheduling only).
// The interface model (88 virtual slots of vtable 0x01260F0C, simplified signatures) is the landed one.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CScriptDataBase;
class CScriptBase { public:                                    // 0x40 bytes, the quest-script root
    virtual ~CScriptBase();
    unsigned char _body[0x3c];                          // +0x04..+0x3f
    CScriptBase();                                      // 0x00CB8110
};
template <class TData>
struct __declspec(novtable) CLevelScriptBase : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterface& game) : CScriptBase(), Game(&game), MasterScriptData((TData*)data) { }
};
class CTimer { public:
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};
class CHeroSoulsRoundDef;
template <class T> struct allocator {                  // STLport allocator<T>: empty
    allocator() { }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T*                    _M_start;
    T*                    _M_finish;
    _STLP_alloc_proxy<T>  _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
};
template <class T> struct vector : _Vector_base<T> {   // STLport vector<T>, 12 bytes in retail
    vector() : _Vector_base<T>(allocator<T>()) { }
};
namespace NScript {
class CEmptyData;
class CQ_HeroSoulsArenaScript : public CLevelScriptBase<CEmptyData> {
public:
    long CreatureCount;                                 // +0x48
    bool PlayedArenaCS;                                 // +0x4c
    bool PlayerHitSummoner1;
    bool PlayerHitSummoner2;
    bool PlayerAttacksGuard;
    bool JackTauntNearHOH;
    bool FirstWaveAttack;
    bool PlayedHoHCS;                                   // +0x52
    long ArenaLevelReached;                             // +0x54
    CTimer GuardChatTimer;                              // +0x58
    vector<CHeroSoulsRoundDef> Rounds;                  // +0x5c
    CQ_HeroSoulsArenaScript(CScriptDataBase* data, CGameScriptInterface& game) : CLevelScriptBase<CEmptyData>(data, game) { }
    virtual ~CQ_HeroSoulsArenaScript();
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterface& game);
};
CScriptBase* __fastcall CQ_HeroSoulsArenaScript::Alloc(CScriptDataBase* data, CGameScriptInterface& game)
{
    return new CQ_HeroSoulsArenaScript(data, game);
}
}