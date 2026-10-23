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