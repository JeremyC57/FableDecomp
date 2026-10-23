// 00ad85d0 N00ad85d0::vector<Element>::_M_fill_insert (STLport; catalog: resize_COptimisedVertex)
// Flags: /O2 /Oy /W3
// Leaf: _M_fill_insert
// Layout: PDB COptimisedVertex192, retail maps12 shift Exists to144,Unique148,Output152,Groups156,PairContractions168; FaceList132; stride184. Copy00ad77f0 and dtor00ad7400 confirm three owning trees.
namespace N00ad85d0 {
struct __false_type { __false_type() {} }; struct random_access_iterator_tag { random_access_iterator_tag() {} };

extern "C" void __cdecl free(void*);
struct Buffer {
 unsigned int* _M_start; unsigned int* _M_finish; unsigned int* _M_end_of_storage;
 Buffer(); Buffer(const Buffer&); Buffer& operator=(const Buffer&);
 __forceinline ~Buffer() { if(_M_start) free(_M_start); }
};
struct String { unsigned int W0; String(); String(const String&); ~String(); String& operator=(const String&); };

struct RefBlock {unsigned int Refs;void (__fastcall *Destroy)(void*);void* Object;};
struct ContractionReference {void* Object;RefBlock* Block;unsigned int SubIndex;ContractionReference():Object(0),Block(0),SubIndex(0){}ContractionReference(const ContractionReference&);ContractionReference& operator=(const ContractionReference&);~ContractionReference();};
template<class V>struct TreeNode {unsigned char Colour;TreeNode* Parent;TreeNode* Left;TreeNode* Right;unsigned int Key;V Value;};
template<class V>struct RetailTree {TreeNode<V>* Head;unsigned int Count;unsigned char Compare;RetailTree();RetailTree(const RetailTree&);RetailTree& operator=(const RetailTree&);~RetailTree();void Insert(unsigned int,const V&);const V* Find(unsigned int)const;};
struct SetNode {unsigned char Colour;SetNode* Parent;SetNode* Left;SetNode* Right;unsigned int Key;};
struct RetailSet {SetNode* Head;unsigned int Count;unsigned char Compare;RetailSet();RetailSet(const RetailSet&);RetailSet& operator=(const RetailSet&);~RetailSet();void Insert(unsigned int);const unsigned int* Find(unsigned int)const;};
struct Vec3 {float X,Y,Z;};struct Quadric {double Coefficients[11];};
struct COptimisedVertex {Vec3 Position,Normal,TextureCoord;Quadric PositionError;float Cost;RetailSet FaceList;bool Exists;unsigned int UniqueIndex,OutputIndex;RetailTree<float> Groups;RetailTree<ContractionReference> PairContractions;
 COptimisedVertex();COptimisedVertex(const COptimisedVertex&);COptimisedVertex& operator=(const COptimisedVertex&);~COptimisedVertex();};
typedef COptimisedVertex Element;
void _Construct(Element*,const Element&);
#pragma auto_inline(off)
template<class T> __declspec(noinline) T* __fastcall __uninitialized_copy(T* first,T* last,T* out,const __false_type&) { for(;first!=last;++first,++out) _Construct(out,*first); return out; }
template<class T> __declspec(noinline) T* __fastcall __copy_backward(T* first,T* last,T* out,const random_access_iterator_tag&,int*) { for(int n=int(last-first);n>0;--n) *--out=*--last; return out; }
template<class T> __declspec(noinline) void __fastcall fill(T* first,T* last,const T& x) { for(;first!=last;++first) *first=x; }
template<class T> __declspec(noinline) T* __fastcall __uninitialized_fill_n(T* first,unsigned int n,const T& x) { for(;n>0;--n,++first) _Construct(first,x); return first; }
#pragma auto_inline(on)
template<class T> __forceinline T* __copy_backward_ptrs(T* first,T* last,T* out,const __false_type&) { return __copy_backward(first,last,out,random_access_iterator_tag(),(int*)0); }
template<class T> __forceinline T* UninitializedFillN(T* first,unsigned int n,const T& x){return __uninitialized_fill_n(first,n,x);}
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
    UninitializedFillN(_M_finish,n-after,x_copy);
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