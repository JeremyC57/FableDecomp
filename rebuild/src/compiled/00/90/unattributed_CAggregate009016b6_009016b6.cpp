// CAggregate_009016b6::~CAggregate_009016b6() -- implicit destructor: members at +0x24, +0x1c, +0x14 destroyed out of line, the +0 member by tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009016B6, oracle length 35 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x24] ; call CMember_00a01c10::~CMember_00a01c10 ; lea ecx,[esi+0x1c] ; call CMember_00a01c10::~CMember_00a01c10 ; lea ecx,[esi+0x14] ; call CMember_00a01c10::~CMember_00a01c10 ; mov ecx,esi ; pop esi ; jmp CMember_008fd288::~CMember_008fd288
// Implicit (compiler-generated) destructor of a non-polymorphic aggregate: members are destroyed
// in reverse declaration order through their out-of-line destructors and the first member's
// destructor (@ 0x008FD288) is reached by tail call. Member types are named from the destructor
// labels when unique, else by the destructor address. The manifest label (`_Dest_val<...>` /
// `~allocator<CMacro>,CMacro>`) is propagated. One of 13 bodies of this shape.
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();                            // 0x008FD288
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

struct CAggregate_009016b6 {
    CAIStateGroupBase m_first;            // +0 (its destructor is the tail call)
    unsigned char _gap0[0x10];
    CBaseIntelligentPointer m_14;            // +0x14
    CBaseIntelligentPointer m_1c;            // +0x1c
    CBaseIntelligentPointer m_24;            // +0x24
    ~CAggregate_009016b6();
};

CAggregate_009016b6::~CAggregate_009016b6()
{
}