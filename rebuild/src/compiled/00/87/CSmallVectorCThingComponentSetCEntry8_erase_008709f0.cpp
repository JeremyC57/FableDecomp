// 008709f0 SmallVector<Entry9>::Erase
// Flags: /O2 /Oy /Gr
#include <new>
void* __cdecl operator new[](unsigned int size);   // MSVCR71 ??_U, import thunk 0x00BFEB22
void __cdecl operator delete[](void* p);            // MSVCR71 ??_V, import thunk 0x00BFEB1C
// Leaf: Erase
// Layout: retail small vector pointer+0 and signed-byte size+4. Entry_008709f0 packed9: signed key+0, object pointer+4, byte+8.
#pragma pack(push,1)
struct Entry_008709f0{int key;void* object;char flag;Entry_008709f0():key(-1),object(0),flag(0){}};
#pragma pack(pop)
template<class T> struct SmallVector{T* data;signed char count;void Resize(int,const T&);T* Erase(T*);};
template<class T> inline const T& Min(const T&a,const T&b){return a<b?a:b;}
template<class T> T* SmallVector<T>::Erase(T* position){
 T* next=new T[count-1];int index=position-data;
 for(int i=0;i<index;++i)next[i]=data[i];
 for(int j=index+1;j<count;++j)next[j-1]=data[j];
 int oldCount=count;delete[] data;data=next;count=(signed char)(oldCount-1);
 if(index>count)return next+count;return next+index;
}
template Entry_008709f0* SmallVector<Entry_008709f0>::Erase(Entry_008709f0*);