// CTCDRumble::Create(const C3DVector&,EQuakeStrength,EQuakeLength,bool) -- look up the THING_RUMBLE rumble def, map the quake strength and length enums to its float values and create the rumble through the float overload (Ego_r.pdb 0x007EB500; was _global::Create in namespace NFactory007f1b70)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: ?Create@CTCDRumble@@SIPAVCThing@@ABVC3DVector@@W4EQuakeStrength@@W4EQuakeLength@@_N@Z
#include "fable_compiler.h"
#include <stdlib.h>
#include <string.h>
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
// agentNF19: the row's own class, its argument types and the inner Create overload are at global scope, as retail spells them
// (Ego_r.pdb 0x007EB500 ?Create@CTCDRumble@@SIPAVCThing@@ABVC3DVector@@W4EQuakeStrength@@W4EQuakeLength@@_N@Z and 0x007EB2C0
// ?Create@CTCDRumble@@SIPAVCThing@@ABVC3DVector@@MM_N@Z, both public static __fastcall); the helper stand-ins stay in the namespace.
class CThing;
#include "engine/C3DVector.h"
enum EQuakeStrength { StrengthZero };
enum EQuakeLength { LengthZero };
class CTCDRumble { public:
    static CThing* __fastcall Create(const C3DVector& position, float strength, float length, bool flag);
    static CThing* __fastcall Create(const C3DVector& position, EQuakeStrength strength, EQuakeLength length, bool flag);
};
namespace NFactory007f1b70 {
// Layout: CCharString pointer4, node color/parent/left/right/key/value24; maps12 at CRumbleDef+28/+34, reference count+4.
#pragma pack(push,1)
struct CCharStringData{char*text;unsigned length,capacity;bool useFastExtend;int refs;};
#pragma pack(pop)
extern int StringInstances;
struct CCharString {CCharStringData*data;__declspec(noinline) CCharString(const char*p,int n=-1){if(n<0)n=(int)strlen(p);data=new CCharStringData;data->text=new char[n+1];memcpy(data->text,p,n);data->text[n]=0;data->length=n;data->capacity=n+1;data->useFastExtend=true;data->refs=1;++StringInstances;}__declspec(noinline) CCharString(const CCharString&x):data(x.data){if(data)++data->refs;++StringInstances;}__declspec(noinline) ~CCharString(){if(data&&--data->refs<=0){if(data->text){::operator delete[](data->text);data->text=0;}data->length=0;data->capacity&=0x80000000u;delete data;}data=0;--StringInstances;}};
struct Node {unsigned color;Node*parent,*left,*right;int key;float value;};
template<class T>struct Tree {Node*head;unsigned count,extra;__declspec(noinline) void Find(Node*&out,const T&key){Node*p=head->parent;Node*best=head;while(p){if(p->key<key)p=p->right;else{best=p;p=p->left;}}out=(best==head||key<best->key)?head:best;}FABLE_FORCEINLINE bool GetValue(const T&key,float&out){Node*i;Find(i,key);if(i==head)return false;out=i->value;return true;}};
struct CRumbleDef {virtual void v0()=0;virtual void Delete()=0;int refs;char pad[32];Tree<EQuakeStrength>first;Tree<EQuakeLength>second;};
struct CThingBaseDef {virtual void v0()=0;virtual void Delete()=0;int refs;bool GetSubDef(CRumbleDef*&);};
CThingBaseDef* LookupResource(const char*);bool ResolveDefinition(CThingBaseDef*,CRumbleDef*&);
__declspec(noinline) bool CThingBaseDef::GetSubDef(CRumbleDef*&out){CRumbleDef*value=0;if(!ResolveDefinition(this,value))return false;if(value)++value->refs;if(out!=value){if(out&&!--out->refs)out->Delete();out=value;if(value)++value->refs;}if(value&&!--value->refs)value->Delete();return true;}
struct CGameDefinitionManager {__declspec(noinline) void GetDef(const CCharString&name,CThingBaseDef*&out){CThingBaseDef*p=LookupResource(name.data?name.data->text:"");if(out&&!--out->refs)out->Delete();out=p;if(p)++p->refs;}};CGameDefinitionManager* __fastcall GetDefinitionManager();
}
using namespace NFactory007f1b70;
CThing* __fastcall CTCDRumble::Create(const C3DVector&owner,EQuakeStrength key,EQuakeLength kind,bool enabled){CGameDefinitionManager*db=GetDefinitionManager();CThingBaseDef*resource=0;db->GetDef("THING_RUMBLE",resource);CRumbleDef*def=0;resource->GetSubDef(def);float value,length;if(!def->first.GetValue(key,value)){if(!--def->refs)def->Delete();if(resource&&!--resource->refs)resource->Delete();return 0;}if(!def->second.GetValue(kind,length)){if(!--def->refs)def->Delete();if(resource&&!--resource->refs)resource->Delete();return 0;}CThing*result=Create(owner,value,length,enabled);if(!--def->refs)def->Delete();if(resource&&!--resource->refs)resource->Delete();return result;}
