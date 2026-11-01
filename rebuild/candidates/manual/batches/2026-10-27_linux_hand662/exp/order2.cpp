#include "engine/CCountedPointer.h"
class CCharString { public:
    CCharString(); CCharString(const CCharString&); ~CCharString();
    CCharString& operator=(const CCharString&); bool operator<(const CCharString&) const;
    void* PStringData; };
class CEntityScriptBindingBase; class CActiveEntityScriptBase;
namespace std {
template <class _T1, class _T2> struct pair {
  _T1 first; _T2 second;
  pair() : first(_T1()), second(_T2()) {}
  pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer; __pointer->~_Tp(); }
template <class _It> inline void __destroy_aux(_It f, _It l) { for (; f != l; ++f) _Destroy(&*f); }
template <class _It> inline void _Destroy(_It f, _It l) { __destroy_aux(f, l); }
template <class _Tp> class vector { public:
  _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end;
  ~vector() { _Destroy(_M_start, _M_finish); }
};
template <class _It, class _Tp> void __linear_insert(_It first, _It last, _Tp*) {
  _Tp __val = *last; if (__val.first < first->first) { *first = __val; } }
}
typedef std::pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > PA;
typedef std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > PB;
struct CScriptBaseLike { std::vector<PA> a; std::vector<PB> b; ~CScriptBaseLike(); void Sort(); };
CScriptBaseLike::~CScriptBaseLike() {}
void CScriptBaseLike::Sort() { std::__linear_insert(b._M_start, b._M_finish, (PB*)0); }
