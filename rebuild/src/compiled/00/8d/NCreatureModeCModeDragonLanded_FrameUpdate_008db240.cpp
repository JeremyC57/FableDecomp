// NCreatureMode::CModeDragonLanded::FrameUpdate() -- while the target lives and the dragon has a look component (0x43): unless the target is within 12.5 units on the ground plane and almost straight down -Y from it (normalised direction . (0,-1,0) > 0.96), turn the head at the target when head turning is on; otherwise look 50 units ahead of the body
// Leaf: ?FrameUpdate@CModeDragonLanded
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DB240, 543 bytes (`ret`).
// Callees: CModeBase::FrameUpdate 0x0073AC50, CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020
//   (inline TC lookup 0x43), CThing::GetPos 0x004C73D0, CTCLook::LookAtPos(const C3DVector*, long) 0x00753E90; the driver
//   TC's virtual slot 70 (forward vector, returned by value).
// Data: fast inverse-sqrt table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 12.5f 0x01285B48, 0.96f 0x01285B44, 0.0f 0x0122DEDC,
//   1.0f 0x0122DED8, 50.0f 0x0124E7F8.
// Layout: as the landed sibling CModeDragonHover::FrameUpdate 0x008DB460 (PDB CModeDragonLanded: PTarget +0x74, UseHeadTurning
//   +0x7C). Thing: TC-present bits +0x20, interface map +0x44, driver TC +0x60, killed flag +0x91 bit 0.
// Shape: the landed 0x008DB460 body plus FastInvSqrt (0x008C2550 model); the inverse length is a NAMED float local scaled in
//   through a `const float&` helper, so it is stored and re-read from memory (fstp/fxch/fmul [m]); VC7.1 folds the constant dot
//   product X*0 + Y*-1 + Z*0 into (X+Z)*0 - Y*1.
#include <math.h>
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLength2D() const { return sqrtf(X * X + Y * Y); }
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
};
__forceinline C3DVector ScaleRef(const C3DVector& v, const float& s) { return C3DVector(v.X * s, v.Y * s, v.Z * s); }
__forceinline C3DVector Normalise(const C3DVector& v) { return ScaleRef(v, FastInvSqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z)); }
class CTCDriverBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); 
    virtual void MoveTo(const C3DVector& position);   // slot 30 (+0x78)
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); 
    virtual C3DVector GetForward();   // slot 70 (+0x118)
};
class CTCBase;
class CTCLook { public:
    void LookAtPos(const C3DVector* position, long priority);   // 0x00753E90
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CTCDriverBase* PTCDriver;                   // +0x60
    unsigned char  _pad64[0x91 - 0x64];
    unsigned char  Killed;                      // +0x91 (bit 0)
    const C3DVector& GetPos() const;       // 0x004C73D0
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
    }
};
class CThingCreatureBase : public CThing { public: };
class CBaseObject;
struct CBaseIntelligentPointer {
    void* _vptr;
    void* PItem;
    CBaseObject* GetPItem() const;                   // 0x00A01B50
};
namespace NCreatureMode {
class CModeBase {
public:
    void*               _vptr;                  // +0x00
    CThingCreatureBase* Owner;                  // +0x04
    unsigned char       _pad08[0x6c];
    void FrameUpdate();                         // 0x0073AC50
};
class CModeDragonLanded : public CModeBase {
public:
    CBaseIntelligentPointer PTarget;            // +0x74
    bool                    UseHeadTurning;     // +0x7C
    void FrameUpdate();
};
void CModeDragonLanded::FrameUpdate()
{
    CModeBase::FrameUpdate();
    if (!((CThing*)PTarget.GetPItem()) || (((CThing*)PTarget.GetPItem())->Killed & 1))
        return;
    CThingCreatureBase* owner = Owner;
    if (!owner->HasTC(0x43))
        return;
    CTCLook* look = owner->GetTC<CTCLook>(0x43);
    bool facing = false;
    CThing* target = ((CThing*)PTarget.GetPItem());
    C3DVector delta = target->GetPos() - owner->GetPos();
    float invLength = FastInvSqrt(delta.X * delta.X + delta.Y * delta.Y + delta.Z * delta.Z);
    C3DVector direction = ScaleRef(delta, invLength);
    if (delta.GetLength2D() < 12.5f && direction.Dot(C3DVector(0.0f, -1.0f, 0.0f)) > 0.96f)
        facing = true;
    if (UseHeadTurning && !facing) {
        look->LookAtPos(&((CThing*)PTarget.GetPItem())->GetPos(), 10);
    } else {
        C3DVector ahead = Owner->PTCDriver->GetForward() * 50.0f;
        C3DVector position = Owner->GetPos() + ahead;
        look->LookAtPos(&position, 10);
    }
}
}