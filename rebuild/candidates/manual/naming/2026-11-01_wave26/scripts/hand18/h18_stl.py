# h18_stl.py: naming wave 18, family stl -- the placeholder-class STLport own rows (wave 17 DEFERRED scope 1) re-authored on the real
# STLport 4.6 instantiation text the landed model rows carry (name-only: the recheck proves the bytes):
#   map _M_erase (base: the landed 0x009AA780 text): 007acc50 map<CCharString,bool>, 00579435 map<CCharString,unsigned long>,
#     00b2ef20 map<pair<long,long>,CEngineTextureMorph> (c678 / c681 queue rows);
#   _List_base<CCountedPointer<CCompositeSurfaceCache> >::clear 00b2ea90 (base 0x00510D40 + engine/CCountedPointer.h);
#   ~_List_base<CDeviceResetCallback*> 009c2220 (base 0x00A3FF60);
#   ~vector<pair<long,CCharString> > 004e34b2 with its destroy chain 004dd782 (__destroy_aux, __fastcall: a /Gr TU) and 004da68a
#     (the pair's ??_G, called with flag 0 by the non-inlined implicit destructor under optimize("s")) (base 0x00A40BD0);
#   vector<CTVertexWaterBackground>::resize 00b7fd00 (base: the landed erase 0x00B7FD70 text; _M_fill_insert is its own row 0x00B7FDE0).
# The Leaf is the symbol the TU defines for the graded member (read back from the object).
import os as _os
_EXCL = set(l.split()[0] for l in open(_os.path.join(_os.path.dirname(_os.path.abspath(__file__)), '..', '..', 'runs', 'excl_stl.txt')) if l.strip()) if _os.path.exists(_os.path.join(_os.path.dirname(_os.path.abspath(__file__)), '..', '..', 'runs', 'excl_stl.txt')) else set()
import os, sys, re, subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))
import he
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline')
import verify_and_land as V, coff
R = he.R
def defsyms(a, p):
    d = os.path.join(he.W, 'pw', 'h18stl'); os.makedirs(d, exist_ok=True); o = os.path.join(d, a + '.obj')
    if os.path.exists(o): os.unlink(o)
    fl = R[a].get('CompilerFlags', '').split() or ['/O2', '/Oy', '/W3']
    r = subprocess.run(['timeout', '120', V.WINE, str(V.CL_EXE)] + V.tool_args(['/nologo', '/c'] + fl + ['/Fo' + o, p]), capture_output=True, text=True, stdin=subprocess.DEVNULL, env=V.env())
    if not os.path.exists(o): raise SystemExit('compile %s: %s' % (a, [l for l in r.stdout.splitlines() if 'error' in l][:3]))
    return coff.syms(o)[0]
CATLEN = {}
for _l in open('/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv'):
    _q = _l.split('\t')
    if len(_q) >= 3: CATLEN[_q[0].lower()] = _q[2]
def base_text(a): return open(os.path.join(he.rows.ROOT, 'rebuild/src/compiled', R[a]['Source']), encoding='utf-8', newline='').read()
def set_leaf(a, p, t, prefix, pick=None):
    D = defsyms(a, p)
    cand = [s for s in D if s.startswith(prefix)]
    if pick: cand = [s for s in cand if pick(s)]
    if len(cand) != 1: raise SystemExit('leaf %s: %s' % (a, cand))
    t = t.replace('// Leaf: LEAF_PENDING', '// Leaf: ' + cand[0], 1); he._w(p, t); print('ok', a, cand[0][:150])
def done(a, marker='Naming wave 18'):
    if a in _EXCL: print('skip', a, 'runs/excl_stl.txt (DEFERRED: recheck-fail)'); return None
    p, rel, t = he._rw(a, 'src')
    if marker in t: print('already', a); return None
    return p
