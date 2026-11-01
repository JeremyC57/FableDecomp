# h21_setcopy.py: naming wave 21, family setcopy -- DECISIONS_W20_RESOLVED item 0x00AD3A10: the PDB spells the copy constructor as the set form
# ??0?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@QAE@ABV01@@Z (no _Rb_tree copy constructor exists in either PDB; the own row 0x00AD3A10 emits
# both COMDATs with the same bytes). The six callers binding other spellings are respelled name-only (byte-identical):
#   00ad77f0 COptimisedVertex copy ctor: set / allocator keyed class (was U: ??0?$set@KU?$less@K@std@@U?$allocator@K@2@@std@@QAE@ABU01@@Z);
#   00ac8960 CIndexList::GetIntersection and 00ae7880 vector<set<unsigned long> >(n): the set copy constructor declared out of line (the
#     _Rb_tree copy-constructor declaration gone); 00ae7880 was spelled over set<long> -- the element is the PDB set<unsigned long>;
#   00ad7e80 / 00ad62b0 / 00ad6ae0: the RetailSet / CIndexList@N00ad62b0 / SRetailSet_00ad6ae0 stand-ins are std::set<unsigned long> inside
#     QuadricOptimiserInternals::CIndexList, and the element record is QuadricOptimiserInternals::CUniqueVertex {Exists, VertexIndices, FaceList}
#     (Ego_r.pdb); 00ad62b0 / 00ad6ae0 are written as the STLport 4.6 _Construct / __uninitialized_copy instantiations.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_setcopy.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())
def E(a, k, old, new):
    if a not in _X: he.edit(a, k, old, new)
def R(a, k, pat, rep, n=None):
    if a not in _X: he.resub(a, k, pat, rep, n)
def P(a, txt):
    if a not in _X: he.put(a, 'src', txt)
NOTE = '// Naming (naming wave 21, DECISIONS_W20_RESOLVED): the set copy constructor 0x00AD3A10 is bound under its PDB spelling ??0?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@QAE@ABV01@@Z (class-keyed set and allocator; the set form, not the _Rb_tree form).'
def note(a, txt=NOTE, k='src'):
    if a in _X: return
    p, rel, t = he._rw(a, k)
    if txt in t: print('already', a, k); return
    m = re.search(r'^// Leaf: [^\n]*\n', t, re.M)
    i = m.end() if m else (t.index('\n') + 1)
    he._w(p, t[:i] + txt + '\n' + t[i:]); print('ok', a, k)
