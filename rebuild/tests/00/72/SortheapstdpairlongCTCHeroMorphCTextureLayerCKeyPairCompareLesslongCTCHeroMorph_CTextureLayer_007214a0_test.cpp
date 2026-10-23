// 007214a0 N007214a0::sort_heap<Element,CKeyPairCompareLess> (STLport; catalog: _Sort_heap<std::pair<long,CTCHeroMorph::CTextureLayer>*,CKeyPairCompareLess<long,CTCHeroMorph::_Sort_heap<std::pair<long,CTCHeroMorph::CTextureLayer>*,CKeyPairCompareLess<long,CTCHeroMorph::CTextureLa)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: sort_heap
// Layout: PDB8268-8275 physical Colour B0,G1,R2,A3; retail721561-7215a0 copies components at2,1,0,3. Stride24; enclosing payload identity descriptive because manifest type conflicts with observed colors.
#include <new>
namespace N007214a0 {

struct CKeyPairCompareLess {unsigned int State;};

struct Colour {unsigned char B,G,R,A;Colour(){}__forceinline void Set(unsigned char red,unsigned char green,unsigned char blue,unsigned char alpha){R=red;G=green;B=blue;A=alpha;}__forceinline Colour(const Colour& o){Set(o.R,o.G,o.B,o.A);}};
struct Element {int Key;unsigned int A,B,C;Colour D,E;Element(){} __forceinline Element(const Element& o):Key(o.Key),A(o.A),B(o.B),C(o.C),D(o.D),E(o.E) {}};
template<class T,class C> void __fastcall __adjust_heap(T*,int,int,T,C);
template<class T,class C> __forceinline void __pop_heap(T* first,T* last,C comp){T* result=last-1;T value=*result;*result=*first;__adjust_heap(first,0,int(result-first),value,comp);}
template<class T,class C> void __fastcall sort_heap(T* first,T* last,C comp) {
 while(last-first>1) __pop_heap(first,last--,comp);
}
template void __fastcall sort_heap<Element,CKeyPairCompareLess>(Element*,Element*,CKeyPairCompareLess);

}

#include <cstdio>
namespace N007214a0 {
template<class T,class C>void __fastcall __adjust_heap(T* first,int hole,int length,T value,C comp){int top=hole;int child=hole*2+2;while(child<length){if(first[child].Key<first[child-1].Key)--child;first[hole]=first[child];hole=child;child=hole*2+2;}if(child==length){first[hole]=first[child-1];hole=child-1;}int parent=(hole-1)/2;while(hole>top&&first[parent].Key<value.Key){first[hole]=first[parent];hole=parent;parent=(hole-1)/2;}first[hole]=value;}
void Init(Element& e,int key,unsigned int id){e.Key=key;e.A=100+id;e.B=200+id;e.C=300+id;e.D.B=(unsigned char)(11+id);e.D.G=(unsigned char)(21+id);e.D.R=(unsigned char)(31+id);e.D.A=(unsigned char)(41+id);e.E.B=(unsigned char)(51+id);e.E.G=(unsigned char)(61+id);e.E.R=(unsigned char)(71+id);e.E.A=(unsigned char)(81+id);}
bool Payload(const Element& e,unsigned int id){return e.A==100+id&&e.B==200+id&&e.C==300+id&&e.D.B==11+id&&e.D.G==21+id&&e.D.R==31+id&&e.D.A==41+id&&e.E.B==51+id&&e.E.G==61+id&&e.E.R==71+id&&e.E.A==81+id;}
}
int main(){using namespace N007214a0;if(sizeof(Colour)!=4||sizeof(Element)!=24)return 1;CKeyPairCompareLess comp;comp.State=0;Element a[10];for(int i=0;i<10;++i)Init(a[i],7-i,i);sort_heap(a,a+7,comp);for(int j=0;j<7;++j)if(a[j].Key!=j+1||!Payload(a[j],6-j))return 2;
 int keys[7]={4,4,3,2,2,1,1};bool seen[7];for(int k=0;k<7;++k){Init(a[k],keys[k],k);seen[k]=false;}sort_heap(a,a+7,comp);for(int n=0;n<7;++n){unsigned int id=a[n].A-100;if(id>=7||seen[id]||!Payload(a[n],id)||(n&&a[n-1].Key>a[n].Key))return 3;seen[id]=true;}
 Init(a[0],5,8);sort_heap(a,a,comp);sort_heap(a,a+1,comp);if(a[0].Key!=5||!Payload(a[0],8))return 4;Init(a[0],2,8);Init(a[1],1,9);sort_heap(a,a+2,comp);if(a[0].Key!=1||!Payload(a[0],9)||a[1].Key!=2||!Payload(a[1],8))return 5;
 std::printf("007214a0_TEST PASS\n");return 0;}