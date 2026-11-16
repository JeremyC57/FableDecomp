// Native path also tests the actual shared libpk type and original vendor vector.
#ifdef _MSC_VER
// std::vector<CHandleBasedMemoryPool_012b4098::CHandleInfo>::_Insert_n -- whole original parent of assigned catch fragment.
// Leaf: ?_Insert_n@?$vector@VCHandleInfo@CHandleBasedMemoryPool_012b4098@@V?$allocator@VCHandleInfo@CHandleBasedMemoryPool_012b4098@@@std@@@std@@IAEXViterator@12@IABVCHandleInfo@CHandleBasedMemoryPool_012b4098@@@Z
// Retail: parent0x00C298D0..0x00C29AEE,542bytes; assigned0x00C29A3C starts inside its catch block.
// Flags: /EHsc /Gr /MD
#define FABLE_RETAIL_LIBPK_HANDLEINFO_DINKUMWARE
#include "libpk/CHandleBasedMemoryPool_012b4098.h"
// Original VC7.1 Dinkumware vector body, including its real try/catch cleanup.
template void std::vector<CHandleBasedMemoryPool_012b4098::CHandleInfo>::_Insert_n(
 std::vector<CHandleBasedMemoryPool_012b4098::CHandleInfo>::iterator,
 unsigned int,const CHandleBasedMemoryPool_012b4098::CHandleInfo&);

