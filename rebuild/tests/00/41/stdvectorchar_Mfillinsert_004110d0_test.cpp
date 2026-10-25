// std::vector<char>::_M_fill_insert(char* position, size_t n, const char& x) -- STLport fill-insert of n copies of x: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004110D0, length 289 bytes, `ret 0Ch`.
// Leaf: ?_M_fill_insert@?$vector@DV?$allocator@D@std@@@std@@QAEXPADIABD@Z
// Identity: the byte-vector callers are the network code that passes `std::vector<char>&` (Ego_r.pdb
//   ?Send@LSocket@LHNetworkLib@@QAE_NAAV?$vector@DV?$allocator@D@std@@@std@@ABULAddress@2@1@Z and the LSocket /
//   LSocketServer Receive / GetUDPPacket family): LHHttp2::GetUntilCRLF / ReceiveDocumentAsync / SendRawData /
//   ReceiveResponseHeaders and the socket receive / bank-file send paths, plus vector<char>::resize (0x004122F7) and a
//   push_back tail jump (0x00411B30). The retail STL namespace is `std` (Fable.exe RTTI has .?AVlength_error@std@@ and
//   no @_STL@@ string), so the row moves from `_STL::` to `std::`. FINDINGS chunk 401 (catalog was "CString_FillRange").
// Callees: memmove @ 0x00BFEAE6 (both copies); the fills are inlined memsets (rep stosd / rep stosb);
//   vector::_M_insert_overflow(position, x, const __true_type&, n, false) on the grow path.
// Layout: retail [ecx+0,4,8] is STLport vector<char> storage (start / finish / end of storage).
#include <string.h>
#include <stdlib.h>
namespace std {
struct __true_type { __true_type(){} };
template <class _Tp> class allocator { };
__forceinline char* CopyChars(const char*first,const char*last,char*out){if(last!=first)memmove(out,first,last-first);return out+(last-first);}
__forceinline char* CopyBackward(const char*first,const char*last,char*out){int count=last-first;if(count>0)memmove(out-count,first,count);return out-count;}
__forceinline void FillChars(char*first,char*last,const char&value){memset(first,(unsigned char)value,last-first);}
__forceinline char* FillN(char*first,unsigned count,const char&value){memset(first,(unsigned char)value,count);return first+count;}
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
 _Tp *_M_start,*_M_finish,*_M_end_of_storage;
 void _M_insert_overflow(_Tp*,const _Tp&,const __true_type&,size_t,bool);
 void _M_fill_insert(_Tp* pos,size_t n,const _Tp& value);
};
template <class _Tp, class _Alloc> void vector<_Tp, _Alloc>::_M_fill_insert(_Tp*pos,size_t n,const _Tp&value) {
 if(n) {
  if(size_t(_M_end_of_storage-_M_finish)>=n) {
   _Tp copy=value; size_t after=_M_finish-pos; _Tp* old=_M_finish;
   if(after>n) {
    CopyChars(old-n,old,old);
    _M_finish+=n;
    CopyBackward(pos,old-n,old);
    FillChars(pos,pos+n,copy);
   } else {
    FillN(_M_finish,n-after,copy); _M_finish+=n-after;
    CopyChars(pos,old,_M_finish);
    _M_finish+=after; FillChars(pos,old,copy);
   }
  } else { __true_type t; _M_insert_overflow(pos,value,t,n,false); }
 }
}
template class vector<char>;
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
namespace std {
static int overflows;
template<>void vector<char>::_M_insert_overflow(char*pos,const char&value,const __true_type&,size_t n,bool atEnd){assert(!atEnd);++overflows;unsigned off=pos-_M_start,old=_M_finish-_M_start;char copy=value;char*q=(char*)malloc(old+n);memcpy(q,_M_start,off);memset(q+off,(unsigned char)copy,n);memcpy(q+off+n,pos,old-off);free(_M_start);_M_start=q;_M_finish=q+old+n;_M_end_of_storage=_M_finish;}
void Reset(vector<char>&v,const unsigned char*p,unsigned n,unsigned capacity){if(v._M_start)free(v._M_start);v._M_start=(char*)malloc(capacity);memset(v._M_start,0x55,capacity);memcpy(v._M_start,p,n);v._M_finish=v._M_start+n;v._M_end_of_storage=v._M_start+capacity;}
void Check(vector<char>&v,const unsigned char*p,unsigned n){assert(unsigned(v._M_finish-v._M_start)==n&&!memcmp(v._M_start,p,n));}
}
int main(){using namespace std;vector<char>v;v._M_start=v._M_finish=v._M_end_of_storage=0;unsigned char base[4]={0x10,0xfe,0x30,0x40};char value=(char)0x80;
 Reset(v,base,4,32);v._M_fill_insert(v._M_start+1,2,value);unsigned char a[6]={0x10,0x80,0x80,0xfe,0x30,0x40};Check(v,a,6);
 value=(char)0xe5;v._M_fill_insert(v._M_finish,3,value);unsigned char b[9]={0x10,0x80,0x80,0xfe,0x30,0x40,0xe5,0xe5,0xe5};Check(v,b,9);char*old=v._M_start;v._M_fill_insert(v._M_start,0,value);assert(v._M_start==old&&overflows==0);Check(v,b,9);
 Reset(v,base,4,32);value=(char)0x80;v._M_fill_insert(v._M_start+2,2,value);unsigned char c[6]={0x10,0xfe,0x80,0x80,0x30,0x40};Check(v,c,6);
 Reset(v,base,4,32);value=(char)0x9a;v._M_fill_insert(v._M_start+3,3,value);unsigned char d[7]={0x10,0xfe,0x30,0x9a,0x9a,0x9a,0x40};Check(v,d,7);
 Reset(v,base,4,32);v._M_fill_insert(v._M_start,2,v._M_start[2]);unsigned char e[6]={0x30,0x30,0x10,0xfe,0x30,0x40};Check(v,e,6);
 Reset(v,base,4,4);v._M_fill_insert(v._M_start+2,5,v._M_start[1]);unsigned char f[9]={0x10,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0x30,0x40};Check(v,f,9);assert(overflows==1);free(v._M_start);puts("004110d0_TEST PASS");}