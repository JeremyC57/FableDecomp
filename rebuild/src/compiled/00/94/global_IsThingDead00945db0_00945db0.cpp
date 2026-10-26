// IS_THING_DEAD<CIntelligentPointer<T>>(pointer) -- GetPItem @ 0x00A01B50 twice: null, then the dead bit (+0x91 & 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00945DB0, oracle length 36 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call GetPItem ; test eax,eax ; je F ; mov ecx,esi ; call GetPItem ; test byte ptr [eax+91h],1 ; jne F ; xor al,al ; pop esi ; ret ; F: mov al,1 ; pop esi ; ret
// Script-side liveness predicate over an intelligent pointer: the item is
// fetched twice through CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (null means dead),
// and its dead bit (bit 0 of the byte at +0x91) decides; the nested-if form keeps both tests as
// branches with the shared true tail at the end. Fastcall: the reference in ecx. The manifest
// label is propagated; the thing class is opaque. One of 6 bodies of this shape.
class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags;                 // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
};

struct CBaseIntelligentPointer {
    CThing* GetPItem();                    // 0x00A01B50
};

bool __fastcall IsThingDead_00945db0(CBaseIntelligentPointer& pointer)
{
    if (pointer.GetPItem() != 0) {
        if (!pointer.GetPItem()->IsDead())
            return false;
    }
    return true;
}