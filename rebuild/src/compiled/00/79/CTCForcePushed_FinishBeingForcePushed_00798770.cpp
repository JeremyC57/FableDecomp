// CTCForcePushed::FinishBeingForcePushed(bool reversed, float damage) -- end of a force push: a game object takes a knock-down
// hit along the push direction (reversed with the given damage, else 1 point), the owner's animation resumes, the push particle
// is killed and the component removes itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00798770, 451 bytes (+ int3 padding), `ret 8`.
// Leaf: FinishBeingForcePushed
// Shape: the hit is built by a __forceinline helper taking the owner position BY VALUE (`HitOwner(owner, owner->GetPos(),
// direction, damage)`): the by-value argument is an aggregate copy of the GetPos() reference, so VC loads the three words
// into ecx/edx/eax before the CHitParameters argument pushes and stores them between the pushes (retail); a named
// `C3DVector position = owner->GetPos();` local interleaves load/store, whatever the copy-ctor spelling. C3DVector has no
// user copy constructor (the position copy must be the implicit aggregate one); `direction` is built memberwise from the
// DirectionBeingPushed members through the three-float constructor (retail `mov eax,[edi+2Ch] ; mov ecx,[edi+30h] ; ...`).
// Layout: landed CTCForcePushed_SetUpForcePush_007986a0 model; CHitParameters CreateDustParticleEffectOnHit +0x24, GuaranteeHit +0x25 (PDB).
class CThing;
class CBaseObject;
namespace NHitLocations { class CHitLocation; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
};

class CHitParameters {                              // 0x60 bytes in retail
public:
    unsigned char _pad00[0x22];
    bool MoveBack;                                  // +0x22
    bool CreateParticleEffectOnHit;                 // +0x23
    bool CreateDustParticleEffectOnHit;             // +0x24
    bool GuaranteeHit;                              // +0x25
    unsigned char _pad26[0x60 - 0x26];
    CHitParameters(NHitLocations::CHitLocation* hitLocation, CThing* thingHitting, CThing* weapon,
                   const C3DVector& position, const C3DVector& direction, bool knockDown, bool blockable,
                   bool flourish, bool blockCounter);                       // 0x00789238
    ~CHitParameters();                              // 0x00789358
    void AddDamageWithAttribute(long attribute, float damage);             // 0x00868CA0 (EDamageAttribute)
};

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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

enum ETCInterfaceType { };
class CThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void OnHit(CHitParameters& hit);        // slot 65 (+0x104)
    unsigned char _pad04[0x10 - 0x04];
    unsigned char ThingClass;                       // +0x10 (1: AI creature)
    unsigned char _pad11[0x6c - 0x11];
    unsigned long ThingTypeFlags;                   // +0x6C (bit 3)
    unsigned char _pad70[0x91 - 0x70];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    const C3DVector& GetPos() const;           // 0x004C73D0
    void Kill(bool immediately);                    // 0x004C9B80
    void RemoveTC(ETCInterfaceType interfaceType);              // 0x004C9840
};
class CThingAnimatable : public CThing {
public:
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCForcePushed {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[4];
    CIntelligentPointer<CThing> PThingCausingForcePush; // +0x0C
    CIntelligentPointer<CThing> PParticle;              // +0x14
    C3DVector PositionForcePushStarted;             // +0x1C
    float DistanceBeingPushed;                      // +0x28
    C3DVector DirectionBeingPushed;                 // +0x2C
    float DistanceMovingPerFrame;                   // +0x38
    bool Initialised;                               // +0x3C
    bool StartedMoving;                             // +0x3D
    void FinishBeingForcePushed(bool reversed, float damage);
    __forceinline void HitOwner(CThing* owner, C3DVector position, const C3DVector& direction, float damage)
    {
        CHitParameters hit(0, PThingCausingForcePush.GetPItem(), 0, position, direction, true, false, false, false);
        hit.GuaranteeHit = true;
        hit.AddDamageWithAttribute(8, damage);
        hit.CreateDustParticleEffectOnHit = false;
        owner->OnHit(hit);
    }
};

void CTCForcePushed::FinishBeingForcePushed(bool reversed, float damage)
{
    if (damage < 0.0001f)
        damage = 0.001f;
    CThing* owner = ParentThing;
    if (owner->ThingTypeFlags & 8) {
        C3DVector direction(DirectionBeingPushed.X, DirectionBeingPushed.Y, DirectionBeingPushed.Z);
        if (reversed) {
            direction = -direction;
            HitOwner(owner, owner->GetPos(), direction, damage);
        } else
            HitOwner(owner, owner->GetPos(), direction, 1.0f);
    }
    if (ParentThing->ThingClass == 1)
        ((CThingAnimatable*)ParentThing)->SetSuspendAnimation(false, 0);
    if (IS_THING_ALIVE(PParticle))
        PParticle.GetPItem()->Kill(true);
    ParentThing->RemoveTC((ETCInterfaceType)(0x8d));
}