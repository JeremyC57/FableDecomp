// CMessageOnSetThingOwner::CMessageOnSetThingOwner(void*) -- base CBase_0099a2d0(), the argument stored at +0x4, then this vtable
// Leaf: ??0CMessageOnSetThingOwner@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E8310, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBaseClass::CBaseClass ; mov eax,[esp+8] ; mov [esi+0x4],eax ; mov dword ptr [esi],offset CMessageOnSetThingOwner::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor: base CBaseClass() @ 0x0099A2D0, the argument kept at +0x4 and
// this class's vtable (0x01270BF0, named through rtti-vtables.tsv). The argument type is not
// recoverable from the body; `void*` models it. One of 15 bodies of this shape.
class CBaseClass {
public:
    virtual ~CBaseClass();   // 0x0099A2E0
    CBaseClass();                           // 0x0099A2D0
};

struct CMessageOnSetThingOwner : CBaseClass {
    void* m_argument;                      // +0x4
    CMessageOnSetThingOwner(void* argument);
};

CMessageOnSetThingOwner::CMessageOnSetThingOwner(void* argument)
    : CBaseClass()
    , m_argument(argument)
{
}