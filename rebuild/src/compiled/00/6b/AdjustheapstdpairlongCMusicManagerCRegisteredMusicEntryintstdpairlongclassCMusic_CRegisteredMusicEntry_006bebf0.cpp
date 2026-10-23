// 006bebf0 AdjustHeap
// Flags: /O2 /Oy /W3 /Gr
// Leaf: AdjustHeap
// Layout: PDB19267-19276 CRegisteredMusicEntry20 has two WideStrings0/4, Gain8, Offset12, CacheToHD16; pair24 places it at4. Retail99b758/99b7f5 increments shared DWORD+12.
#include <new>
namespace N006bebf0 {
struct WideStringData;
struct WideString {WideStringData* Data;WideString();WideString(const WideString&);~WideString();WideString& operator=(const WideString&);};
struct RegisteredMusicEntry {WideString SrcFileName,DstFileName;float Gain,Offset;bool CacheToHD;};
struct Element {int Key;RegisteredMusicEntry Music;};
struct Compare {unsigned int State;__forceinline bool operator()(const Element& a,const Element& b)const{return a.Key<b.Key;}};
template<class T,class C>void __fastcall PushHeap(T*,int,int,T,C);
template<class T,class C>void __fastcall AdjustHeap(T* first,int hole,int length,T value,C comp){
 int top=hole;int second=2*hole+2;
 while(second<length){if(comp(first[second],first[second-1]))--second;first[hole]=first[second];hole=second;second=2*(second+1);}
 if(second==length){first[hole]=first[second-1];hole=second-1;}
 PushHeap(first,hole,top,value,comp);
}
template void __fastcall AdjustHeap<Element,Compare>(Element*,int,int,Element,Compare);
}