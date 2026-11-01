# h27_c695.py: naming wave 27, family c695 (the chunk 695 queue rows' binders) -- callers respelled to the spellings chunk 695 proved:
#  0x00854380 ??1CScriptModuleFOVZoom@NCameraModes@@UAE@XZ (binder 0x008544C0, the StayRelative ??_G stand-in: RTTI StayRelative ->
#  FOVZoom -> CScriptModuleBase), 0x005BC527 ?AddCloseInventoryEvent@CTCInventoryBase@NInventory@@UAEXXZ (binder 0x00584A73, whose own
#  Leaf is the Ego_r / FableWin virtual ?AddCloseInventoryEvent@CTCInventory@@UAEXXZ).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
def note(a, txt, leaf=None):
    p, rel, t = he._rw(a, 'src')
    if 'family c695' in t: return
    L = t.split('\n'); i = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')]
    if leaf:
        if i: L[i[0]] = '// Leaf: ' + leaf
        else: L.insert(1 if L[0].startswith('//') else 0, '// Leaf: ' + leaf); i = [1 if L[0].startswith('//') and not L[0].startswith('// Leaf') else 0]
    i = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')]
    L.insert((i[0] + 1) if i else 1, '// Naming wave 27 (family c695): ' + txt); he._w(p, '\n'.join(L))
a = '008544c0'
he.edit(a, 'src', '''extern void __fastcall CScriptModuleFOVZoomStayRelative_Destruct(void* relative);
''', '''namespace NCameraModes {
class CScriptModuleBase { public:                     // vtable 0x01255F74 (.?AVCScriptModuleBase@NCameraModes@@)
    virtual ~CScriptModuleBase();                     // 0x005ED120
};
class CScriptModuleFOVZoom : public CScriptModuleBase { public:   // RTTI: the direct base of CScriptModuleFOVZoomStayRelative
    virtual ~CScriptModuleFOVZoom();                  // 0x00854380 (chunk 695)
};
}
''')
he.edit(a, 'src', '''    CScriptModuleFOVZoomStayRelative_Destruct(relative);''',
        '''    static_cast<NCameraModes::CScriptModuleFOVZoom*>(relative)->NCameraModes::CScriptModuleFOVZoom::~CScriptModuleFOVZoom();''')
note(a, 'the base destructor 0x00854380 is ??1CScriptModuleFOVZoom@NCameraModes@@UAE@XZ (Ego_r.pdb\n//   name set; RTTI StayRelative -> FOVZoom -> CScriptModuleBase; chunk 695), called qualified. Was a free\n//   ?CScriptModuleFOVZoomStayRelative_Destruct@@YIXPAX@Z.',
     '?CScriptModuleFOVZoomStayRelative_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z')
a = '00584a73'
he.edit(a, 'src', '''struct CTCInventoryBase {
    void*   __vftable;                               // +0x00
    CThing* ParentThing;                             // +0x04 CTCBase
    void AddCloseInventoryEvent();                   // 0x005BC527
};

class CTCInventory : public CTCInventoryBase { public:
    void AddCloseInventoryEvent();
};''', '''namespace NInventory {
class CTCInventoryBase { public:                     // vtable 0x01253DBC (vptr +0x00)
    CThing* ParentThing;                             // +0x04 CTCBase
    virtual void AddCloseInventoryEvent();           // slot 32  0x005BC527
};
}
using NInventory::CTCInventoryBase;

class CTCInventory : public CTCInventoryBase { public:
    virtual void AddCloseInventoryEvent();           // Ego_r.pdb / FableWin.pdb UAE
};''')
note(a, 'NInventory::CTCInventoryBase::AddCloseInventoryEvent 0x005BC527 is the virtual\n//   ?AddCloseInventoryEvent@CTCInventoryBase@NInventory@@UAEXXZ (Ego_r.pdb + FableWin.pdb; chunk 695), called qualified, and the\n//   override is ?AddCloseInventoryEvent@CTCInventory@@UAEXXZ. Was the global CTCInventoryBase with non-virtual members (QAE).',
     '?AddCloseInventoryEvent@CTCInventory@@UAEXXZ')

