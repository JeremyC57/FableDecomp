// 005dcf70 NErase005dcf70::Manager::RemoveMatching
// Flags: /O2 /Oy /W3
// Leaf: ?RemoveMatching@Manager@NErase005dcf70@@QAEXABUString@2@@Z
#include <string.h>
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
namespace NErase005dcf70 {
#pragma pack(push,1)
struct Data {char* text;int size;unsigned capacity;unsigned char flags;int refs;};
#pragma pack(pop)
extern const char Empty[];extern int StringCount;
struct String {
 Data* data;
 __declspec(noinline) String():data(0){++StringCount;}
 __declspec(noinline) String(const char* text,int length=-1):data(0){++StringCount;if(text&&*text)data=Allocate(text,length);}
 __declspec(noinline) String(const String& x):data(0){++StringCount;if(this!=&x&&x.data){data=x.data;++data->refs;}}
 Data* Allocate(const char*,int);
 __declspec(noinline) void Clear(){if(data){if(--data->refs<=0){Data* p=data;if(p->text){operator delete[](p->text);p->text=0;}p->size=0;p->capacity&=0x80000000;delete p;}data=0;}}
 __declspec(noinline) ~String(){Clear();--StringCount;}
 const char* Text()const{return !data?Empty:data->text;}
 int Size()const{return !data?0:data->size;}
};
struct Buffer {char pad[8];char *first,*last,*end;char* Start(){return last!=first?first:reinterpret_cast<char*>(&first);}};
struct Names {
 char pad[12];Buffer buffer;
 __declspec(noinline) String Get(int index){if(index==-1)return String();return String(buffer.Start()+index+4);}
 const char* Text(int index){return index==-1?"":buffer.Start()+index+4;}
};extern Names gNames;
struct NameHandle {
 int index;
 __declspec(noinline) bool Equals(const String& x)const{return strcmp(gNames.Text(index),x.Text())==0;}
 __forceinline String GetString()const{String result=gNames.Get(index);return result;}
};
unsigned __fastcall Hash(unsigned,const char*,int);
struct Resource {virtual void v0()=0;virtual void Delete()=0;int refs;};
struct Entry {
 unsigned crc;int a,b;NameHandle name;Resource* resource;
 __forceinline Entry& operator=(const Entry& x){crc=x.crc;a=x.a;b=x.b;name=x.name;if(resource!=x.resource){if(resource){if(!--resource->refs)resource->Delete();resource=0;}resource=x.resource;if(resource)++resource->refs;}return *this;}
 __forceinline void Destroy(){Resource* r=resource;if(r){if(!--r->refs)r->Delete();resource=0;}}
};
struct Tag {Tag(){}};
template<class T> __declspec(noinline) Entry* __fastcall Copy(Entry* first,Entry* last,Entry* dest,const T&,int){int count=last-first;for(;count>0;--count,++first,++dest)*dest=*first;return dest;}
// Layout: target5dcfc0 key+28; Entry stride20, crc+0, interned index+12, resource+16.
// 4b7993 passes index to9d49f0;5e18f1 copies it raw. String Data refs+13 is packed17.
struct Manager {Entry *first,*last,*end;char gap[16];NameHandle key;__forceinline Entry* Begin()const{return first;}void RemoveMatching(const String&);};
void Manager::RemoveMatching(const String& name){unsigned crc=Hash(0,name.Text(),name.Size());for(Entry* p=Begin();p!=last;){bool found=p->crc==crc&&p->name.Equals(key.GetString());if(found){Entry* next=p+1;Entry* dest=p;p=next;if(next!=last)Copy(next,last,dest,Tag(),0);Entry* tail=--last;tail->Destroy();}else ++p;}}
}

