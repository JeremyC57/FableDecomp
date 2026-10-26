// CTCNymph::FrameUpdate() -- while flying: once faded in, follow the flight spline for a second of world frames (then pick a new target position), keep ReadyToAttack from the live target's range or else adopt the owner's targeting target; while fading out after flying, fade the alpha down and hide the owner at zero; while fading in, fade the alpha up and kill the old effect at zero; then the base FrameUpdate
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080FAD0, 593 bytes, plain `ret`.
// Callees: CTCBase::GetWorld @ 0x00686A70, CWorld::GetWorldFrame @ 0x0049D870, CCardinalSpline::GetSplinePosition @ 0x00A567A0,
//   the physics body's slot 30 (+0x78), CTCNymph::PickNewTargetPos @ 0x0080F270, IsDecentAttackRange @ 0x0080F190,
//   CThing::GetPosition @ 0x004C73D0, LowerBound @ 0x0040F020 (interface 8), the targeting component's slot 29 (+0x74),
//   CIntelligentPointer<const CThing>(const CThing*) @ 0x00668B60 and operator= @ 0x00668B80 (both out of line),
//   CTCGraphicAppearance::SetAlpha @ 0x004BFAC0, __ftol2 @ 0x00BFEA70, the owner's slot 28, CThing::Kill @ 0x004C9B80,
//   CBaseIntelligentPointer @ 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10, CTCBase::FrameUpdate @ 0x006868A0.
// Data: float frames per second 0x013844F4 (30.0), 1.0f/0.75f/0.25f/255.0f constants.
// Layout: PTarget +0x68 (retail; Ego_r.pdb CTCNymph is 0x74, retail 0x70 with the narrower spline vector).
// Shape: both CIntelligentPointer temporaries are named block locals -- the targeting one so operator= gets the local's
//   address (a temporary passes the ctor's eax), the POldEffect reset one so the two destructor calls tail-merge as retail.
#include "engine/CTCBase.h"

class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class C3DVector { public:
    float X, Y, Z;
};

template <class T> class CDefPointer;
class CDefClassBase { public:
    virtual void v00();
    virtual void Destroy() const;                           // slot 1
    mutable long RefCount;                                  // +0x04
    bool GetSubDef(CDefPointer<const class CSpecialEffectsDef>* out) const;   // GetSubDef<CSpecialEffectsDef> 0x00669580
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
class CSpecialEffectsDef : public CDefClassBase { public:
    bool GetEffectFromString(const CCharString& name, long* effect) const;   // 0x00669D30
};

class CTCBaseComponent;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBaseComponent* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail @ 0x00669D30): it writes the out value only, so VC7.1 knows the local's address
// does not escape the call (the 0x008D9740 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long* value) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    *value = found;
    return true;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
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

struct CInterfaceBits {                                     // bitset<> at CThing+0x20
    unsigned long _M_w[9];
    bool test(unsigned long pos) const { return (_M_w[pos / 32] & (1ul << (pos % 32))) != 0; }
};
class CThing { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetVisible(bool visible);                  // slot 28 (+0x70)
    unsigned char _pad04[0x20 - 0x04];
    CInterfaceBits present;                                 // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    struct CPhysicsBody* PhysicsBody;                       // +0x60
    struct CTCGraphicAppearance* GraphicAppearance;         // +0x64
    unsigned char _pad68[0x70 - 0x68];
    CDefClassBase* PDef;                                    // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                                  // +0x91 (bit 0: dead)
    unsigned char _pad92[0xbc - 0x92];
    unsigned char FlagsBC;                                  // +0xbc
    void Kill(bool removeNow);                              // 0x004C9B80
    bool GetDummyObject(const CCharString& name, C3DVector* position, class CRightHandedSet* orientation);
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
// Same-TU stand-in for the GetSubDef<T> COMDAT (as the landed 0x00801A50): with the body visible VC7.1 sees that the out
// pointer does not escape and keeps the def object in ebp through to its release, as retail.
const CDefClassBase* LookUpSubDef(const CDefClassBase* def);
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<const CSpecialEffectsDef>* out) const
{
    const CSpecialEffectsDef* def = (const CSpecialEffectsDef*)LookUpSubDef(this);
    out->Object = def;
    return def != 0;
}
#pragma auto_inline(on)



class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
struct CTCGraphicAppearance {
    void SetAlpha(unsigned char alpha);                     // 0x004BFAC0
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
inline bool CThing::GetDummyObject(const CCharString& name, C3DVector* position, CRightHandedSet* orientation)
{
    return GraphicAppearance->GetDummyObject(name, 0, 0, position, orientation, 1.0f);
}
class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(long effect, const C3DVector& position, bool attached);   // 0x006E0880
    void AttachToThing(CThing* thing, long mode, const CCharString& bone, long flags, float offset);   // 0x006E0BE0
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                          // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    CThing* GetPItem() const;                               // 0x00A01B50
    void SetPItem(CThing* item);                            // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
    __forceinline bool IsAlive() const { return GetPItem() && !(GetPItem()->Flags91 & 1); }
};
#pragma pack(pop)