# 00ad77f0: class keys
E('00ad77f0', 'src', 'template <class T> struct allocator { };', 'template <class T> class allocator { };')
E('00ad77f0', 'src', 'template <class K, class C = less<K>, class A = allocator<K> > struct set {', 'template <class K, class C = less<K>, class A = allocator<K> > class set { public:')
E('00ad77f0', 'src', 'template <class K, class V, class C = less<K>, class A = allocator<pair<const K, V> > > struct map {', 'template <class K, class V, class C = less<K>, class A = allocator<pair<const K, V> > > class map { public:')
note('00ad77f0')
# 00ac8960: the set copy constructor out of line
E('00ac8960', 'src', '    _Rb_tree(const _Rb_tree& __x);                      // 0x00AD3A10 for set<unsigned long>\n', '')
E('00ac8960', 'src', '    set() : _M_t() { }\n', '    set() : _M_t() { }\n    set(const set<_Key, _Compare, _Alloc>& __x);        // 0x00AD3A10 for set<unsigned long> (the PDB spelling; _M_t(__x._M_t) in the own row)\n')
note('00ac8960')
# 00ae7880: set<unsigned long>, the set copy constructor out of line
E('00ae7880', 'src', '    _Rb_tree(const _Rb_tree& __x);                                                             // 0x00AD3A10\n', '')
E('00ae7880', 'src', '    set() : _M_t(_Compare(), _Alloc()) { }\n', '    set() : _M_t(_Compare(), _Alloc()) { }\n    set(const set<_Key, _Compare, _Alloc>& __x);                                               // 0x00AD3A10 (the PDB spelling ??0?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@QAE@ABV01@@Z)\n')
E('00ae7880', 'src', 'template std::vector<std::set<long> >::vector(unsigned int);', 'template std::vector<std::set<unsigned long> >::vector(unsigned int);')
R('00ae7880', 'src', r'std::set<long>', 'std::set<unsigned long>')
R('00ae7880', 'src', r'\bset<long>', 'set<unsigned long>')
note('00ae7880', NOTE + '\n// Naming (naming wave 21): the element is set<unsigned long> (the one set copy constructor 0x00AD3A10 is the PDB set<unsigned long>; Ego_r names the vector<set<unsigned long> > instantiations), not set<long>.')
STD_SET = '''namespace std {
template <class _Tp> struct less { };
template <class _Tp> class allocator { };
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {                                     // STLport 4.6 set = _Rb_tree {header-node proxy, node count, compare}: 12 bytes
public:
    void*         _M_header;                    // +0x00 (the 0x14-byte header node)
    unsigned int  _M_node_count;                // +0x04
    unsigned char _M_key_compare;               // +0x08
    set();
    set(const set<_Key, _Compare, _Alloc>& __x);        // 0x00AD3A10 for set<unsigned long> (PDB spelling)
    set<_Key, _Compare, _Alloc>& operator=(const set<_Key, _Compare, _Alloc>& __x);
    ~set();
};
}
namespace QuadricOptimiserInternals {
class CIndexList { public:                      // Ego_r.pdb: one std::set<unsigned long> Indices
    std::set<unsigned long> Indices;
};
class CUniqueVertex { public:                   // Ego_r.pdb CUniqueVertex, sizeof 0x1C
    bool       Exists;                          // +0x00
    CIndexList VertexIndices;                   // +0x04
    CIndexList FaceList;                        // +0x10
};
}
'''
# 00ad7e80: RetailSet / Element -> std::set<unsigned long> / QuadricOptimiserInternals::CUniqueVertex (name-only; the vector model stays)
E('00ad7e80', 'src', '''struct TreeNode {unsigned char Colour;TreeNode* Parent;TreeNode* Left;TreeNode* Right;int Key;};
struct RetailSet {TreeNode* Head;unsigned int Count;unsigned char Compare;
 RetailSet();RetailSet(const RetailSet&);RetailSet& operator=(const RetailSet&);~RetailSet();
 void Insert(int);bool Contains(int)const;
};
struct String { unsigned int W0; String(); String(const String&); ~String(); String& operator=(const String&); };
struct Element { unsigned char Flag; RetailSet A,B; ~Element(); };
''', '''}
''' + STD_SET + '''namespace N00ad7e80 {
typedef QuadricOptimiserInternals::CUniqueVertex Element;   // the 28-byte element {Exists, VertexIndices, FaceList}
''')
E('00ad7e80', 'src', '// Layout: record flag0, set<long> A+4 and B+16; set Head0/Count4/Compare8 (12 bytes). Callee00ad3a10 allocates20-byte sentinel and copies tree;00ad4a70 destroys both sets.',
  '// Layout: the element is QuadricOptimiserInternals::CUniqueVertex {Exists +0, CIndexList VertexIndices +4, CIndexList FaceList +0x10} (Ego_r.pdb), each list one STLport set<unsigned long> (header, count, compare: 12 bytes). Callee 0x00AD3A10 is the set copy constructor (allocates the 20-byte header node and copies the tree); 0x00AD4A70 the element destructor (both sets).\n' + NOTE)
