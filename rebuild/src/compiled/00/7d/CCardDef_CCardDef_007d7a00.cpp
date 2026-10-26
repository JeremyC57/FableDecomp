// CCardDef::CCardDef() -- CResource base, the inlined abstract def base (index 0, 3-bit flags cleared), the inlined abstract card base (id -1, empty attitude vector, flag cleared), then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D7A00, length 51 bytes.
// Callees: CResource::CResource @ 0x009FBEC0.
// Vtable 0x01243A44 = CCardDef (rtti-vtables.tsv); the manifest label CBlurredAttitudesDef is a
// propagated name. The dword at +0x1c and the low three bits of the byte at +0x24 are cleared by
// the inlined intermediate def base (abstract, `__declspec(novtable)`, same shape as the landed
// CAIScratchpadDef constructor). The class is packed: a CDefString at +0x25 (-1, an immediate
// store), the 12-byte vector at +0x29 and the byte at +0x36 are zeroed from one register, and the
// vtable store comes LAST: every member belongs to a second abstract (`novtable`) base whose inlined
// constructor is a scheduling region of its own, so the most-derived class's vtable store cannot
// move above it (a member list on the vtable-owning class puts the store before the -1).
#pragma pack(push, 1)
class CResource { public:
    virtual void v00();
    unsigned char _body[0x18];                  // +0x04..+0x1b
    CResource();                                // 0x009FBEC0
};

struct __declspec(novtable) CDefinitionBase : CResource {   // abstract: no vtable store of its own
    long          m_defIndex;                   // +0x1c
    long          m_reserved;                   // +0x20 (not initialised here)
    unsigned char m_flags : 3;                  // +0x24 low three bits
    CDefinitionBase() : CResource(), m_defIndex(0), m_flags(0) { }
};

class CDefString { public:                             // a name index into the def string table
    long m_index;
    CDefString() : m_index(-1) { }
};

struct __declspec(novtable) CCardDefBase : CDefinitionBase {   // abstract: the card members
    CDefString    m_name;                       // +0x25
    long*         m_first;                      // +0x29 retail vector<long>: first
    long*         m_last;                       // +0x2d                      last
    long*         m_end;                        // +0x31                      end
    unsigned char m_pad35;                      // +0x35
    unsigned char m_flag;                       // +0x36
    CCardDefBase() : CDefinitionBase(), m_name(), m_first(0), m_last(0), m_end(0), m_flag(0) { }
};

class CCardDef : public CCardDefBase { public:                // vtable 0x01243A44
    virtual void v00();
    CCardDef();
};
#pragma pack(pop)

CCardDef::CCardDef()
    : CCardDefBase()
{
}