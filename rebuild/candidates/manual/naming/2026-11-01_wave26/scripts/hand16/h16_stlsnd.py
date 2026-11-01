# h16_stlsnd.py: naming wave 16, family stl -- the STLport COMDATs of the ~CSoundManagerLH TU (c670) re-authored from their
# stand-ins on the real STLport 4.6 text that the landed 0x00A3EDE0 row compiles (copied verbatim from that row), explicitly
# instantiated for the proven element types: 0x00A3FF60 ~_List_base<CSoundObjectLH*>, 0x00A40BD0 ~vector<pair<pair<CCharString,
# CCharString>, CSoundBankLH*> >.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import path, _w, R
import rows
src = open(os.path.join(rows.ROOT, 'rebuild/src/compiled', R['00a3ede0']['Source']), encoding='utf-8').read()
i = src.index('extern "C" void __cdecl free(void* block);'); j = src.index('template <class K, class V> class CKeyPairCompareLess')
STD = src[i:j]
assert STD.count('namespace std {') == 1
# STLport 4.6 allocator<T>::deallocate tests the pointer (stl/_alloc.h); the ~CSoundManagerLH row's model drops the test (its TU only
# calls these COMDATs), the out-of-line ~_List_base keeps it on the sentinel free (retail `test edi,edi ; je` at 0x00A3FF8E)
OLD = '    void deallocate(_Tp* __p, unsigned int) { free(__p); }'
assert STD.count(OLD) == 1
STD = STD.replace(OLD, '    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }')
HDR = {
'00a3ff60': ('''// std::_List_base<CSoundObjectLH*, std::allocator<CSoundObjectLH*> >::~_List_base() -- free every node (trivially destructible
//   pointers: clear() inlined), re-link the sentinel to itself, then free the sentinel node
// Leaf: ??1?$_List_base@PAVCSoundObjectLH@@V?$allocator@PAVCSoundObjectLH@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3FF60, 62 bytes, thiscall.
// Identity (proven, chunk c670): called by ~CSoundManagerLH 0x00A3EE3B on Sounds (+0x4C, the list<CSoundObjectLH*> of the landed
//   vfunc_5/6/10) and jumped to from 0x00A40190; body = STLport ~_List_base with clear() inlined (free nodes, reset sentinel, free
//   sentinel); the element class CSoundObjectLH is the landed descriptive spelling (the sound module is in neither PDB).
// Callees: free 0x00BFEA14 (allocator<_Node>::deallocate, per node and for the sentinel).
// Shape: the real STLport 4.6 text, copied from the landed ~CSoundManagerLH 0x00A3EDE0 row (whose TU emits this COMDAT) with
//   STLport's null-tested allocator<T>::deallocate, explicitly instantiated for the proven element type.
// Naming wave 16: landed as the free __fastcall SortTreeRecursively_00436900(TreeList*) copy of the 0x00436900 model;
//   re-authored on the proven STLport instantiation (name-only: the bytes are unchanged).
''', '''class CSoundObjectLH;
template class std::_List_base<CSoundObjectLH*, std::allocator<CSoundObjectLH*> >;
'''),
'00a40bd0': ('''// std::vector<std::pair<std::pair<CCharString, CCharString>, CSoundBankLH*> >::~vector() -- destroy the two strings of every
//   12-byte element, then free the storage
// Leaf: ??1?$vector@U?$pair@U?$pair@VCCharString@@V1@@std@@PAVCSoundBankLH@@@std@@V?$allocator@U?$pair@U?$pair@VCCharString@@V1@@std@@PAVCSoundBankLH@@@std@@@2@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A40BD0, 57 bytes, thiscall.
// Identity (proven, chunk c670): called by ~CSoundManagerLH 0x00A3EE33 on NamePairMap.Data (+0x54, CVectorMap<pair<CCharString,
//   CCharString>, CSoundBankLH*>; landed FindByNamePair 0x00A3EE50) and jumped to from 0x00A41300 / 0x00A417B0; body = STLport
//   ~vector (two ~CCharString per 12-byte element, then free).
// Callees: CCharString::~CCharString 0x0099EAE0 (second, then first of each key pair), free 0x00BFEA14.
// Shape: the real STLport 4.6 text, copied from the landed ~CSoundManagerLH 0x00A3EDE0 row (whose TU emits this COMDAT) with
//   STLport's null-tested allocator<T>::deallocate, explicitly instantiated for the proven element type.
// Naming wave 16: landed as the descriptive CRetailVector_SPairCMember_0099eae012::~CRetailVector_SPairCMember_0099eae012;
//   re-authored on the proven STLport instantiation (name-only: the bytes are unchanged).
''', '''class CSoundBankLH;
template class std::vector<std::pair<std::pair<CCharString, CCharString>, CSoundBankLH*> >;
'''),
}
for a, (h, tail) in HDR.items():
    p, rel = path(a, 'src')
    t = h + STD + tail
    if open(p, encoding='utf-8', newline='').read() == t: print('already', a)
    else: _w(p, t); print('ok', a, 'src')
