// 00ad85d0 RetailVector::FillInsert
// Flags: /O2 /Oy /W3
// Leaf: FillInsert
// Layout: PDB COptimisedVertex192, retail maps12 shift Exists to144,Unique148,Output152,Groups156,PairContractions168; FaceList132; stride184. Copy00ad77f0 and dtor00ad7400 confirm three owning trees.
namespace N00ad85d0 {
struct FalseTag { FalseTag() {} }; struct RandomTag { RandomTag() {} };

extern "C" void __cdecl free(void*);
struct Buffer {
 unsigned int* Start; unsigned int* Finish; unsigned int* End;
 Buffer(); Buffer(const Buffer&); Buffer& operator=(const Buffer&);
 __forceinline ~Buffer() { if(Start) free(Start); }
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
void Construct(Element*,const Element&);
#pragma auto_inline(off)
template<class T> __declspec(noinline) T* __fastcall UninitCopy(T* first,T* last,T* out,const FalseTag&) { for(;first!=last;++first,++out) Construct(out,*first); return out; }
template<class T> __declspec(noinline) T* __fastcall CopyBackward(T* first,T* last,T* out,const RandomTag&,int*) { for(int n=int(last-first);n>0;--n) *--out=*--last; return out; }
template<class T> __declspec(noinline) void __fastcall Fill(T* first,T* last,const T& x) { for(;first!=last;++first) *first=x; }
template<class T> __declspec(noinline) T* __fastcall UninitFill(T* first,unsigned int n,const T& x) { for(;n>0;--n,++first) Construct(first,x); return first; }
#pragma auto_inline(on)
template<class T> __forceinline T* CopyBack(T* first,T* last,T* out,const FalseTag&) { return CopyBackward(first,last,out,RandomTag(),(int*)0); }
template<class T> __forceinline T* UninitializedFillN(T* first,unsigned int n,const T& x){return UninitFill(first,n,x);}
template<class T> struct RetailVector {
 T* Start; T* Finish; T* End;
 void Overflow(T* pos,const T& x,const FalseTag&,unsigned int n,bool atend=false);
 void FillInsert(T* pos,unsigned int n,const T& x);
};
template<class T> void RetailVector<T>::FillInsert(T* pos,unsigned int n,const T& x) {
 if(n!=0) {
  if((unsigned int)(End-Finish)>=n) {
   T x_copy=x;
   const unsigned int after=Finish-pos;
   T* old_finish=Finish;
   if(after>n) {
    UninitCopy(Finish-n,Finish,Finish,FalseTag());
    Finish+=n;
    CopyBack(pos,old_finish-n,old_finish,FalseTag());
    Fill(pos,pos+n,x_copy);
   } else {
    UninitializedFillN(Finish,n-after,x_copy);
    Finish+=n-after;
    UninitCopy(pos,old_finish,Finish,FalseTag());
    Finish+=after;
    Fill(pos,old_finish,x_copy);
   }
  } else Overflow(pos,x,FalseTag(),n);
 }
}
template void RetailVector<Element>::FillInsert(Element*,unsigned int,const Element&);

}