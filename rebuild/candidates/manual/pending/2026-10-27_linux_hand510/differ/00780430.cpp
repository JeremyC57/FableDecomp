// CTCFlammable::GetClosestFirePointTo(const C3DVector& position, bool) -- the fire point whose helper point lies nearest to position (end() for no points, begin() for one)
// Grade: DIFFER(273v271) -- register allocation: retail keeps the closest pointer in ebp and spills the appearance to the frame, this build keeps the appearance in ebp and spills closest; frame, FPU order and dead-arg slot match. Tried: one helper (frame 0x10), block-scoped helper (frame shared), `closest = begin; it = closest + 1` (frame shared, `lea esi,[ebp+2Ch]`), function-scope helper (this). Next: the `it = closest + 1` form with a function-scope helper
// Leaf: ?GetClosestFirePointTo@CTCFlammable@@AAEPAVCFirePoint@NFire@@ABVC3DVector@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00780430, 271 bytes, thiscall, `ret 8`. Caller 0x0078086A (CTCFlammable TU).
// Identity: Ego_r.pdb `?GetClosestFirePointTo@CTCFlammable@@AAE?AV?$_Vector_iterator@..CFirePoint..@@ABVC3DVector@@_N@Z`
//   (private; Dinkumware iterator); retail STLport's vector iterator is the raw `NFire::CFirePoint*`. The bool is unread
//   in the retail body. The 0x2C stride (0x2E8BA2E9 / sar 3) is Ego_r.pdb NFire::CFirePoint.
// Callees: NGraphicAppearance::CTCGraphicAppearance::GetHelperPoint(const CCharString&, long, unsigned long, C3DVector&,
//   float) const @ 0x004C0560 (on the owner thing's appearance, CThing +0x64).
// Layout: CTCFlammable (landed ctor 0x00780820): CTCBase PThing +0x04, FirePoints (STLport vector) +0x10.
// Shape: the squared distances are inline C3DVector differences kept on the FPU stack, summed z-first (the
//   GetLengthSquared statement form of landed 0x00815F00); the best distance lives in the dead `position` argument slot.
class CCharString { public: void* PStringData; };

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { float s = Z * Z; s += Y * Y; s += X * X; return s; }
};

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector& position, float time) const;   // 0x004C0560
};
}

class CThing {
public:
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* m_pGraphicAppearance;   // +0x64
};

class CTCBase {
public:
    void*   __vfptr;                           // +0x00
    CThing* PThing;                            // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
};

namespace NFire {
class CFirePoint {                             // 0x2c
public:
    CCharString   HelperName;                  // +0x00
    long          HelperIndex;                 // +0x04
    long          EffectIndex;                 // +0x08
    long          Resistance;                  // +0x0c
    bool          Burning;                     // +0x10
    long          FireDamageAfterFrame;        // +0x14
    CThing*       PParentThing;                // +0x18
    unsigned char PEffect[8];                  // +0x1c CIntelligentPointer<CThing>
    unsigned char PSingeEffect[8];             // +0x24 CIntelligentPointer<CThing>
};
}

namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
}

class CTCFlammable : public CTCBase {
public:
    void* PDef;                                  // +0x0c CDefPointer<const CFlammableDef>
    std::vector<NFire::CFirePoint> FirePoints;   // +0x10
private:
    NFire::CFirePoint* GetClosestFirePointTo(const C3DVector& position, bool burningOnly);
};

NFire::CFirePoint* CTCFlammable::GetClosestFirePointTo(const C3DVector& position, bool)
{
    unsigned int count = FirePoints.size();
    if (count == 0)
        return FirePoints.end();
    if (count == 1)
        return FirePoints.begin();
    NGraphicAppearance::CTCGraphicAppearance* appearance = PThing->m_pGraphicAppearance;
    NFire::CFirePoint* it = FirePoints.begin();
    NFire::CFirePoint* closest = it;
    C3DVector closestHelper;
    appearance->GetHelperPoint(closest->HelperName, closest->HelperIndex, 0, closestHelper, 1.0f);
    ++it;
    float closestDistance = (position - closestHelper).GetLengthSquared();
    for (; it != FirePoints.end(); ++it) {
        C3DVector helper;
        appearance->GetHelperPoint(it->HelperName, it->HelperIndex, 0, helper, 1.0f);
        float distance = (position - helper).GetLengthSquared();
        if (distance < closestDistance) {
            closestDistance = distance;
            closest = it;
        }
    }
    return closest;
}