#include <assert.h>
#include <stdio.h>
#include <stddef.h>
namespace NErase005dcf70 {
const char Empty[]="";int StringCount=0;Names gNames;
Data* String::Allocate(const char* s,int n){if(n==-1)n=(int)strlen(s);Data* d=new Data;d->size=n;d->capacity=n+1;d->flags=1;d->refs=1;d->text=static_cast<char*>(operator new[](n+1));memcpy(d->text,s,n);d->text[n]=0;return d;}
unsigned __fastcall Hash(unsigned seed,const char* s,int n){unsigned value=seed;for(int i=0;i<n;++i){value^=(unsigned char)s[i];for(int b=0;b<8;++b)value=(value>>1)^((value&1)?0xedb88320u:0);}return value;}
static int live[8],deaths[8];
struct Tracked:Resource {int id;Tracked(int n):id(n){refs=0;++live[n];}virtual void v0(){}virtual void Delete(){assert(refs==0);--live[id];++deaths[id];delete this;}};
static void Bind(Entry& e,unsigned c,int index,int id,Resource* r){e.crc=c;e.a=id;e.b=id+100;e.name.index=index;e.resource=r;if(r)++r->refs;}
static void Cleanup(Entry* first,Entry* last){for(;first!=last;++first)first->Destroy();}
}
int main(){using namespace NErase005dcf70;
assert(sizeof(Data)==17&&offsetof(Data,refs)==13&&sizeof(Entry)==20&&sizeof(NameHandle)==4);
char names[32];memset(names,0,sizeof(names));memcpy(names+4,"foo",4);memcpy(names+12,"bar",4);memcpy(names+20,"baz",4);memset(&gNames,0,sizeof(gNames));gNames.buffer.first=names;gNames.buffer.last=gNames.buffer.end=names+sizeof(names);
{String input("foo");assert(StringCount==1&&input.data->refs==1);unsigned hash=Hash(0,input.Text(),input.Size());assert(hash!=0);Manager manager;memset(&manager,0,sizeof(manager));manager.key.index=0;manager.RemoveMatching(input);assert(StringCount==1);
Entry entries[5];memset(entries,0,sizeof(entries));Tracked* a=new Tracked(0);Tracked* shared=new Tracked(1);Tracked* c=new Tracked(2);Tracked* e=new Tracked(3);
Bind(entries[0],hash,0,10,a);Bind(entries[1],hash,0,11,shared);Bind(entries[2],hash,8,12,c);Bind(entries[3],hash,0,13,shared);Bind(entries[4],hash+1,16,14,e);
manager.first=entries;manager.last=manager.end=entries+5;assert(shared->refs==2);manager.RemoveMatching(input);
assert(manager.last==entries+3&&manager.end==entries+5);assert(entries[0].a==11&&entries[1].a==12&&entries[2].a==14);assert(entries[0].b==111&&entries[1].name.index==8);assert(entries[3].resource==0&&entries[4].resource==0);assert(deaths[0]==1&&shared->refs==1&&c->refs==1&&e->refs==1);assert(StringCount==1&&input.data->refs==1);
// Retail advances past the shifted replacement, so the adjacent matching entry remains.
assert(entries[0].name.index==0&&entries[0].crc==hash);
Cleanup(manager.first,manager.last);assert(deaths[1]==1&&deaths[2]==1&&deaths[3]==1);
// CRC mismatch, equal CRC but unequal interned name, and null resources.
memset(entries,0,sizeof(entries));Bind(entries[0],hash+1,0,20,0);Bind(entries[1],hash,8,21,0);Bind(entries[2],hash,16,22,0);manager.first=entries;manager.last=manager.end=entries+3;manager.RemoveMatching(input);assert(manager.last==entries+3&&entries[1].a==21&&StringCount==1);
// Empty key and empty String use the -1 interned-name sentinel.
{String empty;assert(empty.data==0);manager.key.index=-1;Bind(entries[0],0,-1,30,0);Bind(entries[1],1,8,31,0);manager.last=manager.end=entries+2;manager.RemoveMatching(empty);assert(manager.last==entries+1&&entries[0].a==31);assert(StringCount==2);}
// Direct overlapping copy exercises self assignment and equal shared pointers.
Tracked* pair=new Tracked(4);memset(entries,0,sizeof(entries));Bind(entries[0],hash,0,40,pair);Bind(entries[1],hash,8,41,pair);Entry* finish=Copy(entries,entries+2,entries,Tag(),0);assert(finish==entries+2&&pair->refs==2);finish=Copy(entries+1,entries+2,entries,Tag(),0);assert(finish==entries+1&&entries[0].a==41&&pair->refs==2);Cleanup(entries,entries+2);assert(deaths[4]==1);
{String copied(input);assert(input.data->refs==2&&StringCount==2);}assert(input.data->refs==1&&StringCount==1);
}
assert(StringCount==0);for(int i=0;i<8;++i)assert(live[i]==0);puts("005dcf70_TEST PASS");return 0;}