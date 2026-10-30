// NScript::CQ_UndeadRisingScript::Alloc(CScriptDataBase* data, CGameScriptInterface& game) -- script factory: `new CQ_UndeadRisingScript(data, game)` with the constructor inlined (registered at 0x00CD79B0 as "S_QUR" / "Q_UndeadRising")
// Leaf: ?Alloc@CQ_UndeadRisingScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterface@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0E760, 77 bytes, static __fastcall (data in ecx, game in edx). PDB:
//   ?Alloc@CQ_UndeadRisingScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z.
// Callees: operator new 0x00BFEA1A (0x78 bytes), CScriptBase::CScriptBase 0x00CB8110. Vtable 0x012DFD3C.
// Inline constructor: the two empty CScriptThing vectors (+0x60, +0x6C, six zero words from one zeroed register).
// Layout: PDB (Ego_r) offsets minus 0x14 (the retail script base is 0x14 bytes smaller), STLport vectors 12 bytes (PDB 16).
// Model: the landed quest-script Alloc (NScript::CQ_HeroSoulsArenaScript::Alloc 0x00D80BA0): CLevelScriptBase<T> is the
//   `__declspec(novtable)` argument-carrying base (Game +0x40, MasterScriptData +0x44, stored before the class's own vtable
//   store), the CGameScriptInterface model is the landed 88-slot one (RegisterTimer = slot 87, +0x15C).
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
class CScriptThing;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_UndeadRisingScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    unsigned long number_of_zombie_tokens_in_right_area;   // +0x48
    unsigned long number_of_boss_kills_in_right_area;      // +0x4c
    bool HeroReachedCentre;                             // +0x50
    bool BossCutscenePlayed;
    bool MissionSucceeded;
    bool MissionFailed;                                 // +0x53
    bool LightsActivated[8];                            // +0x54
    long quest_status;                                  // +0x5c (an unnamed enum in the PDB)
    vector<CScriptThing> activation_points;             // +0x60
    vector<CScriptThing> boss_points;                   // +0x6c
    CQ_UndeadRisingScript(CScriptDataBase* data, CGameScriptInterface& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQ_UndeadRisingScript();
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterface& game);
};
CScriptBase* __fastcall CQ_UndeadRisingScript::Alloc(CScriptDataBase* data, CGameScriptInterface& game)
{
    return new CQ_UndeadRisingScript(data, game);
}
}