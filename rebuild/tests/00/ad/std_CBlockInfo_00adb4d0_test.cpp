#include <stdio.h>
#include <stdlib.h>
// 00adb4d0 std::__pop_heap_aux
// Leaf: ??$__pop_heap_aux@PAVCBlockInfo@CVertexGroupBlockOptimiser@@V12@U?$less@VCBlockInfo@CVertexGroupBlockOptimiser@@@std@@@std@@YIXPAVCBlockInfo@CVertexGroupBlockOptimiser@@00U?$less@VCBlockInfo@CVertexGroupBlockOptimiser@@@0@@Z
// Flags:
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
#endif
// Layout: PDB CBlockInfo has BlockIndex+0, Cost+4, Benefit+8; retail moves all 12 bytes and compares Cost.
// No engine header exists for this nested record or its naming-scope outer class.
class CVertexGroupBlockOptimiser { public: class CBlockInfo { public: unsigned long BlockIndex; long Cost,Benefit; bool operator<(const CBlockInfo&other)const{return Cost<other.Cost;} }; };
namespace std {
template<class A,class B,class R>struct binary_function {typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class Iterator,class Distance,class T,class Compare>void __fastcall __adjust_heap(Iterator,Distance,Distance,T,Compare);
template<class Iterator,class T,class Compare,class Distance>inline void __fastcall __pop_heap(Iterator first,Iterator last,Iterator result,T value,Compare comp,Distance*){*result=*first;__adjust_heap(first,Distance(0),Distance(last-first),value,comp);}
template<class Iterator,class T,class Compare>inline void __fastcall __pop_heap_aux(Iterator first,Iterator last,T*,Compare comp){__pop_heap(first,last-1,last-1,T(*(last-1)),comp,(ptrdiff_t*)0);}
template void __fastcall __pop_heap_aux<CVertexGroupBlockOptimiser::CBlockInfo*,CVertexGroupBlockOptimiser::CBlockInfo,less<CVertexGroupBlockOptimiser::CBlockInfo> >(CVertexGroupBlockOptimiser::CBlockInfo*,CVertexGroupBlockOptimiser::CBlockInfo*,CVertexGroupBlockOptimiser::CBlockInfo*,less<CVertexGroupBlockOptimiser::CBlockInfo>);
}

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, 
          class _Compare>

void
__fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex,
            _Distance __topIndex, _Tp __val, _Compare __comp)
{
  _Distance __parent = (__holeIndex - 1) / 2;
  while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
    *(__first + __holeIndex) = *(__first + __parent);
    __holeIndex = __parent;
    __parent = (__holeIndex - 1) / 2;
  }
  *(__first + __holeIndex) = __val;
}
template <class _RandomAccessIterator, class _Distance,
          class _Tp, class _Compare>
void
__fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex,
              _Distance __len, _Tp __val, _Compare __comp)
{
  _Distance __topIndex = __holeIndex;
  _Distance __secondChild = 2 * __holeIndex + 2;
  while (__secondChild < __len) {
    if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
      __secondChild--;
    *(__first + __holeIndex) = *(__first + __secondChild);
    __holeIndex = __secondChild;
    __secondChild = 2 * (__secondChild + 1);
  }
  if (__secondChild == __len) {
    *(__first + __holeIndex) = *(__first + (__secondChild - 1));
    __holeIndex = __secondChild - 1;
  }
  __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}
}

static void check(bool p){if(!p)abort();}
typedef CVertexGroupBlockOptimiser::CBlockInfo Record;
static unsigned cases;
static bool equal(const Record&a,const Record&b){return a.BlockIndex==b.BlockIndex&&a.Cost==b.Cost&&a.Benefit==b.Benefit;}
int main(){check(sizeof(Record)==12);for(int count=1;count<=48;++count)for(int seed=1;seed<=9;++seed){Record data[50],original[48];data[0].BlockIndex=987;data[0].Cost=-999;data[0].Benefit=456;data[49]=data[0];Record guard=data[0];Record*p=data+1;for(int i=0;i<count;++i){p[i].BlockIndex=i;p[i].Cost=(i*seed*13+seed)%31-15;p[i].Benefit=i*19+seed;original[i]=p[i];int index=i;while(index>0){int parent=(index-1)/2;if(p[parent].Cost>=p[index].Cost)break;Record tmp=p[index];p[index]=p[parent];p[parent]=tmp;index=parent;}}for(int size=count;size>0;--size){Record root=p[0];std::__pop_heap_aux<Record*,Record,std::less<Record> >(p,p+size,(Record*)0,std::less<Record>());check(equal(p[size-1],root));for(int i=1;i<size-1;++i)check(p[(i-1)/2].Cost>=p[i].Cost);bool seen[48]={false};for(int i=0;i<count;++i){check(p[i].BlockIndex<(unsigned long)count);unsigned int k=(unsigned int)p[i].BlockIndex;check(!seen[k]&&equal(p[i],original[k]));seen[k]=true;}check(equal(data[0],guard)&&equal(data[49],guard));++cases;}for(int i=1;i<count;++i)check(p[i-1].Cost<=p[i].Cost);}check(cases==10584);puts("00adb4d0_TEST PASS");return 0;}