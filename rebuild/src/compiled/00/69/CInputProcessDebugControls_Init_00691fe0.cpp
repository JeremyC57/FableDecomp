// CInputProcessDebugControls::Init() -- point PCreature (+0x28, a CIntelligentPointer) at the player's creature: a temporary intelligent pointer is built from CPlayer::GetPlayerThing and assigned through SetPItem/GetPItem, then destroyed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00691FE0, length 77 bytes. Vtable slot 2 (Init) of CInputProcessDebugControls (the chunk label "CInputProcessControlSpirit::Init" is propagated: rtti-vtables puts 0x00691FE0 in CInputProcessDebugControls' vtable 0x01237F9C).
// Callees: CPlayer::GetPlayerThing @ 0x00487DD0 (`add ecx,2Ch ; jmp CBaseIntelligentPointer::GetPItem`), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10.
// MyPlayer is CInputProcessGameBase::MyPlayer (+0x18); the temporary's vtable is 0x01237F1C (the CIntelligentPointer instantiation).
// Shape: the creature pointer is fetched first (edi), the 8-byte temporary lives at [esp+8] (base constructor, then its own vtable
// store, then SetPItem), the member takes the temporary's item, and the temporary's base destructor runs last.
struct CThingPlayerCreature;
struct CBaseIntelligentPointer {                    // 8 bytes: vptr + the pointee link
    virtual void v00();
    CThingPlayerCreature* m_pItem;                  // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    void SetPItem(CThingPlayerCreature* item);      // 0x00A01B90
    CThingPlayerCreature* GetPItem() const;         // 0x00A01B50
    ~CBaseIntelligentPointer();                     // 0x00A01C10
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(CThingPlayerCreature* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CPlayer {
    CThingPlayerCreature* GetPlayerThing();                     // 0x00487DD0
};
struct CInputProcessDebugControls {
    virtual void v00(); virtual void v01();
    virtual void Init();                            // slot 2
    unsigned char       _pad04[0x10];               // +0x04 CAInputProcess
    void*               World;                      // +0x14
    CPlayer*            MyPlayer;                   // +0x18
    void*               DefinitionManager;          // +0x1c
    void*               Component;                  // +0x20
    void*               PGamePlayerInterface;       // +0x24
    CIntelligentPointer PCreature;                  // +0x28
};

void CInputProcessDebugControls::Init()
{
    PCreature = CIntelligentPointer(MyPlayer->GetPlayerThing());
}