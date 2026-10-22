// NCreatureMode::CModeBriarRoseTeleportIntangible::CModeBriarRoseTeleportIntangible(CThing*, CThingCreatureBase*) -- mode ctor: filter at +0x68, intelligent pointer to the target creature at +0x78
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C3700, 144 bytes (padding included).
// Shape: base mode ctor @ 0x00861E00, vtable 0x0128026C (rtti NCreatureMode::CModeBriarRoseTeleportIntangible),
// CAnimFilterOfGroup at +0x68 from the literal @ 0x0128029C (inline ctor: default string 0x0099E4B0,
// assign 0x0099EFB0, CRC 0x004014A0; temporary freed by 0x0099EAE0), then the
// CIntelligentPointer<CThingCreatureBase> member at +0x78: the out-of-line CBaseIntelligentPointer
// ctor (0x00A01B10), the template's vtable (0x0123E7D8) and SetPItem(target) (0x00A01B90) from
// its inline one-argument ctor; CRC arguments via the GetPString/GetLength accessors (same model as the landed CThingGameObject::OnCreate 0x00838A40).
struct CThing;
struct CThingCreatureBase;

struct CCharStringData {
    const char* m_pData;                   // +0
    unsigned long m_length;                // +4
};

struct CCharString {
    CCharStringData* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }   // "" = the shared empty literal 0x0129AAF4
};

unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC

struct CAnimFilterOfGroup {
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

struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature(CThingCreatureBase* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

namespace NCreatureMode {

struct CModeBriarRoseTeleportIntangible : CModeBase_00861e00 {
    CAnimFilterOfGroup m_animFilter;       // +0x68
    unsigned long Timer;             // +0x74 (not touched by the ctor)
    CIntelligentPointerCreature m_pTarget; // +0x78
    virtual void v00();
    CModeBriarRoseTeleportIntangible(CThing* thing, CThingCreatureBase* target);
};

CModeBriarRoseTeleportIntangible::CModeBriarRoseTeleportIntangible(CThing* thing, CThingCreatureBase* target)
    : CModeBase_00861e00(thing)
    , m_animFilter("TELEPORT")             // literal @ 0x0128029C (masked)
    , m_pTarget(target)
{
}

}