#endif
#include <stdio.h>
#include <stdlib.h>
static void Check(bool b){if(!b)abort();}
static int LiveBlocks,HelperCalls,ThrowAt,Destroyed;static bool FailAllocation;
#if !defined(_MSC_VER) || defined(_CPPUNWIND)
#define TEST_EH 1
#define _TRY_BEGIN try {
#define _CATCH_ALL } catch(...) {
#define _RERAISE throw
#define _CATCH_END }
#else
#define _TRY_BEGIN {
#define _CATCH_ALL } if(false) {
#define _RERAISE Check(false)
#define _CATCH_END }
#endif
#define _ITER_BASE(x) (x)
static void HelperStep(){++HelperCalls;
#ifdef TEST_EH
 if(ThrowAt==HelperCalls)throw 123;
#else
 Check(ThrowAt==0);
#endif
}
struct Element {int Value;};
static void fill(Element* first,Element* last,const Element& value){for(;first!=last;++first)*first=value;}
static Element* copy_backward(Element* first,Element* last,Element* dest){while(first!=last)*--dest=*--last;return dest;}
struct Allocator {
 Element* allocate(unsigned int n){
#ifdef TEST_EH
  if(FailAllocation)throw 456;
#else
  Check(!FailAllocation);
#endif
  Element*p=static_cast<Element*>(malloc(n*sizeof(Element)));Check(p!=0);++LiveBlocks;return p;
 }
 void deallocate(Element*p,unsigned int){Check(p!=0);free(p);--LiveBlocks;}
};
// Fixture host adapter: the original algorithm below is copied verbatim from
// the actual VC7.1 header. These operations expose independent call failures;
// their layout and pointer width are not a production ABI model.
class VectorTest {
public:
 typedef Element _Ty;typedef unsigned int size_type;typedef Element* pointer;typedef Element* iterator;
 Allocator _Alval;pointer _Myfirst,_Mylast,_Myend;
 VectorTest(unsigned int n,unsigned int cap){_Myfirst=cap?_Alval.allocate(cap):0;_Mylast=cap?_Myfirst+n:0;_Myend=cap?_Myfirst+cap:0;for(unsigned int i=0;i<n;++i)_Myfirst[i].Value=10+static_cast<int>(i);}
 ~VectorTest(){if(_Myfirst)_Alval.deallocate(_Myfirst,capacity());}
 size_type size()const{return _Myfirst?static_cast<size_type>(_Mylast-_Myfirst):0;}
 size_type capacity()const{return _Myfirst?static_cast<size_type>(_Myend-_Myfirst):0;}
 size_type max_size()const{return 64;}
 void _Xlen()const{
#ifdef TEST_EH
  throw 789;
#else
  Check(false);
#endif
 }
 pointer _Ucopy(pointer first,pointer last,pointer dest){HelperStep();while(first!=last)*dest++=*first++;return dest;}
 pointer _Ufill(pointer dest,size_type count,const _Ty& value){HelperStep();while(count--)*dest++=value;return dest;}
 void _Destroy(pointer first,pointer last){while(first!=last){++Destroyed;++first;}}
	void _Insert_n(iterator _Where,
		size_type _Count, const _Ty& _Val)
		{	// insert _Count * _Val at _Where
		_Ty _Tmp = _Val;	// in case _Val is in sequence
		size_type _Capacity = capacity();

		if (_Count == 0)
			;
		else if (max_size() - size() < _Count)
			_Xlen();	// result too long
		else if (_Capacity < size() + _Count)
			{	// not enough room, reallocate
			_Capacity = max_size() - _Capacity / 2 < _Capacity
				? 0 : _Capacity + _Capacity / 2;	// try to grow by 50%
			if (_Capacity < size() + _Count)
				_Capacity = size() + _Count;
			pointer _Newvec = this->_Alval.allocate(_Capacity);
			pointer _Ptr = _Newvec;

			_TRY_BEGIN
			_Ptr = _Ucopy(_Myfirst, _ITER_BASE(_Where),
				_Newvec);	// copy prefix
			_Ptr = _Ufill(_Ptr, _Count, _Tmp);	// add new stuff
			_Ucopy(_ITER_BASE(_Where), _Mylast, _Ptr);	// copy suffix
			_CATCH_ALL
			_Destroy(_Newvec, _Ptr);
			this->_Alval.deallocate(_Newvec, _Capacity);
			_RERAISE;
			_CATCH_END

			_Count += size();
			if (_Myfirst != 0)
				{	// destroy and deallocate old array
				_Destroy(_Myfirst, _Mylast);
				this->_Alval.deallocate(_Myfirst, _Myend - _Myfirst);
				}
			_Myend = _Newvec + _Capacity;
			_Mylast = _Newvec + _Count;
			_Myfirst = _Newvec;
			}
		else if ((size_type)(_Mylast - _ITER_BASE(_Where)) < _Count)
			{	// new stuff spills off end
			_Ucopy(_ITER_BASE(_Where), _Mylast,
				_ITER_BASE(_Where) + _Count);	// copy suffix

			_TRY_BEGIN
			_Ufill(_Mylast, _Count - (_Mylast - _ITER_BASE(_Where)),
				_Tmp);	// insert new stuff off end
			_CATCH_ALL
			_Destroy(_ITER_BASE(_Where) + _Count, _Mylast + _Count);
			_RERAISE;
			_CATCH_END

			_Mylast += _Count;
			fill(_ITER_BASE(_Where), _Mylast - _Count,
				_Tmp);	// insert up to old end
			}
		else
			{	// new stuff can all be assigned
			pointer _Oldend = _Mylast;
			_Mylast = _Ucopy(_Oldend - _Count, _Oldend,
				_Mylast);	// copy suffix
			copy_backward(_ITER_BASE(_Where), _Oldend - _Count,
				_Oldend);	// copy hole
			fill(_ITER_BASE(_Where), _ITER_BASE(_Where) + _Count,
				_Tmp);	// insert into hole
			}
		}

};

#ifdef _MSC_VER
CHandleBasedMemoryPool_012b4098::CHandleInfo::CHandleInfo(){NextUnusedHandle=0;}
static void VerifyActualVendorVector(){
 typedef CHandleBasedMemoryPool_012b4098::CHandleInfo Info;
 typedef std::vector<Info> Vector;
 typedef char CheckInfoSize[sizeof(Info)==4?1:-1];
 typedef char CheckVectorSize[sizeof(Vector)==16?1:-1];
 unsigned int cases=0;
 for(unsigned int n=0;n<=8;++n)for(unsigned int extra=0;extra<=4;++extra)
 for(unsigned int pos=0;pos<=n;++pos)for(unsigned int count=0;count<=5;++count)
 for(unsigned int alias=0;alias<=1;++alias){
  Vector v;v.reserve(n+extra);Info zero;v.resize(n,zero);
  for(unsigned int j=0;j<n;++j)v[j].NextUnusedHandle=10+(long)j;
  Info value;value.NextUnusedHandle=-7;unsigned int cap=v.capacity();
  const Info& chosen=alias&&n?v[n/2]:value;long inserted=chosen.NextUnusedHandle;
  v.insert(v.begin()+pos,count,chosen);
  Check(v.size()==n+count);unsigned int wantcap=cap;
  if(count&&cap<n+count){wantcap=cap+cap/2;if(wantcap<n+count)wantcap=n+count;}
  Check(v.capacity()==wantcap);
  for(unsigned int i=0;i<n+count;++i){long expected=i<pos?10+(long)i:i<pos+count?inserted:10+(long)(i-count);Check(v[i].NextUnusedHandle==expected);}
  ++cases;
 }
 Check(cases==2700);
}
#endif

