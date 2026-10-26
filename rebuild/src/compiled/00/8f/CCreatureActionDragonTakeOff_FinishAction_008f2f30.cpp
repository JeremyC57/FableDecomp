// CCreatureAction_DragonTakeOff::FinishAction() -- switch the dragon's physics TC to airborne, put it back at its position raised by a fixed height, remove its interface-0x4d component, then the base FinishAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F2F30, length 119 bytes, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CThing::GetPosition @ 0x004C73D0,
//          CTCPhysicsBase slot 36 (`call [eax+90h]`, one bool; the base body 0x006B10B0 is an empty `ret 4`, name
//          descriptive), CTCPhysicsBase::ResetPosition(const C3DVector&) (virtual slot 31, `call [eax+7Ch]`, rtti-vtables slot 31 =
//          0x006B01D0 ResetPosition) on the thing's physics TC at +0x60, CThing::RemoveTC(long) @ 0x004C9840,
//          CCombatActionBase::FinishAction @ 0x00859A20.
// Shape: the airborne switch first through a named creature local (`mov ecx,[eax+60h]` straight into ecx); then the
//   DragonHoverOutOf body (0x008F2FB0): a frame C3DVector built field by field from the position reference (X/Y through integer registers, Z
//   = current Z + the float constant 0x0128838C (30.148378f) on the FPU, `fld [eax+8] ; fadd`; copy-then-`+=` goes
//   through the frame), then passed by reference to the physics TC of a named creature local (loaded into ecx);
//   PCreature's address is taken once (esi); the base FinishAction is a direct call.
// Layout: CCombatActionBase (retail 0x114 bytes) with PCreature at +8; CThing's PhysicsTC at +0x60 (landed
//   CThingPhysical 0x006A4D90 model).
class C3DVector { public: float X, Y, Z; };

class CTCPhysicsBase { public:                     // the thing's physics TC; only slots 31 and 36 are modelled
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void ResetPosition(const C3DVector& position);   // slot 31 (+0x7c)
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void SetAirborne(bool airborne);                 // slot 36 (+0x90)
};

class CThing { public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;              // +0x60
    const C3DVector& GetPosition() const;   // 0x004C73D0
    void RemoveTC(long interfaceType);      // 0x004C9840
};

struct CBaseIntelligentPointer {            // CIntelligentPointer<T>: vptr + item
    void*   __vftable;                      // +0x00
    CThing* m_pItem;                        // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];
    virtual void FinishAction();            // 0x00859A20
};

struct CCreatureAction_DragonTakeOff : CCombatActionBase {
    virtual void FinishAction();
};

void CCreatureAction_DragonTakeOff::FinishAction()
{
    {
        CThing* creature = PCreature.GetPItem();
        creature->PhysicsTC->SetAirborne(true);
    }
    const C3DVector& current = PCreature.GetPItem()->GetPosition();
    C3DVector position;
    position.X = current.X;
    position.Y = current.Y;
    position.Z = current.Z + 30.148378f;    // 0x0128838C
    CThing* creature = PCreature.GetPItem();   // named: the physics TC then loads straight into ecx
    creature->PhysicsTC->ResetPosition(position);
    PCreature.GetPItem()->RemoveTC(0x4d);
    CCombatActionBase::FinishAction();
}