// Behaviour test for std::vector<std::pair<long, CIntelligentPointer<CThing> > >::_M_insert_overflow (0x006a3a70): the STLport 4.6 _M_insert_overflow body (the shared header's text) over a fixture element whose
// intelligent pointer counts live references (copy = SetPointer(GetPointer()), destructor releases), run for every old size 0..6, every
// insert position, fill_len 1..3 and both atend values: the new block must hold head + fill_len copies of x + tail (unless atend),
// capacity old + max(old, fill_len), the old block freed and every reference released, with no leaked or double-released pointer.
#include <stdio.h>
#include <stdlib.h>
#include <new>
static int failures; static void check(bool ok, const char* what) { if (!ok) { ++failures; printf("FAIL: %s\n", what); } }
class CThing { public: int refs; int id; };
static int live_pointers;
class CBaseIntelligentPointer { public: CThing* PItem;
    CBaseIntelligentPointer() : PItem(0) { ++live_pointers; }
    ~CBaseIntelligentPointer() { if (PItem) --PItem->refs; --live_pointers; }
    CThing* GetPItem() const { return PItem; }
    void SetPItem(CThing* p) { if (PItem) --PItem->refs; PItem = p; if (PItem) ++PItem->refs; } };
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() {}
    CIntelligentPointer(const CIntelligentPointer& item) { SetPointer(item.GetPointer()); }
    T* GetPointer() const { return static_cast<T*>(GetPItem()); }
    void SetPointer(T* p) { SetPItem(p); } };
template<class T1, class T2> struct pair { T1 first; T2 second; pair() {} pair(const pair& o) : first(o.first), second(o.second) {} };
typedef pair<long, CIntelligentPointer<CThing> > E;
#define SETKEY(e, k) ((e).first = (k))
#define GETKEY(e) ((e).first)
static int mallocs, frees;
struct __false_type { };
template<class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template<class T1, class T2> inline void _Construct(T1* p, const T2& v) { new ((void*)p) T1(v); }
template<class I,class O> O __uninitialized_copy(I first,I last,O result,const __false_type&) { O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur; }
template<class F,class S,class T> F __uninitialized_fill_n(F first,S n,const T& x,const __false_type&) { F cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur; }
template<class _Tp> class vector { public:
    _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage;
    typedef unsigned int size_type; typedef _Tp* iterator;
    size_type size() const { return size_type(_M_finish - _M_start); }
    _Tp* allocate(size_type n) { ++mallocs; return n != 0 ? (_Tp*)malloc(n * sizeof(_Tp)) : 0; }
    void _M_clear() { for (_Tp* p = _M_start; p != _M_finish; ++p) p->~_Tp(); if (_M_start) { ++frees; free(_M_start); } }
    void _M_set(iterator s, iterator f, iterator e) { _M_start = s; _M_finish = f; _M_end_of_storage = e; }
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd);
};
template<class _Tp> void vector<_Tp>::_M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
    const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
    _Tp* newStart=allocate(len);_Tp* newFinish=newStart;
    newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
    if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
    else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
    if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
    _M_clear();_M_set(newStart,newFinish,newStart+len);
}
int main()
{
    long cases = 0; CThing things[16];
    for (int old = 0; old <= 6; ++old) for (int pos = 0; pos <= old; ++pos) for (unsigned int fill = 1; fill <= 3; ++fill) for (int atend = 0; atend < 2; ++atend) {
        if (atend && pos != old) continue;
        for (int i = 0; i < 16; ++i) { things[i].refs = 0; things[i].id = i; }
        live_pointers = 0; mallocs = frees = 0;
        vector<E> v; v.size(); v._M_start = old ? (E*)malloc(old * sizeof(E)) : 0; v._M_finish = v._M_start; v._M_end_of_storage = v._M_start + old;
        for (int i = 0; i < old; ++i) { new ((void*)v._M_finish) E(); SETKEY(*v._M_finish, 100 + i); v._M_finish->second.SetPItem(&things[i]); ++v._M_finish; }
        { E x; SETKEY(x, 999); x.second.SetPItem(&things[15]);
          v._M_insert_overflow(v._M_start + pos, x, __false_type(), fill, atend != 0);
          unsigned int want = old + (old > (int)fill ? old : fill);
          check(v._M_end_of_storage - v._M_start == (int)want, "capacity");
          check((int)v.size() == old + (int)fill, "size");
          check(mallocs == 1 && frees == (old ? 1 : 0), "alloc / free");
          for (int i = 0; i < (int)v.size(); ++i) {
              int src = i < pos ? i : (i < pos + (int)fill ? -1 : i - (int)fill);
              if (src < 0) check(GETKEY(v._M_start[i]) == 999 && v._M_start[i].second.GetPItem() == &things[15], "fill element");
              else check(GETKEY(v._M_start[i]) == 100 + src && v._M_start[i].second.GetPItem() == &things[src], "moved element");
          }
          for (int i = 0; i < old; ++i) check(things[i].refs == 1, "old element references released");
          check(things[15].refs == 1 + (int)fill, "fill references");
        }
        check(things[15].refs == (int)fill, "x released");
        for (E* p = v._M_start; p != v._M_finish; ++p) p->~E();
        free(v._M_start);
        check(live_pointers == 0, "no leaked pointer");
        for (int i = 0; i < 16; ++i) check(things[i].refs == 0, "all released");
        ++cases;
    }
    printf("%ld cases\n", cases);
    if (failures) { printf("%d failures\n", failures); return 1; }
    puts("006a3a70_TEST PASS");
    return 0;
}