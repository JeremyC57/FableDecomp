// CMessageEventFilter_MessageIDGreaterThan::operator()(CMessageEvent*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AF280, oracle length 16 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[esp+4] ; mov edx,[eax+40h] ; cmp [ecx],edx ; sbb eax,eax ; neg eax ; ret 4
// PDB: the filter holds the threshold id at +0x00; CMessageEvent carries its id at +0x40. True when
// the event's id is above the threshold; the unsigned `cmp ; sbb ; neg` is the plain
// `return a < b;` form for unsigned operands (bool via a full register).
struct CMessageEventCalls {
    unsigned char _pad_0x00[0x40];
    unsigned long MessageID;                // +0x40
};

class CMessageEventFilter_MessageIDGreaterThan { public:
    unsigned long MessageID;                // +0x00 threshold
    bool operator()(CMessageEventCalls* event);
};

bool CMessageEventFilter_MessageIDGreaterThan::operator()(CMessageEventCalls* event)
{
    return event->MessageID > this->MessageID;   // operand order gives retail's `cmp [this],edx`
}