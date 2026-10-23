#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
// BEGIN exact source snapshot SHA256 63e3a8f46903a6aef1931de0bcb4528e2c1aef2fa0bfd0717da2b0565baa563a
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

// END exact source snapshot
template<class T> T* Vector<T>::AllocateCopy(unsigned n,const T* first,const T* last) { T* out=(T*)malloc(n*sizeof(T)); Construct(first,last,out); return out; }
template<class T> void Vector<T>::Dispose() { free(begin_); }
int main() {
 Record24_0045cc79 data[3]; for(int i=0;i<3;++i) {data[i].first=10+i;for(int j=0;j<5;++j)data[i].rest[j]=100+i+j;}
 Vector<Record24_0045cc79> source={data,data+3,data+3};
 Vector<Record24_0045cc79> target={(Record24_0045cc79*)malloc(sizeof(Record24_0045cc79)),0,0}; target.end_=target.begin_;target.limit_=target.begin_+1;
 assert(&target.Assign(source)==&target); assert(target.end_-target.begin_==3);assert(target.begin_[2].first==12);assert(target.begin_[1].rest[0]==101);
 Record24_0045cc79* saved=target.begin_; target.Assign(target);assert(target.begin_==saved);
 source.end_=data+1;target.Assign(source); assert(target.end_-target.begin_==1);assert(target.begin_[0].first==10);
 source.end_=data+3;target.Assign(source); assert(target.end_-target.begin_==3);assert(target.begin_[2].rest[0]==102);
 source.end_=source.begin_;target.Assign(source);assert(target.end_==target.begin_);free(target.begin_);
 puts("0045cc79_TEST PASS");return 0;
}