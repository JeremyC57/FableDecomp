# h26_vlong.py: naming wave 26, family vlong -- the vector<long>::_M_insert_overflow 0x00440990 callers (c689 queue row; scope 2):
#  each row's 12-byte vector<long> stand-in becomes STLport 4.6 std::vector<long, allocator<long> > with the protected
#  _M_insert_overflow(..., const __true_type&, size_type I, bool) the own row 0x00440990 defines (vecify.py).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from vecify import vecify
NOTE = ('// Naming wave 26 (family vlong): the vector is STLport 4.6 std::vector<long, allocator<long> >; its grow-insert 0x00440990 is the\n'
        '//   protected ?_M_insert_overflow@?$vector@JV?$allocator@J@std@@@std@@IAEXPAJABJABU__true_type@2@I_N@Z (size_type unsigned int; the\n'
        '//   own row\'s spelling, c689 queue). Was %s::%s(..., const %s&, unsigned long, bool).')
ROWS = [
    ('007714b0', 'CRetailVector_long', 'InsertGrow', 'SScalarTag'),
    ('00a86e20', 'CLongVector', 'InsertGrow', 'SScalarTag'),
    ('00f37030', 'CLongVector', 'InsertGrow', 'SScalarTag'),
    ('00666b50', 'CLongVector', 'InsertGrow', 'SScalarTag'),
    ('0098a9a0', 'CLongArray', 'InsertGrow', 'SScalarTag'),
    ('00aebaf0', 'CLongVector', 'InsertGrow', 'SScalarTag'),
    ('004ff080', 'CLongVector', '_M_insert_overflow', '__true_type'),
    ('007fede0', 'CLongVector', '_M_insert_overflow', '__true_type'),
    ('005fc620', 'CLongVector', '_M_insert_overflow', '__false_type'),
]
for a, cls, meth, tag in ROWS:
    vecify(a, cls, meth, tag, note=NOTE % (cls, meth, tag))
NOTE2 = ('// Naming wave 26 (family vlong): std::vector takes STLport\'s allocator<_Tp> parameter and its grow-insert 0x00440990 is the protected\n'
         '//   ?_M_insert_overflow@?$vector@JV?$allocator@J@std@@@std@@IAEXPAJABJABU__true_type@2@I_N@Z (size_type unsigned int; the own\n'
         '//   row\'s spelling, c689 queue). Was the public vector<long> member without the allocator%s.')
from vecify import stdvec
for a, tag in (('007cb850', '__true_type'), ('008285d0', '__true_type'), ('0077a350', '__true_type'), ('0086d240', 'SScalarTag'), ('007dc750', '__true_type')):
    stdvec(a, tag, note=NOTE2 % ('' if tag == '__true_type' else ' (tag %s, unsigned long)' % tag))

# 0x007FECB0 CTCUserActionPointBase::UpdateBlockedActionPoints: CLongVector also carries the out-of-line operator= 0x00454886, the
#  own row's ??4?$vector@JV?$allocator@J@std@@@std@@QAEAAV01@ABV01@@Z (bound by 0x00454886 itself and its callers)
vecify('007fecb0', 'CLongVector', '_M_insert_overflow', '__true_type', note=(NOTE % ('CLongVector', '_M_insert_overflow', '__true_type')) +
       '\n//   The same-TU operator= body (0x00454886) is the vector\'s ??4?$vector@JV?$allocator@J@std@@@std@@QAEAAV01@ABV01@@Z (was ??4CLongVector@@).')
he.edit('007fecb0', 'src', 'std::vector<long>& std::vector<long>::operator=(const std::vector<long>& other)',
        'template <class _Tp, class _Alloc>\n__declspec(noinline) std::vector<_Tp, _Alloc>& std::vector<_Tp, _Alloc>::operator=(const std::vector<_Tp, _Alloc>& other)')

# 0x00AEBC90: the grow-insert was a member template over the tag (??$_M_insert_overflow@U__true_type@@@CLongVector@@); STLport's is a
#  plain member taking const __true_type&
he.edit('00aebc90', 'src', '    template <class Tag> void _M_insert_overflow(long* position, const long& x, const Tag&, unsigned long fill_len, bool atend);   // 0x00440990',
        '    void _M_insert_overflow(long* position, const long& x, const __true_type&, unsigned long fill_len, bool atend);   // 0x00440990')
he.edit('00aebc90', 'src', 'template <class Tag> __declspec(noinline) void CLongVector::_M_insert_overflow(long* position, const long& x, const Tag&, unsigned long fill_len, bool atend)',
        '__declspec(noinline) void CLongVector::_M_insert_overflow(long* position, const long& x, const __true_type&, unsigned long fill_len, bool atend)')
p, rel, t = he._rw('00aebc90', 'src')
t = t.replace('struct __true_type { __true_type() {} };', 'struct __true_type { __true_type() { } };'); he._w(p, t)
vecify('00aebc90', 'CLongVector', '_M_insert_overflow', '__true_type', note=(NOTE % ('CLongVector', '_M_insert_overflow<Tag>', 'Tag')))
p, rel, t = he._rw('00aebc90', 'src')
if '// Leaf: ' not in t:
    L = t.split('\n'); L.insert(1, '// Leaf: ?GetVisibleEmitterNames@CParticleEmitterManager@@'); he._w(p, '\n'.join(L)); print('ok 00aebc90 src')

# 0x0098A9A0: the local vector is Fable's CArray<long> (the PDB CPixelShader ctor parameter ABV?$CArray@J@@, the spelling the own row
#  0x00A5ECB0 defines), CArray<T> deriving from the STLport vector (the landed 0x00B1B380 / 0x00B158A0 model) -- so the ctor keeps its
#  CArray<long> spelling and push_back is the vector's (grow-insert 0x00440990)
he.edit('0098a9a0', 'src', '''template<class T> class CArray { public:
    T* first; T* last; T* end_of_storage;
    unsigned long Size() const { return (unsigned long)(last - first); }
    const T& operator[](unsigned long i) const { return first[i]; }
    const T* GetData() const { return Size() > 0 ? first : (const T*)this; }
};
''', '')
he.edit('0098a9a0', 'src', '''#pragma auto_inline(on)
''', '''#pragma auto_inline(on)

template<class T> class CArray : public std::vector<T, std::allocator<T> > { public:   // Fable's CArray over the STLport vector
    unsigned long Size() const { return (unsigned long)(this->last - this->first); }
    const T& operator[](unsigned long i) const { return this->first[i]; }
    const T* GetData() const { return Size() > 0 ? this->first : (const T*)this; }
};
''')
he.edit('0098a9a0', 'src', 'const std::vector<long>& constIndices', 'const CArray<long>& constIndices')
he.edit('0098a9a0', 'src', '    std::vector<long> indices;', '    CArray<long> indices;')
he.edit('0098a9a0', 'src', "//   own row's spelling, c689 queue). Was CLongArray::InsertGrow(..., const SScalarTag&, unsigned long, bool).",
        "//   own row's spelling, c689 queue). Was CLongArray::InsertGrow(..., const SScalarTag&, unsigned long, bool). The local is CArray<long>\n"
        "//   (CArray<T> : std::vector<T>, the landed 0x00B1B380 model), the CPixelShader ctor 0x00A5ECB0's PDB parameter type.")
