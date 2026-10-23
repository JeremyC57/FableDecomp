// 00ad7e80 RetailVector::FillInsert
// Flags: /O2 /Oy /W3
// Leaf: FillInsert
// Layout: record flag0, set<long> A+4 and B+16; set Head0/Count4/Compare8 (12 bytes). Callee00ad3a10 allocates20-byte sentinel and copies tree;00ad4a70 destroys both sets.
namespace N00ad7e80 {
struct FalseTag { FalseTag() {} }; struct RandomTag { RandomTag() {} };

extern "C" void __cdecl free(void*);
struct TreeNode {unsigned char Colour;TreeNode* Parent;TreeNode* Left;TreeNode* Right;int Key;};
struct RetailSet {TreeNode* Head;unsigned int Count;unsigned char Compare;
 RetailSet();RetailSet(const RetailSet&);RetailSet& operator=(const RetailSet&);~RetailSet();
 void Insert(int);bool Contains(int)const;
};
struct String { unsigned int W0; String(); String(const String&); ~String(); String& operator=(const String&); };
struct Element { unsigned char Flag; RetailSet A,B; ~Element(); };
void Construct(Element*,const Element&);
#pragma auto_inline(off)
template<class T> __declspec(noinline) T* __fastcall UninitCopy(T* first,T* last,T* out,const FalseTag&) { for(;first!=last;++first,++out) Construct(out,*first); return out; }
template<class T> __declspec(noinline) T* __fastcall CopyBackward(T* first,T* last,T* out,const RandomTag&,int*) { for(int n=int(last-first);n>0;--n) *--out=*--last; return out; }
template<class T> __declspec(noinline) void __fastcall Fill(T* first,T* last,const T& x) { for(;first!=last;++first) *first=x; }
template<class T> __declspec(noinline) T* __fastcall UninitFill(T* first,unsigned int n,const T& x,const FalseTag&) { for(;n>0;--n,++first) Construct(first,x); return first; }
#pragma auto_inline(on)
template<class T> __forceinline T* CopyBack(T* first,T* last,T* out,const FalseTag&) { return CopyBackward(first,last,out,RandomTag(),(int*)0); }
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
    UninitFill(Finish,n-after,x_copy,FalseTag());
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

#include <cstdio>
#include <cstdlib>
#include <new>
namespace N00ad7e80 {
static int overflowCalls;
static TreeNode* Clone(TreeNode* n,TreeNode* parent){if(!n)return 0;TreeNode* p=(TreeNode*)std::malloc(sizeof(TreeNode));p->Colour=n->Colour;p->Parent=parent;p->Key=n->Key;p->Left=Clone(n->Left,p);p->Right=Clone(n->Right,p);return p;}
static void DeleteTree(TreeNode* n){if(n){DeleteTree(n->Left);DeleteTree(n->Right);std::free(n);}}
RetailSet::RetailSet():Count(0),Compare(0){Head=(TreeNode*)std::malloc(sizeof(TreeNode));Head->Colour=0;Head->Parent=0;Head->Left=Head;Head->Right=Head;Head->Key=0;}
RetailSet::RetailSet(const RetailSet& o):Count(o.Count),Compare(o.Compare){Head=(TreeNode*)std::malloc(sizeof(TreeNode));Head->Colour=0;Head->Parent=Clone(o.Head->Parent,Head);TreeNode* l=Head->Parent;TreeNode* r=l;if(l){while(l->Left)l=l->Left;while(r->Right)r=r->Right;}Head->Left=l?l:Head;Head->Right=r?r:Head;Head->Key=0;}
RetailSet& RetailSet::operator=(const RetailSet& o){if(this!=&o){DeleteTree(Head->Parent);Count=o.Count;Compare=o.Compare;Head->Parent=Clone(o.Head->Parent,Head);TreeNode* l=Head->Parent;TreeNode* r=l;if(l){while(l->Left)l=l->Left;while(r->Right)r=r->Right;}Head->Left=l?l:Head;Head->Right=r?r:Head;}return *this;}
RetailSet::~RetailSet(){DeleteTree(Head->Parent);std::free(Head);}
void RetailSet::Insert(int key){TreeNode** p=&Head->Parent;TreeNode* parent=Head;while(*p){parent=*p;if(key==parent->Key)return;p=key<parent->Key?&parent->Left:&parent->Right;}*p=(TreeNode*)std::malloc(sizeof(TreeNode));(*p)->Colour=1;(*p)->Parent=parent;(*p)->Left=0;(*p)->Right=0;(*p)->Key=key;++Count;TreeNode* l=Head->Parent;TreeNode* r=l;while(l->Left)l=l->Left;while(r->Right)r=r->Right;Head->Left=l;Head->Right=r;}
bool RetailSet::Contains(int key)const{TreeNode* p=Head->Parent;while(p){if(p->Key==key)return true;p=key<p->Key?p->Left:p->Right;}return false;}
Element::~Element(){}
void Construct(Element* p,const Element& x){new(p)Element(x);}
template<class T>void RetailVector<T>::Overflow(T*,const T&,const FalseTag&,unsigned int,bool){++overflowCalls;}
}
int main(){using namespace N00ad7e80;Element* a=(Element*)std::malloc(sizeof(Element)*12);for(int i=0;i<5;++i){new(a+i)Element();a[i].Flag=(unsigned char)(i+1);a[i].A.Insert(i+1);a[i].B.Insert(100+i);}Element x;x.Flag=99;x.A.Insert(17);x.A.Insert(23);x.B.Insert(31);RetailVector<Element> v={a,a+5,a+12};v.FillInsert(a+1,2,x);unsigned int want[]={1,99,99,2,3,4,5};for(int j=0;j<7;++j)if(a[j].Flag!=want[j])return 1;if(a[1].A.Head==x.A.Head||a[1].A.Head==a[2].A.Head||!a[1].A.Contains(23)||!a[2].B.Contains(31))return 2;for(int k=0;k<7;++k)a[k].~Element();for(int q=0;q<3;++q){new(a+q)Element();a[q].Flag=(unsigned char)(q+1);}v.Finish=a+3;v.FillInsert(a+2,4,x);if(v.Finish-a!=7||a[6].Flag!=3||!a[5].A.Contains(17))return 3;v.FillInsert(a+2,1,a[0]);if(v.Finish-a!=8||a[2].Flag!=1)return 4;v.FillInsert(a,0,x);v.End=v.Finish;v.FillInsert(a,1,x);if(overflowCalls!=1)return 5;for(int z=0;z<8;++z)a[z].~Element();std::free(a);std::printf("00ad7e80_TEST PASS\n");return 0;}