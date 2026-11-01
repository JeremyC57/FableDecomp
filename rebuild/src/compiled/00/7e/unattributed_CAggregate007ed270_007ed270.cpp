// CAggregate_007ed270::~CAggregate_007ed270() -- implicit destructor: members at +0xb0, +0xa8 destroyed out of line, the +0 member by tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ED270, oracle length 33 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0xb0] ; call CMember_00a01c10::~CMember_00a01c10 ; lea ecx,[esi+0xa8] ; call CMember_00a01c10::~CMember_00a01c10 ; mov ecx,esi ; pop esi ; jmp CPhysicsMeshInfo::~CPhysicsMeshInfo
// Implicit (compiler-generated) destructor of a non-polymorphic aggregate: members are destroyed
// in reverse declaration order through their out-of-line destructors and the first member's
// destructor (@ 0x00693EF0) is reached by tail call. Member types are named from the destructor
// labels when unique, else by the destructor address. The manifest label (`_Dest_val<...>` /
// `~CSymbolInfo>_>`) is propagated. One of 19 bodies of this shape.
class CCreatureActionBase { public:   // CCreatureActionBase (Ego_r.pdb ??1CCreatureActionBase@@UAE@XZ; was stand-in CPhysicsMeshInfo, naming wave 15)
    virtual ~CCreatureActionBase();                            // 0x00693EF0
};

class CBaseIntelligentPointer { public:   // CBaseIntelligentPointer (Ego_r.pdb ??1CBaseIntelligentPointer@@UAE@XZ; was stand-in CMember_00a01c10, naming wave 15)
    unsigned char _body[0x4];   // +0x04 (vptr at +0x00)
    virtual ~CBaseIntelligentPointer();                            // 0x00A01C10
};

struct CAggregate_007ed270 {
    CCreatureActionBase m_first;            // +0 (its destructor is the tail call)
    unsigned char _gap0[0xa4];
    CBaseIntelligentPointer m_a8;            // +0xa8
    CBaseIntelligentPointer m_b0;            // +0xb0
    ~CAggregate_007ed270();
};

CAggregate_007ed270::~CAggregate_007ed270()
{
}