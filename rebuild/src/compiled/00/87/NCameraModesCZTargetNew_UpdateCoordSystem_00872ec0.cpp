// NCameraModes::CZTargetNew::UpdateCoordSystem() -- when the observed thing targets a live thing, point the X axis from the target to the observed thing (flattened, normalised) with the Y axis perpendicular, restart the transition on a target change, and remember the targeted thing
// Leaf: UpdateCoordSystem
// Retail: Fable.exe (TLC, VC7.1) @ 0x00872EC0, 365 bytes (+ int3 padding), plain `ret`; virtual slot 24 of
//   NCameraModes::CZTargetNew (called through +0x60 by the landed OnInit 0x00872A20).
// Callees: NCameraModes::CBase::GetObservedThing() const 0x0084F1B0 (twice), the const CVectorMap::LowerBound 0x004365B0
//   (interface lookup, key 8 = the targeting TC), the TC's virtual slot 36 (+0x90) GetTargetedThing, CThing::GetPosition
//   0x004C73D0 (const C3DVector&, twice), C3DVector::AccurateNormalise @ 0x00A14440, CZTargetNew::InitialiseTransitionVariables
//   0x00872490, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10;
//   CIntelligentPointer<const CThing> vtable 0x01237F1C; 1e-4f @ 0x0129BA3C (squared at run time).
// Layout (retail NCameraModes::CBase is 0xA4 bytes, PDB 0xC8): CZTargetNew PTargetedThing +0xA4 (PDB +0xC8),
//   XAxis +0xD4, YAxis +0xE0 (as the landed OnInit). CThing: TC bit mask +0x20, interface map +0x44, dead bit +0x91.
// Shape: HasTC is a forced-inline `unsigned long` mask (`test ch,1`); the TC is the one-ternary entry pick over a
//   template constant key; the flattened direction is built as C3DVector(dx, dy, 0) (dy evaluated first), its XY
//   length squared compared against epsilon squared; the axes are stored through the implicit operator= and the inline
//   (Y, -X, 0) perpendicular; `PTargetedThing = target` builds a CIntelligentPointer temporary (base ctor, vtable store,
//   SetPItem) and assigns it through `SetPItem(other.GetPItem())`.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector& AccurateNormalise();                                   // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
    float GetLengthSquaredXY() const { return X * X + Y * Y; }
    C3DVector GetPerpendicularXY() const { return C3DVector(Y, -X, 0.0f); }
};
extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C (1e-4f)

class CThing;
class CTCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual const CThing* GetTargetedThing() const;     // slot 36 (+0x90)
};

struct SInterfaceEntry { long type; CTCBase* component; };
struct CInterfaceMap {                                  // CVectorMap<long, CTCBase*>, 12 bytes
    SInterfaceEntry* _M_start; SInterfaceEntry* _M_finish; SInterfaceEntry* _M_end_of_storage;
    const SInterfaceEntry* LowerBound(const long& key) const;   // 0x004365B0
    const SInterfaceEntry* end() const { return _M_finish; }
};

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long TCFlags;                              // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap Interfaces;                           // +0x44
    unsigned char _pad50[0x41];
    unsigned char Dead : 1;                             // +0x91 bit 0
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return TCFlags & (1 << type); }
    template<long TYPE> __forceinline CTCBase* GetTC() const
    {
        long type = TYPE;
        const CInterfaceMap* map = &Interfaces;
        const SInterfaceEntry* it = map->LowerBound(type);
        const SInterfaceEntry* last = map->end();
        return (it == last || TYPE < it->type ? last : it)->component;
    }
};

template<class T> inline bool IS_THING_ALIVE(T* thing) { return thing != 0 && !thing->Dead; }

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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {    // CIntelligentPointer<const CThing>, vtable 0x01237F1C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        T* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

namespace NCameraModes {
class CBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void UpdateCoordSystem();                   // slot 24 (+0x60)
    unsigned char _pad04[0xa0];                         // +0x04..+0xa3
    const CThing* GetObservedThing() const;             // 0x0084F1B0
};
class CZTargetNew : public CBase {
public:
    CIntelligentPointer<const CThing> PTargetedThing;            // +0xa4
    float BaseOffsetXY;                                 // +0xac
    float BaseHeightOffset;                             // +0xb0
    float Theta;                                        // +0xb4
    float DeltaBaseOffsetXY;                            // +0xb8
    float DeltaBaseHeightOffset;                        // +0xbc
    float DeltaTheta;                                   // +0xc0
    float TargetBaseOffsetXY;                           // +0xc4
    float TargetBaseHeightOffset;                       // +0xc8
    float StringLengthXY;                               // +0xcc
    float CageRadius;                                   // +0xd0
    C3DVector XAxis;                                    // +0xd4
    C3DVector YAxis;                                    // +0xe0
    long FramesForTransition;                           // +0xec
    void InitialiseTransitionVariables();               // 0x00872490
    virtual void UpdateCoordSystem();
};
}

void NCameraModes::CZTargetNew::UpdateCoordSystem()
{
    const CThing* thing = GetObservedThing();
    if (thing->HasTC(8)) {
        const CThing* target = thing->GetTC<8>()->GetTargetedThing();
        if (IS_THING_ALIVE(target)) {
            const C3DVector& targetPos = target->GetPosition();
            const C3DVector& pos = GetObservedThing()->GetPosition();
            C3DVector axis(pos.X - targetPos.X, pos.Y - targetPos.Y, 0.0f);
            if (axis.GetLengthSquaredXY() > g_epsilon_0129ba3c * g_epsilon_0129ba3c) {
                axis.AccurateNormalise();
                XAxis = axis;
                YAxis = XAxis.GetPerpendicularXY();
            }
            if (PTargetedThing.GetPItem() != target)
                InitialiseTransitionVariables();
        }
        if (PTargetedThing.GetPItem() != target)
            PTargetedThing = target;
    }
}