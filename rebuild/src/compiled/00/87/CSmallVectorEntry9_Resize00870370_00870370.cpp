// 00870370 SmallVector<Entry9>::Resize
// Flags: /O2 /Oy /Gr
#include <new>
void* __cdecl operator new[](unsigned int size);   // MSVCR71 ??_U, import thunk 0x00BFEB22
void __cdecl operator delete[](void* p);            // MSVCR71 ??_V, import thunk 0x00BFEB1C
// Leaf: Resize
// Layout: retail small vector pointer+0 and signed-byte size+4. Entry_00870370 packed9: signed key+0, object pointer+4, byte+8.
#pragma pack(push,1)
struct Entry_00870370{int key;void* object;char flag;Entry_00870370():key(-1),object(0),flag(0){}};
#pragma pack(pop)
template<class T> struct SmallVector{T* data;signed char count;void Resize(int,const T&);T* Erase(T*);};
template<class T> inline const T& Min(const T&a,const T&b){return a<b?a:b;}
template<class T> void SmallVector<T>::Resize(int requested,const T& fill){
 if(requested>0){
  T* next=new T[requested];
  for(int i=0;i<Min(requested,(int)count);++i)next[i]=data[i];
  int extra=requested-count;
  if(extra<=0x7fffffff){
   if(extra>0){for(int j=0;j<requested-count;++j)next[count+j]=fill;}
   delete[] data;data=next;count=(signed char)requested;
  }
 }else{delete[] data;data=0;count=0;}
}
template void SmallVector<Entry_00870370>::Resize(int,const Entry_00870370&);