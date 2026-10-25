// std::vector<char>::_M_fill_insert(char* position, size_t n, const char& x) -- STLport fill-insert of n copies of x: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004110D0, length 289 bytes, `ret 0Ch`.
// Leaf: ?_M_fill_insert@?$vector@DV?$allocator@D@std@@@std@@QAEXPADIABD@Z
// Identity: the byte-vector callers are the network code that passes `std::vector<char>&` (Ego_r.pdb
//   ?Send@LSocket@LHNetworkLib@@QAE_NAAV?$vector@DV?$allocator@D@std@@@std@@ABULAddress@2@1@Z and the LSocket /
//   LSocketServer Receive / GetUDPPacket family): LHHttp2::GetUntilCRLF / ReceiveDocumentAsync / SendRawData /
//   ReceiveResponseHeaders and the socket receive / bank-file send paths, plus vector<char>::resize (0x004122F7) and a
//   push_back tail jump (0x00411B30). The retail STL namespace is `std` (Fable.exe RTTI has .?AVlength_error@std@@ and
//   no @std@@ string), so the row moves from `std::` to `std::`. FINDINGS chunk 401 (catalog was "CString_FillRange").
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
