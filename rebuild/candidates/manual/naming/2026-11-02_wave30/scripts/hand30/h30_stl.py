# h30_stl.py: naming wave 30, family stl (scope 1, queue rows 14 / 48 / 69): three byte-identical own rows landed as the free __fastcall stand-in
#  ?SortTreeRecursively_00436900@@YIXPAUTreeList@@@Z (catalog CGuiControlTreePane::SortTreeRecursively, propagated across identical bodies) are
#  STLport 4.6 std::_List_base<T, allocator<T> >::~_List_base() (clear() expanded: free each node, re-link the sentinel, free the sentinel),
#  each the out-of-line COMDAT its owner's destructor calls (no ICF in retail: one copy per T):
#   0x0052D5C0 T = NUISystem::CChangingStateComponent::EChangingStateComponentState (~CChangingStateComponent 0x0052CCF0, StatesToDo +0x13C)
#   0x00B43BB0 T = CEngineMap* (~CEngineWorldMap 0x00B42E70, VisibleMapList +0x2C; its landed TU emits the same COMDAT)
#   0x00B09E40 T = CEnginePrimitiveSpriteTrail::CSpriteTrailEntry (~CPSCSingleSprite 0x00B0A160, TrailUpdatePrimitive.Trail; same)
#  Model: the landed 0x00B42E70 source's STLport list (Ego_r.pdb member types: list<CEngineMap*>, list<CSpriteTrailEntry>,
#  list<EChangingStateComponentState>).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
STL = '''extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
    ~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }
    void clear();
protected:
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;
};

template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}

}   // namespace std
'''
def row(a, title, leaf, owner, types, T, was='?SortTreeRecursively_00436900@@YIXPAUTreeList@@@Z'):
    he.put(a, 'src', '''// std::_List_base<%s, std::allocator<..> >::~_List_base() -- STLport 4.6: free every node (clear() expanded), re-link the sentinel to itself,
//   free the sentinel
// Leaf: %s
// Naming wave 30 (family stl): was the free __fastcall stand-in %s (catalog
//   "CGuiControlTreePane::SortTreeRecursively", propagated across the byte-identical list destructors 0x0052D5C0 / 0x00B09E40 / 0x00B43BB0).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x%s, thiscall, plain `ret`; the out-of-line COMDAT %s.
// Shape: the node loop (`mov eax,esi; mov esi,[esi]; push eax; call free`) compares against the reloaded sentinel; the element destructor
//   is trivial; the sentinel's two self-links are stored through the reloaded pointer, then the sentinel is freed when non-null
//   (allocator::deallocate). Model: the landed 0x00B42E70 source's STLport list. DestroyOne (a destruction by name, ungraded) makes VC7.1
//   emit the out-of-line `??1` (the chunk 656/657 idiom).
%s
%s
// Emission helper: destroying the base by name is what makes VC7.1 emit the out-of-line COMDAT.
void DestroyOne(std::_List_base<%s, std::allocator<%s > >* list)
{
    list->~_List_base();
}''' % (title, leaf, was, a.upper(), owner, STL, types, T, T))
row('0052d5c0', 'NUISystem::CChangingStateComponent::EChangingStateComponentState',
    '??1?$_List_base@W4EChangingStateComponentState@CChangingStateComponent@NUISystem@@V?$allocator@W4EChangingStateComponentState@CChangingStateComponent@NUISystem@@@std@@@std@@QAE@XZ',
    'called by ~CChangingStateComponent 0x0052CCF0 on StatesToDo (+0x13C, Ego_r.pdb list<EChangingStateComponentState>)',
    'namespace NUISystem {\nclass CChangingStateComponent {                             // only the nested enum (its enumerators are not used here)\npublic:\n    enum EChangingStateComponentState { };\n};\n}   // namespace NUISystem\n',
    'NUISystem::CChangingStateComponent::EChangingStateComponentState')
row('00b43bb0', 'CEngineMap*',
    '??1?$_List_base@PAVCEngineMap@@V?$allocator@PAVCEngineMap@@@std@@@std@@QAE@XZ',
    'called by ~CEngineWorldMap 0x00B42E70 on VisibleMapList (+0x2C, Ego_r.pdb list<CEngineMap*>; that landed TU emits the same COMDAT)',
    'class CEngineMap;\n', 'CEngineMap*')
row('00b09e40', 'CEnginePrimitiveSpriteTrail::CSpriteTrailEntry',
    '??1?$_List_base@VCSpriteTrailEntry@CEnginePrimitiveSpriteTrail@@V?$allocator@VCSpriteTrailEntry@CEnginePrimitiveSpriteTrail@@@std@@@std@@QAE@XZ',
    'called by ~CPSCSingleSprite 0x00B0A160 on TrailUpdatePrimitive.Trail (+0xB8, Ego_r.pdb list<CSpriteTrailEntry>; that landed TU emits the same COMDAT)',
    'class C3DVector { public: float X, Y, Z; };\nclass CEnginePrimitiveSpriteTrail {                         // only the nested entry (Ego_r.pdb: Position, Width)\npublic:\n    class CSpriteTrailEntry {\n    public:\n        C3DVector Position;\n        float     Width;\n    };\n};\n',
    'CEnginePrimitiveSpriteTrail::CSpriteTrailEntry')
# 0x0052D770 (queue row 14 binder): the ??_G calls the StatesToDo destructor 0x0052D5C0 under the invented ??1CStateList@NUISystem@@QAE@XZ;
#  StatesToDo is the one-word STLport std::list<EChangingStateComponentState> of the landed own row 0x0052CCF0 (its model: the out-of-line
#  ~_List_base), so the call binds ??1?$_List_base@W4EChangingStateComponentState@CChangingStateComponent@NUISystem@@...@std@@QAE@XZ.
a = '0052d770'
he.edit(a, 'src', '''namespace NUISystem {

struct CComponentRoot {''', '''namespace std {                                          // STLport 4.6 list as the landed own row 0x0052CCF0 models it
template <class _Tp> class allocator { public: };
template <class _Tp> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    ~_List_base();                                       // 0x0052D5C0 for EChangingStateComponentState (out of line)
    _List_node<_Tp>* _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
};
}
namespace NUISystem {

struct CComponentRoot {''')
he.edit(a, 'src', '''struct CStateList {                                      // list<EChangingStateComponentState>
    void* m_pHead;
    void* m_size;
    CStateList();
    ~CStateList();                                       // 0x0052D5C0
};

''', '')
he.edit(a, 'src', '''class CChangingStateComponent : public CComponent { public:
    long       StatesBeingDone;                          // +0x134
    long       StatesDone;                               // +0x138
    CStateList StatesToDo;                               // +0x13c''', '''class CChangingStateComponent : public CComponent { public:
    enum EChangingStateComponentState { };
    EChangingStateComponentState StatesBeingDone;        // +0x134
    EChangingStateComponentState StatesDone;             // +0x138
    std::list<EChangingStateComponentState> StatesToDo;  // +0x13c (one-word STLport list)''')
from common import note
note(a, 'stl', 'StatesToDo is the one-word STLport std::list<EChangingStateComponentState> (Ego_r.pdb; the landed own row 0x0052CCF0 model):\n'
     '//   its out-of-line destructor 0x0052D5C0 is ??1?$_List_base@W4EChangingStateComponentState@CChangingStateComponent@NUISystem@@...\n'
     '//   (was the invented ??1CStateList@NUISystem@@QAE@XZ); StatesBeingDone / StatesDone take the PDB enum type.')
