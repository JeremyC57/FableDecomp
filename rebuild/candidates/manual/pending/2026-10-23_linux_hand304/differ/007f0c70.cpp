// Grade: DIFFER(718v715) -- regalloc: centre marker/position in ebp/edi swapped vs retail edi/ebp, parent in edi vs ebx; offset *= dist keeps dist on the x87 stack (retail folds fmul [Z]); final angle+facing operand order
// CTCKrakenTentacle::TrackTarget() -- turn the tentacle toward the kraken's live target (clamped to the def's turn speed, flagging CanStrike when within the strike angle) and move it toward the target's position clamped into the kraken pool, by at most the def's move speed
// Leaf: TrackTarget
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F0C70, 715 bytes, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, LowerBound @ 0x0040F020 (CTCKraken = 0xE2, bitset word +0x3C
//   bit 2), CThing::GetPosition @ 0x004C73D0, _CIfmod @ 0x00BFEB28 (intrinsic fmod), AngleDifference @ 0x00A441E0
//   (`__stdcall`), C3DVector::Normalise @ 0x00A14480 (returns the length), the physics TC's GetFacingAngleXY (+0xFC),
//   SetPosition (+0x78) and SetFacingAngleXY (+0x108).
// Data: 0x01237F08 1/(2 pi), 0x01238E00 1/360, 0x0122DEDC 0.0f, 0x0122DED8 1.0f, 0x01237F00 1.0 (double).
// Layout: Ego_r.pdb CTCKrakenTentacle: PTentacleDef +0x0C, PKraken +0x10, CanStrike +0x28; CTCKraken: PKrakenDef +0x0C,
//   PCentreMarker +0x18, PTarget +0x20; CKrakenDef PoolRadius +0x2C; CKrakenTentacleDef TentacleTurnSpeed +0x2C,
//   TentacleMoveSpeed +0x30, TentacleStrikeAngleTolerence +0x34, TentacleStrikeDistTolerence +0x38.
#include "engine/CTCBase.h"
#include <math.h>
#pragma intrinsic(atan2, fmod, fabs)


struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    float Normalise();                                      // 0x00A14480
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z = s * Z; return *this; }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    C3DVector operator*(float s) const { C3DVector r; r.X = X * s; r.Y = Y * s; r.Z = Z * s; return r; }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
float __stdcall AngleDifference(float a, float b);         // 0x00A441E0

struct CPhysicsBody {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void SetPosition(const C3DVector& position);   // slot 30 (+0x78)
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetFacingAngleXY();                       // slot 63 (+0xFC)
    virtual void v64(); virtual void v65();
    virtual void SetFacingAngleXY(float angle);             // slot 66 (+0x108)
};

class CTCBaseComponent;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseComponent* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (as the landed 0x007F0B10): with it
// VC7.1 sees the key's address does not escape, so the key shares the facing angle's frame word as in retail.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CInterfaceBits {
    unsigned long _M_w[9];
    bool test(unsigned long pos) const { return (_M_w[pos / 32] & (1ul << (pos % 32))) != 0; }
};
struct CThing {
    unsigned char _pad00[0x20];
    CInterfaceBits present;                                 // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsBody* m_pPhysics;                               // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Flags91;                                  // +0x91 (bit 0: dead)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};
template<class T> inline bool IS_THING_ALIVE(T* const& p)
{
    if (p != 0) {
        if (!(p->Flags91 & 1))
            return true;
    }
    return false;
}

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                          // +0x04
    CThing* GetPItem() const;                               // 0x00A01B50
};
#pragma pack(pop)

__forceinline float FacingFraction(float x, float y)
{
    float fraction = (float)atan2(x, y) * 0.15915493667125702f;
    if (fraction < 0.0f || fraction >= 1.0f) {
        fraction = (float)fmod((double)fraction, 1.0);
        if (fraction < 0.0f)
            fraction += 1.0f;
    }
    return fraction;
}

