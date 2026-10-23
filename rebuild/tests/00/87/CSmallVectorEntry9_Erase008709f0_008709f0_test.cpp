#include <stdio.h>
#include <assert.h>
// BEGIN exact source snapshot SHA256 888daaf58c9f8887d44ebfc7e85f603156cec6f3dce04af17029a2a0235bee14
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

// END exact source snapshot
int main(){SmallVector<Entry_008709f0> v;v.data=new Entry_008709f0[3];v.count=3;for(int i=0;i<3;++i){v.data[i].key=i;v.data[i].object=(void*)(i+1);v.data[i].flag=(char)i;}
Entry_008709f0* result=v.Erase(v.data+1);assert(v.count==2 && result==v.data+1 && v.data[1].key==2 && v.data[0].object==(void*)1);result=v.Erase(v.data+1);assert(v.count==1 && result==v.data+1);result=v.Erase(v.data);assert(v.count==0 && result==v.data);delete[] v.data;
puts("008709f0_TEST PASS");return 0;}