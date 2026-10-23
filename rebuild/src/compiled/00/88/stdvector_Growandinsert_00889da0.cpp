// 00889da0 RetailVector::FillInsert
// Flags: /O2 /Oy /W3
// Leaf: FillInsert
// Layout: PDB CEnvironmentThemePriorityStackEntry56 atlines25599-25608; retail Themes vector12bytes shifts TargetID to20, InitialWeight24, Active28, StartTime32, Duration40; stride48. CThemeWeight12 atPDB25632-25634.
namespace N00889da0 {
struct FalseTag { FalseTag() {} }; struct RandomTag { RandomTag() {} };

extern "C" void __cdecl free(void*);
struct ThemeWeight {int ThemeID;float Weight;bool LockWeight;};
struct ThemeVector {ThemeWeight* Start;ThemeWeight* Finish;ThemeWeight* End;
 ThemeVector();ThemeVector(const ThemeVector&);ThemeVector& operator=(const ThemeVector&);
 __forceinline ~ThemeVector(){if(Start)free(Start);}
};
struct CEnvironmentThemePriorityStackEntry {
 float BaseWeight;bool BaseWeightLocked;ThemeVector Themes;int ThemeTransitionTargetID;float ThemeTransitionInitialWeight;bool ThemeTransitionActive;double ThemeTransitionStartTime;float ThemeTransitionDuration;
 CEnvironmentThemePriorityStackEntry();CEnvironmentThemePriorityStackEntry(const CEnvironmentThemePriorityStackEntry&);CEnvironmentThemePriorityStackEntry& operator=(const CEnvironmentThemePriorityStackEntry&);
};
typedef CEnvironmentThemePriorityStackEntry Element;
void Construct(Element*,const Element&);
#pragma auto_inline(off)
template<class T> __declspec(noinline) T* __fastcall UninitCopy(T* first,T* last,T* out,const FalseTag&) { for(;first!=last;++first,++out) Construct(out,*first); return out; }
template<class T> __declspec(noinline) T* __fastcall CopyBackward(T* first,T* last,T* out,const RandomTag&,int*) { for(int n=int(last-first);n>0;--n) *--out=*--last; return out; }
template<class T> __declspec(noinline) void __fastcall Fill(T* first,T* last,const T& x) { for(;first!=last;++first) *first=x; }
template<class T> __declspec(noinline) T* __fastcall UninitFill(T* first,unsigned int n,const T& x,const FalseTag&) { for(;n>0;--n,++first) Construct(first,x); return first; }
#pragma auto_inline(on)
template<class T> __forceinline T* CopyBack(T* first,T* last,T* out,const FalseTag&) { return CopyBackward(first,last,out,RandomTag(),(int*)0); }
template<class T> struct RetailVector {
 T* Start; T* Finish; T* End;
 void Overflow(T* pos,const T& x,const FalseTag&,unsigned int n,bool atend=false);
 void FillInsert(T* pos,unsigned int n,const T& x);
};
template<class T> void RetailVector<T>::FillInsert(T* pos,unsigned int n,const T& x) {
 if(n!=0) {
  if((unsigned int)(End-Finish)>=n) {
   T x_copy=x;
   const unsigned int after=Finish-pos;
   T* old_finish=Finish;
   if(after>n) {
    UninitCopy(Finish-n,Finish,Finish,FalseTag());
    Finish+=n;
    CopyBack(pos,old_finish-n,old_finish,FalseTag());
    Fill(pos,pos+n,x_copy);
   } else {
    UninitFill(Finish,n-after,x_copy,FalseTag());
    Finish+=n-after;
    UninitCopy(pos,old_finish,Finish,FalseTag());
    Finish+=after;
    Fill(pos,old_finish,x_copy);
   }
  } else Overflow(pos,x,FalseTag(),n);
 }
}
template void RetailVector<Element>::FillInsert(Element*,unsigned int,const Element&);

}