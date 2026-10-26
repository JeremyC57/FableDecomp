// Alloc_CAnimComponentDelayTime() -- new CAnimComponentDelayTime with the constructor inlined: member +0x4 = 1, vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DF650, oracle length 31 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0xc ; call operator new ; add esp,4 ; test eax,eax ; je N ; mov dword ptr [eax+0x4],1 ; mov dword ptr [eax],offset CAnimComponentDelayTime::`vftable' ; ret ; N: xor eax,eax ; ret
// Factory for a root polymorphic class: allocates sizeof(CAnimComponentDelayTime) = 0xc bytes (operator new
// @ 0x00BFEA1A, landed-trusted), null-checks and runs the inlined default constructor (the counter
// at +0x4 set to 1, then the vtable 0x01255698 from rtti-vtables.tsv). One of 7 bodies.
// The counter store comes BEFORE the vtable store: the counter is initialised by an inlined base
// constructor (whose own vtable store is dead and dropped); a member of CAnimComponentDelayTime itself would be
// stored after the vtable. The base is not identifiable from the body.
struct CAnimComponentDelayTime_Base {
    virtual void v00();
    long m_count;                          // +0x4
    CAnimComponentDelayTime_Base() : m_count(1) { }
};

class CAnimComponentDelayTime : public CAnimComponentDelayTime_Base { public:
    virtual void v00();
    unsigned char _tail[0x4];
    CAnimComponentDelayTime() : CAnimComponentDelayTime_Base() { }
};

CAnimComponentDelayTime* Alloc_CAnimComponentDelayTime()
{
    return new CAnimComponentDelayTime;
}