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

void* __cdecl operator new[](unsigned int);

#include <new>

#include <assert.h>
#include <stdio.h>
namespace NFactory007f1b70 {
int StringInstances;static int deletes,creates;struct TestDef:Def{void v0(){}void Delete(){++deletes;}};struct TestResource:Resource{void v0(){}void Delete(){++deletes;}};static TestDef def;static TestResource resource;static Database database;struct CThing{int marker;};static CThing token;static bool expectedEnabled;
Resource* LookupResource(const char*name){assert(!strcmp(name,"THING_RUMBLE"));return &resource;}static bool propertyPresent=true;static Def*resolved=&def;bool ResolveDefinition(Resource*p,Def*&out){assert(p==&resource);if(!propertyPresent)return false;out=resolved;return true;}Database* __fastcall DatabaseInstance(){return &database;}
CThing* __fastcall CreateFrom(const Vec3&owner,float first,float second,bool enabled){assert(owner.x==31&&owner.y==-2&&owner.z==5&&enabled==expectedEnabled&&first==0.75f&&second==2.5f);++creates;return &token;}
}

static void*repairWatched;static int repairDeletes;void __cdecl operator delete[](void*p){if(p==repairWatched)++repairDeletes;free(p);}
namespace NFactory007f1b70{static void OwnershipContractProbe(){{String x("ownership");x.data->useFastExtend=false;repairWatched=x.data->text;{String copy(x);assert(x.data->refs==2);}assert(x.data->refs==1&&!repairDeletes);}assert(repairDeletes==1);{String x("ownership");x.data->useFastExtend=false;x.data->refs=0;repairWatched=x.data->text;}assert(repairDeletes==2);}}

namespace NFactory007f1b70 {
static void ResolutionContractProbe(){
 TestDef previous;previous.refs=1;Def*out=&previous;int before=deletes;
 propertyPresent=false;resolved=0;assert(!resource.Get(out)&&out==&previous&&previous.refs==1&&deletes==before);
 propertyPresent=true;assert(resource.Get(out)&&out==0&&previous.refs==0&&deletes==before+1);
 def.refs=2;resolved=&def;assert(resource.Get(out)&&out==&def&&def.refs==3);assert(resource.Get(out)&&out==&def&&def.refs==3);
 --out->refs;out=0;assert(def.refs==2);previous.refs=1;out=&previous;assert(resource.Get(out)&&out==&def&&previous.refs==0&&def.refs==3&&deletes==before+2);
 --out->refs;assert(def.refs==2);def.refs=0;deletes=before;
}
}
int main(){NFactory007f1b70::ResolutionContractProbe();NFactory007f1b70::OwnershipContractProbe();using namespace NFactory007f1b70;assert(sizeof(StringData)==17);{String shared("many");String*copies=(String*)operator new(140*sizeof(String));for(int i=0;i<140;++i)new(copies+i)String(shared);assert(shared.data->refs==141);for(int j=0;j<140;++j)copies[j].~String();operator delete(copies);assert(shared.data->refs==1&&StringInstances==1);}assert(!StringInstances);Vec3 position={31,-2,5};Node h1,h2,a,b;memset(&h1,0,sizeof h1);memset(&h2,0,sizeof h2);memset(&a,0,sizeof a);memset(&b,0,sizeof b);a.key=2;a.value=0.75f;b.key=3;b.value=2.5f;def.refs=resource.refs=1;def.first.head=&h1;def.second.head=&h2;h1.parent=&a;h2.parent=&b;assert(!Create(position,(EQuakeStrength)1,(EQuakeLength)3,true)&&!creates&&def.refs==1&&resource.refs==1&&!StringInstances);assert(!Create(position,(EQuakeStrength)2,(EQuakeLength)4,true)&&!creates&&def.refs==1&&resource.refs==1&&!StringInstances);for(int i=0;i<2;++i){expectedEnabled=i!=0;assert(Create(position,(EQuakeStrength)2,(EQuakeLength)3,expectedEnabled)==&token&&creates==i+1&&def.refs==1&&resource.refs==1&&!StringInstances);}h1.parent=0;assert(!Create(position,(EQuakeStrength)2,(EQuakeLength)3,true)&&creates==2&&def.refs==1&&resource.refs==1);assert(!deletes);h1.parent=&a;def.refs=resource.refs=0;expectedEnabled=true;assert(Create(position,(EQuakeStrength)2,(EQuakeLength)3,true)==&token&&deletes==2&&def.refs==0&&resource.refs==0&&!StringInstances);puts("007f1b70_TEST PASS");return 0;}