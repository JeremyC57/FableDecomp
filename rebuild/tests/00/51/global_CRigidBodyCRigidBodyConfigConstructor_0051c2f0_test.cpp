// 0051c2f0 Row0051c2f0::Convert
// Flags: /O2 /Oy /W3 
// Leaf: Convert
#include <new>
namespace Row0051c2f0 {
struct Input{char pad[4];unsigned char kind;};
struct Block{char storage[0x8400];void Init();};
// Layout: exact retail allocation 0x10822; Block members at +0x16 and +0x841e each span 0x8400 bytes; prefix 0x16, gap 0x08, tail 0x04. Control size 0x0c: refs +0x00, deleter +0x04, data +0x08.
#pragma pack(push,1)
struct Payload{char prefix[0x16];Block firstBlock;char gap[0x08];Block secondBlock;char tail[0x04];__forceinline Payload(){firstBlock.Init();secondBlock.Init();}void Apply(Input*);};
#pragma pack(pop)
void __fastcall DeletePayload(void*);
struct Control{unsigned refs;void(__fastcall*destroy)(void*);void*object;__forceinline Control(void*p):refs(1),destroy(DeletePayload),object(p){}};
struct Pair{Payload*object;Control*control;
 __forceinline Pair(Payload*p):object(p),control(p?new Control(p):0){}
 __forceinline Pair(const Pair&x):object(x.object),control(x.control){if(control)++control->refs;}
 __forceinline Pair&operator=(const Pair&x){Control*c=x.control;Payload*o=x.object;if(control!=c){if(control&&--control->refs==0){control->destroy(control->object);operator delete(control);}object=o;control=c;if(control)++control->refs;}return *this;}
 __forceinline ~Pair(){if(control&&--control->refs==0){control->destroy(control->object);operator delete(control);}}
};
Pair __fastcall ConvertEarlier(Input*);
Pair __fastcall Convert(Input*source){Pair result(new Payload);Pair held((Payload*)0);Input*arg=source;
 if(source->kind!=8){held=ConvertEarlier(source);arg=(Input*)held.object;}
 result.object->Apply(arg);return Pair(result);
}
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>
namespace Row0051c2f0 {
static int blocks,applies,deleted,earlierCalls,refsAtApply;static bool returnEmpty;static Input*seen;static Payload*liveObject;static Control*liveControl;
void Block::Init(){++blocks;memset(storage,0,sizeof(storage));}
void Payload::Apply(Input*p){seen=p;++applies;if(p==(Input*)liveObject&&liveObject){assert(liveControl&&liveControl->refs==1);refsAtApply=liveControl->refs;}prefix[0]=37;}
void __fastcall DeletePayload(void*p){++deleted;if(p==liveObject){assert(liveControl->refs==0);liveObject=0;liveControl=0;}operator delete(p);}
Pair __fastcall ConvertEarlier(Input*){++earlierCalls;if(returnEmpty)return Pair((Payload*)0);Pair out(new Payload);liveObject=out.object;liveControl=out.control;return Pair(out);}
}
int main(){using namespace Row0051c2f0;Input in;memset(&in,0,sizeof(in));in.kind=8;
 {Pair p=Convert(&in);assert(p.object&&p.control->refs==1&&seen==&in&&blocks==2&&earlierCalls==0&&p.object->prefix[0]==37);assert(sizeof(Block)==0x8400&&sizeof(Payload)==0x10822&&offsetof(Payload,firstBlock)==0x16&&offsetof(Payload,secondBlock)==0x841e);for(unsigned j=0;j<0x8400;++j)assert(p.object->firstBlock.storage[j]==0&&p.object->secondBlock.storage[j]==0);{Pair copied(p);assert(copied.object==p.object&&p.control->refs==2);copied=copied;assert(p.control->refs==2);}assert(p.control->refs==1);}assert(deleted==1);
 in.kind=0;{Pair p=Convert(&in);assert(p.object&&p.control->refs==1&&applies==2&&blocks==6&&earlierCalls==1&&refsAtApply==1&&!liveObject&&!liveControl&&deleted==2);}assert(deleted==3);
 returnEmpty=true;{Pair p=Convert(&in);assert(p.object&&p.control->refs==1&&seen==0&&applies==3&&earlierCalls==2&&blocks==8&&deleted==3);}assert(deleted==4);
 puts("0051c2f0_TEST PASS");}