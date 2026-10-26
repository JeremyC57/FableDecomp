// CInputProcessControlSpirit::Init() -- point PCreature (+0x28, a CIntelligentPointer) at the player's creature: a temporary intelligent pointer is built from CPlayer::GetPlayerCreature and assigned through SetPItem/GetPItem, then destroyed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A9180, length 77 bytes (+ int3 padding). Vtable slot 2 (Init) of CInputProcessControlSpirit (vtable 0x0125D110).
// Callees: CPlayer::GetPlayerCreature @ 0x00487DC0 (`add ecx,2Ch ; jmp CBaseIntelligentPointer::GetPItem`), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10.
// MyPlayer is CInputProcessGameBase::MyPlayer (+0x18); the temporary's vtable is 0x01237F14 (the CIntelligentPointer instantiation).
// Shape: the creature pointer is fetched first (edi), the 8-byte temporary lives at [esp+8] (base constructor, then its own vtable
// store, then SetPItem), the member takes the temporary's item, and the temporary's base destructor runs last.
class CThingPlayerCreature;
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01237F14
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CPlayer { public:
    CThingPlayerCreature* GetPlayerCreature();                     // 0x00487DC0
};
class CInputProcessControlSpirit { public:
    virtual void v00(); virtual void v01();
    virtual void Init();                            // slot 2
    unsigned char       _pad04[0x10];               // +0x04 CAInputProcess
    void*               World;                      // +0x14
    CPlayer*            MyPlayer;                   // +0x18
    void*               DefinitionManager;          // +0x1c
    void*               Component;                  // +0x20
    void*               PGamePlayerInterface;       // +0x24
    CIntelligentPointer<const CThingPlayerCreature> PCreature;                  // +0x28
};

void CInputProcessControlSpirit::Init()
{
    PCreature = CIntelligentPointer<const CThingPlayerCreature>(MyPlayer->GetPlayerCreature());
}