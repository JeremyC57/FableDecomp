# h26_rbt.py: naming wave 26, family rbt -- 0x007AD180 is STLport 4.6 _Rb_tree<CCharString, pair<const CCharString, bool>, ...>::erase(iterator,
#  iterator) (the spelling map::erase 0x007ADE60's binding names): the own row is re-authored on the STLport text of the landed
#  _M_erase row 0x007ACC50 (which already carries the map<CCharString, bool> _Rb_tree model), so its _M_erase call binds the PDB spelling
#  (c689 queue row) and its rebalance call ?_Rebalance_for_erase@?$_Rb_global@_N@std@@... (0x0042A34E). Was CStringBoolTree::erase(SNode*, SNode*).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he, rows
R = rows.rows()
base = open(os.path.join(rows.ROOT, 'rebuild/src/compiled', R['007acc50']['Source']), encoding='utf-8').read()
i = base.index('extern "C" void* __cdecl malloc')
body = base[i:]
d0 = body.index('template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>\nvoid _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(')
body = body[:d0] + '''typedef pair<const CCharString, bool> _ErasedPair;
typedef _Rb_tree<CCharString, _ErasedPair, _Select1st<_ErasedPair>, less<CCharString >, allocator<_ErasedPair> > _ErasedTree;
template void _ErasedTree::erase(_ErasedTree::iterator, _ErasedTree::iterator);
}
'''
HEAD = '''// std::_Rb_tree<CCharString, std::pair<const CCharString, bool>, std::_Select1st<...>, std::less<CCharString>, std::allocator<...> >::erase(iterator first,
//   iterator last) -- STLport 4.6 range erase of a map<CCharString, bool>: [begin, end) clears the whole tree (_M_erase from the root,
//   header reset); any other range unlinks node by node (_Rebalance_for_erase, ~CCharString of the key, free), the successor inline
// Leaf: ?erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@_N@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@_N@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@_N@std@@@3@@std@@QAEXU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@_N@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@_N@std@@@2@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AD180, 196 bytes, thiscall, `ret 8`.
// Identity: the spelling map<CCharString, bool>::erase(first, last) 0x007ADE60 binds here (reloc_bindings); the only caller.
// Callees: _Rb_tree::_M_erase(_Link_type) 0x007ACC50 (private, Ego_r.pdb; c689 queue row), _Rb_global<bool>::_Rebalance_for_erase 0x0042A34E,
//   ~CCharString 0x0099EAE0, free 0x00BFEA14.
// Naming wave 26 (family rbt): re-authored on the STLport model of the landed _M_erase row 0x007ACC50 (stl/_tree.h + stl/_tree.c text,
//   explicit instantiation of the range erase; _M_erase declared, defined in its own row). Was CStringBoolTree::erase(SNode*, SNode*) with
//   ?_M_erase@CStringBoolTree@@QAEXPAUSNode@@@Z and an extern "C" _Rb_tree_rebalance_for_erase stand-in.
'''
he.put('007ad180', 'src', HEAD + body)
