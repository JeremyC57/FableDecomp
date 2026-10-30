// NScript::CV_ChickenKickingScript::CChickenSign::CChickenSign(CV_ChickenKickingScript& parent, CScriptDataBase* data, const CScriptThing& me) -- entity-script constructor: CEntityScriptBase(*parent.Game, me), ParentClass = &parent, an empty TextKeys, then PMasterData = data
// Leaf: ??0CChickenSign@CV_ChickenKickingScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E64D90, 67 bytes, thiscall, `ret 0Ch`, returns this.
// Identity (probable): stores the entity vtable 0x012E7648 (after CEntityScriptBase's 0x012C3224); called only from the class's
//   Alloc (call at 0x00E64D31); the factory call at 0x00E64D31 sits in the CV_ChickenKickingScript code (landed CChickenMaster::OnPersist 0x00E641B0 next door); Ego_r.pdb CChickenSign is that script's entity class with a vector (TextKeys, vector<CWideString>) after PMasterData. FableWin.pdb ??0CChickenSign@CV_ChickenKickingScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 (Me).
// Shape (family K068, representative 0x00D73220): the PDB constructor: base and ParentClass in the init list, the STLport
//   vector's inline default constructor (three zero stores after the vtable store), PMasterData assigned in the body.
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CWideString;
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
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
class CV_ChickenKickingScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    class CChickenSign : public CEntityScriptBase {
    public:
        CChickenSign(CV_ChickenKickingScript& parent, CScriptDataBase* data, const CScriptThing& me);
        CV_ChickenKickingScript* ParentClass;                             // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        std::vector<CWideString> TextKeys;                       // +0x1C (STLport, 12 bytes)
    };
};
CV_ChickenKickingScript::CChickenSign::CChickenSign(CV_ChickenKickingScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
}