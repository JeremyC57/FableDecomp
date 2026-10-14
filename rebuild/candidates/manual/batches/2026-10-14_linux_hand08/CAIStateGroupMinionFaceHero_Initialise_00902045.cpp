// CAIStateGroup_MinionFaceHero::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const
// CCharString&) -- clear the hero pointer and the cleanup frame, then the base initialisation.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00902045, length 89 bytes, `ret 14h`, frame pointer kept
// (`push ebp ; mov ebp,esp` with `leave`) and the size-optimised `and dword ptr [esi+1Ch],0` zero store:
// this translation unit was built with frame pointers and /Os, reproduced in-source with
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)`.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, SetPItem @ 0x00A01B90 (twice),
// GetPItem @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10 on the converting temporary (vtable 0x01237F14
// = CIntelligentPointer<CThingPlayerCreature>), then CAIStateGroupBase::Initialise @ 0x008FCEC0 with the five
// arguments pushed straight from the frame (`push dword ptr [ebp+..]`).
// `PHero = 0` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment shape; the zero
// store at +0x1c is scheduled between the argument pushes. No engine header; layout local, names descriptive.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CThing;
struct CAIBrain;
struct CThingAICreature;
struct CCharString;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                          // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    CThing* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
    ~CBaseIntelligentPointer();             // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {    // vtable 0x01237F14
    // __forceinline: under optimize("s") VC7.1 otherwise calls these small inline members out of line
    __forceinline CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];               // +0x04..+0x13
    void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name);  // 0x008FCEC0
};

struct CAIStateGroup_MinionFaceHero : CAIStateGroupBase {
    CIntelligentPointer PHero;              // +0x14 CIntelligentPointer<CThingPlayerCreature>
    long m_frameCleanedUp;                  // +0x1c
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name);
};

void CAIStateGroup_MinionFaceHero::Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name)
{
    PHero = 0;
    m_frameCleanedUp = 0;
    CAIStateGroupBase::Initialise(brain, creature, id, priority, name);
}
