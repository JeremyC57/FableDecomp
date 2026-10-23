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

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
namespace N00889da0 {
static int overflowCalls;
ThemeVector::ThemeVector():Start(0),Finish(0),End(0){}
ThemeVector::ThemeVector(const ThemeVector& o){unsigned int n=o.Start?(unsigned int)(o.Finish-o.Start):0;Start=n?(ThemeWeight*)std::malloc(n*sizeof(ThemeWeight)):0;Finish=n?Start+n:0;End=Finish;for(unsigned int i=0;i<n;++i)Start[i]=o.Start[i];}
ThemeVector& ThemeVector::operator=(const ThemeVector& o){if(this!=&o){if(Start)std::free(Start);unsigned int n=o.Start?(unsigned int)(o.Finish-o.Start):0;Start=n?(ThemeWeight*)std::malloc(n*sizeof(ThemeWeight)):0;Finish=n?Start+n:0;End=Finish;for(unsigned int i=0;i<n;++i)Start[i]=o.Start[i];}return *this;}
CEnvironmentThemePriorityStackEntry::CEnvironmentThemePriorityStackEntry():BaseWeight(0),BaseWeightLocked(false),ThemeTransitionTargetID(0),ThemeTransitionInitialWeight(0),ThemeTransitionActive(false),ThemeTransitionStartTime(0),ThemeTransitionDuration(0){}
CEnvironmentThemePriorityStackEntry::CEnvironmentThemePriorityStackEntry(const CEnvironmentThemePriorityStackEntry& o):BaseWeight(o.BaseWeight),BaseWeightLocked(o.BaseWeightLocked),Themes(o.Themes),ThemeTransitionTargetID(o.ThemeTransitionTargetID),ThemeTransitionInitialWeight(o.ThemeTransitionInitialWeight),ThemeTransitionActive(o.ThemeTransitionActive),ThemeTransitionStartTime(o.ThemeTransitionStartTime),ThemeTransitionDuration(o.ThemeTransitionDuration){}
CEnvironmentThemePriorityStackEntry& CEnvironmentThemePriorityStackEntry::operator=(const CEnvironmentThemePriorityStackEntry& o){BaseWeight=o.BaseWeight;BaseWeightLocked=o.BaseWeightLocked;Themes=o.Themes;ThemeTransitionTargetID=o.ThemeTransitionTargetID;ThemeTransitionInitialWeight=o.ThemeTransitionInitialWeight;ThemeTransitionActive=o.ThemeTransitionActive;ThemeTransitionStartTime=o.ThemeTransitionStartTime;ThemeTransitionDuration=o.ThemeTransitionDuration;return *this;}
void Construct(Element* p,const Element& o){new(p)Element(o);}
template<class T>void RetailVector<T>::Overflow(T*,const T&,const FalseTag&,unsigned int,bool){++overflowCalls;}
void Init(Element& e,int n){e.BaseWeight=(float)n;e.Themes.Start=(ThemeWeight*)std::malloc(sizeof(ThemeWeight));e.Themes.Finish=e.Themes.Start+1;e.Themes.End=e.Themes.Finish;e.Themes.Start->ThemeID=n;e.Themes.Start->Weight=0.5f;e.Themes.Start->LockWeight=true;}
}
int main(){using namespace N00889da0;Element* a=(Element*)std::malloc(12*sizeof(Element));for(int i=0;i<5;++i){new(a+i)Element();Init(a[i],i+1);}Element x;Init(x,99);RetailVector<Element> v={a,a+5,a+12};v.FillInsert(a+1,2,x);int want[]={1,99,99,2,3,4,5};for(int j=0;j<7;++j)if(a[j].BaseWeight!=(float)want[j]||a[j].Themes.Start->ThemeID!=want[j])return 1;if(a[1].Themes.Start==x.Themes.Start||a[1].Themes.Start==a[2].Themes.Start||a[1].Themes.Start->Weight!=0.5f||!a[1].Themes.Start->LockWeight)return 2;for(int k=0;k<7;++k)a[k].~Element();for(int q=0;q<3;++q){new(a+q)Element();Init(a[q],q+1);}v.Finish=a+3;v.FillInsert(a+2,4,x);if(v.Finish-a!=7||a[6].BaseWeight!=3.0f)return 3;v.FillInsert(a+2,1,a[0]);if(v.Finish-a!=8||a[2].BaseWeight!=1.0f||a[2].Themes.Start==a[0].Themes.Start)return 4;v.FillInsert(a,0,x);v.End=v.Finish;v.FillInsert(a,1,x);if(overflowCalls!=1)return 5;for(int z=0;z<8;++z)a[z].~Element();std::free(a);std::printf("00889da0_TEST PASS\n");return 0;}