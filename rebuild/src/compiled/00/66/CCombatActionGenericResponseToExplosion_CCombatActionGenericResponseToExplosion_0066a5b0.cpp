// CCombatAction_GenericResponseToExplosion::CCombatAction_GenericResponseToExplosion(CThingCreatureBase*, CThing*, const C3DVector&, const C3DVector&) -- constructor
// Leaf: ??0CCombatAction_GenericResponseToExplosion
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066A5B0, 160-byte slot (157 bytes of code).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030,
//          CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: the base constructor takes the owner, action code 250 (0xfa) and a zeroed two-dword CSoundPair
// temporary in the 8-byte frame; then this class's vtable 0x0125AEDC; the PExplosion member
// (CIntelligentPointer<CThing>, vtable 0x01238C6C) is built inline (base ctor, vtable, SetPItem(explosion));
// PointOfHit and HitDir are copied word by word from the two vector references; FlipFacing is cleared
// last. Retail offsets = PDB header offsets - 0x18 (the retail CCombatActionBase is 0x114 bytes, the PDB
// build 0x12c), so a local model replaces engine/CCombatAction_GenericResponseToExplosion.h.
class CThingCreatureBase;
class CThing;
class CBaseObject;

class C3DVector { public:
    float X, Y, Z;
};

class CSoundPair { public:
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};

struct CIntelligentPointer_CThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer_CThing(CThing* item) { SetPItem((CBaseObject*)item); }
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

class CCombatAction_GenericResponseToExplosion : public CCombatActionBase { public:   // vtable 0x0125AEDC
    CIntelligentPointer_CThing PExplosion;  // +0x114
    float PointOfHit_X;                     // +0x11c
    float PointOfHit_Y;                     // +0x120
    float PointOfHit_Z;                     // +0x124
    float HitDir_X;                         // +0x128
    float HitDir_Y;                         // +0x12c
    float HitDir_Z;                         // +0x130
    bool FlipFacing;                        // +0x134
    virtual void v00();
    CCombatAction_GenericResponseToExplosion(CThingCreatureBase* owner, CThing* explosion,
                                             const C3DVector& pointOfHit, const C3DVector& hitDir);
};

CCombatAction_GenericResponseToExplosion::CCombatAction_GenericResponseToExplosion(CThingCreatureBase* owner, CThing* explosion,
                                                                                   const C3DVector& pointOfHit, const C3DVector& hitDir)
    : CCombatActionBase(owner, 250, CSoundPair()),
      PExplosion(explosion),
      PointOfHit_X(pointOfHit.X), PointOfHit_Y(pointOfHit.Y), PointOfHit_Z(pointOfHit.Z),
      HitDir_X(hitDir.X), HitDir_Y(hitDir.Y), HitDir_Z(hitDir.Z),
      FlipFacing(false)
{
}