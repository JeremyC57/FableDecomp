// NScript::CV_BodyGuardScript::CHenchmanRegionExitComment::CHenchmanRegionExitComment(CV_BodyGuardScript& parent, CScriptDataBase* data, const CScriptThing& me) -- entity-script constructor: CEntityScriptBase(*parent.Game, me), ParentClass = &parent, an empty HenchmanVector, then PMasterData = data
// Leaf: ??0CHenchmanRegionExitComment@CV_BodyGuardScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E5F880, 67 bytes, thiscall, `ret 0Ch`, returns this.
// Identity (probable): stores the entity vtable 0x012E7100 (after CEntityScriptBase's 0x012C3224); called only from the class's
//   Alloc (call at 0x00E5F821); third of the three henchman classes (vtable Main 0x00E5F8F0, the region-comment twin of 0x00E5F6B0). FableWin.pdb ??0CHenchmanRegionExitComment@CV_BodyGuardScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z.
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
class CV_BodyGuardScript : public CLevelScriptBase {
public:
    class CHenchmanRegionExitComment : public CEntityScriptBase {
    public:
        CHenchmanRegionExitComment(CV_BodyGuardScript& parent, CScriptDataBase* data, const CScriptThing& me);
        CV_BodyGuardScript* ParentClass;                             // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        std::vector<CScriptThing> HenchmanVector;                       // +0x1C (STLport, 12 bytes)
    };
};
CV_BodyGuardScript::CHenchmanRegionExitComment::CHenchmanRegionExitComment(CV_BodyGuardScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
}