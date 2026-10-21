// NCreatureMode::CModeDragonHover::FrameUpdate() -- while the target lives: turn the dragon's head (at the target when
// head turning is on, else 50 units ahead of the body) and, when not busy with an action and within 14 units of the
// target on the ground plane, drift back by 0.04 of the forward vector
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DB460, 0x1BB bytes (padded to 448), plain `ret`.
// Callees: CModeBase::FrameUpdate 0x0073AC50, CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound
//   0x0040F020 (inline TC lookup 0x43), CTCLook::LookAtPos(const C3DVector*, long) 0x00753E90, CThing::GetPosition
//   0x004C73D0, CThingCreatureBase::IsPerformingAnyAction 0x006622A0; the driver TC's virtual slots 70 (forward vector,
//   returned by value) and 30 (move to). Data: 50.0f 0x0124E7F8, 14.0f 0x01266304, 0.04f 0x0125B25C.
// Layout: local model. Mode: owner +4, PTarget +0x74 (CIntelligentPointer<CThing>), UseHeadTurning +0x7C (PDB). Thing:
//   TC-present bits +0x20, interface map +0x44, driver TC +0x60 (PDB PTCDriver +0x6C, 12-byte list), killed flag byte
//   +0x91 bit 0.
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLength2D() const { return sqrtf(X * X + Y * Y); }
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct CTCDriverBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void MoveTo(const C3DVector& position);   // slot 30 (+0x78)
    virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038();
    virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046();
    virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062();
    virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual C3DVector GetForward();   // slot 70 (+0x118)
};
struct CTCBase;
struct CTCLook {
    void LookAtPos(const C3DVector* position, long priority);   // 0x00753E90
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CTCDriverBase* PTCDriver;                   // +0x60
    unsigned char  _pad64[0x91 - 0x64];
    unsigned char  Killed;                      // +0x91 (bit 0)
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CThingCreatureBase : CThing {
    bool IsPerformingAnyAction() const;         // 0x006622A0
};
struct CBaseIntelligentPointer {
    void* _vptr;
    void* PItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
namespace NCreatureMode {
class CModeBase {
public:
    void*               _vptr;                  // +0x00
    CThingCreatureBase* Owner;                  // +0x04
    unsigned char       _pad08[0x6c];
    void FrameUpdate();                         // 0x0073AC50
};
class CModeDragonHover : public CModeBase {
public:
    CBaseIntelligentPointer PTarget;            // +0x74
    bool                    UseHeadTurning;     // +0x7C
    void FrameUpdate();
};
void CModeDragonHover::FrameUpdate()
{
    CModeBase::FrameUpdate();
    if (!PTarget.GetPItem() || (PTarget.GetPItem()->Killed & 1))
        return;
    CThingCreatureBase* owner = Owner;
    if (owner->HasTC(0x43)) {
        CTCLook* look = owner->GetTC<CTCLook>(0x43);
        if (UseHeadTurning) {
            look->LookAtPos(&PTarget.GetPItem()->GetPosition(), 10);
        } else {
            C3DVector ahead = owner->PTCDriver->GetForward() * 50.0f;
            C3DVector position = Owner->GetPosition() + ahead;
            look->LookAtPos(&position, 10);
        }
    }
    if (Owner->IsPerformingAnyAction())
        return;
    CThing* me = Owner;
    CThing* target = PTarget.GetPItem();
    C3DVector delta = target->GetPosition() - me->GetPosition();
    if (delta.GetLength2D() < 14.0f) {
        C3DVector back = Owner->PTCDriver->GetForward() * 0.04f;
        C3DVector destination = Owner->GetPosition() - back;
        Owner->PTCDriver->MoveTo(destination);
    }
}
}