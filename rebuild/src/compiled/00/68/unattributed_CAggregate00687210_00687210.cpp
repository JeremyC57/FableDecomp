// CAggregate_00687210::~CAggregate_00687210() -- implicit destructor: members at +0x50, +0x48 destroyed out of line, the +0 member by tail call
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687210, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x50] ; call CMember_00a01c10::~CMember_00a01c10 ; lea ecx,[esi+0x48] ; call CMember_00a01c10::~CMember_00a01c10 ; mov ecx,esi ; pop esi ; jmp CMember_0099a430::~CMember_0099a430
// Implicit (compiler-generated) destructor of a non-polymorphic aggregate: members are destroyed
// in reverse declaration order through their out-of-line destructors and the first member's
// destructor (@ 0x0099A430) is reached by tail call. Member types are named from the destructor
// labels when unique, else by the destructor address. The manifest label (`_Dest_val<...>` /
// `~CSymbolInfo>_>`) is propagated. One of 19 bodies of this shape.
struct CMember_0099a430 {
    unsigned char _body[0x4];
    ~CMember_0099a430();                            // 0x0099A430
};

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CAggregate_00687210 {
    CMember_0099a430 m_first;            // +0 (its destructor is the tail call)
    unsigned char _gap0[0x44];
    CBaseIntelligentPointer m_48;            // +0x48
    CBaseIntelligentPointer m_50;            // +0x50
    ~CAggregate_00687210();
};

CAggregate_00687210::~CAggregate_00687210()
{
}