# ---- map _M_erase on the 0x009AA780 text
def regen_map(a, title, ident, key, mapped, prelude, old, leaf=True):
    p = done(a)
    if not p: return
    base = base_text('009aa780')
    cut = base.index('// Model: STLport 4.6')
    head = title + '\n// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %s bytes.\n' % (a.upper(), CATLEN[a]) + ident
    body = base[cut:]
    body = re.sub(r'Replaces the invented stand-in \(was [^)]*\)\.', 'Replaces the invented stand-in (was %s).' % old, body)
    body = body.replace('// Leaf: ' + re.search(r'^// Leaf: (\S+)', body, re.M).group(1), '// Leaf: LEAF_PENDING', 1)
    s = body.index('namespace NBankFileManager {'); e = body.index('namespace std {')
    body = body[:s] + prelude + body[e:]
    val = 'pair<const %s, %s>' % (key, mapped)
    body = re.sub(r'typedef pair<const CCharString, NBankFileManager::CContainedBank> _ContainedBankPair;\ntemplate void .*\n',
                  'typedef %s _ErasedPair;\ntemplate void _Rb_tree<%s, _ErasedPair, _Select1st<_ErasedPair>, less<%s >, allocator<_ErasedPair> >::_M_erase(_Rb_tree_node<_ErasedPair>*);\n' % (val, key, key), body)
    t = head + body
    he._w(p, t)
    if leaf: set_leaf(a, p, t, '?_M_erase@?$_Rb_tree@')
regen_map('007acc50', '// std::_Rb_tree<CCharString, std::pair<const CCharString, bool>, std::_Select1st<...>, std::less<CCharString>, std::allocator<...> >::_M_erase\n'
          '//   (_Rb_tree_node<...>*) -- STLport 4.6 _M_erase(_Link_type) of std::map<CCharString, bool>: erase without rebalancing -- recurse right, destroy the\n'
          '//   key string, free the node, continue left',
          '// Identity (chunk c678 queue row ?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@_N@std@@...): the map<CCharString, bool>\n'
          '//   tree erase (node 0x18 = 0x10 header + pair<const CCharString, bool>; the value\'s only destructor is the key\'s ~CCharString at +0x10);\n'
          '//   called by the landed tree destructors / clear rows 0x007AD180, 0x007ADB50, 0x007ADBF0, 0x007ADD50, 0x008A78E0, 0x008A7930.\n'
          '// Naming wave 18: landed as the invented CTreeOwner_007acc50::FreeTree_CTreeOwner_007acc50(SNode*) (child +0xC = _M_right, sibling +0x8 =\n'
          '//   _M_left, CMember_0099eae0 = CCharString); re-authored on the real STLport text (name-only: the bytes are unchanged).\n',
          'CCharString', 'bool', '', '_unattributed::FreeTree_CTreeOwner_007acc50')
regen_map('00579435', '// std::_Rb_tree<CCharString, std::pair<const CCharString, unsigned long>, std::_Select1st<...>, std::less<CCharString>, std::allocator<...> >::_M_erase\n'
          '//   (_Rb_tree_node<...>*) -- STLport 4.6 _M_erase(_Link_type) of std::map<CCharString, unsigned long>: erase without rebalancing -- recurse right,\n'
          '//   destroy the key string, free the node, continue left',
          '// Identity (chunk c678 queue row ?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@K@std@@...): the map<CCharString, unsigned long>\n'
          '//   tree erase (node 0x18 = 0x10 header + pair<const CCharString, unsigned long>; the value\'s only destructor is the key\'s ~CCharString at\n'
          '//   +0x10); called by the landed bank-file symbol / handle map rows 0x009CCC00, 0x009CD850, 0x009CEAE0, 0x009D3420, 0x009D38D0, 0x009F1560.\n'
          '// Naming wave 18: landed as the CTextTreeWalkThrough::BuildTreeArray(TreeNode*) stand-in (the propagated LTextTreeWalkThrough manifest label;\n'
          '//   SubObject::Cleanup = ~CCharString); re-authored on the real STLport text (name-only: the bytes are unchanged).\n',
          'CCharString', 'unsigned long', '', 'LTextTreeWalkThrough::BuildTreeArray')
