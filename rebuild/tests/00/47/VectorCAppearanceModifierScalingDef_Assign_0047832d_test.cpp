#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
// BEGIN exact source snapshot SHA256 1ddf978b65efd68c9dfd1a1d4be0e4e12b8a9a64b7401623e2da1d313f9bcbca
// 0047832d Vector<CAppearanceModifierScalingDef>::Assign
// Flags: /O1 /Oy-
// Leaf: Assign
// Layout: retail 0047832d reads vector pointers at +0,+4,+8; signed pointer differences divide by 72. Element identity follows the constructor vtable and PDB; donor layout differs.
#include <stddef.h>
#include <new>
// Layout: quarantined donor definition differs from retail; retail element size 72 from idiv and constructor increments.
class CAppearanceModifierScalingDef {
public:
 virtual ~CAppearanceModifierScalingDef();unsigned rest[17];
 CAppearanceModifierScalingDef() {}
 CAppearanceModifierScalingDef(const CAppearanceModifierScalingDef&);
 CAppearanceModifierScalingDef& operator=(const CAppearanceModifierScalingDef&);
};
struct EmptyTag {};
#pragma auto_inline(off)
void __fastcall DestroyRange(CAppearanceModifierScalingDef* first,CAppearanceModifierScalingDef* last,const EmptyTag&){for(;first!=last;++first)first->~CAppearanceModifierScalingDef();}
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
template Vector<CAppearanceModifierScalingDef>& Vector<CAppearanceModifierScalingDef>::Assign(const Vector<CAppearanceModifierScalingDef>&);

// END exact source snapshot
template<class T> T* Vector<T>::AllocateCopy(unsigned n,const T* first,const T* last) { T* out=(T*)malloc(n*sizeof(T)); Construct(first,last,out); return out; }
template<class T> void Vector<T>::Dispose() { free(begin_); }
CAppearanceModifierScalingDef::CAppearanceModifierScalingDef(const CAppearanceModifierScalingDef& r) {for(int i=0;i<17;++i)rest[i]=r.rest[i];}
CAppearanceModifierScalingDef& CAppearanceModifierScalingDef::operator=(const CAppearanceModifierScalingDef& r) {for(int i=0;i<17;++i)rest[i]=r.rest[i];return *this;}
static int destroyed;CAppearanceModifierScalingDef::~CAppearanceModifierScalingDef(){++destroyed;}
int main() {
 CAppearanceModifierScalingDef data[3]; for(int i=0;i<3;++i) {for(int j=0;j<17;++j)data[i].rest[j]=100+i+j;}
 Vector<CAppearanceModifierScalingDef> source={data,data+3,data+3};
 Vector<CAppearanceModifierScalingDef> target={(CAppearanceModifierScalingDef*)malloc(sizeof(CAppearanceModifierScalingDef)),0,0}; target.end_=target.begin_;target.limit_=target.begin_+1;
 assert(&target.Assign(source)==&target); assert(target.end_-target.begin_==3);assert(target.begin_[2].rest[0]==102);assert(target.begin_[1].rest[0]==101);
 CAppearanceModifierScalingDef* saved=target.begin_; target.Assign(target);assert(target.begin_==saved);
 source.end_=data+1;target.Assign(source); assert(target.end_-target.begin_==1);assert(destroyed==2);assert(target.begin_[0].rest[0]==100);
 source.end_=data+3;target.Assign(source); assert(target.end_-target.begin_==3);assert(target.begin_[2].rest[0]==102);
 source.end_=source.begin_;target.Assign(source);assert(target.end_==target.begin_);assert(destroyed==5);free(target.begin_);
 puts("0047832d_TEST PASS");return 0;
}