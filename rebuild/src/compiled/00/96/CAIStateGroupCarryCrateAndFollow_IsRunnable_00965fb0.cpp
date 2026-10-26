// CAIStateGroup_CarryCrateAndFollow::IsRunnable() -- resets the action-point state, looks up the nearest crate into PCrate and is runnable while that crate is alive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00965FB0, length 126 bytes (+ int3 padding).
// Callees: CAIStateGroup_CarryCrateAndFollow::FindPCrate @ 0x00965F30, CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10, ::SetPItem @ 0x00A01B90 (twice), ::GetPItem @ 0x00A01B50 (three times), ::~CBaseIntelligentPointer
// @ 0x00A01C10.
// Shape: ActionPointIndex / GotActionPoint / PickUpActionID reset in source order from one `or eax,-1`;
// `PCrate = CIntelligentPointer<CThing>(FindPCrate())`: a frame temporary built by the inline converting
// constructor (base ctor, vtable 0x01238C6C, SetPItem(p)), assigned through the inline operator=
// (SetPItem(other.GetPItem())) and destroyed through the base destructor; `this` is then advanced to the member
// (`add esi,14h`). The alive test is nested ifs with `mov al,1` / shared `xor al,al`; dead = bit 0 of +0x91.
// Model: local (engine/CAIStateGroup_CarryCrateAndFollow.h flattens the base; member names from it).
// Shape: the CIntelligentPointer operator= names the GetPItem() CALL result (`item = other.GetPItem(); SetPItem(item);`), which
//   keeps the member's address (this of SetPItem) after the temporary's GetPItem call as retail; the inline form hoists it.
class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
};
class CThingObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                  // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    void* PBrain;                                   // +0x04
    long ScriptAIPriority;                          // +0x08
    void* Name_PStringData;                         // +0x0c
    unsigned char Terminated;                       // +0x10
    unsigned char Priority;                         // +0x11
    unsigned char Enabled;                          // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_CarryCrateAndFollow : CAIStateGroupBase {
    CIntelligentPointerThing PCrate;                // +0x14
    CIntelligentPointerThing PPreviouslyCarriedItem;// +0x1c
    CIntelligentPointerThing PThingToFollow;        // +0x24
    bool GotActionPoint;                            // +0x2c
    long ActionPointIndex;                          // +0x30
    long PickUpActionID;                            // +0x34
    CThingObject* FindPCrate();                     // 0x00965F30
    virtual bool IsRunnable();
};

bool CAIStateGroup_CarryCrateAndFollow::IsRunnable()
{
    ActionPointIndex = -1;
    GotActionPoint = false;
    PickUpActionID = -1;
    PCrate = CIntelligentPointerThing((CThing*)FindPCrate());
    if (PCrate.GetPItem() != 0) {
        if (!PCrate.GetPItem()->IsDead())
            return true;
    }
    return false;
}