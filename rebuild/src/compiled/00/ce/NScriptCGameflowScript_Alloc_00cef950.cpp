// NScript::CGameflowScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- script factory of the "Gameflow" quest: `new CGameflowScript(data, game)` with the constructor inlined
// Leaf: ?Alloc@CGameflowScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CEF950, 77 bytes, static __fastcall (data in ecx, game in edx), plain
//   `ret`. It is the pAllocFunc of the "Gameflow" CScriptInfo record (section "S_GF") stored by 0x00CD55AA; the quest
//   registry names it NScript::CGameflowScript::Alloc.
// Callees: scalar operator new @ 0x00BFEA1A (0x64 bytes), CScriptBase::CScriptBase() @ 0x00CB8110. Vtable 0x012C3FA4.
// Inline constructor: CLevelScriptBase<T> (PDB: CScriptBase + { Game; MasterScriptData; }, a novtable intermediate base)
//   stores Game +0x40 and the data +0x44 before the class's own vtable store, then the two empty STLport vectors
//   SavedScriptNames (+0x4C) and SavedCardDefNames (+0x58). CoreQuestWaiting (+0x48) stays uninitialised.
// Layout: Ego_r.pdb NScript::CGameflowScript (128 bytes) with the retail 0x40-byte script base and 12-byte STLport
//   vectors (PDB 16): retail sizeof 0x64 (`push 64h`).
// Shape: speed TU (no pragma); the landed CQ_HeroSoulsArenaScript::Alloc (0x00D80BA0) model. With six zero stores VC
//   keeps 0 in edi and tests the `new` result with `cmp esi,edi`.
class CGameScriptInterfaceBase;
class CScriptDataBase;
class CCharString;
class CScriptBase { public:                                    // 0x40 bytes, the quest-script root
    virtual ~CScriptBase();
    unsigned char _body[0x3c];                          // +0x04..+0x3f
    CScriptBase();                                      // 0x00CB8110
};
template <class TData>
struct __declspec(novtable) CLevelScriptBase : CScriptBase {
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData*                    MasterScriptData;         // +0x44
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CScriptBase(), Game(&game), MasterScriptData((TData*)data) { }
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
namespace NScript {
class CQ_SunnyvaleMasterData;
class CGameflowScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    unsigned long CoreQuestWaiting;                     // +0x48
    vector<CCharString> SavedScriptNames;               // +0x4c
    vector<CCharString> SavedCardDefNames;              // +0x58
    CGameflowScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CGameflowScript();
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
CScriptBase* __fastcall CGameflowScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CGameflowScript(data, game);
}
}