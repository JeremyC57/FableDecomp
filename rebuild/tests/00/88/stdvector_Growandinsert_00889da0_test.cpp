// 00889da0 N00889da0::vector<Element>::_M_fill_insert (STLport; catalog: _Grow_and_insert)
// Flags: /O2 /Oy /W3
// Leaf: _M_fill_insert
// Layout: PDB CEnvironmentThemePriorityStackEntry56 atlines25599-25608; retail Themes vector12bytes shifts TargetID to20, InitialWeight24, Active28, StartTime32, Duration40; stride48. CThemeWeight12 atPDB25632-25634.
namespace N00889da0 {
struct __false_type { __false_type() {} }; struct random_access_iterator_tag { random_access_iterator_tag() {} };

extern "C" void __cdecl free(void*);
struct ThemeWeight {int ThemeID;float Weight;bool LockWeight;};
struct ThemeVector {ThemeWeight* _M_start;ThemeWeight* _M_finish;ThemeWeight* _M_end_of_storage;
 ThemeVector();ThemeVector(const ThemeVector&);ThemeVector& operator=(const ThemeVector&);
 __forceinline ~ThemeVector(){if(_M_start)free(_M_start);}
};
struct CEnvironmentThemePriorityStackEntry {
 float BaseWeight;bool BaseWeightLocked;ThemeVector Themes;int ThemeTransitionTargetID;float ThemeTransitionInitialWeight;bool ThemeTransitionActive;double ThemeTransitionStartTime;float ThemeTransitionDuration;
 CEnvironmentThemePriorityStackEntry();CEnvironmentThemePriorityStackEntry(const CEnvironmentThemePriorityStackEntry&);CEnvironmentThemePriorityStackEntry& operator=(const CEnvironmentThemePriorityStackEntry&);
};
typedef CEnvironmentThemePriorityStackEntry Element;
void _Construct(Element*,const Element&);
#pragma auto_inline(off)
template<class T> __declspec(noinline) T* __fastcall __uninitialized_copy(T* first,T* last,T* out,const __false_type&) { for(;first!=last;++first,++out) _Construct(out,*first); return out; }
template<class T> __declspec(noinline) T* __fastcall __copy_backward(T* first,T* last,T* out,const random_access_iterator_tag&,int*) { for(int n=int(last-first);n>0;--n) *--out=*--last; return out; }
template<class T> __declspec(noinline) void __fastcall fill(T* first,T* last,const T& x) { for(;first!=last;++first) *first=x; }
template<class T> __declspec(noinline) T* __fastcall __uninitialized_fill_n(T* first,unsigned int n,const T& x,const __false_type&) { for(;n>0;--n,++first) _Construct(first,x); return first; }
#pragma auto_inline(on)
template<class T> __forceinline T* __copy_backward_ptrs(T* first,T* last,T* out,const __false_type&) { return __copy_backward(first,last,out,random_access_iterator_tag(),(int*)0); }
template<class T> struct vector {
 T* _M_start; T* _M_finish; T* _M_end_of_storage;
 void _M_insert_overflow(T* pos,const T& x,const __false_type&,unsigned int n,bool atend=false);
 void _M_fill_insert(T* pos,unsigned int n,const T& x);
};
template<class T> void vector<T>::_M_fill_insert(T* pos,unsigned int n,const T& x) {
 if(n!=0) {
  if((unsigned int)(_M_end_of_storage-_M_finish)>=n) {
   T x_copy=x;
   const unsigned int after=_M_finish-pos;
   T* old_finish=_M_finish;
   if(after>n) {
    __uninitialized_copy(_M_finish-n,_M_finish,_M_finish,__false_type());
    _M_finish+=n;
    __copy_backward_ptrs(pos,old_finish-n,old_finish,__false_type());
    fill(pos,pos+n,x_copy);
   } else {
    __uninitialized_fill_n(_M_finish,n-after,x_copy,__false_type());
    _M_finish+=n-after;
    __uninitialized_copy(pos,old_finish,_M_finish,__false_type());
    _M_finish+=after;
    fill(pos,old_finish,x_copy);
   }
  } else _M_insert_overflow(pos,x,__false_type(),n);
 }
}
template void vector<Element>::_M_fill_insert(Element*,unsigned int,const Element&);

}

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
namespace N00889da0 {
static int overflowCalls;
ThemeVector::ThemeVector():_M_start(0),_M_finish(0),_M_end_of_storage(0){}
ThemeVector::ThemeVector(const ThemeVector& o){unsigned int n=o._M_start?(unsigned int)(o._M_finish-o._M_start):0;_M_start=n?(ThemeWeight*)std::malloc(n*sizeof(ThemeWeight)):0;_M_finish=n?_M_start+n:0;_M_end_of_storage=_M_finish;for(unsigned int i=0;i<n;++i)_M_start[i]=o._M_start[i];}
ThemeVector& ThemeVector::operator=(const ThemeVector& o){if(this!=&o){if(_M_start)std::free(_M_start);unsigned int n=o._M_start?(unsigned int)(o._M_finish-o._M_start):0;_M_start=n?(ThemeWeight*)std::malloc(n*sizeof(ThemeWeight)):0;_M_finish=n?_M_start+n:0;_M_end_of_storage=_M_finish;for(unsigned int i=0;i<n;++i)_M_start[i]=o._M_start[i];}return *this;}
CEnvironmentThemePriorityStackEntry::CEnvironmentThemePriorityStackEntry():BaseWeight(0),BaseWeightLocked(false),ThemeTransitionTargetID(0),ThemeTransitionInitialWeight(0),ThemeTransitionActive(false),ThemeTransitionStartTime(0),ThemeTransitionDuration(0){}
CEnvironmentThemePriorityStackEntry::CEnvironmentThemePriorityStackEntry(const CEnvironmentThemePriorityStackEntry& o):BaseWeight(o.BaseWeight),BaseWeightLocked(o.BaseWeightLocked),Themes(o.Themes),ThemeTransitionTargetID(o.ThemeTransitionTargetID),ThemeTransitionInitialWeight(o.ThemeTransitionInitialWeight),ThemeTransitionActive(o.ThemeTransitionActive),ThemeTransitionStartTime(o.ThemeTransitionStartTime),ThemeTransitionDuration(o.ThemeTransitionDuration){}
CEnvironmentThemePriorityStackEntry& CEnvironmentThemePriorityStackEntry::operator=(const CEnvironmentThemePriorityStackEntry& o){BaseWeight=o.BaseWeight;BaseWeightLocked=o.BaseWeightLocked;Themes=o.Themes;ThemeTransitionTargetID=o.ThemeTransitionTargetID;ThemeTransitionInitialWeight=o.ThemeTransitionInitialWeight;ThemeTransitionActive=o.ThemeTransitionActive;ThemeTransitionStartTime=o.ThemeTransitionStartTime;ThemeTransitionDuration=o.ThemeTransitionDuration;return *this;}
void _Construct(Element* p,const Element& o){new(p)Element(o);}
template<class T>void vector<T>::_M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool){++overflowCalls;}
void Init(Element& e,int n){e.BaseWeight=(float)n;e.Themes._M_start=(ThemeWeight*)std::malloc(sizeof(ThemeWeight));e.Themes._M_finish=e.Themes._M_start+1;e.Themes._M_end_of_storage=e.Themes._M_finish;e.Themes._M_start->ThemeID=n;e.Themes._M_start->Weight=0.5f;e.Themes._M_start->LockWeight=true;}
}
int main(){using namespace N00889da0;Element* a=(Element*)std::malloc(12*sizeof(Element));for(int i=0;i<5;++i){new(a+i)Element();Init(a[i],i+1);}Element x;Init(x,99);vector<Element> v={a,a+5,a+12};v._M_fill_insert(a+1,2,x);int want[]={1,99,99,2,3,4,5};for(int j=0;j<7;++j)if(a[j].BaseWeight!=(float)want[j]||a[j].Themes._M_start->ThemeID!=want[j])return 1;if(a[1].Themes._M_start==x.Themes._M_start||a[1].Themes._M_start==a[2].Themes._M_start||a[1].Themes._M_start->Weight!=0.5f||!a[1].Themes._M_start->LockWeight)return 2;for(int k=0;k<7;++k)a[k].~Element();for(int q=0;q<3;++q){new(a+q)Element();Init(a[q],q+1);}v._M_finish=a+3;v._M_fill_insert(a+2,4,x);if(v._M_finish-a!=7||a[6].BaseWeight!=3.0f)return 3;v._M_fill_insert(a+2,1,a[0]);if(v._M_finish-a!=8||a[2].BaseWeight!=1.0f||a[2].Themes._M_start==a[0].Themes._M_start)return 4;v._M_fill_insert(a,0,x);v._M_end_of_storage=v._M_finish;v._M_fill_insert(a,1,x);if(overflowCalls!=1)return 5;for(int z=0;z<8;++z)a[z].~Element();std::free(a);std::printf("00889da0_TEST PASS\n");return 0;}