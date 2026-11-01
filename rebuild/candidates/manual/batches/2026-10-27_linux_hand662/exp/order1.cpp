#include "engine/CCountedPointer.h"
class CCharString { public:
    CCharString(); CCharString(const char*); CCharString(const CCharString&); ~CCharString();
    CCharString& operator=(const CCharString&);
    void* PStringData; };
namespace NCreatureAction { class CActionEventBase { public: virtual ~CActionEventBase(); }; }
namespace std {
template <class _T1, class _T2> struct pair {
  typedef _T1 first_type; typedef _T2 second_type;
  _T1 first; _T2 second;
  pair() : first(_T1()), second(_T2()) {}
  pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
  template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}
  pair(const pair<_T1,_T2>& __o) : first(__o.first), second(__o.second) {}
};
template <class _T1, class _T2> inline pair<_T1, _T2> make_pair(const _T1& __x, const _T2& __y) { return pair<_T1, _T2>(__x, __y); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer; __pointer->~_Tp(); }
}
typedef std::pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > VT;
struct CMapLike { void insert(const VT& v); void erase_node(VT* p) { std::_Destroy(p); } };
extern CMapLike GMap;
void RegisterActionEventTypes()
{
    GMap.insert(std::make_pair(CCharString("a"), CCountedPointer<NCreatureAction::CActionEventBase>(0)));
}
void Later(VT* p) { GMap.erase_node(p); }
