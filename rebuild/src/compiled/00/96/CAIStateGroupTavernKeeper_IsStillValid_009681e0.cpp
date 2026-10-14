// CAIStateGroup_TavernKeeper::IsStillValid() const -- valid while both the pub and the bar are alive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009681E0, length 77 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, four times (PPub at +0x24 then PBar
// at +0x1c, engine header CAIStateGroup_TavernKeeper.h): the inlined IS_THING_ALIVE predicate
// fetches the item twice (null test, then the dead bit at +0x91 & 1), the landed 0x00836BE0 shape.
// Shape: `IsAlive(PPub) && IsAlive(PBar)` -- the int-typed `&&` materialises `mov eax,1` /
// `xor eax,eax` (full register), each pointer's address is formed once into esi and reused for
// both GetPItem calls. The base is modelled locally (the generated header has no vtable).
struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags;                      // +0x91 bit 0: dead
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThing* GetPItem();                         // 0x00A01B50
    __forceinline bool IsAlive()
    {
        if (GetPItem() != 0) {
            if (!(GetPItem()->m_flags & 1))
                return true;
        }
        return false;
    }
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // PBrain, ScriptAIPriority, Name, flags (+0x4..+0x13)
};

struct CAIStateGroup_TavernKeeper : CAIStateGroupBase {
    CBaseIntelligentPointer PJug;               // +0x14 CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PBar;               // +0x1c CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PPub;               // +0x24 CIntelligentPointer<CThingBuilding>
    bool IsStillValid() const;
};

bool CAIStateGroup_TavernKeeper::IsStillValid() const
{
    CAIStateGroup_TavernKeeper* self = const_cast<CAIStateGroup_TavernKeeper*>(this);
    return self->PPub.IsAlive() && self->PBar.IsAlive();
}