int main(){
#ifdef _MSC_VER
 VerifyActualVendorVector();
#endif
 unsigned int cases=0;
 for(unsigned int n=0;n<=8;++n)for(unsigned int extra=0;extra<=4;++extra)
 for(unsigned int pos=0;pos<=n;++pos)for(unsigned int count=0;count<=5;++count)
 for(unsigned int alias=0;alias<=1;++alias){
  {
   VectorTest v(n,n+extra);Element value;value.Value=-7;unsigned int cap=n+extra;
   const Element& chosen=alias&&n?v._Myfirst[n/2]:value;int inserted=chosen.Value;
   HelperCalls=0;ThrowAt=0;Destroyed=0;
   v._Insert_n(v._Myfirst?v._Myfirst+pos:0,count,chosen);
   Check(v.size()==n+count);unsigned int wantcap=cap;
   if(count&&cap<n+count){wantcap=cap+cap/2;if(wantcap<n+count)wantcap=n+count;}
   Check(v.capacity()==wantcap);
   for(unsigned int i=0;i<n+count;++i){int expected=i<pos?10+static_cast<int>(i):i<pos+count?inserted:10+static_cast<int>(i-count);Check(v._Myfirst[i].Value==expected);}
  }
  Check(LiveBlocks==0);++cases;
 }
 Check(cases==2700);
#ifdef TEST_EH
 for(int stage=1;stage<=3;++stage){
  {VectorTest v(3,3);Element value;value.Value=99;Element*old=v._Myfirst;HelperCalls=0;ThrowAt=stage;Destroyed=0;bool caught=false;
   try{v._Insert_n(v._Myfirst+1,4,value);}catch(int code){Check(code==123);caught=true;}
   Check(caught&&v._Myfirst==old&&v.size()==3&&v.capacity()==3&&LiveBlocks==1);
   for(int j=0;j<3;++j)Check(v._Myfirst[j].Value==10+j);
   Check(Destroyed==(stage==1?0:stage==2?1:5));ThrowAt=0;
  }Check(LiveBlocks==0);
 }
 {VectorTest v(3,10);Element value;value.Value=99;HelperCalls=0;ThrowAt=2;Destroyed=0;bool caught=false;
  try{v._Insert_n(v._Myfirst+1,5,value);}catch(int code){Check(code==123);caught=true;}
  Check(caught&&v.size()==3&&v.capacity()==10&&Destroyed==2&&LiveBlocks==1);for(int j=0;j<3;++j)Check(v._Myfirst[j].Value==10+j);ThrowAt=0;
 }Check(LiveBlocks==0);
 {VectorTest v(3,3);Element value;value.Value=99;FailAllocation=true;bool caught=false;
  try{v._Insert_n(v._Myfirst+1,5,value);}catch(int code){Check(code==456);caught=true;}
  Check(caught&&v.size()==3&&v.capacity()==3&&LiveBlocks==1);FailAllocation=false;
 }Check(LiveBlocks==0);
 {VectorTest v(3,3);Element value;value.Value=99;bool caught=false;
  try{v._Insert_n(v._Myfirst+1,65,value);}catch(int code){Check(code==789);caught=true;}
  Check(caught&&v.size()==3&&v.capacity()==3&&LiveBlocks==1);
 }Check(LiveBlocks==0);
#endif
 puts("00c29a3c_TEST PASS");return 0;
}
