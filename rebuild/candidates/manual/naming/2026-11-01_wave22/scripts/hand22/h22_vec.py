# h22_vec.py: naming wave 22, family vec -- DECISIONS_W21_RESOLVED "00b67ec0 vector templates": 0x00B67EC0 models its three STLport vectors on
# one hand CVector<T>; re-modelled in one edit as std::vector<float> / <unsigned char> / <unsigned long> (STLport 4.6 with the default
# allocator), so the callees take their STLport instantiation spellings: vector<float>::_M_fill_insert 0x00441020
# (?_M_fill_insert@?$vector@MV?$allocator@M@std@@@std@@QAEXPAMIABM@Z, already bound by the landed 0x00AC9D50), vector<unsigned char>::
# _M_fill_insert 0x00485F3A, and the same-TU body of vector<unsigned long>::_M_insert_overflow 0x00464ACE (protected, non-template, the
# std::__true_type tag: ?_M_insert_overflow@?$vector@KV?$allocator@K@std@@@std@@IAEXPAKABKABU__true_type@2@I_N@Z, the spelling the landed
# 0x00AA65A0 already binds). Name-only.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
a = '00b67ec0'
he.edit(a, 'src', '''struct __true_type { __true_type() {} };

template <class T> struct CVector {                // STLport vector<T> (12 bytes)
    T* _M_start;''', '''namespace std {
struct __true_type { __true_type() {} };
template <class _Tp> class allocator { };

template <class T, class _Alloc = allocator<T> >
class vector {                                     // STLport 4.6 vector<T> (12 bytes; the base's three pointers)
public:
    T* _M_start;''')
he.edit(a, 'src', '''    void _M_fill_insert(T* position, unsigned int n, const T& x);
    template <class Tag> void _M_insert_overflow(T* position, const T& x, const Tag&, unsigned int n, bool atend);   // 0x00464ACE
    void insert(''', '''    void _M_fill_insert(T* position, unsigned int n, const T& x);   // 0x00441020 (float) / 0x00485F3A (unsigned char)
protected:
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int n, bool atend);   // 0x00464ACE (unsigned long)
public:
    void insert(''')
he.edit(a, 'src', '''// Same-TU body of vector<unsigned long>::_M_insert_overflow 0x00464ACE (empty tag unnamed, never stored through).
template <class T> template <class Tag>
__declspec(noinline) void CVector<T>::_M_insert_overflow(T* position, const T& x, const Tag&, unsigned int n, bool atend)''', '''// Same-TU body of vector<unsigned long>::_M_insert_overflow 0x00464ACE (empty tag unnamed, never stored through).
template <class T, class _Alloc>
__declspec(noinline) void vector<T, _Alloc>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int n, bool atend)''')
he.edit(a, 'src', '''    _M_end_of_storage = newStart + len;
}
''', '''    _M_end_of_storage = newStart + len;
}
}   // namespace std
''')
he.edit(a, 'src', '''    CVector<unsigned long>        FrameForegroundTexturesUsed;        // +0x68c
    CVector<float>                FrameForegroundTexturePriorities;   // +0x698
    CVector<unsigned char>        FrameForegroundTextureFlags;        // +0x6a4''', '''    std::vector<unsigned long>    FrameForegroundTexturesUsed;        // +0x68c
    std::vector<float>            FrameForegroundTexturePriorities;   // +0x698
    std::vector<unsigned char>    FrameForegroundTextureFlags;        // +0x6a4''')
p, rel, t = he._rw(a, 'src')
N = '// Naming (naming wave 22, DECISIONS_W21_RESOLVED): the three vectors are STLport std::vector<unsigned long> / <float> / <unsigned char> (were one hand CVector<T>); the callees carry their STLport spellings.'
if N in t: print('already', a, 'note')
else:
    i = t.index('#include <new>'); he._w(p, t[:i] + N + '\n' + t[i:]); print('ok', a, 'note')