regen_map('00b2ef20', '// std::_Rb_tree<std::pair<long, long>, std::pair<const std::pair<long, long>, CEngineTextureMorph>, std::_Select1st<...>, std::less<...>,\n'
          '//   std::allocator<...> >::_M_erase(_Rb_tree_node<...>*) -- STLport 4.6 _M_erase(_Link_type) of std::map<std::pair<long, long>,\n'
          '//   CEngineTextureMorph>: erase without rebalancing -- recurse right, destroy the morph (free its Blends vector storage), free the node, continue left',
          '// Identity (chunk c681 queue row ?_M_erase@?$_Rb_tree@U?$pair@JJ@std@@U?$pair@$$CBU?$pair@JJ@std@@VCEngineTextureMorph@@@2@...): the\n'
          '//   CEngineSurfaceCompositionManager morph map (node 0x2C = 0x10 header + pair<const pair<long, long>, CEngineTextureMorph>; Ego_r\n'
          '//   CEngineTextureMorph {OriginalTextureIndex, MorphID, vector<CCompositeBlendDesc> Blends} = 8 + the 12-byte retail vector whose\n'
          '//   ~vector frees the storage at +0x20); called by the landed tree destructor / clear rows 0x00B2F1F0, 0x00B2F520.\n'
          '// Naming wave 18: landed as CRetailTree_erase00b2ef20::Erase(node*) on a SNodeValue_00b2ef20 {key[4], CRetailBuffer} stand-in; re-authored on\n'
          '//   the real STLport text (name-only: the bytes are unchanged).\n',
          'pair<long, long>', 'CEngineTextureMorph', '', 'CRetailTree_erase00b2ef20::Erase', leaf=False)
MORPH = '''class CCompositeBlendDesc {                                 // Ego_r.pdb, 16 bytes: TextureIndex, BlendType, DiffuseColour, SpecularColour
public:
    unsigned long TextureIndex;
    long          BlendType;                                // ECompositeBlendType
    unsigned long DiffuseColour;                            // CRGBColour
    unsigned long SpecularColour;                           // CRGBColour
};
class CEngineTextureMorph {                                 // Ego_r.pdb: OriginalTextureIndex +0, MorphID +4, Blends +8 (retail 12 bytes)
public:
    long OriginalTextureIndex;
    long MorphID;
    std::vector<CCompositeBlendDesc, std::allocator<CCompositeBlendDesc> > Blends;
};
'''
# the 00b2ef20 TU needs the vector (its ~vector frees the storage): add the STLport vector shape after the allocator
p, rel, t = he._rw('00b2ef20', 'src')
if 'class vector' not in t:
    VEC = '''template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};
template <class _Tp, class _Alloc> class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { }                                                            // trivially destroyed elements: nothing to destroy
};
} // namespace std (the value type needs the vector shape)
''' + MORPH + '''namespace std {
'''
    t = t.replace('''template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
''', VEC, 1)
    he._w(p, t); set_leaf('00b2ef20', p, t, '?_M_erase@?$_Rb_tree@')
