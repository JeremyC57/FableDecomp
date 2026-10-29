// CLandscapeGPUUpdateQueue::CLandscapeGPUUpdateQueue() -- base CBase() then this class's vtable
// Leaf: ??0CLandscapeGPUUpdateQueue@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B67040, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBaseClassNonCopyable::CBaseClassNonCopyable ; mov dword ptr [esi],offset CLandscapeGPUUpdateQueue::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x0099A2F0 runs, then the compiler stores this class's vtable (0x012A2B1C
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 265
// bodies of this shape.
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                           // 0x0099A2F0
};

class CLandscapeGPUUpdateQueue : public CBaseClassNonCopyable { public:
    CLandscapeGPUUpdateQueue();
};

CLandscapeGPUUpdateQueue::CLandscapeGPUUpdateQueue()
    : CBaseClassNonCopyable()
{
}