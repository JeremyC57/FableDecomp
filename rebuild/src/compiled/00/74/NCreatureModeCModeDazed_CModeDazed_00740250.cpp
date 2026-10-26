// NCreatureMode::CModeDazed::CModeDazed(CThing*) -- out-of-action mode ctor: CModeOutOfActionBase (counter 0, flag set, empty attacker pointer) then the DAZED CAnimFilterOfGroup at +0x78
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00740250, 160 bytes (padding included).
// Leaf: CModeDazed
// Shape: base mode ctor @ 0x00861E00, then the inline CModeOutOfActionBase ctor (vtable 0x0126743C):
// +0x68 = 0, +0x6c = true and the CIntelligentPointer<CThing> at the unaligned +0x6d (out-of-line
// CBaseIntelligentPointer ctor 0x00A01B10, then its vtable 0x01238C6C), then this class's vtable
// 0x01267CC4 and the CAnimFilterOfGroup at +0x78 from the literal @ 0x01267D04 (default string
// 0x0099E4B0, assign 0x0099EFB0, CRC 0x004014A0 via the GetPString/GetLength accessors, temporary
// freed by 0x0099EAE0). The base is byte-packed (#pragma pack(1)): that is the real layout, the
// pointer sits at +0x6d in retail.
class CThing;
class CThingCreatureBase;

class CCharStringData { public:
    const char* m_pData;                   // +0
    unsigned long m_length;                // +4
};

class CCharString { public:
    CCharStringData* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }   // "" = the shared empty literal 0x0129AAF4
};

unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC

class CAnimFilterOfGroup { public:
    virtual void v00();
    CCharString m_string;                  // +4
    unsigned long m_hash;                  // +8
    CAnimFilterOfGroup(const CCharString& name)
        : m_string()
    {
        m_string = name;
        m_hash = Calc_004014a0(0, name.GetPString(), name.GetLength());
    }
};

struct CModeBase_00861e00 {
    virtual void v00();
    unsigned char _body[0x64];
    CModeBase_00861e00(CThing* thing);     // 0x00861E00
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* PItem;                           // +0x04
    CBaseIntelligentPointer();             // 0x00A01B10
    void SetPItem(void* item);             // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() : CBaseIntelligentPointer() { }
};

namespace NCreatureMode {

// The out-of-action base packs its pointer member right after the flag byte (lea edi,[esi+6Dh]).
#pragma pack(push, 1)
struct CModeOutOfActionBase : CModeBase_00861e00 {     // vtable 0x0126743C
    long m_frames;                             // +0x68
    bool m_canRecover;                         // +0x6c
    CIntelligentPointerThing m_pAttacker;      // +0x6d
    CModeOutOfActionBase(CThing* thing)
        : CModeBase_00861e00(thing)
        , m_frames(0)
        , m_canRecover(true)
    {
    }
};
#pragma pack(pop)

struct CModeDazed : CModeOutOfActionBase {
    CAnimFilterOfGroup m_animFilter;           // +0x78
    virtual void v00();
    CModeDazed(CThing* thing);
};

CModeDazed::CModeDazed(CThing* thing)
    : CModeOutOfActionBase(thing)
    , m_animFilter("DAZED")              // literal @ 0x01267D04 (masked)
{
}

}