extern float g_FramesPerSecondF;                            // 0x013844F4
class CWorld { public:
    static long GetWorldFrame();                            // 0x0049D870
};
class CCardinalSpline { public:                                    // retail 0x14 bytes
    void* ControlPoints[3];
    float Tension;
    bool EndPointsConnected;
    void GetSplinePosition(C3DVector* out, float t) const;  // 0x00A567A0
};
#pragma pack(push, 1)
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item);              // 0x00668B60
    CIntelligentPointerConstThing& operator=(const CIntelligentPointerConstThing& other);   // 0x00668B80
    __forceinline bool IsAlive() const { return GetPItem() && !(GetPItem()->Flags91 & 1); }
};
#pragma pack(pop)
struct CTCTargeting {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetTarget() const;                      // slot 29 (+0x74)
};
struct CPhysicsBody {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void SetPosition(const C3DVector& position);    // slot 30 (+0x78)
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43();
    virtual void Reset();                                   // slot 44 (+0xb0)
};
class CNymphDef { public:
    unsigned char _pad00[0x48];
    float FadeInTime;                                       // +0x48
    float FadeOutTime;                                      // +0x4c
};
struct CTCBaseFrame : CTCBase {
    class CWorld* GetWorld();                              // 0x00686A70 (CTCBase)
    void FrameUpdate();                                     // 0x006868A0 (CTCBase)
};
class CTCNymph : public CTCBaseFrame { public:
    const CNymphDef* PDef;                                  // +0x0c
    C3DVector PointToFlyAround;                             // +0x10
    float EffectHeightOffset;                               // +0x1c
    C3DVector InitialPos;                                   // +0x20
    CCardinalSpline FlightSpline;                           // +0x2c
    bool FlyingAround;                                      // +0x40
    CIntelligentPointer<CThing> PEffect;                    // +0x41
    CIntelligentPointer<CThing> POldEffect;                 // +0x49
    unsigned char _pad51[0x54 - 0x51];
    unsigned long LastTargetTick;                           // +0x54
    bool CanAttack;                                         // +0x58
    bool ReadyToAttack;                                     // +0x59
    bool ReadyToSummon;                                     // +0x5a
    bool HasBeenInLimbo;                                    // +0x5b
    bool HasSpline;                                         // +0x5c
    bool BeenHit;                                           // +0x5d
    unsigned char _pad5e[0x60 - 0x5e];
    long FadeCounter;                                       // +0x60
    long TotalFadeTime;                                     // +0x64
    CIntelligentPointerConstThing PTarget;                  // +0x68
    bool IsDecentAttackRange(const C3DVector& target);      // 0x0080F190
    void FrameUpdate();
    void PickNewTargetPos();                                // 0x0080F270
};


void CTCNymph::FrameUpdate()
{
    if (FlyingAround) {
        if (FadeCounter == 0) {
            float t = (float)(GetWorld()->GetWorldFrame() - LastTargetTick) / g_FramesPerSecondF;
            if (t < 1.0f) {
                C3DVector position;
                FlightSpline.GetSplinePosition(&position, t * 0.75f + 0.25f);
                position.Z -= EffectHeightOffset;
                ParentThing->PhysicsBody->SetPosition(position);
            } else
                PickNewTargetPos();
            if (PTarget.IsAlive()) {
                if (CanAttack)
                    ReadyToAttack = IsDecentAttackRange(PTarget.GetPItem()->GetPosition());
            } else {
                ReadyToAttack = false;
                CTCTargeting* targeting;
                if (ParentThing->GetInterface(&targeting, 8) && targeting->GetTarget()) {
                    CIntelligentPointerConstThing target(targeting->GetTarget());
                    PTarget = target;
                }
            }
        } else if (--FadeCounter != 0)
            ParentThing->GraphicAppearance->SetAlpha((unsigned char)(long)((float)FadeCounter / TotalFadeTime * 255.0f));
        else {
            PickNewTargetPos();
            ParentThing->SetVisible(false);
        }
    } else if (FadeCounter != 0) {
        --FadeCounter;
        ParentThing->GraphicAppearance->SetAlpha((unsigned char)(long)((1.0f - (float)FadeCounter / TotalFadeTime) * 255.0f));
        if (FadeCounter == 0 && POldEffect.IsAlive()) {
            POldEffect.GetPItem()->Kill(true);
            CIntelligentPointer<CThing> none(0);
            POldEffect = none;
        }
    }
    CTCBaseFrame::FrameUpdate();
}