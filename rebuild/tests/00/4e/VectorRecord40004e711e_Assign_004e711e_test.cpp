#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
// BEGIN exact source snapshot SHA256 8e2b1e350b954a81b9e2a2d83be832e02a9b940af32a04789d4ba90e1637636a
// 004e711e Vector<Record40_004e711e>::Assign
// Flags: /O1 /Oy-
// Leaf: Assign
// Layout: retail 004e711e reads vector pointers at +0,+4,+8; signed pointer differences divide by 40. Element identity remains uncertain.
#include <stddef.h>
#include <new>
struct Payload_004e711e {unsigned word[9];};
struct Record40_004e711e {
 unsigned first; Payload_004e711e payload;
 Record40_004e711e() {}
 __forceinline Record40_004e711e(const Record40_004e711e& other):first(other.first),payload(other.payload) {}
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
template Vector<Record40_004e711e>& Vector<Record40_004e711e>::Assign(const Vector<Record40_004e711e>&);

// END exact source snapshot
template<class T> T* Vector<T>::AllocateCopy(unsigned n,const T* first,const T* last) { T* out=(T*)malloc(n*sizeof(T)); Construct(first,last,out); return out; }
template<class T> void Vector<T>::Dispose() { free(begin_); }
int main() {
 Record40_004e711e data[3]; for(int i=0;i<3;++i) {data[i].first=10+i;for(int j=0;j<9;++j)data[i].payload.word[j]=100+i+j;}
 Vector<Record40_004e711e> source={data,data+3,data+3};
 Vector<Record40_004e711e> target={(Record40_004e711e*)malloc(sizeof(Record40_004e711e)),0,0}; target.end_=target.begin_;target.limit_=target.begin_+1;
 assert(&target.Assign(source)==&target); assert(target.end_-target.begin_==3);assert(target.begin_[2].first==12);assert(target.begin_[1].payload.word[0]==101);
 Record40_004e711e* saved=target.begin_; target.Assign(target);assert(target.begin_==saved);
 source.end_=data+1;target.Assign(source); assert(target.end_-target.begin_==1);assert(target.begin_[0].first==10);
 source.end_=data+3;target.Assign(source); assert(target.end_-target.begin_==3);assert(target.begin_[2].payload.word[0]==102);
 source.end_=source.begin_;target.Assign(source);assert(target.end_==target.begin_);free(target.begin_);
 puts("004e711e_TEST PASS");return 0;
}