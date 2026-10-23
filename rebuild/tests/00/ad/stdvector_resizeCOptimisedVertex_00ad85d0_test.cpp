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

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#include <assert.h>
#include <stddef.h>
namespace N00ad85d0 {
static int liveNodes,liveControls,liveObjects,overflowCalls;

template<class N>void Ends(N* h){N*l=h->Parent;N*r=l;if(l){while(l->Left)l=l->Left;while(r->Right)r=r->Right;}h->Left=l?l:h;h->Right=r?r:h;}
static SetNode* CloneSet(SetNode*n,SetNode*parent){if(!n)return 0;SetNode*p=(SetNode*)std::malloc(sizeof(SetNode));*p=*n;p->Parent=parent;p->Left=CloneSet(n->Left,p);p->Right=CloneSet(n->Right,p);++liveNodes;return p;}
static void DeleteSet(SetNode*p){if(p){DeleteSet(p->Left);DeleteSet(p->Right);std::free(p);--liveNodes;}}
RetailSet::RetailSet():Count(0),Compare(0){Head=(SetNode*)std::malloc(sizeof(SetNode));Head->Colour=0;Head->Parent=0;Ends(Head);++liveNodes;}
RetailSet::RetailSet(const RetailSet&o):Count(o.Count),Compare(o.Compare){Head=(SetNode*)std::malloc(sizeof(SetNode));Head->Colour=0;Head->Parent=CloneSet(o.Head->Parent,Head);Ends(Head);++liveNodes;}
RetailSet& RetailSet::operator=(const RetailSet&o){if(this!=&o){DeleteSet(Head->Parent);Count=o.Count;Compare=o.Compare;Head->Parent=CloneSet(o.Head->Parent,Head);Ends(Head);}return *this;}
RetailSet::~RetailSet(){DeleteSet(Head->Parent);std::free(Head);--liveNodes;}
void RetailSet::Insert(unsigned int key){SetNode**p=&Head->Parent;SetNode*parent=Head;while(*p){parent=*p;if(key==parent->Key)return;p=key<parent->Key?&parent->Left:&parent->Right;}*p=(SetNode*)std::malloc(sizeof(SetNode));(*p)->Colour=1;(*p)->Parent=parent;(*p)->Left=(*p)->Right=0;(*p)->Key=key;++Count;++liveNodes;Ends(Head);}
const unsigned int* RetailSet::Find(unsigned int key)const{SetNode*p=Head->Parent;while(p){if(p->Key==key)return &p->Key;p=key<p->Key?p->Left:p->Right;}return 0;}

void __fastcall DisposeObject(void* p){delete (int*)p;--liveObjects;}
ContractionReference::ContractionReference(const ContractionReference& o):Object(o.Object),Block(o.Block),SubIndex(o.SubIndex){if(Block)++Block->Refs;}
ContractionReference& ContractionReference::operator=(const ContractionReference& o){if(Block!=o.Block){if(o.Block)++o.Block->Refs;if(Block&&--Block->Refs==0){Block->Destroy(Block->Object);delete Block;--liveControls;}Block=o.Block;Object=o.Object;SubIndex=o.SubIndex;}return *this;}
ContractionReference::~ContractionReference(){if(Block&&--Block->Refs==0){Block->Destroy(Block->Object);delete Block;--liveControls;}}
template<class V>TreeNode<V>* Clone(TreeNode<V>* n,TreeNode<V>* parent){if(!n)return 0;TreeNode<V>* p=new(std::malloc(sizeof(TreeNode<V>)))TreeNode<V>;std::memset(p,0,16);p->Parent=parent;p->Colour=n->Colour;p->Key=n->Key;p->Value=n->Value;p->Left=Clone(n->Left,p);p->Right=Clone(n->Right,p);++liveNodes;return p;}
template<class V>void DeleteTree(TreeNode<V>* p){if(p){DeleteTree(p->Left);DeleteTree(p->Right);p->~TreeNode<V>();std::free(p);--liveNodes;}}
template<class V>RetailTree<V>::RetailTree():Count(0),Compare(0){Head=(TreeNode<V>*)std::malloc(sizeof(TreeNode<V>));Head->Colour=0;Head->Parent=0;Ends(Head);++liveNodes;}
template<class V>RetailTree<V>::RetailTree(const RetailTree& o):Count(o.Count),Compare(o.Compare){Head=(TreeNode<V>*)std::malloc(sizeof(TreeNode<V>));Head->Colour=0;Head->Parent=Clone(o.Head->Parent,Head);Ends(Head);++liveNodes;}
template<class V>RetailTree<V>& RetailTree<V>::operator=(const RetailTree& o){if(this!=&o){DeleteTree(Head->Parent);Count=o.Count;Compare=o.Compare;Head->Parent=Clone(o.Head->Parent,Head);Ends(Head);}return *this;}
template<class V>RetailTree<V>::~RetailTree(){DeleteTree(Head->Parent);std::free(Head);--liveNodes;}
template<class V>void RetailTree<V>::Insert(unsigned int key,const V& v){TreeNode<V>** p=&Head->Parent;TreeNode<V>* parent=Head;while(*p){parent=*p;if(key==parent->Key){parent->Value=v;return;}p=key<parent->Key?&parent->Left:&parent->Right;}*p=new(std::malloc(sizeof(TreeNode<V>)))TreeNode<V>;(*p)->Colour=1;(*p)->Parent=parent;(*p)->Left=0;(*p)->Right=0;(*p)->Key=key;(*p)->Value=v;++Count;++liveNodes;Ends(Head);}
template<class V>const V* RetailTree<V>::Find(unsigned int key)const{TreeNode<V>* p=Head->Parent;while(p){if(p->Key==key)return &p->Value;p=key<p->Key?p->Left:p->Right;}return 0;}
COptimisedVertex::COptimisedVertex():Cost(0),Exists(false),UniqueIndex(0),OutputIndex(0){std::memset(&Position,0,sizeof Position);std::memset(&Normal,0,sizeof Normal);std::memset(&TextureCoord,0,sizeof TextureCoord);std::memset(&PositionError,0,sizeof PositionError);}
COptimisedVertex::COptimisedVertex(const COptimisedVertex& o):Position(o.Position),Normal(o.Normal),TextureCoord(o.TextureCoord),PositionError(o.PositionError),Cost(o.Cost),FaceList(o.FaceList),Exists(o.Exists),UniqueIndex(o.UniqueIndex),OutputIndex(o.OutputIndex),Groups(o.Groups),PairContractions(o.PairContractions){}
COptimisedVertex& COptimisedVertex::operator=(const COptimisedVertex& o){Position=o.Position;Normal=o.Normal;TextureCoord=o.TextureCoord;PositionError=o.PositionError;Cost=o.Cost;FaceList=o.FaceList;Exists=o.Exists;UniqueIndex=o.UniqueIndex;OutputIndex=o.OutputIndex;Groups=o.Groups;PairContractions=o.PairContractions;return *this;}
COptimisedVertex::~COptimisedVertex(){}
void _Construct(Element* p,const Element& o){new(p)Element(o);}
template<class T>void vector<T>::_M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool){++overflowCalls;}
void Init(Element& e,int value){e.UniqueIndex=value;e.Exists=true;e.Position.X=(float)value;e.FaceList.Insert(value);e.Groups.Insert(value,0.5f);ContractionReference r;r.Block=new RefBlock;r.Block->Refs=1;r.Object=new int(value);r.Block->Object=r.Object;r.Block->Destroy=DisposeObject;r.SubIndex=value;++liveControls;++liveObjects;e.PairContractions.Insert(value,r);}
}
int main(){using namespace N00ad85d0;
 assert(sizeof(SetNode)==20&&sizeof(TreeNode<float>)==24&&sizeof(TreeNode<ContractionReference>)==32);
 assert(sizeof(RetailSet)==12&&sizeof(Element)==184&&offsetof(Element,FaceList)==132&&offsetof(Element,Groups)==156&&offsetof(Element,PairContractions)==168);
 {Element original;Init(original,20);original.FaceList.Insert(5);original.FaceList.Insert(30);original.FaceList.Insert(20);assert(original.FaceList.Count==3);
 original.Groups.Insert(5,1.5f);original.Groups.Insert(30,2.5f);
 ContractionReference ref=*original.PairContractions.Find(20);original.PairContractions.Insert(5,ref);original.PairContractions.Insert(30,ref);assert(ref.Block->Refs==4);
 Element copy(original);Element assigned;assigned=original;assigned=assigned;
 assert(copy.FaceList.Head!=original.FaceList.Head&&copy.FaceList.Head->Left->Key==5&&copy.FaceList.Head->Right->Key==30);
 assert(assigned.Groups.Head->Left->Key==5&&assigned.Groups.Head->Right->Key==30&&assigned.Groups.Head->Parent->Parent==assigned.Groups.Head);
 assert(copy.PairContractions.Head->Left->Key==5&&copy.PairContractions.Head->Right->Key==30&&ref.Block->Refs==10);
 copy.FaceList.Insert(1);assert(!original.FaceList.Find(1));
 Element empty;assigned=empty;assert(!assigned.FaceList.Count&&assigned.FaceList.Head->Left==assigned.FaceList.Head&&assigned.Groups.Head->Right==assigned.Groups.Head&&!assigned.PairContractions.Head->Parent);
 }
 assert(!liveNodes&&!liveControls&&!liveObjects);
{Element* a=(Element*)std::malloc(12*sizeof(Element));for(int i=0;i<5;++i){new(a+i)Element();Init(a[i],i+1);}Element x;Init(x,99);vector<Element> v={a,a+5,a+12};v._M_fill_insert(a+1,2,x);unsigned int want[]={1,99,99,2,3,4,5};for(int j=0;j<7;++j)if(a[j].UniqueIndex!=want[j]||!a[j].FaceList.Find(want[j])||!a[j].Groups.Find(want[j])||!a[j].PairContractions.Find(want[j]))return 1;if(a[1].FaceList.Head==x.FaceList.Head||a[1].PairContractions.Head==a[2].PairContractions.Head||*a[1].Groups.Find(99)!=0.5f)return 2;for(int k=0;k<7;++k)a[k].~Element();for(int q=0;q<3;++q){new(a+q)Element();Init(a[q],q+1);}v._M_finish=a+3;v._M_fill_insert(a+2,4,x);if(v._M_finish-a!=7||a[6].UniqueIndex!=3)return 3;v._M_fill_insert(a+2,1,a[0]);if(v._M_finish-a!=8||a[2].UniqueIndex!=1||a[2].FaceList.Head==a[0].FaceList.Head)return 4;v._M_fill_insert(a,0,x);v._M_end_of_storage=v._M_finish;v._M_fill_insert(a,1,x);if(overflowCalls!=1)return 5;for(int z=0;z<8;++z)a[z].~Element();std::free(a);}if(liveNodes!=0||liveControls!=0||liveObjects!=0)return 6;std::printf("00ad85d0_TEST PASS\n");return 0;}