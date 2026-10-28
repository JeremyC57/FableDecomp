// CInputProcessDebugControls::Init() -- point PCreature (+0x28, a CIntelligentPointer) at the player's creature: a temporary intelligent pointer is built from CPlayer::PeekPControlledCreature and assigned through SetPItem/GetPItem, then destroyed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00691FE0, length 77 bytes. Vtable slot 2 (Init) of CInputProcessDebugControls (the chunk label "CInputProcessControlSpirit::Init" is propagated: rtti-vtables puts 0x00691FE0 in CInputProcessDebugControls' vtable 0x01237F9C).
// Callees: CPlayer::PeekPControlledCreature @ 0x00487DD0 (`add ecx,2Ch ; jmp CBaseIntelligentPointer::GetPItem`), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10.
// MyPlayer is CInputProcessGameBase::MyPlayer (+0x18); the temporary's vtable is 0x01237F1C (the CIntelligentPointer instantiation).
// Shape: the creature pointer is fetched first (edi), the 8-byte temporary lives at [esp+8] (base constructor, then its own vtable
// store, then SetPItem), the member takes the temporary's item, and the temporary's base destructor runs last.
class CThing { };                                   // opaque here: the pointer is only stored
class CThingPlayerCreature : public CThing { };
class CBaseObject;
class CBaseIntelligentPointer { public:               // 8 bytes: vptr + the pointee link
    virtual void v00();
    CBaseObject* m_pItem;                           // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    void SetPItem(CBaseObject* item);               // 0x00A01B90 (Ego_r.pdb: PAVCBaseObject)
    CBaseObject* GetPItem() const;                  // 0x00A01B50 (Ego_r.pdb: QBEPAVCBaseObject)
    ~CBaseIntelligentPointer();                     // 0x00A01C10
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // CIntelligentPointer<const CThing>, vtable 0x01237F1C (RTTI .?AV?$CIntelligentPointer@$$CBVCThing@@@@)
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;                     // 0x00487DD0
};
class CInputProcessDebugControls { public:
    virtual void v00(); virtual void v01();
    virtual void Init();                            // slot 2
    unsigned char       _pad04[0x10];               // +0x04 CAInputProcess
    void*               World;                      // +0x14
    CPlayer*            MyPlayer;                   // +0x18
    void*               DefinitionManager;          // +0x1c
    void*               Component;                  // +0x20
    void*               PGamePlayerInterface;       // +0x24
    CIntelligentPointer<const CThing> PCreature;    // +0x28
};

void CInputProcessDebugControls::Init()
{
    PCreature = CIntelligentPointer<const CThing>(MyPlayer->PeekPControlledCreature());
}