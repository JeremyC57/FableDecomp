#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE __declspec(noinline)
#endif
class C3DVector { public: float X,Y,Z; C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z) {} C3DVector(const C3DVector& p):X(p.X),Y(p.Y),Z(p.Z) {} C3DVector operator-(const C3DVector& p) const { return C3DVector(X-p.X,Y-p.Y,Z-p.Z); } float GetLengthSquared() const { return X*X+Y*Y+Z*Z; } };
class CThing { public: const C3DVector& GetPosition() const; }; class CThingObject:public CThing {};
class CThingCompare_Nearest { public: C3DVector Pos; template<class T> NOINLINE T* operator()(T* a,T* b) const { if(!a) return b; if(!b) return a; C3DVector da=a->GetPosition()-Pos; C3DVector db=b->GetPosition()-Pos; return da.GetLengthSquared()<db.GetLengthSquared()?a:b; } };
typedef const CThingObject* Element;
namespace std {
template<class Iter,class Distance,class T,class Compare> NOINLINE void __fastcall __push_heap(Iter first,Distance hole,Distance top,T value,Compare comp) { Distance parent=(hole-1)/2; while(hole>top && comp(*(first+parent),value)) { *(first+hole)=*(first+parent); hole=parent; parent=(hole-1)/2; } *(first+hole)=value; }
template<class Iter,class Distance,class T,class Compare> NOINLINE void __fastcall __adjust_heap(Iter first,Distance hole,Distance length,T value,Compare comp) { Distance top=hole; Distance secondChild=2*hole+2; while(secondChild<length) { if(comp(*(first+secondChild),*(first+secondChild-1))) --secondChild; *(first+hole)=*(first+secondChild); hole=secondChild; secondChild=2*(secondChild+1); } if(secondChild==length) { *(first+hole)=*(first+secondChild-1); hole=secondChild-1; } __push_heap(first,hole,top,value,comp); }
template<class Iter,class Compare,class T,class Distance> void __fastcall __make_heap(Iter first,Iter last,Compare comp,T*,Distance*) { if(last-first<2) return; Distance length=last-first; Distance parent=(length-2)/2; while(true) { __adjust_heap(first,parent,length,T(*(first+parent)),comp); if(parent==0) return; --parent; } }
template<class Iter,class T,class Compare> inline void __fastcall __unguarded_linear_insert(Iter last,T value,Compare comp) { Iter next=last; --next; while(comp(value,*next)) { *last=*next; last=next; --next; } *last=value; }
template<class Iter,class T,class Compare> void __fastcall __unguarded_insertion_sort_aux(Iter first,Iter last,T*,Compare comp) { for(Iter i=first;i!=last;++i) __unguarded_linear_insert(i,T(*i),comp); }
template<class T> inline T* copy_backward(T* first,T* last,T* result) { long bytes=(const char*)last-(const char*)first; if(bytes>0) memmove((char*)result-bytes,first,bytes); return result-(last-first); }
template<class Iter,class Compare> void __fastcall __insertion_sort(Iter first,Iter last,Compare comp) { if(first==last) return; for(Iter i=first+1;i!=last;++i) { Element value=*i; if(comp(value,*first)) { copy_backward(first,i,i+1); *first=value; } else __unguarded_linear_insert(i,value,comp); } }
template<class Iter,class Compare> void __fastcall __final_insertion_sort(Iter first,Iter last,Compare comp) { if(last-first>16) { __insertion_sort(first,first+16,comp); __unguarded_insertion_sort_aux(first+16,last,(Element*)0,comp); } else __insertion_sort(first,last,comp); }
template<class T,class Compare> NOINLINE const T& __fastcall __median(const T& a,const T& b,const T& c,Compare comp) { if(comp(a,b)) { if(comp(b,c)) return b; else if(comp(a,c)) return c; else return a; } else if(comp(a,c)) return a; else if(comp(b,c)) return c; else return b; }
template<class Iter,class T,class Compare> NOINLINE Iter __fastcall __unguarded_partition(Iter first,Iter last,T pivot,Compare comp) { while(true) { while(comp(*first,pivot)) ++first; --last; while(comp(pivot,*last)) --last; if(!(first<last)) return first; T value=*first; *first=*last; *last=value; ++first; } }
template<class Iter,class Compare> NOINLINE void __fastcall sort_heap(Iter first,Iter last,Compare comp) { while(last-first>1) { --last; Element value=*last; *last=*first; __adjust_heap(first,0,int(last-first),value,comp); } }
template<class Iter,class T,class Compare> NOINLINE void __fastcall __partial_sort(Iter first,Iter middle,Iter last,T*,Compare comp) { __make_heap(first,middle,comp,(T*)0,(int*)0); for(Iter i=middle;i<last;++i) if(comp(*i,*first)) { T value=*i; *i=*first; __adjust_heap(first,0,int(middle-first),value,comp); } sort_heap(first,middle,comp); }
template<class Iter,class T,class Size,class Compare> void __fastcall __introsort_loop(Iter first,Iter last,T*,Size depth,Compare comp) { while(last-first>16) { if(depth==0) { __partial_sort(first,last,last,(T*)0,comp); return; } --depth; Iter cut=__unguarded_partition(first,last,T(__median(*first,*(first+(last-first)/2),*(last-1),comp)),comp); __introsort_loop(cut,last,(T*)0,depth,comp); last=cut; } }
}

template void __fastcall std::__final_insertion_sort<Element*,CThingCompare_Nearest>(Element*,Element*,CThingCompare_Nearest);
class TestThing:public CThingObject { public: C3DVector Position; TestThing(float x):Position(x,0,0) {} };
static int positionCalls; const C3DVector& CThing::GetPosition() const { ++positionCalls; return static_cast<const TestThing*>(this)->Position; }
int main() { CThingCompare_Nearest comp; comp.Pos=C3DVector(0,0,0); TestThing a(3),b(2),c(1); Element values[24]; for(int i=0;i<24;++i) values[i]=0;
values[0]=&a; values[1]=&b; values[2]=&c; values[3]=0; std::__final_insertion_sort(values,values+4,comp); if(values[0] || values[1]!=&c || values[2]!=&b || values[3]!=&a) goto fail;
for(int k=0;k<24;++k) values[k]=0; std::__final_insertion_sort(values,values+24,comp); for(int q=0;q<24;++q) if(values[q]) goto fail;
std::__final_insertion_sort(values,values,comp); puts("00920480_TEST PASS"); return 0; fail: puts("00920480_TEST FAIL"); return 1; }