# 0x00A0D780 ?RemoveInputProcess@CInputProcessManager@@IAEXAAVCAInputProcess@@@Z (Ego_r.pdb: protected, by reference; the only
#  spelling; chunk 695) -- binder 0x00A0D410 (the CAInputProcess ??_G with ~CAInputProcess inline). Retail calls the protected member
#  from CAInputProcess, so retail's CInputProcessManager declared CAInputProcess a friend.
a = '00a0d410'
he.edit(a, 'src', '''class CInputProcessManager { public:
    void RemoveInputProcess(CAInputProcess* process);    // 0x00A0D780
};''', '''class CInputProcessManager {
protected:                                               // Ego_r.pdb IAE (protected, by reference)
    void RemoveInputProcess(CAInputProcess& process);    // 0x00A0D780
    friend class CAInputProcess;                         // retail's ~CAInputProcess / RemoveFromInterface call it
};''')
he.edit(a, 'src', '            PPlayerInterface->RemoveInputProcess(this);', '            PPlayerInterface->RemoveInputProcess(*this);')
note(a, 'CInputProcessManager::RemoveInputProcess 0x00A0D780 is protected and takes the process by\n//   reference (Ego_r.pdb ?RemoveInputProcess@CInputProcessManager@@IAEXAAVCAInputProcess@@@Z; chunk 695); CAInputProcess is its friend.\n//   Was public by pointer (QAE ..PAV).', '??_GCAInputProcess@@UAEPAXI@Z')

# 0x007196B0 (c695 queue: identity proven, spelling hand-mangled): own row re-authored as the STLport 4.6 _List_base destructor of
#  CTCScriptedControl::PActionList -- clear() 0x00719280 then the node block freed (allocator::deallocate) -- emitted by an explicit
#  instantiation; the Leaf is the symbol the compiler emits for that text. Was a free __fastcall CActiveFile_OnReadFinished stand-in
#  calling ?CActiveFile_base_00719280@@ (manifest "CActiveFile::OnReadFinished").
he.put('007196b0', 'src', '''// std::_List_base<CCountedPointer<CTCScriptedControl::CActionBase> >::~_List_base() -- STLport 4.6: clear() the action list,
//   then free the sentinel node (the out-of-line COMDAT of the pair ~CTCScriptedControl 0x00719C70 expands at +0x14)
// Leaf: ??1?$_List_base@V?$CCountedPointer@VCActionBase@CTCScriptedControl@@@@V?$allocator@V?$CCountedPointer@VCActionBase@CTCScriptedControl@@@@@std@@@std@@QAE@XZ
// Naming wave 27 (family c695, chunk 695 queue): body = _List_base::clear 0x00719280 (the c695 spelling
//   ?clear@?$_List_base@V?$CCountedPointer@VCActionBase@CTCScriptedControl@@@@V?$allocator@..@std@@@std@@QAEXXZ) then free(_M_node) =
//   STLport's ~_List_base; no caller (an unreferenced COMDAT). The spelling is the compiler's for this text (the queue's hand-mangled
//   claim confirmed). Was a free __fastcall CActiveFile_OnReadFinished stand-in (manifest "CActiveFile::OnReadFinished").
// Retail: Fable.exe @ 0x007196B0: push esi; mov esi,ecx; call 0x00719280; mov esi,[esi]; test esi,esi; je; push esi;
//   call free 0x00BFEA14; add esp,4; pop esi; ret.
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

class CCPPointerInfo;
template <class T> class CCountedPointer { public:   // engine/CCountedPointer.h shape: Data +0, Info +4
    T*              Data;
    CCPPointerInfo* Info;
};
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
''')
