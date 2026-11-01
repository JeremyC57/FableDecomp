// CAggregate_0057a545::~CAggregate_0057a545() -- implicit destructor: members at +0xc0, +0xbc destroyed out of line, the +0 member by tail call
// Leaf: ??1CAggregate_0057a545@
// Naming (naming wave 22, ccs): the string type is the PDB CCharString (was CMember_0099eae0); name-only.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A545, oracle length 33 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0xc0] ; call CCharString::~CCharString ; lea ecx,[esi+0xbc] ; call CCharString::~CCharString ; mov ecx,esi ; pop esi ; jmp CPhysicsMeshInfo::~CPhysicsMeshInfo
// Implicit (compiler-generated) destructor of a non-polymorphic aggregate: members are destroyed
// in reverse declaration order through their out-of-line destructors and the first member's
// destructor (@ 0x00693EF0) is reached by tail call. Member types are named from the destructor
// labels when unique, else by the destructor address. The manifest label (`_Dest_val<...>` /
// `~CSymbolInfo>_>`) is propagated. One of 19 bodies of this shape.
class CCreatureActionBase { public:   // CCreatureActionBase (Ego_r.pdb ??1CCreatureActionBase@@UAE@XZ; was stand-in CPhysicsMeshInfo, naming wave 15)
    virtual ~CCreatureActionBase();                            // 0x00693EF0
};

class CCharString { public:
    unsigned char _body[0x4];
    ~CCharString();                            // 0x0099EAE0
};

struct CAggregate_0057a545 {
    CCreatureActionBase m_first;            // +0 (its destructor is the tail call)
    unsigned char _gap0[0xb8];
    CCharString m_bc;            // +0xbc
    CCharString m_c0;            // +0xc0
    ~CAggregate_0057a545();
};

CAggregate_0057a545::~CAggregate_0057a545()
{
}