// CDefPointeeBase_Derived_004d9861::~CDefPointeeBase_Derived_004d9861() -- empty body: destroys the CCharString members at +0x2c, +0x28, then the inlined CDefPointeeBase::~CDefPointeeBase stores its vtable and tail-calls CBase_009fc550::~CBase_009fc550
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D9861, oracle length 33 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x2c] ; call CCharString::~CCharString ; lea ecx,[esi+0x28] ; call CCharString::~CCharString ; mov dword ptr [esi],offset CDefPointeeBase::`vftable' ; mov ecx,esi ; pop esi ; jmp CBase_009fc550::~CBase_009fc550
// Plain destructor (`??1`) with an empty body of a class derived from CDefPointeeBase (the vtable stored,
// 0x01230BA0 in rtti-vtables.tsv): the CCharString members are destroyed in reverse declaration
// order (~CCharString @ 0x0099EAE0, fable_string.h), then the inline, empty CDefPointeeBase::~CDefPointeeBase is
// inlined: its vtable store (this class has none: it is `__declspec(novtable)`, i.e. abstract) and the
// tail call to the next base destructor @ 0x009FC550. The derived class cannot be named from the body.
// Non-virtual destructors keep the lander on the plain `??1` body (GOTCHAS). One of 9 bodies.
#include "fable_string.h"

class CResource { public:
    virtual void v00();
    virtual ~CResource();                          // 0x009FC550
};

class CDefPointeeBase : public CResource { public:
    virtual void v00();
    ~CDefPointeeBase() { }                          // inline: inlined into every derived destructor
};

// __declspec(novtable): this (abstract) class never stores its own vtable, so only the inlined
// CDefPointeeBase destructor's store appears -- the only way VC7.1 emits the retail order.
struct __declspec(novtable) CDefPointeeBase_Derived_004d9861 : CDefPointeeBase {
    unsigned char _pad[0x24];
    CCharString m_string0;              // +0x28, destroyed in reverse order
    CCharString m_string1;              // +0x2c, destroyed in reverse order
    virtual void v00();
    ~CDefPointeeBase_Derived_004d9861();
};

CDefPointeeBase_Derived_004d9861::~CDefPointeeBase_Derived_004d9861()
{
}