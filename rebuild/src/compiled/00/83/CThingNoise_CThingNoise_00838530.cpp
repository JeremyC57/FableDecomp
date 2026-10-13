// CThingNoise::CThingNoise() -- base CThing(14) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00838530, oracle length 20 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xe ; mov esi,ecx ; call CThing::CThing ; mov dword ptr [esi],offset CThingNoise::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor that passes the constant 14 (0xe) to the base constructor @ 0x004C9030
// (an identifier/type code of this class in the base's scheme) and installs this class's vtable
// (0x01272EE4, named through rtti-vtables.tsv). One of 12 bodies of this shape.
struct CThing {
    virtual void v00();
    CThing(long typeCode);              // 0x004C9030
};

struct CThingNoise : CThing {
    virtual void v00();
    CThingNoise();
};

CThingNoise::CThingNoise()
    : CThing(14)
{
}