__forceinline void ClampSymmetric(float& value, float limit)
{
    if (value < 0.0f) {
        if (value < -limit)
            value = -limit;
    } else if (value > limit)
        value = limit;
}

struct CKrakenDef {
    unsigned char _pad00[0x2c];
    float PoolRadius;                                       // +0x2c
};
struct CKrakenTentacleDef {
    unsigned char _pad00[0x2c];
    float TentacleTurnSpeed;                                // +0x2c
    float TentacleMoveSpeed;                                // +0x30
    float TentacleStrikeAngleTolerence;                     // +0x34
    float TentacleStrikeDistTolerence;                      // +0x38
};
struct CTCKraken : CTCBase {
    const CKrakenDef* PKrakenDef;                           // +0x0c (CDefPointer)
    CBaseIntelligentPointer PChamber;                       // +0x10
    CBaseIntelligentPointer PCentreMarker;                  // +0x18
    CBaseIntelligentPointer PTarget;                        // +0x20
};

struct CTCKrakenTentacle : CTCBase {
    const CKrakenTentacleDef* PTentacleDef;                 // +0x0c (CDefPointer)
    CBaseIntelligentPointer PKraken;                        // +0x10
    CBaseIntelligentPointer PObstruction;                   // +0x18
    CBaseIntelligentPointer PRippleEffect;                  // +0x20
    bool CanStrike;                                         // +0x28
    void TrackTarget();
};

void CTCKrakenTentacle::TrackTarget()
{
    CTCKraken* kraken;
    PKraken.GetPItem()->GetInterface(&kraken, 0xe2);
    CThing* target = kraken->PTarget.GetPItem();
    if (!IS_THING_ALIVE(target))
        return;
    const C3DVector& me = ((CThing*)ParentThing)->GetPosition();
    const C3DVector& there = target->GetPosition();
    float angle = (float)atan2(there.X - me.X, there.Y - me.Y) * 0.15915493667125702f;
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    float facing = ((CThing*)ParentThing)->m_pPhysics->GetFacingAngleXY();
    angle = AngleDifference(facing, angle);
    bool canStrike = false;
    const CKrakenTentacleDef* def = PTentacleDef;
    if (fabs(angle) < def->TentacleStrikeAngleTolerence * 0.0027777778f)
        canStrike = true;
    float turn = def->TentacleTurnSpeed * 0.0027777778f;
    if (angle < 0.0f) {
        if (-turn > angle)
            angle = -turn;
    } else if (turn < angle)
        angle = turn;
    CBaseIntelligentPointer* marker = &kraken->PCentreMarker;
    const C3DVector& centre = marker->GetPItem()->GetPosition();
    const C3DVector& targetPos = target->GetPosition();
    C3DVector offset(targetPos.X - centre.X, targetPos.Y - centre.Y, 0.0f);
    float dist = offset.Normalise();
    if (dist > kraken->PKrakenDef->PoolRadius)
        dist = kraken->PKrakenDef->PoolRadius;
    CThing* parent = (CThing*)ParentThing;
    offset *= dist;
    C3DVector desired = offset + marker->GetPItem()->GetPosition();
    const C3DVector& here = parent->GetPosition();
    C3DVector move(desired.X - here.X, desired.Y - here.Y, 0.0f);
    float moveDist = move.Normalise();
    if (moveDist > PTentacleDef->TentacleStrikeDistTolerence) {
        if (moveDist > PTentacleDef->TentacleMoveSpeed)
            moveDist = PTentacleDef->TentacleMoveSpeed;
        CPhysicsBody* physics = ((CThing*)ParentThing)->m_pPhysics;
        physics->SetPosition(move * moveDist + ((CThing*)ParentThing)->GetPosition());
    } else
        CanStrike = canStrike;
    ((CThing*)ParentThing)->m_pPhysics->SetFacingAngleXY(angle += facing);
}
