// CCombatAction_GenericResponseToExplosion::CCombatAction_GenericResponseToExplosion(const CCombatAction_GenericResponseToExplosion&) -- copy constructor
// Leaf: ??0CCombatAction_GenericResponseToExplosion
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066A7E0, 160-byte slot (151 bytes of code).
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0,
//          CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: the base copy constructor runs out of line, this class's vtable 0x0125AEDC is stored, then the
// PExplosion member is copy-constructed through CIntelligentPointer<CThing>'s inline copy constructor
// (base ctor, vtable 0x01238C6C, SetPItem(other.GetPItem())) with the member address held in ebx; the two
// vectors and FlipFacing follow word by word (separate scalar members). Retail offsets = PDB header
// offsets - 0x18 (the retail CCombatActionBase is 0x114 bytes), so a local model is used.
class CThing;
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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);  // 0x0062F3B0
};

class CCombatAction_GenericResponseToExplosion : public CCombatActionBase { public:   // vtable 0x0125AEDC
    CIntelligentPointer<CThing> PExplosion;  // +0x114
    float PointOfHit_X;                     // +0x11c
    float PointOfHit_Y;                     // +0x120
    float PointOfHit_Z;                     // +0x124
    float HitDir_X;                         // +0x128
    float HitDir_Y;                         // +0x12c
    float HitDir_Z;                         // +0x130
    bool FlipFacing;                        // +0x134
    virtual void v00();
    CCombatAction_GenericResponseToExplosion(const CCombatAction_GenericResponseToExplosion& other);
};

CCombatAction_GenericResponseToExplosion::CCombatAction_GenericResponseToExplosion(const CCombatAction_GenericResponseToExplosion& other)
    : CCombatActionBase(other),
      PExplosion(other.PExplosion),
      PointOfHit_X(other.PointOfHit_X), PointOfHit_Y(other.PointOfHit_Y), PointOfHit_Z(other.PointOfHit_Z),
      HitDir_X(other.HitDir_X), HitDir_Y(other.HitDir_Y), HitDir_Z(other.HitDir_Z),
      FlipFacing(other.FlipFacing)
{
}