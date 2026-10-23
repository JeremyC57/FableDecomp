// 006bebf0 N006bebf0::__adjust_heap<Element,CKeyPairCompareLess> (STLport; catalog: _Adjust_heap<std::pair<long,CMusicManager::CRegisteredMusicEntry>*,int,std::pair<long,class_CMusicManager::CRegisteredMusicEntry>,CKeyPairCompareLess<long,CMusicManager::_Adjust_heap<std::pair<long,CM)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: __adjust_heap
// Layout: PDB19267-19276 CRegisteredMusicEntry20 has two WideStrings0/4, Gain8, Offset12, CacheToHD16; pair24 places it at4. Retail99b758/99b7f5 increments shared DWORD+12.
#include <new>
namespace N006bebf0 {
struct CWideStringData;
struct CWideString {CWideStringData* Data;CWideString();CWideString(const CWideString&);~CWideString();CWideString& operator=(const CWideString&);};
struct CRegisteredMusicEntry {CWideString SrcFileName,DstFileName;float Gain,Offset;bool CacheToHD;};
struct Element {int Key;CRegisteredMusicEntry Music;};
struct CKeyPairCompareLess {unsigned int State;__forceinline bool operator()(const Element& a,const Element& b)const{return a.Key<b.Key;}};
template<class T,class C>void __fastcall __push_heap(T*,int,int,T,C);
template<class T,class C>void __fastcall __adjust_heap(T* first,int hole,int length,T value,C comp){
 int top=hole;int second=2*hole+2;
 while(second<length){if(comp(first[second],first[second-1]))--second;first[hole]=first[second];hole=second;second=2*(second+1);}
 if(second==length){first[hole]=first[second-1];hole=second-1;}
 __push_heap(first,hole,top,value,comp);
}
template void __fastcall __adjust_heap<Element,CKeyPairCompareLess>(Element*,int,int,Element,CKeyPairCompareLess);
}