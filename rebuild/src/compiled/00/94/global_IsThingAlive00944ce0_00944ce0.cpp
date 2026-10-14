// IS_THING_ALIVE<CIntelligentPointer<T>>(holder with the pointer at +0x15) -- GetPItem @ 0x00A01B50 twice: null, then the dead bit (+0x91 & 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00944CE0, oracle length 39 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; lea esi,[ecx+0x15] ; mov ecx,esi ; call GetPItem ; test eax,eax ; je F ; mov ecx,esi ; call GetPItem ; test byte ptr [eax+91h],1 ; jne F ; mov al,1 ; pop esi ; ret ; F: xor al,al ; pop esi ; ret
// Script-side liveness predicate over an intelligent pointer embedded at +0x15 of its holder: the item is
// fetched twice through CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (null means not alive),
// and its dead bit (bit 0 of the byte at +0x91) decides; the nested-if form keeps both tests as
// branches with the shared false tail at the end. Fastcall: the reference in ecx. The manifest
// label is propagated; the thing class is opaque. One of 8 bodies of this shape.
struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags;                 // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
};

struct CBaseIntelligentPointer {
    CThing* GetPItem();                    // 0x00A01B50
};

inline bool IsThingAlive(CBaseIntelligentPointer& pointer)
{
    if (pointer.GetPItem() != 0) {
        if (!pointer.GetPItem()->IsDead())
            return true;
    }
    return false;
}

struct CHolder { unsigned char _pad[0x15]; CBaseIntelligentPointer m_pointer; };

bool __fastcall IsThingAlive_00944ce0(CHolder& holder)
{
    return IsThingAlive(holder.m_pointer);
}