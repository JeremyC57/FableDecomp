// 007f1b70 CTCDRumble::Create(const C3DVector&,EQuakeStrength,EQuakeLength,bool) (catalog: Create; bsim 1.00; callers 00871250/0080d0b0 bind it as CreateCameraShake)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: Create
#include <stdlib.h>
#include <string.h>
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
namespace NFactory007f1b70 {
// Layout: CCharString pointer4, node color/parent/left/right/key/value24; maps12 at CRumbleDef+28/+34, reference count+4.
#pragma pack(push,1)
struct CCharStringData{char*text;unsigned length,capacity;bool useFastExtend;int refs;};
#pragma pack(pop)
extern int StringInstances;
struct CCharString {CCharStringData*data;__declspec(noinline) CCharString(const char*p,int n=-1){if(n<0)n=(int)strlen(p);data=new CCharStringData;data->text=new char[n+1];memcpy(data->text,p,n);data->text[n]=0;data->length=n;data->capacity=n+1;data->useFastExtend=true;data->refs=1;++StringInstances;}__declspec(noinline) CCharString(const CCharString&x):data(x.data){if(data)++data->refs;++StringInstances;}__declspec(noinline) ~CCharString(){if(data&&--data->refs<=0){if(data->text){::operator delete[](data->text);data->text=0;}data->length=0;data->capacity&=0x80000000u;delete data;}data=0;--StringInstances;}};
struct Node {unsigned color;Node*parent,*left,*right;int key;float value;};enum EQuakeStrength{StrengthZero};enum EQuakeLength{LengthZero};struct C3DVector{float x,y,z;};struct CThing;
template<class T>struct Tree {Node*head;unsigned count,extra;__declspec(noinline) void Find(Node*&out,const T&key){Node*p=head->parent;Node*best=head;while(p){if(p->key<key)p=p->right;else{best=p;p=p->left;}}out=(best==head||key<best->key)?head:best;}__forceinline bool GetValue(const T&key,float&out){Node*i;Find(i,key);if(i==head)return false;out=i->value;return true;}};
struct CRumbleDef {virtual void v0()=0;virtual void Delete()=0;int refs;char pad[32];Tree<EQuakeStrength>first;Tree<EQuakeLength>second;};
struct CThingBaseDef {virtual void v0()=0;virtual void Delete()=0;int refs;bool GetSubDef(CRumbleDef*&);};
CThingBaseDef* LookupResource(const char*);bool ResolveDefinition(CThingBaseDef*,CRumbleDef*&);
__declspec(noinline) bool CThingBaseDef::GetSubDef(CRumbleDef*&out){CRumbleDef*value=0;if(!ResolveDefinition(this,value))return false;if(value)++value->refs;if(out!=value){if(out&&!--out->refs)out->Delete();out=value;if(value)++value->refs;}if(value&&!--value->refs)value->Delete();return true;}
struct CGameDefinitionManager {__declspec(noinline) void GetDef(const CCharString&name,CThingBaseDef*&out){CThingBaseDef*p=LookupResource(name.data?name.data->text:"");if(out&&!--out->refs)out->Delete();out=p;if(p)++p->refs;}};CGameDefinitionManager* __fastcall GetDefinitionManager();
struct CTCDRumble{static CThing* __fastcall Create(const C3DVector&,float,float,bool);static CThing* __fastcall Create(const C3DVector&,EQuakeStrength,EQuakeLength,bool);};
CThing* __fastcall CTCDRumble::Create(const C3DVector&owner,EQuakeStrength key,EQuakeLength kind,bool enabled){CGameDefinitionManager*db=GetDefinitionManager();CThingBaseDef*resource=0;db->GetDef("THING_RUMBLE",resource);CRumbleDef*def=0;resource->GetSubDef(def);float value,length;if(!def->first.GetValue(key,value)){if(!--def->refs)def->Delete();if(resource&&!--resource->refs)resource->Delete();return 0;}if(!def->second.GetValue(kind,length)){if(!--def->refs)def->Delete();if(resource&&!--resource->refs)resource->Delete();return 0;}CThing*result=Create(owner,value,length,enabled);if(!--def->refs)def->Delete();if(resource&&!--resource->refs)resource->Delete();return result;}
}

void* __cdecl operator new[](unsigned int);

#include <new>

