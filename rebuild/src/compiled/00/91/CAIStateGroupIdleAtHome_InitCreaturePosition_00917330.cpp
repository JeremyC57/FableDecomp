// CAIStateGroup_IdleAtHome::InitCreaturePosition() -- place the creature next to its home building.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00917330, length 69 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 twice (the creature's home pointer at
// CThingAICreature+0x168), CThingCreatureBase::AutoPlaceNearBuilding(CThingBuilding*, float) @ 0x00666EF0.
//
// Shape: `if (creature->GetPHome()) { c = PBrain->PCreature; if (c->AutoPlaceNearBuilding(c->GetPHome(), 3.0f))
// return 1; } return 0;` where GetPHome() is the creature's inline accessor `PHome.GetPItem()` -- going through
// the accessor is what makes retail form the member address with `lea ecx,[eax+168h]` (a direct member call
// gives `add ecx,168h`). The first test reaches the home pointer through the brain without keeping the
// creature, the second re-reads it into a local (esi) that serves both the member address and `this` of the
// placer; the radius is pushed before the inner GetPItem (right-to-left arguments). The result is a
// full register (`mov eax,1` / `xor eax,eax`), i.e. this override returns a long, not a bool. The engine
// header flattens everything, so the base layout is modelled locally.
class CThingBuilding;

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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CThingAICreature { public:
    unsigned char _pad[0x168];
    CIntelligentPointer<CThingBuilding> PHome;              // +0x168 CIntelligentPointer<CThingBuilding>
    CThingBuilding* GetPHome() { return PHome.GetPItem(); }
    bool AutoPlaceNearBuilding(CThingBuilding* pBuilding, float radius);   // 0x00666EF0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};

class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_IdleAtHome : CAIStateGroupBase {
    long InitCreaturePosition();
};

long CAIStateGroup_IdleAtHome::InitCreaturePosition()
{
    if (PBrain->PCreature->GetPHome() != 0) {
        CThingAICreature* pCreature = PBrain->PCreature;
        if (pCreature->AutoPlaceNearBuilding(pCreature->GetPHome(), 3.0f))
            return 1;
    }
    return 0;
}