// CCreatureAction_DragonSwoop::Initialise() -- base Initialise, then asks the creature's +0x60 component (virtual slot 31) to fill the three SwoopPoints
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F2F00, length 48 bytes. Vtable slot 12.
// Callees: CCombatActionBase::Initialise @ 0x00857520 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (PCreature at +8), then `[creature+0x60]->vtable[31]` with the array as the one argument.
// Shape: `add esi,114h ; push esi` is the natural address of the last member (SwoopPoints, engine-header name, PDB +0x12c,
// C3DVector[3]; retail CCombatActionBase is 0x114 bytes). The component at +0x60 is the same object whose +0xc is the
// position vector (CThing::GetPosition @ 0x004C73D0), i.e. the physics/transform component; its slot 31 is modelled by
// name only.
class C3DVector { public: float X, Y, Z; };

struct CTCPhysicsComponent {                // the object at creature+0x60; only slot 31 is modelled
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void GetSwoopPoints(C3DVector* points);   // slot 31 (+0x7c)
};

struct CThingCreatureBase_Retail {          // engine/CThingCreatureBase.h exists but is not used: only the +0x60 component is read
    unsigned char _pad[0x60];
    CTCPhysicsComponent* m_pPhysics;        // +0x60
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThingCreatureBase_Retail* m_pItem;     // +0x04
    CThingCreatureBase_Retail* GetPItem() const;   // 0x00A01B50
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12, 0x00857520
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

class CCreatureAction_DragonSwoop : public CCombatActionBase { public:
    C3DVector SwoopPoints[3];               // +0x114 (PDB +0x12c)
    virtual void Initialise();
};

void CCreatureAction_DragonSwoop::Initialise()
{
    CCombatActionBase::Initialise();
    CThingCreatureBase_Retail* pCreature = PCreature.GetPItem();   // named: the vcall object is then a plain load (ecx directly)
    pCreature->m_pPhysics->GetSwoopPoints(SwoopPoints);
}