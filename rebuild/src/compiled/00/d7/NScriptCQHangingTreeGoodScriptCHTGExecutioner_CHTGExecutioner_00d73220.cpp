// NScript::CQ_HangingTreeGoodScript::CHTG_Executioner::CHTG_Executioner(CQ_HangingTreeGoodScript& parent, CScriptDataBase* data, const CScriptThing& me) -- entity-script constructor: CEntityScriptBase(*parent.Game, me), ParentClass = &parent, an empty AudienceMembers, then PMasterData = data
// Leaf: ??0CHTG_Executioner@CQ_HangingTreeGoodScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D73220, 67 bytes, thiscall, `ret 0Ch`, returns this.
// Identity (probable): stores the entity vtable 0x012D29B8 (after CEntityScriptBase's 0x012C3224); called only from the class's
//   Alloc (call at 0x00D731C1); the factory call at 0x00D731C1 sits in the CQ_HangingTreeGoodScript code; Ego_r.pdb CHTG_Executioner is that script's entity class with a vector (AudienceMembers, vector<CScriptThing>) after PMasterData. FableWin.pdb ??0CHTG_Executioner@CQ_HangingTreeGoodScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 (Me).
// Shape (family K068, representative 0x00D73220): the PDB constructor: base and ParentClass in the init list, the STLport
//   vector's inline default constructor (three zero stores after the vtable store), PMasterData assigned in the body.
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CWideString;
class CScriptThing {                                    // 12 bytes (Ego_r): vptr, PImp
public:
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    virtual ~CScriptThing();
    void* PImpData;                                     // +0x04
    void* PImpInfo;                                     // +0x08
};
class CEntityScriptBase {                               // 20 bytes (Ego_r), vtable 0x012C3224
public:
    CEntityScriptBase(CGameScriptInterfaceBase& game, const CScriptThing& me) : Game(&game), Me(me) { }
    virtual ~CEntityScriptBase();
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing Me;                                    // +0x08
};
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
    unsigned char _body[0x3c];
};
class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    CScriptDataBase* MasterScriptData;                  // +0x44
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
};
}
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HangingTreeGoodScript : public CLevelScriptBase {
public:
    class CHTG_Executioner : public CEntityScriptBase {
    public:
        CHTG_Executioner(CQ_HangingTreeGoodScript& parent, CScriptDataBase* data, const CScriptThing& me);
        CQ_HangingTreeGoodScript* ParentClass;                             // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        std::vector<CScriptThing> AudienceMembers;                       // +0x1C (STLport, 12 bytes)
    };
};
CQ_HangingTreeGoodScript::CHTG_Executioner::CHTG_Executioner(CQ_HangingTreeGoodScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
}