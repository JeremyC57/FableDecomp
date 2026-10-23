// 00ad7e80 N00ad7e80::vector<Element>::_M_fill_insert (STLport; catalog: vector_InsertFillN_Element28)
// Flags: /O2 /Oy /W3
// Leaf: _M_fill_insert
// Layout: record flag0, set<long> A+4 and B+16; set Head0/Count4/Compare8 (12 bytes). Callee00ad3a10 allocates20-byte sentinel and copies tree;00ad4a70 destroys both sets.
namespace N00ad7e80 {
struct __false_type { __false_type() {} }; struct random_access_iterator_tag { random_access_iterator_tag() {} };

extern "C" void __cdecl free(void*);
struct TreeNode {unsigned char Colour;TreeNode* Parent;TreeNode* Left;TreeNode* Right;int Key;};
struct RetailSet {TreeNode* Head;unsigned int Count;unsigned char Compare;
 RetailSet();RetailSet(const RetailSet&);RetailSet& operator=(const RetailSet&);~RetailSet();
 void Insert(int);bool Contains(int)const;
};
struct String { unsigned int W0; String(); String(const String&); ~String(); String& operator=(const String&); };
struct Element { unsigned char Flag; RetailSet A,B; ~Element(); };
void _Construct(Element*,const Element&);
#pragma auto_inline(off)
template<class T> __declspec(noinline) T* __fastcall __uninitialized_copy(T* first,T* last,T* out,const __false_type&) { for(;first!=last;++first,++out) _Construct(out,*first); return out; }
template<class T> __declspec(noinline) T* __fastcall __copy_backward(T* first,T* last,T* out,const random_access_iterator_tag&,int*) { for(int n=int(last-first);n>0;--n) *--out=*--last; return out; }
template<class T> __declspec(noinline) void __fastcall fill(T* first,T* last,const T& x) { for(;first!=last;++first) *first=x; }
template<class T> __declspec(noinline) T* __fastcall __uninitialized_fill_n(T* first,unsigned int n,const T& x,const __false_type&) { for(;n>0;--n,++first) _Construct(first,x); return first; }
#pragma auto_inline(on)
template<class T> __forceinline T* __copy_backward_ptrs(T* first,T* last,T* out,const __false_type&) { return __copy_backward(first,last,out,random_access_iterator_tag(),(int*)0); }
template<class T> struct vector {
 T* _M_start; T* _M_finish; T* _M_end_of_storage;
 void _M_insert_overflow(T* pos,const T& x,const __false_type&,unsigned int n,bool atend=false);
 void _M_fill_insert(T* pos,unsigned int n,const T& x);
};
template<class T> void vector<T>::_M_fill_insert(T* pos,unsigned int n,const T& x) {
 if(n!=0) {
  if((unsigned int)(_M_end_of_storage-_M_finish)>=n) {
   T x_copy=x;
   const unsigned int after=_M_finish-pos;
   T* old_finish=_M_finish;
   if(after>n) {
    __uninitialized_copy(_M_finish-n,_M_finish,_M_finish,__false_type());
    _M_finish+=n;
    __copy_backward_ptrs(pos,old_finish-n,old_finish,__false_type());
    fill(pos,pos+n,x_copy);
   } else {
    __uninitialized_fill_n(_M_finish,n-after,x_copy,__false_type());
    _M_finish+=n-after;
    __uninitialized_copy(pos,old_finish,_M_finish,__false_type());
    _M_finish+=after;
    fill(pos,old_finish,x_copy);
   }
  } else _M_insert_overflow(pos,x,__false_type(),n);
 }
}
template void vector<Element>::_M_fill_insert(Element*,unsigned int,const Element&);

}