#include <assert.h>
#include <stdio.h>
namespace NFactory007f1b70 {
int StringInstances;static int deletes,creates;struct TestRumbleDef:CRumbleDef{void v0(){}void Delete(){++deletes;}};struct TestThingBaseDef:CThingBaseDef{void v0(){}void Delete(){++deletes;}};static TestRumbleDef def;static TestThingBaseDef resource;static CGameDefinitionManager database;struct CThing{int marker;};static CThing token;static bool expectedEnabled;
CThingBaseDef* LookupResource(const char*name){assert(!strcmp(name,"THING_RUMBLE"));return &resource;}static bool propertyPresent=true;static CRumbleDef*resolved=&def;bool ResolveDefinition(CThingBaseDef*p,CRumbleDef*&out){assert(p==&resource);if(!propertyPresent)return false;out=resolved;return true;}CGameDefinitionManager* __fastcall GetDefinitionManager(){return &database;}
CThing* __fastcall CTCDRumble::Create(const C3DVector&owner,float first,float second,bool enabled){assert(owner.x==31&&owner.y==-2&&owner.z==5&&enabled==expectedEnabled&&first==0.75f&&second==2.5f);++creates;return &token;}
}

static void*repairWatched;static int repairDeletes;void __cdecl operator delete[](void*p){if(p==repairWatched)++repairDeletes;free(p);}
namespace NFactory007f1b70{static void OwnershipContractProbe(){{CCharString x("ownership");x.data->useFastExtend=false;repairWatched=x.data->text;{CCharString copy(x);assert(x.data->refs==2);}assert(x.data->refs==1&&!repairDeletes);}assert(repairDeletes==1);{CCharString x("ownership");x.data->useFastExtend=false;x.data->refs=0;repairWatched=x.data->text;}assert(repairDeletes==2);}}

namespace NFactory007f1b70 {
static void ResolutionContractProbe(){
 TestRumbleDef previous;previous.refs=1;CRumbleDef*out=&previous;int before=deletes;
 propertyPresent=false;resolved=0;assert(!resource.GetSubDef(out)&&out==&previous&&previous.refs==1&&deletes==before);
 propertyPresent=true;assert(resource.GetSubDef(out)&&out==0&&previous.refs==0&&deletes==before+1);
 def.refs=2;resolved=&def;assert(resource.GetSubDef(out)&&out==&def&&def.refs==3);assert(resource.GetSubDef(out)&&out==&def&&def.refs==3);
 --out->refs;out=0;assert(def.refs==2);previous.refs=1;out=&previous;assert(resource.GetSubDef(out)&&out==&def&&previous.refs==0&&def.refs==3&&deletes==before+2);
 --out->refs;assert(def.refs==2);def.refs=0;deletes=before;
}
}
int main(){NFactory007f1b70::ResolutionContractProbe();NFactory007f1b70::OwnershipContractProbe();using namespace NFactory007f1b70;assert(sizeof(CCharStringData)==17);{CCharString shared("many");CCharString*copies=(CCharString*)operator new(140*sizeof(CCharString));for(int i=0;i<140;++i)new(copies+i)CCharString(shared);assert(shared.data->refs==141);for(int j=0;j<140;++j)copies[j].~CCharString();operator delete(copies);assert(shared.data->refs==1&&StringInstances==1);}assert(!StringInstances);C3DVector position={31,-2,5};Node h1,h2,a,b;memset(&h1,0,sizeof h1);memset(&h2,0,sizeof h2);memset(&a,0,sizeof a);memset(&b,0,sizeof b);a.key=2;a.value=0.75f;b.key=3;b.value=2.5f;def.refs=resource.refs=1;def.first.head=&h1;def.second.head=&h2;h1.parent=&a;h2.parent=&b;assert(!CTCDRumble::Create(position,(EQuakeStrength)1,(EQuakeLength)3,true)&&!creates&&def.refs==1&&resource.refs==1&&!StringInstances);assert(!CTCDRumble::Create(position,(EQuakeStrength)2,(EQuakeLength)4,true)&&!creates&&def.refs==1&&resource.refs==1&&!StringInstances);for(int i=0;i<2;++i){expectedEnabled=i!=0;assert(CTCDRumble::Create(position,(EQuakeStrength)2,(EQuakeLength)3,expectedEnabled)==&token&&creates==i+1&&def.refs==1&&resource.refs==1&&!StringInstances);}h1.parent=0;assert(!CTCDRumble::Create(position,(EQuakeStrength)2,(EQuakeLength)3,true)&&creates==2&&def.refs==1&&resource.refs==1);assert(!deletes);h1.parent=&a;def.refs=resource.refs=0;expectedEnabled=true;assert(CTCDRumble::Create(position,(EQuakeStrength)2,(EQuakeLength)3,true)==&token&&deletes==2&&def.refs==0&&resource.refs==0&&!StringInstances);puts("007f1b70_TEST PASS");return 0;}