# 00ad62b0: the STLport _Construct instantiation
P('00ad62b0', '''// std::_Construct<QuadricOptimiserInternals::CUniqueVertex,QuadricOptimiserInternals::CUniqueVertex>(CUniqueVertex* p, const CUniqueVertex& value) -- placement copy of an optimiser unique vertex: exists flag and two index lists (out-of-line set copies)
// Retail: Fable.exe @ 0x00AD62B0, 41 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: the set<unsigned long> copy constructor @ 0x00AD3A10 (x2; the PDB spelling ??0?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@QAE@ABV01@@Z).
// Naming (naming wave 21, DECISIONS_W20_RESOLVED): the element is Ego_r.pdb QuadricOptimiserInternals::CUniqueVertex {Exists, CIndexList VertexIndices,
//   CIndexList FaceList} (each list one STLport set<unsigned long>); the body is STLport 4.6 _construct.h `_Construct(_T1* __p, const _T2& __val)
//   { new (__p) _T1(__val); }` (VC7.1's null test on the placement destination), instantiated here (was the N00ad62b0 stand-in namespace).
// Leaf: ??$_Construct@VCUniqueVertex@QuadricOptimiserInternals@@V12@@std@@
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
''' + STD_SET + '''namespace std {
template <class _T1, class _T2> void __fastcall _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}
template void __fastcall _Construct<QuadricOptimiserInternals::CUniqueVertex, QuadricOptimiserInternals::CUniqueVertex>(QuadricOptimiserInternals::CUniqueVertex*, const QuadricOptimiserInternals::CUniqueVertex&);
}
''')
# 00ad6ae0: the STLport __uninitialized_copy instantiation
P('00ad6ae0', '''// std::__uninitialized_copy<QuadricOptimiserInternals::CUniqueVertex*,QuadricOptimiserInternals::CUniqueVertex*>(const CUniqueVertex* first, const CUniqueVertex* last, CUniqueVertex* result, const __false_type&) -- STLport __uninitialized_copy over the 28-byte {Exists, VertexIndices, FaceList} records: placement-copy each (two out-of-line set copies per record), return the end of the copies
// Leaf: ??$__uninitialized_copy@PAVCUniqueVertex@QuadricOptimiserInternals@@PAV12@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD6AE0, length 69 bytes, `ret 8`.
// Callers: 0x00AD7CD7 / 0x00AD7D36 (insert-fill 0x00AD7C70) and 0x00AD7F16 / 0x00AD7F6D (the landed
//   vector<CUniqueVertex>::_M_fill_insert 0x00AD7E80, whose same-TU stand-in `__uninitialized_copy` is this body).
// Callees: the set<unsigned long> copy constructor @ 0x00AD3A10 (twice; the PDB spelling ??0?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@QAE@ABV01@@Z).
// Naming (naming wave 21, DECISIONS_W20_RESOLVED): the element is Ego_r.pdb QuadricOptimiserInternals::CUniqueVertex (each CIndexList one STLport
//   set<unsigned long>); the body is STLport 4.6 _uninitialized.h `__uninitialized_copy(_InputIter, _InputIter, _ForwardIter, const __false_type&)`
//   instantiated over CUniqueVertex* (was the descriptive UninitCopy_FlagSetPair_00ad6ae0 over SRetailSet_00ad6ae0 stand-ins).
// Shape: `__fastcall` (first in ecx -> esi, last in edx -> ebp, result + the tag reference on the stack -> edi, loaded before the empty
//   test through the named `__cur` copy and returned from the shared epilogue). The implicit memberwise copy stores the flag byte and calls
//   the set copy twice; VC7.1 keeps `&dst->FaceList` in ebx across the first call and addresses `&dst->VertexIndices` as `ebx-0Ch`;
//   placement-new null test on the destination.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)
''' + STD_SET + '''namespace std {
struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template QuadricOptimiserInternals::CUniqueVertex* __fastcall __uninitialized_copy<QuadricOptimiserInternals::CUniqueVertex*, QuadricOptimiserInternals::CUniqueVertex*>(QuadricOptimiserInternals::CUniqueVertex*, QuadricOptimiserInternals::CUniqueVertex*, QuadricOptimiserInternals::CUniqueVertex*, const __false_type&);
}
''')
