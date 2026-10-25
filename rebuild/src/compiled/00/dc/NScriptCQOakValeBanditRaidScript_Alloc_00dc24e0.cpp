// NScript::CQ_OakValeBanditRaidScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- script factory of the Q_OakValeBanditRaid quest: `new CQ_OakValeBanditRaidScript(data, game)` with the constructor inlined
// Leaf: ?Alloc@CQ_OakValeBanditRaidScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DC24E0, 90 bytes, static __fastcall (data in ecx, game in edx), plain
//   `ret`. It is the pAllocFunc of the "Q_OakValeBanditRaid" CScriptInfo record (section "S_QOVBR") stored by 0x00CD6EEE;
//   the quest registry names it NScript::CQ_OakValeBanditRaidScript::Alloc.
// Callees: scalar operator new @ 0x00BFEA1A (0x78 bytes), CScriptBase::CScriptBase() @ 0x00CB8110, CBaseClass::CBaseClass()
//   @ 0x0099A2D0 (BanditInRange's root). Vtables: the script 0x012D9E18, CScriptThing 0x01238C8C.
// Inline constructor: CLevelScriptBase<T> (a novtable intermediate base) stores Game +0x40 and the data +0x44 before the
//   class's own vtable store, then the empty HerosFollowers vector (+0x4C..+0x54) and the default CScriptThing
//   BanditInRange (+0x64: root ctor, vtable, PImp {0, 0}). The bools/counters stay uninitialised.
// Layout: Ego_r.pdb NScript::CQ_OakValeBanditRaidScript (144 bytes) with the retail 0x40-byte script base (PDB offsets
//   minus 0x14) and a 12-byte STLport vector (PDB 16): retail sizeof 0x78 (`push 78h`).
// Shape: speed TU (no pragma); the landed CQ_HeroSoulsArenaScript::Alloc (0x00D80BA0) model. Five zero stores: VC keeps 0
//   in ebx and tests the `new` result with `cmp esi,ebx`; the address of BanditInRange is formed in edi (the dead
//   game register) ahead of the vector stores.
class CGameScriptInterfaceBase;
class CScriptDataBase;
class CBaseClass {                                      // polymorphic root of the script handles
public:
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();
};
template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
class CScriptThing : public CBaseClass {               // 12 bytes, vtable 0x01238C8C
public:
    CScriptThing() { }
    virtual ~CScriptThing();
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
struct CScriptBase {                                    // 0x40 bytes, the quest-script root
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
class CQ_OakValeBanditRaidScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool BanditsAwareOfPlayer;                          // +0x48
    bool BanditsAwareOfPlayerAgain;
    bool GateClosed;                                    // +0x4a
    vector<CScriptThing> HerosFollowers;                // +0x4c
    bool OpenGates;                                     // +0x58
    bool FirstSetup;
    long SoundPlayed;                                   // +0x5c
    bool StopWhistling;                                 // +0x60
    bool BanditsKilled;
    bool IsBanditInRange;
    CScriptThing BanditInRange;                         // +0x64
    long BanditsStopChasing;                            // +0x70
    bool NoPersistence;                                 // +0x74
    CQ_OakValeBanditRaidScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQ_OakValeBanditRaidScript();
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
CScriptBase* __fastcall CQ_OakValeBanditRaidScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_OakValeBanditRaidScript(data, game);
}
}