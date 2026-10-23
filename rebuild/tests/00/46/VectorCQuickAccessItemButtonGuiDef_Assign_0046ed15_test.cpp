#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
// BEGIN exact source snapshot SHA256 72ebbb61cecf2abf6bd7370bd669cb26d0e196a8ad9897ab81588feb1e9dc1c2
// 0046ed15 Vector<CQuickAccessItemButtonGuiDef>::Assign
// Flags: /O1 /Oy-
// Leaf: Assign
// Layout: retail 0046ed15 reads vector pointers at +0,+4,+8; signed pointer differences divide by 80. Element identity follows the constructor vtable and PDB; donor layout differs.
#include <stddef.h>
#include <new>
// Layout: quarantined donor definition differs from retail; retail element size 80 from idiv and constructor increments.
class CQuickAccessItemButtonGuiDef {
public:
 virtual ~CQuickAccessItemButtonGuiDef();unsigned rest[19];
 CQuickAccessItemButtonGuiDef() {}
 CQuickAccessItemButtonGuiDef(const CQuickAccessItemButtonGuiDef&);
 CQuickAccessItemButtonGuiDef& operator=(const CQuickAccessItemButtonGuiDef&);
};
struct EmptyTag {};
#pragma auto_inline(off)
void __fastcall DestroyRange(CQuickAccessItemButtonGuiDef* first,CQuickAccessItemButtonGuiDef* last,const EmptyTag&){for(;first!=last;++first)first->~CQuickAccessItemButtonGuiDef();}
#pragma auto_inline(on)

template<class T> class Vector {
public:
 T *begin_, *end_, *limit_;
 T* AllocateCopy(unsigned n,const T* first,const T* last);
 void Dispose();
 static T* __fastcall Copy(const T* first,const T* last,T* result,const EmptyTag& tag=EmptyTag());
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
template Vector<CQuickAccessItemButtonGuiDef>& Vector<CQuickAccessItemButtonGuiDef>::Assign(const Vector<CQuickAccessItemButtonGuiDef>&);

// END exact source snapshot
template<class T> T* Vector<T>::AllocateCopy(unsigned n,const T* first,const T* last) { T* out=(T*)malloc(n*sizeof(T)); Construct(first,last,out); return out; }
template<class T> void Vector<T>::Dispose() { free(begin_); }
CQuickAccessItemButtonGuiDef::CQuickAccessItemButtonGuiDef(const CQuickAccessItemButtonGuiDef& r) {for(int i=0;i<19;++i)rest[i]=r.rest[i];}
CQuickAccessItemButtonGuiDef& CQuickAccessItemButtonGuiDef::operator=(const CQuickAccessItemButtonGuiDef& r) {for(int i=0;i<19;++i)rest[i]=r.rest[i];return *this;}
static int destroyed;CQuickAccessItemButtonGuiDef::~CQuickAccessItemButtonGuiDef(){++destroyed;}
template<class T> T* __fastcall Vector<T>::Copy(const T* first,const T* last,T* result,const EmptyTag&) {for(;first!=last;++first,++result)*result=*first;return result;}
int main() {
 CQuickAccessItemButtonGuiDef data[3]; for(int i=0;i<3;++i) {for(int j=0;j<19;++j)data[i].rest[j]=100+i+j;}
 Vector<CQuickAccessItemButtonGuiDef> source={data,data+3,data+3};
 Vector<CQuickAccessItemButtonGuiDef> target={(CQuickAccessItemButtonGuiDef*)malloc(sizeof(CQuickAccessItemButtonGuiDef)),0,0}; target.end_=target.begin_;target.limit_=target.begin_+1;
 assert(&target.Assign(source)==&target); assert(target.end_-target.begin_==3);assert(target.begin_[2].rest[0]==102);assert(target.begin_[1].rest[0]==101);
 CQuickAccessItemButtonGuiDef* saved=target.begin_; target.Assign(target);assert(target.begin_==saved);
 source.end_=data+1;target.Assign(source); assert(target.end_-target.begin_==1);assert(destroyed==2);assert(target.begin_[0].rest[0]==100);
 source.end_=data+3;target.Assign(source); assert(target.end_-target.begin_==3);assert(target.begin_[2].rest[0]==102);
 source.end_=source.begin_;target.Assign(source);assert(target.end_==target.begin_);assert(destroyed==5);free(target.begin_);
 puts("0046ed15_TEST PASS");return 0;
}