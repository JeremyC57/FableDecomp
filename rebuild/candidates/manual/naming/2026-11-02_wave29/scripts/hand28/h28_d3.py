# h28_d3.py: naming wave 28, family d3 (DECISIONS_W27_RESOLVED D3, proven from the bytes this wave):
#  0x004125A0 is STLport 4.6 vector<CDateAndTime>::_M_insert_overflow(.., const __false_type&, ..): its head / fill / tail copies are
#  null-checked placement copies (`test eax,eax ; je` around `rep movsd` x7 per element at 0x00412610 / 0x00412636 / 0x00412656 /
#  0x00412688), the __uninitialized_copy / _Construct path of the __false_type overload; the __true_type overload copies through
#  __copy_trivial (memmove), which the body never calls. Its callers are all CDateAndTime contexts (0x004091C0 LoadFileList,
#  0x00486160 _M_fill_insert<CDateAndTime>, 0x004129A7 the 28-byte push_back called by CTCInGameMenu::GetFileNameList).
#  Spelling: the protected STLport member with the allocator (as every landed _M_insert_overflow, e.g. 0x00412330):
#  ?_M_insert_overflow@?$vector@VCDateAndTime@@V?$allocator@VCDateAndTime@@@std@@@std@@IAEXPAVCDateAndTime@@ABV3@ABU__false_type@2@I_N@Z.
#  0x0040E350 is a folded copy (identical-code folding): its five callers copy 28-byte records of three element types -- CDateAndTime
#  (0x00409207 LoadFileList, 0x0059AECA vector<CDateAndTime>::erase), CActionInputControl (0x00411B17) and the CDisplayModeDesc
#  __uninitialized_copy 0x0040F5E0 (0x0040F69C) -- so its own row keeps its spelling (HOLD, as 0x005359A0); the CDateAndTime caller
#  0x004091C0 takes the CDateAndTime spelling 0x0059AEAF already defines,
#  ??$__copy@PAVCDateAndTime@@PAV1@H@std@@YIPAVCDateAndTime@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
F = 'd3'
LOV = '?_M_insert_overflow@?$vector@VCDateAndTime@@V?$allocator@VCDateAndTime@@@std@@@std@@IAEXPAVCDateAndTime@@ABV3@ABU__false_type@2@I_N@Z'

a = '004125a0'
he.edit(a, 'src', '''template <class _Tp> class vector {
public:
    typedef _Tp* pointer;''', '''template <class _Tp> class allocator;
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;''')
he.edit(a, 'src', '''    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);
};

template <class _Tp>
void vector<_Tp>::_M_insert_overflow(''', '''protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(''')
note(a, F, 'own row is the protected STLport member with the allocator (DECISIONS_W27 D3: the __false_type\n'
     '//   overload proven from the bytes -- null-checked placement copies, no __copy_trivial / memmove). Was the allocator-less public\n'
     '//   ?_M_insert_overflow@?$vector@VCDateAndTime@@@std@@QAEX..@Z.', leaf=LOV)

a = '0048608c'
he.edit(a, 'src', '''    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                                                           // 0x004125A0
    void _M_fill_insert(''', '''protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                                                           // 0x004125A0
public:
    void _M_fill_insert(''')
note(a, F, 'the grow-insert 0x004125A0 is the protected STLport member (IAE; DECISIONS_W27 D3, the\n'
     '//   own row\'s spelling ' + LOV + ').\n//   Was public (QAE).')

# ---- 0x004091C0 CUserProfileManager::LoadFileList over the STLport vectors (DECISIONS_W26 D7 / W27 D3): vector<CWideString>
#  erase 0x00412450 / _M_insert_overflow 0x00412330 and vector<CDateAndTime> _M_insert_overflow 0x004125A0 / the inline erase's
#  __copy 0x0040E350, each on its bound (own-row or landed) spelling.
a = '004091c0'
he.edit(a, 'src', '''namespace std {
struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };
CDateAndTime* __fastcall __copy(CDateAndTime* first, CDateAndTime* last, CDateAndTime* result, const random_access_iterator_tag&, int*);   // 0x0040E350
template <class T, class Trivial> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    template <class Tag> void _M_insert_overflow(T* pos, const T& x, const Tag&, unsigned int fillLen, bool atEnd);
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, Trivial(), 1, true);
    }
};
class CWideStringVector : public vector<CWideString, __false_type> {
public:
    CWideString* erase(CWideString* first, CWideString* last);   // 0x00412450
    void clear() { erase(begin(), end()); }
};
template <class T, class Trivial> template <class Tag>
__declspec(noinline) void vector<T, Trivial>::_M_insert_overflow(T* pos, const T& x, const Tag&, unsigned int fillLen, bool atEnd)
{
    StandInSink(pos, fillLen);
}
class CDateAndTimeVector : public vector<CDateAndTime, __true_type> {
public:
    CDateAndTime* erase(CDateAndTime* first, CDateAndTime* last)
    {
        CDateAndTime* i = __copy(last, _M_finish, first, random_access_iterator_tag(), (int*)0);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};
}''', '''namespace std {
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);   // 0x0040E350 (<CDateAndTime*, CDateAndTime*, int>)
template <class _Tp> class allocator { public: allocator() {} };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport 4.6 vector, 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
protected:
    void _M_insert_overflow(_Tp* pos, const _Tp& x, const __false_type&, unsigned int fillLen, bool atEnd);   // 0x00412330 / 0x004125A0
public:
    void push_back(const _Tp& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
    _Tp* erase(_Tp* first, _Tp* last)
    {
        _Tp* i = __copy(last, _M_finish, first, random_access_iterator_tag(), (int*)0);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};
// vector<CWideString>::erase is the out-of-line instantiation 0x00412450 (retail did not inline it here)
template <> CWideString* vector<CWideString>::erase(CWideString* first, CWideString* last);
// Same-TU stand-in body of the two grow-inserts (retail calls them out of line): VC7.1 then sees the tag temporaries'
// addresses do not escape and shares their frame slot (an external declaration costs 4 frame bytes, 0x254 v 0x250).
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* pos, const _Tp& x, const __false_type&, unsigned int fillLen, bool atEnd)
{
    StandInSink(pos, fillLen);
}
}''')
he.edit(a, 'src', '''    void LoadFileList(std::CWideStringVector& names, std::CDateAndTimeVector& times);
};

void CUserProfileManager::LoadFileList(std::CWideStringVector& names, std::CDateAndTimeVector& times)''', '''    void LoadFileList(std::vector<CWideString>& names, std::vector<CDateAndTime>& times);
};

void CUserProfileManager::LoadFileList(std::vector<CWideString>& names, std::vector<CDateAndTime>& times)''')
note(a, F, 'the vectors are STLport vector<T, allocator<T> > (DECISIONS_W26 D7 / W27 D3): erase\n'
     '//   0x00412450 ?erase@?$vector@VCWideString@@V?$allocator@..@std@@@std@@QAEPAVCWideString@@PAV3@0@Z, the grow-inserts 0x00412330 /\n'
     '//   0x004125A0 the protected __false_type members (IAE), the inline CDateAndTime erase\'s __copy 0x0040E350 the template\n'
     '//   ??$__copy@PAVCDateAndTime@@PAV1@H@std@@YI..@Z (0x0059AEAF\'s spelling; 0x0040E350 itself is a folded copy, held). Was\n'
     '//   CWideStringVector / CDateAndTimeVector stand-ins with a same-TU member-template grow-insert body.')
