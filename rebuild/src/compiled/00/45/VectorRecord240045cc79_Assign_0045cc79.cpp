// 0045cc79 Vector<Record24_0045cc79>::Assign
// Flags: /O1 /Oy-
// Leaf: Assign
// Layout: retail 0045cc79 reads vector pointers at +0,+4,+8; signed pointer differences divide by 24. Element identity remains uncertain.
#include <stddef.h>
#include <new>
struct Record24_0045cc79 {
 unsigned first; unsigned rest[5];
 Record24_0045cc79() {}
};
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
    Copy(other.begin_,other.end_,begin_);
   } else {
    Copy(other.begin_,other.begin_+(end_-begin_),begin_);
    Construct(other.begin_+(end_-begin_),other.end_,end_);
   }
   end_=begin_+n;
  }
  return *this;
 }
};
template Vector<Record24_0045cc79>& Vector<Record24_0045cc79>::Assign(const Vector<Record24_0045cc79>&);