# ---- _List_base<CCountedPointer<CCompositeSurfaceCache> >::clear on the 0x00510D40 text
a = '00b2ea90'; p = done(a)
if p:
    base = base_text('00510d40')
    cut = base.index('extern "C" void __cdecl free')
    head = ('// std::_List_base<CCountedPointer<CCompositeSurfaceCache>, std::allocator<CCountedPointer<CCompositeSurfaceCache> > >::clear() -- release every\n'
            '//   node\'s counted pointer (count block decrement, DeleteFunc + delete at zero, both words cleared), free the node, re-link the sentinel to itself\n'
            '// Leaf: LEAF_PENDING\n'
            '// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2EA90, 89 bytes, thiscall.\n'
            '// Identity (chunk c681 queue row ?clear@?$_List_base@V?$CCountedPointer@VCCompositeSurfaceCache@@@@...): STLport 4.6 _List_base::clear of the\n'
            '//   CEngineSurfaceCompositionManager cache list (~CEngineSurfaceCompositionManager 0x00B2D1B0 binds it); the element destructor is the\n'
            '//   engine/CCountedPointer.h ~CCountedPointer (count block at +4, DeleteFunc slot +4 of the block, operator delete).\n'
            '// Callees: CCPPointerInfo::DeleteFunc (indirect), operator delete 0x00BFE9BC, free 0x00BFEA14 (allocator<_Node>::deallocate).\n'
            '// Shape: the real STLport 4.6 _list.c clear() text (the landed 0x00510D40 row), explicitly instantiated for the proven element type.\n'
            '// Naming wave 18: landed as the descriptive CRetailList_CountedPointer::clear() on a local CCountedPointer / SCountBlock model; re-authored on the\n'
            '//   STLport instantiation over the shared CCountedPointer header (name-only: the bytes are unchanged).\n')
    body = base[cut:]
    s = body.index('class CBaseObjectPointer;'); e = body.index('template void std::_List_base<')
    body = body[:s] + '#include "engine/CCountedPointer.h"\nclass CCompositeSurfaceCache;\n\n' + 'template void std::_List_base<CCountedPointer<CCompositeSurfaceCache>, std::allocator<CCountedPointer<CCompositeSurfaceCache> > >::clear();\n'
    t = head + body; he._w(p, t); set_leaf(a, p, t, '?clear@?$_List_base@')
# ---- ~_List_base<CDeviceResetCallback*> on the 0x00A3FF60 text
a = '009c2220'; p = done(a)
if p:
    base = base_text('00a3ff60')
    cut = base.index('extern "C" void __cdecl free')
    head = ('// std::_List_base<CDeviceResetCallback*, std::allocator<CDeviceResetCallback*> >::~_List_base() -- free every node (trivially destructible\n'
            '//   pointers: clear() inlined), re-link the sentinel to itself, then free the sentinel node\n'
            '// Leaf: LEAF_PENDING\n'
            '// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C2220, thiscall.\n'
            '// Identity (chunk c678 queue row ??1?$_List_base@PAVCDeviceResetCallback@@...): ~CDisplayManager 0x009C01C0 binds it on its\n'
            '//   list<CDeviceResetCallback*> (Ego_r CDisplayManager DeviceResetCallbacks); body = STLport ~_List_base with clear() inlined.\n'
            '// Callees: free 0x00BFEA14 (allocator<_Node>::deallocate, per node and for the sentinel).\n'
            '// Shape: the real STLport 4.6 text (the landed 0x00A3FF60 ~_List_base<CSoundObjectLH*> row), explicitly instantiated for the proven element type.\n'
            '// Naming wave 18: landed as the free __fastcall SortTreeRecursively_00436900(TreeList*) copy of the 0x00436900 model (a propagated\n'
            '//   CGuiControlTreePane label); re-authored on the proven STLport instantiation (name-only: the bytes are unchanged).\n')
    body = base[cut:]
    body = body.replace('class CSoundObjectLH;\ntemplate class std::_List_base<CSoundObjectLH*, std::allocator<CSoundObjectLH*> >;', 'class CDeviceResetCallback;\ntemplate class std::_List_base<CDeviceResetCallback*, std::allocator<CDeviceResetCallback*> >;')
    assert 'CDeviceResetCallback' in body
    t = head + body; he._w(p, t); set_leaf(a, p, t, '??1?$_List_base@')
