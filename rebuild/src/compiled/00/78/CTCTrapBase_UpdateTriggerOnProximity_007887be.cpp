// CTCTrapBase::UpdateTriggerOnProximity() -- proximity trigger: is any trigger thing inside the trap def's trigger radius; a triggered opening trap with nobody near resets, an untriggered trap triggers when somebody newly comes near
// Leaf: UpdateTriggerOnProximity
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007887BE, 204 bytes (plain `ret`, `leave`): frame-pointer size TU, so
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)` (the CTCTrapBase TU is `optimize("s")`, landed ManualReset
// 0x00787C0C and UpdateTrigger 0x0078888A). Ego_r.pdb `?UpdateTriggerOnProximity@CTCTrapBase@@IAEXXZ`.
// Callees: CThing::GetPos() const 0x004C73D0 (owner, then each trigger thing); CBaseIntelligentPointer::GetPItem()
//          const 0x00A01B50; CFadeInFadeOutBase::IsOpen() 0x0048DA30 (the landed callers' non-const spelling; Ego_r.pdb
//          declares it const); CTCTrapBase::Reset() 0x00787BF1; CTCTrapBase::Trigger() 0x00788229.
// Layout: engine/CTCTrapBase.h carries Ego_r.pdb offsets; retail (12-byte STLport vectors, as the landed constructor
//         0x0078918D): TriggerThings +0x2c, TrapOpener (CFadeInFadeOutBase) +0x38, PTrapDef_Object +0x54, Active +0x58,
//         Triggered +0x59, LastTriggerOnProximity +0x5a, TrapType +0x5c; retail CTrapDef TriggerRadius +0x3c (PDB - 4).
// Shape: the owner position is copied into a frame C3DVector through its memberwise user copy constructor (integer
//        moves; `__forceinline` in this size TU, the implicit copy is `movsd x3`); each delta component is a float local the
//        x87 keeps unrounded; the radius is read before the squared length, which is `dx*dx + dy*dy + dz*dz` against
//        `radius * radius` (`fcompp ; test ah,41h`); the trap types 1/4/2 are an `||` chain; the `Triggered = false`
//        store reuses the false bool register.
#pragma optimize("y", off)
#pragma optimize("s", on)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    __forceinline C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};

class CThing { public:
    const C3DVector& GetPos() const;       // 0x004C73D0
};
struct CBaseIntelligentPointer {                // CIntelligentPointer<CThing>: vptr + item (8 bytes)
    void* __vftable;
    void* PData;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CTriggerThingVector {                    // STLport vector<CIntelligentPointer<CThing> > (12 bytes)
    CBaseIntelligentPointer* _M_start;
    CBaseIntelligentPointer* _M_finish;
    CBaseIntelligentPointer* _M_end_of_storage;
    CBaseIntelligentPointer* begin() { return _M_start; }
    CBaseIntelligentPointer* end() { return _M_finish; }
};
class CFadeInFadeOutBase { public:                     // 0x1c bytes
    unsigned char _data[0x1c];
    bool IsOpen();                              // 0x0048DA30
};
class CTrapDef { public:
    unsigned char _pad[0x34];
    long          TriggerType;                  // +0x34
    long          TrapType;                     // +0x38
    float         TriggerRadius;                // +0x3c
};

class CTCTrapBase { public:
    void*         __vftable;                    // +0x00
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x24];
    CTriggerThingVector TriggerThings;          // +0x2c
    CFadeInFadeOutBase  TrapOpener;             // +0x38
    CTrapDef*     PTrapDef_Object;              // +0x54 CDefPointer<const CTrapDef>::Object
    bool          Active;                       // +0x58
    bool          Triggered;                    // +0x59
    bool          LastTriggerOnProximity;       // +0x5a
    long          TrapType;                     // +0x5c ETrapType
    void Reset();                               // 0x00787BF1
    void Trigger();                             // 0x00788229
    void UpdateTriggerOnProximity();
};

void CTCTrapBase::UpdateTriggerOnProximity()
{
    C3DVector position = ParentThing->GetPos();
    bool inRange = false;
    for (CBaseIntelligentPointer* it = TriggerThings.begin(); it != TriggerThings.end(); ++it) {
        const C3DVector& thing = it->GetPItem()->GetPos();
        float dx = thing.X - position.X;
        float dy = thing.Y - position.Y;
        float dz = thing.Z - position.Z;
        float radius = PTrapDef_Object->TriggerRadius;
        if (dx * dx + dy * dy + dz * dz < radius * radius)
            inRange = true;
    }
    if (Triggered) {
        if ((TrapType == 1 || TrapType == 4 || TrapType == 2) && TrapOpener.IsOpen() && !inRange) {
            Triggered = false;
            Reset();
        }
    } else if (inRange && !LastTriggerOnProximity)
        Trigger();
    LastTriggerOnProximity = inRange;
}