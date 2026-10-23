// 007f1b70 NFactory007f1b70::Create
// Flags: /O2 /Oy /W3 /Gr
// Leaf: Create
#include <stdlib.h>
#include <string.h>
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
namespace NFactory007f1b70 {
// Layout: String pointer4, node color/parent/left/right/key/value24; maps12 at Def+28/+34, reference count+4.
#pragma pack(push,1)
struct StringData{char*text;unsigned length,capacity;bool useFastExtend;int refs;};
#pragma pack(pop)
extern int StringInstances;
struct String {StringData*data;__declspec(noinline) String(const char*p,int n=-1){if(n<0)n=(int)strlen(p);data=new StringData;data->text=new char[n+1];memcpy(data->text,p,n);data->text[n]=0;data->length=n;data->capacity=n+1;data->useFastExtend=true;data->refs=1;++StringInstances;}__declspec(noinline) String(const String&x):data(x.data){if(data)++data->refs;++StringInstances;}__declspec(noinline) ~String(){if(data&&--data->refs<=0){if(data->text){::operator delete[](data->text);data->text=0;}data->length=0;data->capacity&=0x80000000u;delete data;}data=0;--StringInstances;}};
struct Node {unsigned color;Node*parent,*left,*right;int key;float value;};enum EQuakeStrength{StrengthZero};enum EQuakeLength{LengthZero};struct Vec3{float x,y,z;};struct CThing;
template<class T>struct Tree {Node*head;unsigned count,extra;__declspec(noinline) void Find(Node*&out,const T&key){Node*p=head->parent;Node*best=head;while(p){if(p->key<key)p=p->right;else{best=p;p=p->left;}}out=(best==head||key<best->key)?head:best;}__forceinline bool GetValue(const T&key,float&out){Node*i;Find(i,key);if(i==head)return false;out=i->value;return true;}};
struct Def {virtual void v0()=0;virtual void Delete()=0;int refs;char pad[32];Tree<EQuakeStrength>first;Tree<EQuakeLength>second;};
struct Resource {virtual void v0()=0;virtual void Delete()=0;int refs;bool Get(Def*&);};
Resource* LookupResource(const char*);bool ResolveDefinition(Resource*,Def*&);
__declspec(noinline) bool Resource::Get(Def*&out){Def*value=0;if(!ResolveDefinition(this,value))return false;if(value)++value->refs;if(out!=value){if(out&&!--out->refs)out->Delete();out=value;if(value)++value->refs;}if(value&&!--value->refs)value->Delete();return true;}
struct Database {__declspec(noinline) void Lookup(const String&name,Resource*&out){Resource*p=LookupResource(name.data?name.data->text:"");if(out&&!--out->refs)out->Delete();out=p;if(p)++p->refs;}};Database* __fastcall DatabaseInstance();
CThing* __fastcall CreateFrom(const Vec3&,float,float,bool);
CThing* __fastcall Create(const Vec3&owner,EQuakeStrength key,EQuakeLength kind,bool enabled){Database*db=DatabaseInstance();Resource*resource=0;db->Lookup("THING_RUMBLE",resource);Def*def=0;resource->Get(def);float value,length;if(!def->first.GetValue(key,value)){if(!--def->refs)def->Delete();if(resource&&!--resource->refs)resource->Delete();return 0;}if(!def->second.GetValue(kind,length)){if(!--def->refs)def->Delete();if(resource&&!--resource->refs)resource->Delete();return 0;}CThing*result=CreateFrom(owner,value,length,enabled);if(!--def->refs)def->Delete();if(resource&&!--resource->refs)resource->Delete();return result;}
}