# ---- ~vector<pair<long, CCharString> > and its destroy chain (a /Gr TU under optimize("s"))
a = '004e34b2'; p = done(a)
if p:
    base = base_text('00a40bd0')
    cut = base.index('extern "C" void __cdecl free')
    head = ('#pragma optimize("s", on)\n'
            '// std::vector<std::pair<long, CCharString> >::~vector() -- destroy the elements through the out-of-line __destroy_aux 0x004DD782, then free the storage\n'
            '// Leaf: LEAF_PENDING\n'
            '// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E34B2, 19 bytes + the destroy chain, thiscall (optimize("s") TU, free functions __fastcall).\n'
            '// Identity (chunk c678 queue row ??1?$vector@U?$pair@JVCCharString@@@std@@...): the CTCContainerRewardHero items vector (RewardHeroWithItems\n'
            '//   0x0074EE90 and the quest scripts bind it); body = STLport ~vector: _Destroy(start, finish) -> __destroy_aux(first, last, __false_type())\n'
            '//   out of line at 0x004DD782 (its own row), then ~_Vector_base frees the storage.\n'
            '// Callees: __destroy_aux<pair<long,CCharString>*> 0x004DD782 (__fastcall, the __false_type tag by address), free 0x00BFEA14.\n'
            '// Naming wave 18: landed as the free OnReadFinished(AutoTinyActiveFile*) stand-in (a CActiveFile label) calling AutoTinyFinishRead_004dd782;\n'
            '//   re-authored on the STLport instantiation (name-only: the bytes are unchanged).\n')
    body = base[cut:]
    body = body.replace('''template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
''', '''template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x004DD782 (its own row)
''')
    body = body.replace('class CSoundBankLH;\ntemplate class std::vector<std::pair<std::pair<CCharString, CCharString>, CSoundBankLH*> >;', 'template class std::vector<std::pair<long, CCharString> >;')
    assert 'pair<long, CCharString>' in body and '0x004DD782' in body
    t = head + body; he._w(p, t); set_leaf(a, p, t, '??1?$vector@')
a = '004dd782'; p = done(a)
if p:
    base = base_text('00a40bd0')
    cut = base.index('extern "C" void __cdecl free')
    head = ('#pragma optimize("s", on)\n'
            '// std::__destroy_aux<std::pair<long, CCharString>*>(first, last, const std::__false_type&) -- STLport 4.6 _construct.h: destroy every element of\n'
            '//   [first, last) through its (non-inlined) implicit destructor, the pair\'s scalar deleting destructor 0x004DA68A called with flag 0\n'
            '// Leaf: LEAF_PENDING\n'
            '// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DD782, 29 bytes, __fastcall (first in ecx, last in edx, the tag pushed), `ret 4`.\n'
            '// Identity: called by ~vector<pair<long, CCharString> > 0x004E34B2 (_Destroy(start, finish) -> __destroy_aux(..., __false_type())); the\n'
            '//   8-byte stride is pair<long, CCharString>; the callee 0x004DA68A is that pair\'s ??_G (catalog). The TU is an optimize("s") TU: the implicit\n'
            '//   ~pair is not inlined, VC7.1 then calls the scalar deleting destructor with flag 0.\n'
            '// Naming wave 18: landed as the free __fastcall Destroy_range_CFlash(CFlash*, CFlash*, allocator_flash*) stand-in (NCreatureHitNotification::CFlash\n'
            '//   label); re-authored on the STLport text (name-only: the bytes are unchanged).\n')
    body = base[cut:]
    body = body.replace('''template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
''', '''template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
''')
    body = body.replace('class CSoundBankLH;\ntemplate class std::vector<std::pair<std::pair<CCharString, CCharString>, CSoundBankLH*> >;', 'template void std::__destroy_aux<std::pair<long, CCharString>*>(std::pair<long, CCharString>*, std::pair<long, CCharString>*, const std::__false_type&);')
    assert 'pair<long, CCharString>*>' in body
    t = head + body; he._w(p, t); set_leaf(a, p, t, '??$__destroy_aux@')
