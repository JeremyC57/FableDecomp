# h24_vws.py: naming wave 24, family vws -- STLport vector<CWideString>::_M_insert_overflow 0x00412330 is the protected member
# ?_M_insert_overflow@?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@IAEXPAVCWideString@@ABV3@ABU__false_type@2@I_N@Z
# (labels_chunk691: protected, IAE as every landed _M_insert_overflow; the landed 0x005FD9C0 and 0x00CD3F50 already bind it). Two callers
# that model the real std::vector are respelled: 0x004126E0 (_M_fill_insert, which had dropped the allocator argument) and 0x00E81B40
# (public member). The CWideStringVector stand-in callers (0x0071CE80, 0x009AEF60, 0x004091C0, 0x004128A0) need per-row re-models.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
A = '004126e0'
he.edit(A, 'src', '// Model: CWideString is the engine\'s one-pointer wide-string handle; STLport\'s three vector words.\n',
        '''// Model: CWideString is the engine's one-pointer wide-string handle; STLport's three vector words.
// Naming (naming wave 24): the vector carries STLport's allocator argument and _M_insert_overflow is protected, so the growth call
//   binds ?_M_insert_overflow@?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@IAE... (labels_chunk691; was the
//   allocator-less public spelling); the own row is vector<CWideString, allocator<CWideString> >::_M_fill_insert.
''')
he.edit(A, 'src', '''template <class _Tp> class vector {
public:
    typedef _Tp* pointer;''', '''template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;''')
he.edit(A, 'src', '''    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                                                    // 0x00412330
    void _M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x);
};

template <class _Tp>
void vector<_Tp>::_M_insert_overflow(''', '''protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                                                    // 0x00412330
public:
    void _M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(''')
he.edit(A, 'src', '''template <class _Tp>
void vector<_Tp>::_M_fill_insert(''', '''template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(''')
A = '00e81b40'
he.edit(A, 'src', '''    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00412330 (CWideString)
    void push_back(''', '''protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00412330 (CWideString; protected, naming wave 24)
public:
    void push_back(''')
# 0x00412330 itself (the own row) and 0x00495890 CFrontendGameComponent::GetWorldNames modelled the vector without STLport's allocator
#   argument and with a public _M_insert_overflow.
A = '00412330'
he.edit(A, 'src', '''template <class _Tp> class vector {
public:
    typedef _Tp* pointer;''', '''template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // naming wave 24: STLport's allocator argument; the member protected
public:
    typedef _Tp* pointer;''')
he.edit(A, 'src', '''    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);
};

template <class _Tp>
void vector<_Tp>::_M_insert_overflow(''', '''protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(''')
he.edit(A, 'src', '// Leaf: ?_M_insert_overflow@?$vector@VCWideString@@\n', '''// Leaf: ?_M_insert_overflow@?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@IAEXPAVCWideString@@ABV3@ABU__false_type@2@I_N@Z
// Naming (naming wave 24, labels_chunk691): the protected STLport member of vector<CWideString, allocator<CWideString> > (IAE as every
//   landed _M_insert_overflow; 0x005FD9C0 and 0x00CD3F50 bind it); the row had dropped the allocator argument and made it public.
''')
A = '00495890'
he.edit(A, 'src', '''template <class T> class vector {
public:
    T* _M_start;''', '''template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {   // naming wave 24: STLport's allocator argument; _M_insert_overflow protected
public:
    T* _M_start;''')
he.edit(A, 'src', '''    void _M_insert_overflow(T* pos, const T& x, const __false_type&, unsigned int fillLen, bool atEnd);   // 0x00412330
    void push_back(''', '''protected:
    void _M_insert_overflow(T* pos, const T& x, const __false_type&, unsigned int fillLen, bool atEnd);   // 0x00412330
public:
    void push_back(''')
