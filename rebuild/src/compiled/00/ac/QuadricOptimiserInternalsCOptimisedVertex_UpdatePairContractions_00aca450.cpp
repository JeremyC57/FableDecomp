// 00aca450 QuadricOptimiserInternals::COptimisedVertex::UpdatePairContractions
// Flags: /O2 /Oy /W3
// Leaf: UpdatePairContractions
#include <stdlib.h>
namespace X_00aca450 {
struct CCPPointerInfo {unsigned RefCount;void(__fastcall*DeleteFunc)(void*);void*Data;};
template<class T>struct CCountedPointer{T*Data;CCPPointerInfo*Info;__forceinline CCountedPointer():Data(0),Info(0){}__forceinline CCountedPointer(const CCountedPointer&o):Data(o.Data),Info(o.Info){if(Info)++Info->RefCount;}__forceinline ~CCountedPointer(){if(Info&&--Info->RefCount==0){Info->DeleteFunc(Info->Data);operator delete(Info);}Data=0;Info=0;}};
namespace QuadricOptimiserInternals {
struct CSubContraction {unsigned Index1,Index2;};
// Layout: contraction Exists0,Unsplit1,SubContractions first4/finish8/end12; only first is read.
struct CVertexPairContraction {bool Exists,Unsplit;char Pad[2];CSubContraction*First,*Finish,*End;};
struct CContractionReference {CCountedPointer<CVertexPairContraction> Contraction;unsigned SubIndex;CContractionReference&operator=(const CContractionReference&);};
}
namespace QuadricOptimiserInternals {__declspec(noinline) CContractionReference&CContractionReference::operator=(const CContractionReference&o){if(this!=&o){if(o.Contraction.Info)++o.Contraction.Info->RefCount;if(Contraction.Info&&--Contraction.Info->RefCount==0){Contraction.Info->DeleteFunc(Contraction.Info->Data);operator delete(Contraction.Info);}Contraction.Data=o.Contraction.Data;Contraction.Info=o.Contraction.Info;SubIndex=o.SubIndex;}return *this;}}
namespace std {
using QuadricOptimiserInternals::CContractionReference;
struct _Rb_tree_node {unsigned char Colour;unsigned char Pad[3];_Rb_tree_node*Parent,*Left,*Right;unsigned Key;CContractionReference Value;};
__forceinline _Rb_tree_node*_Rb_global_increment(_Rb_tree_node*node){if(node->Right){node=node->Right;while(node->Left)node=node->Left;}else{_Rb_tree_node*p=node->Parent;while(node==p->Right){node=p;p=p->Parent;}if(node->Right!=p)node=p;}return node;}
struct _Rb_tree_iterator {_Rb_tree_node*Node;__forceinline _Rb_tree_iterator(){}__forceinline _Rb_tree_iterator(const _Rb_tree_iterator&o):Node(o.Node){}__forceinline void increment(){Node=_Rb_global_increment(Node);}__forceinline _Rb_tree_iterator operator++(int){_Rb_tree_iterator old=*this;increment();return old;}};
template<class T>struct less{bool operator()(const T&a,const T&b)const{return a<b;}};
// Layout: Groups9c and PairContractionsa8 establish12-byte retail map; empty key comparator follows header/count.
template<class K,class V>struct map;template<>struct map<unsigned,CContractionReference>{_Rb_tree_node*Header;unsigned Count;less<unsigned> _M_key_compare;__forceinline map():Header(0){Header=(_Rb_tree_node*)malloc(sizeof(_Rb_tree_node));Count=0;Header->Colour=0;Header->Parent=0;Header->Left=Header;Header->Right=Header;}void _M_erase(_Rb_tree_node*);__forceinline ~map(){if(Count){_M_erase(Header->Parent);Header->Left=Header;Header->Parent=0;Header->Right=Header;Count=0;}if(Header)free(Header);}CContractionReference&operator[](const unsigned&);map&operator=(const map&);};
}
namespace QuadricOptimiserInternals {
struct COptimisedPrimitive;
// Layout: retail Exists90, PairContractions a8; donor COptimisedVertex layout differs in container sizes.
struct COptimisedVertex {char Earlier[0x90];bool Exists;char Other[0x17];std::map<unsigned,CContractionReference> PairContractions;void UpdatePairContractions(unsigned,COptimisedPrimitive&);};
void COptimisedVertex::UpdatePairContractions(unsigned index,COptimisedPrimitive&){if(Exists){std::map<unsigned,CContractionReference> updated;std::_Rb_tree_iterator i;i.Node=PairContractions.Header->Left;for(;i.Node!=PairContractions.Header;i++){CContractionReference reference=i.Node->Value;CCountedPointer<CVertexPairContraction> contraction=reference.Contraction;if(contraction.Data->Exists){const CSubContraction&sub=contraction.Data->First[reference.SubIndex];unsigned other;if(sub.Index1==index)other=sub.Index2;else if(sub.Index2==index)other=sub.Index1;else other=~0u;if(other!=index)updated[other]=reference;}}PairContractions=updated;}}
}
}