// NCreatureMode::CModeScreamerSuperAttack::CModeScreamerSuperAttack(CThing*, CThingCreatureBase*) -- mode ctor: filter at +0x68, target creature pointer at +0x74, empty thing pointer at +0x7c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AC3F0, 160 bytes (padding included).
// Leaf: CModeScreamerSuperAttack
// Label correction: the catalog row says CModeBriarRoseTeleportInvulnerable, but the vtable store
// 0x01278338 is NCreatureMode::CModeScreamerSuperAttack (rtti-vtables.tsv).
// Shape: base mode ctor @ 0x00861E00, vtable 0x01278338, CAnimFilterOfGroup at +0x68 from the
// literal @ 0x01278368 (inline ctor: default string 0x0099E4B0, assign 0x0099EFB0, CRC 0x004014A0
// with the GetPString/GetLength accessors; temporary freed by 0x0099EAE0), then two
// CIntelligentPointer members over the out-of-line CBaseIntelligentPointer ctor (0x00A01B10):
// <CThingCreatureBase> at +0x74 (vtable 0x01237F1C) set from the target (SetPItem 0x00A01B90) and
// <CThing> at +0x7c (vtable 0x01238C6C) left empty.
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

struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerCreature(CThingCreatureBase* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() : CBaseIntelligentPointer() { }
};

namespace NCreatureMode {

struct CModeScreamerSuperAttack : CModeBase_00861e00 {
    CAnimFilterOfGroup m_animFilter;           // +0x68
    CIntelligentPointerCreature m_pTarget;     // +0x74
    CIntelligentPointerThing m_pVictim;        // +0x7c
    virtual void v00();
    CModeScreamerSuperAttack(CThing* thing, CThingCreatureBase* target);
};

CModeScreamerSuperAttack::CModeScreamerSuperAttack(CThing* thing, CThingCreatureBase* target)
    : CModeBase_00861e00(thing)
    , m_animFilter("SUPER_ATTACK")             // literal @ 0x01278368 (masked)
    , m_pTarget(target)
{
}

}