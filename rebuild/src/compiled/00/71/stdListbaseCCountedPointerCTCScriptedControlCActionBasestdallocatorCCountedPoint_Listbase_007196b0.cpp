// std::_List_base<CCountedPointer<CTCScriptedControl::CActionBase> >::~_List_base() -- STLport 4.6: clear() the action list,
//   then free the sentinel node (the out-of-line COMDAT of the pair ~CTCScriptedControl 0x00719C70 expands at +0x14)
// Leaf: ??1?$_List_base@V?$CCountedPointer@VCActionBase@CTCScriptedControl@@@@V?$allocator@V?$CCountedPointer@VCActionBase@CTCScriptedControl@@@@@std@@@std@@QAE@XZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 27 (family c695, chunk 695 queue): body = _List_base::clear 0x00719280 (the c695 spelling
//   ?clear@?$_List_base@V?$CCountedPointer@VCActionBase@CTCScriptedControl@@@@V?$allocator@..@std@@@std@@QAEXXZ) then free(_M_node) =
//   STLport's ~_List_base; no caller (an unreferenced COMDAT). The spelling is the compiler's for this text (the queue's hand-mangled
//   claim confirmed). Was a free __fastcall CActiveFile_OnReadFinished stand-in (manifest "CActiveFile::OnReadFinished").
// Retail: Fable.exe @ 0x007196B0: push esi; mov esi,ecx; call 0x00719280; mov esi,[esi]; test esi,esi; je; push esi;
//   call free 0x00BFEA14; add esp,4; pop esi; ret.
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

#include "engine/CCountedPointer.h"
class CTCScriptedControl { public:
    class CActionBase;                               // nested (Ego_r.pdb CTCScriptedControl::CActionBase)
};

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp, class _Alloc> class _List_base {
protected:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
public:
    ~_List_base();
    void clear();                                    // 0x00719280 (_list.c, out of line)
};
template <class _Tp, class _Alloc>
_List_base<_Tp, _Alloc>::~_List_base()
{
    clear();
    _M_node.deallocate(_M_node._M_data, 1);
}
template _List_base<CCountedPointer<CTCScriptedControl::CActionBase>, allocator<CCountedPointer<CTCScriptedControl::CActionBase> > >::~_List_base();
}
