#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
// BEGIN exact source snapshot SHA256 167414e83bad68ab0b341d9816a53700dfabcc43130ffa1c77917449ea61222a
// 0047857d Vector<CAbilityTrackDef>::Assign
// Flags: /O1 /Oy-
// Leaf: Assign
// Layout: retail 0047857d reads vector pointers at +0,+4,+8; signed pointer differences divide by 80. Element identity follows the constructor vtable and PDB; donor layout differs.
#include <stddef.h>
#include <new>
// Layout: quarantined donor definition differs from retail; retail element size 80 from idiv and constructor increments.
class CAbilityTrackDef {
public:
 virtual ~CAbilityTrackDef();unsigned rest[19];
 CAbilityTrackDef() {}
 CAbilityTrackDef(const CAbilityTrackDef&);
 CAbilityTrackDef& operator=(const CAbilityTrackDef&);
};
struct EmptyTag {};
#pragma auto_inline(off)
void __fastcall DestroyRange(CAbilityTrackDef* first,CAbilityTrackDef* last,const EmptyTag&){for(;first!=last;++first)first->~CAbilityTrackDef();}
#pragma auto_inline(on)

template<class T> class Vector {
public:
 T *begin_, *end_, *limit_;
 T* AllocateCopy(unsigned n,const T* first,const T* last);
 void Dispose();
 static __forceinline T* Copy(const T* first,const T* last,T* result) {
  for(ptrdiff_t n=last-first;n>0;--n,++first,++result) *result=*first;
  return result;
 }
 static __forceinline T* Construct(const T* first,const T* last,T* result) {
  T* current=result;
  for(;first!=last;++first,++current) if(current) new(current) T(*first);
  return current;
 }
 Vector& Assign(const Vector& other) {
  if(&other!=this) {
   const unsigned n=other.end_-other.begin_;
   if(n>unsigned(limit_-begin_)) {
    T* fresh=AllocateCopy(n,other.begin_,other.end_); Dispose(); begin_=fresh;limit_=begin_+n;
   } else if(unsigned(end_-begin_)>=n) {
    T* last=Copy(other.begin_,other.end_,begin_); DestroyRange(last,end_,EmptyTag());
   } else {
    Copy(other.begin_,other.begin_+(end_-begin_),begin_);
    Construct(other.begin_+(end_-begin_),other.end_,end_);
   }
   end_=begin_+n;
  }
  return *this;
 }
};
template Vector<CAbilityTrackDef>& Vector<CAbilityTrackDef>::Assign(const Vector<CAbilityTrackDef>&);

// END exact source snapshot
template<class T> T* Vector<T>::AllocateCopy(unsigned n,const T* first,const T* last) { T* out=(T*)malloc(n*sizeof(T)); Construct(first,last,out); return out; }
template<class T> void Vector<T>::Dispose() { free(begin_); }
CAbilityTrackDef::CAbilityTrackDef(const CAbilityTrackDef& r) {for(int i=0;i<19;++i)rest[i]=r.rest[i];}
CAbilityTrackDef& CAbilityTrackDef::operator=(const CAbilityTrackDef& r) {for(int i=0;i<19;++i)rest[i]=r.rest[i];return *this;}
static int destroyed;CAbilityTrackDef::~CAbilityTrackDef(){++destroyed;}
int main() {
 CAbilityTrackDef data[3]; for(int i=0;i<3;++i) {for(int j=0;j<19;++j)data[i].rest[j]=100+i+j;}
 Vector<CAbilityTrackDef> source={data,data+3,data+3};
 Vector<CAbilityTrackDef> target={(CAbilityTrackDef*)malloc(sizeof(CAbilityTrackDef)),0,0}; target.end_=target.begin_;target.limit_=target.begin_+1;
 assert(&target.Assign(source)==&target); assert(target.end_-target.begin_==3);assert(target.begin_[2].rest[0]==102);assert(target.begin_[1].rest[0]==101);
 CAbilityTrackDef* saved=target.begin_; target.Assign(target);assert(target.begin_==saved);
 source.end_=data+1;target.Assign(source); assert(target.end_-target.begin_==1);assert(destroyed==2);assert(target.begin_[0].rest[0]==100);
 source.end_=data+3;target.Assign(source); assert(target.end_-target.begin_==3);assert(target.begin_[2].rest[0]==102);
 source.end_=source.begin_;target.Assign(source);assert(target.end_==target.begin_);assert(destroyed==5);free(target.begin_);
 puts("0047857d_TEST PASS");return 0;
}