a = '004da68a'; p = done(a)
if p:
    he._w(p, '''#pragma optimize("s", on)
// std::pair<long, CCharString>::`scalar deleting destructor'(unsigned int flags) -- the compiler-emitted ??_G of the implicit destructor: ~CCharString on
//   second, then operator delete when bit 0 of flags is set
// Leaf: LEAF_PENDING
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DA68A, 31 bytes, thiscall, `ret 4`.
// Identity: catalog std::pair<long,CCharString>::`scalar deleting destructor'; called with flag 0 by __destroy_aux 0x004DD782 (the element destroy of
//   ~vector<pair<long, CCharString> > 0x004E34B2) -- VC7.1 calls the ??_G with flag 0 for a non-inlined implicit destructor (optimize("s") TU).
// Emitter: `delete` of a pair<long, CCharString>* makes the TU emit the ??_G (the Kill_ function is not part of the graded body).
// Naming wave 18: landed as the free __fastcall vector_deleting_destructor(CLandscapeBackgroundPatch*, int, unsigned int) stand-in (the
//   propagated CLandscapeBackgroundPatch label); re-authored as the compiler-emitted ??_G (name-only: the bytes are unchanged).
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                             // 0x0099EAE0
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
void Kill_004da68a(std::pair<long, CCharString>* p)   // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete p;
}
'''); p, rel, t = he._rw(a, 'src'); set_leaf(a, p, t, '??_G?$pair@JVCCharString@@@std@@')
# ---- vector<CTVertexWaterBackground>::resize on the landed erase 0x00B7FD70 text
a = '00b7fd00'; p = done(a)
if p:
    base = base_text('00b7fd70')
    cut = base.index('class CTVertexWaterBackground {')
    head = ('// std::vector<CTVertexWaterBackground>::resize(unsigned int __new_size, const CTVertexWaterBackground& __x) -- STLport 4.6: erase the tail\n'
            '//   (erase(begin() + n, end()) 0x00B7FD70) when shrinking, else insert(end(), n - size(), x) = _M_fill_insert 0x00B7FDE0 of the missing elements\n'
            '// Leaf: LEAF_PENDING\n'
            '// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B7FD00, 105 bytes, thiscall, `ret 8`.\n'
            '// Identity (wave 17 DEFERRED ?resize@?$vector@VCTVertexWaterBackground@@...): FableWin.pdb CWaterPatchTesselatorBase<CTVertexWaterBackground>::\n'
            '//   SetBridgeVertexBufferSize resizes BridgeVertexBuffer (CArray<CTVertexWaterBackground>, 56-byte elements: the signed `cdq / idiv 0x38`);\n'
            '//   the callees are the landed erase 0x00B7FD70 and _M_fill_insert 0x00B7FDE0 rows of the same instantiation.\n'
            '// Shape: the landed erase row\'s STLport text (the vertex, the __copy chain) plus the in-class resize / insert; erase and _M_fill_insert are\n'
            '//   the out-of-line members retail called.\n'
            '// Naming wave 18: landed as the descriptive CRetailVector_elem56::resize(unsigned long, const SElement56&) with Erase / Insert stand-ins;\n'
            '//   re-authored on the STLport instantiation (name-only: the bytes are unchanged).\n')
    body = base[cut:]
    body = body.replace('''template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    _Tp* erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = copy((const _Tp*)__last, (const _Tp*)this->_M_finish, __first);
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};

template CTVertexWaterBackground* vector<CTVertexWaterBackground>::erase(CTVertexWaterBackground* __first, CTVertexWaterBackground* __last);
''', '''template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp* erase(_Tp* __first, _Tp* __last);                                      // 0x00B7FD70 (its own row)
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);          // 0x00B7FDE0 (its own row)
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};

template void vector<CTVertexWaterBackground>::resize(unsigned int __new_size, const CTVertexWaterBackground& __x);
''')
    assert 'resize(unsigned int __new_size' in body
    t = head + body; he._w(p, t); set_leaf(a, p, t, '?resize@?$vector@')
