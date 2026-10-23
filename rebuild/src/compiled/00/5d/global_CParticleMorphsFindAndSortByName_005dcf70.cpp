// 005dcf70 NErase005dcf70::Manager::RemoveMatching (catalog CParticleMorphs_FindAndSortByName does not fit: body erases CRC+CDefString-matching entries)
// Flags: /O2 /Oy /W3
// Leaf: ?RemoveMatching@Manager@NErase005dcf70@@QAEXABUCCharString@2@@Z
#include <string.h>
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
namespace NErase005dcf70 {
#pragma pack(push,1)
struct CCharStringData {char* text;int size;unsigned capacity;unsigned char flags;int refs;};
#pragma pack(pop)
extern const char Empty[];extern int StringCount;
struct CCharString {
 CCharStringData* data;
 __declspec(noinline) CCharString():data(0){++StringCount;}
 __declspec(noinline) CCharString(const char* text,int length=-1):data(0){++StringCount;if(text&&*text)data=Allocate(text,length);}
 __declspec(noinline) CCharString(const CCharString& x):data(0){++StringCount;if(this!=&x&&x.data){data=x.data;++data->refs;}}
 CCharStringData* Allocate(const char*,int);
 __declspec(noinline) void Clear(){if(data){if(--data->refs<=0){CCharStringData* p=data;if(p->text){operator delete[](p->text);p->text=0;}p->size=0;p->capacity&=0x80000000;delete p;}data=0;}}
 __declspec(noinline) ~CCharString(){Clear();--StringCount;}
 const char* Text()const{return !data?Empty:data->text;}
 int Size()const{return !data?0:data->size;}
};
struct Buffer {char pad[8];char *first,*last,*end;char* Start(){return last!=first?first:reinterpret_cast<char*>(&first);}};
struct CDefStringTable {
 char pad[12];Buffer buffer;
 __declspec(noinline) CCharString GetString(int index){if(index==-1)return CCharString();return CCharString(buffer.Start()+index+4);}
 const char* Text(int index){return index==-1?"":buffer.Start()+index+4;}
};extern CDefStringTable g_DefStringTable;
struct CDefString {
 int index;
 __declspec(noinline) bool operator==(const CCharString& x)const{return strcmp(g_DefStringTable.Text(index),x.Text())==0;}
 __forceinline CCharString GetString()const{CCharString result=g_DefStringTable.GetString(index);return result;}
};
unsigned __fastcall Hash(unsigned,const char*,int);
struct Resource {virtual void v0()=0;virtual void Delete()=0;int refs;};
struct Entry {
 unsigned crc;int a,b;CDefString name;Resource* resource;
 __forceinline Entry& operator=(const Entry& x){crc=x.crc;a=x.a;b=x.b;name=x.name;if(resource!=x.resource){if(resource){if(!--resource->refs)resource->Delete();resource=0;}resource=x.resource;if(resource)++resource->refs;}return *this;}
 __forceinline void Destroy(){Resource* r=resource;if(r){if(!--r->refs)r->Delete();resource=0;}}
};
struct random_access_iterator_tag {random_access_iterator_tag(){}};
template<class T> __declspec(noinline) Entry* __fastcall __copy(Entry* first,Entry* last,Entry* dest,const T&,int){int count=last-first;for(;count>0;--count,++first,++dest)*dest=*first;return dest;}
// Layout: target5dcfc0 key+28; Entry stride20, crc+0, interned index+12, resource+16.
// 4b7993 passes index to9d49f0;5e18f1 copies it raw. CCharString CCharStringData refs+13 is packed17.
struct Manager {Entry *first,*last,*end;char gap[16];CDefString key;__forceinline Entry* Begin()const{return first;}void RemoveMatching(const CCharString&);};
void Manager::RemoveMatching(const CCharString& name){unsigned crc=Hash(0,name.Text(),name.Size());for(Entry* p=Begin();p!=last;){bool found=p->crc==crc&&p->name==key.GetString();if(found){Entry* next=p+1;Entry* dest=p;p=next;if(next!=last)__copy(next,last,dest,random_access_iterator_tag(),0);Entry* tail=--last;tail->Destroy();}else ++p;}}
}