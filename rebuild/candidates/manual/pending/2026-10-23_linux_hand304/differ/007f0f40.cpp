// Grade: DIFFER(561v559) -- 55 difflines: dist/angle frame slots swapped, kraken GetFacingAngleXY reg choice, Z operand order in the SetPosition sum, vtable-load scheduling. Tried earlier/later decls, inline clamp helpers, operator+ forms.
// CTCKrakenTentacle::ReturnToDesiredPos(bool immediate)
// Leaf: ReturnToDesiredPos
#include "engine/CTCBase.h"


struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    float Normalise();                                      // 0x00A14480
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float LengthSquared() const { return Y * Y + Z * Z + X * X; }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z = s * Z; return *this; }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    C3DVector operator*(float s) const { C3DVector r; r.X = X * s; r.Y = Y * s; r.Z = Z * s; return r; }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};

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
    virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetFacingVector() const;              // slot 70 (+0x118)
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
    struct CTCGraphicAppearance* GraphicAppearance;         // +0x64
    unsigned char _pad68[0x91 - 0x68];
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
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    CThing* GetPItem() const;                               // 0x00A01B50
    void SetPItem(CThing* item);                            // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
#pragma pack(pop)

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->Flags91 & 1))
            return true;
    }
    return false;
}
float __stdcall AngleDifference(float a, float b);         // 0x00A441E0
struct CCharString {
    void* PStringData;
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
struct CKrakenTentacleDef {
    unsigned char _pad00[0x2c];
    float TentacleTurnSpeed;                                // +0x2c
    float TentacleMoveSpeed;                                // +0x30
    float TentacleStrikeAngleTolerence;                     // +0x34
    float TentacleStrikeDistTolerence;                      // +0x38
    float TentacleHeightOffset;                             // +0x3c
};
struct CTCKrakenTentacle : CTCBase {
    const CKrakenTentacleDef* PTentacleDef;                 // +0x0c (CDefPointer)
    CBaseIntelligentPointer PKraken;                        // +0x10
    CBaseIntelligentPointer PObstruction;                   // +0x18
    CBaseIntelligentPointer PRippleEffect;                  // +0x20
    bool CanStrike;                                         // +0x28
    bool Tracking;                                          // +0x29
    bool IsStriking;                                        // +0x2a
    long QueuedStrikeTick;                                  // +0x2c
    long LastStrikeTick;                                    // +0x30
    long TickToRise;                                        // +0x34
    C3DVector DesiredPos;                                   // +0x38
    float FacingAngle;                                      // +0x44
    CCharString DummyName;                                  // +0x48
    void ReturnToDesiredPos(bool immediate);
};

extern const float g_FloatEpsilon;                         // 0x0129BA3C (0.0001f)
__forceinline float NormaliseSafe(C3DVector& v)
{
    if (v.LengthSquared() > g_FloatEpsilon * g_FloatEpsilon)
        return v.Normalise();
    return 0.0f;
}

void CTCKrakenTentacle::ReturnToDesiredPos(bool immediate)
{
    if (!IS_THING_ALIVE(PKraken))
        return;
    C3DVector current = ParentThing->GetPosition();
    C3DVector desired = current;
    CThing* kraken = PKraken.GetPItem();
    const CKrakenTentacleDef* def = PTentacleDef;
    CTCGraphicAppearance* appearance = PKraken.GetPItem()->GraphicAppearance;
    CRightHandedSet orientation;
    if (appearance->GetDummyObject(DummyName, 0, 0, &desired, &orientation, 1.0f))
        desired.Z += def->TentacleHeightOffset;
    C3DVector dir;
    dir = desired - current;
    float dist = NormaliseSafe(dir);
    float angle = PKraken.GetPItem()->m_pPhysics->GetFacingAngleXY();
    float facing = ParentThing->m_pPhysics->GetFacingAngleXY();
    angle += FacingAngle;
    angle = AngleDifference(facing, angle);
    if (!immediate) {
        float turn = def->TentacleTurnSpeed * 0.0027777778f;
        if (angle < 0.0f) {
            if (-turn > angle)
                angle = -turn;
        } else if (turn < angle)
            angle = turn;
        if (!(dist < def->TentacleMoveSpeed))
            dist = def->TentacleMoveSpeed;
    }
    CPhysicsBody* physics = ParentThing->m_pPhysics;
    physics->SetPosition(dir * dist + current);
    ParentThing->m_pPhysics->SetFacingAngleXY(angle += facing);
}
