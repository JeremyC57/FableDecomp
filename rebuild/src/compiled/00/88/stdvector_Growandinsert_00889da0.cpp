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