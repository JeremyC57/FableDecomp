# h30_lhn.py: naming wave 30, family lhn (scope 1, queue rows 33-35, chunk 697 LHNetworkLib group): the two own rows take their PDB names.
#  0x00569C30 ??1LHHttpHeaders@LHNetworkLib@@QAE@XZ (Ego_r 0x005E03E0 + FableWin 0x022DE110): Reset 0x0083FBC0, the HeaderString (+0xC)
#   block freed (STLport basic_string dtor), the tail into the out-of-line ~vector<string> of HdrDocumentHeader (+0). Was the descriptive
#   CBlockVectorOwner_00569c30 over CBlockVector / CRawBuffer stand-ins.
#  0x00569940 ??1?$vector@V?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@...@std@@@2@@std@@QAE@XZ:
#   STLport 4.6 ~vector<std::string> (each 12-byte string's block, then the buffer). Was CRetailVector_00569940::Destroy_CRetailVector_00569940.
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

template <class _CharT> class char_traits { };

template <class _Tp, class _Alloc> class _String_base {             // STLport 4.6: 12 bytes
public:
    ~_String_base() { _M_deallocate_block(); }
    void _M_deallocate_block() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
protected:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string : protected _String_base<_CharT, _Alloc> {
public:
    ~basic_string() { }
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
protected:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector()%s
};

}   // namespace std
'''
a = '00569940'
he.put(a, 'src', '''// std::vector<std::basic_string<char> >::~vector() -- STLport 4.6: destroy each 12-byte string (free its block), then free the buffer
// Leaf: ??1?$vector@V?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@std@@QAE@XZ
// Naming wave 30 (family lhn): was CRetailVector_00569940::Destroy_CRetailVector_00569940 (descriptive). The out-of-line COMDAT
//   ~LHHttpHeaders 0x00569C30 tail-calls for HdrDocumentHeader (Ego_r.pdb vector<basic_string<char> >; the string dtor is expanded inline,
//   as the copy in ~LHHttpHeaderStatus is).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00569940, 57 bytes, thiscall, plain `ret`.
// Shape: the end pointer is cached before the loop, every non-null string block is freed (`add esp,4`: a speed TU), then the buffer.
//   DestroyOne (a destruction by name, ungraded) makes VC7.1 emit the out-of-line `??1` (the chunk 656/657 idiom).
''' + STL % ' { _Destroy(this->_M_start, this->_M_finish); }' + '''
// Emission helper: destroying the vector by name is what makes VC7.1 emit the out-of-line COMDAT.
void DestroyOne(std::vector<std::basic_string<char> >* strings)
{
    strings->~vector();
}''')
a = '00569c30'
he.put(a, 'src', '''// LHNetworkLib::LHHttpHeaders::~LHHttpHeaders() -- Reset(), then the members in reverse order: HeaderString's block freed, then the
//   out-of-line ~vector<string> of HdrDocumentHeader by tail call
// Leaf: ??1LHHttpHeaders@LHNetworkLib@@QAE@XZ
// Naming wave 30 (family lhn): Ego_r.pdb (0x005E03E0) / FableWin.pdb (0x022DE110) ??1LHHttpHeaders@LHNetworkLib@@QAE@XZ; was the descriptive
//   CBlockVectorOwner_00569c30 (??1CBlockVectorOwner_00569c30@@QAE@XZ over CBlockVector / CRawBuffer stand-ins).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00569C30, 32 bytes.
// Callees: ?Reset@LHHttpHeaders@LHNetworkLib@@QAEXXZ 0x0083FBC0 (the landed row), free 0x00BFEA14 (explicit null test, `add esp,4`:
//   a speed TU), std::vector<std::basic_string<char> >::~vector 0x00569940 reached by tail jump with ecx = this.
// Layout: retail LHHttpHeaders (Ego_r.pdb names, 12-byte STLport containers): HdrDocumentHeader +0, HeaderString +0xc (as 0x0083FBC0).
// Shape: no vtable store = a non-polymorphic class; the user destructor body is the one Reset() call, then the members are destroyed in
//   reverse declaration order (the string dtor inline, the vector's out of line).
''' + STL % ';                                              // 0x00569940 for basic_string<char> (out of line)' + '''
namespace LHNetworkLib {

class LHHttpHeaders {
public:
    std::vector<std::basic_string<char> > HdrDocumentHeader;   // +0x00
    std::basic_string<char> HeaderString;                     // +0x0c
    void Reset();                                             // 0x0083FBC0
    ~LHHttpHeaders();
};

LHHttpHeaders::~LHHttpHeaders()
{
    Reset();
}

}   // namespace LHNetworkLib''')
