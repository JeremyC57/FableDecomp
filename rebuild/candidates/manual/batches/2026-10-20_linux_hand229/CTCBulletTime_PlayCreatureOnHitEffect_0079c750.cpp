// CTCBulletTime::PlayCreatureOnHitEffect(CThing* creature) -- remember the hit creature, start the bullet-time radial blur (the damage blur when the hero himself was hit, the normal one otherwise) with its fade, and centre the blur on the creature
// Leaf: PlayCreatureOnHitEffect
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079C750, 298 bytes (+6 int3 padding), `ret 4`.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10
//   (CIntelligentPointer<CThing> vtable 0x01238C6C), CTCBase::GetWorld() 0x00686A70 (twice), the radial-blur manager's
//   add (0x006B8550: two zero words, threshold colour, length, intensity colour, fade, range, twist; returns the key) and
//   fade-out (0x006B82E0: fade seconds, key) members (descriptive names; manifest labels _Ucopy/_Cons_val are
//   propagated), CThing::GetPosition() const 0x004C73D0, CEnvironmentSpellEffectInterface::SetRadialBlurCenterWorldPos
//   (const C3DVector&, bool) 0x00885110.
// Layout: engine/CTCBulletTime.h (PDef +0x20, HitCreatureBlur +0x44, RadialBlurKey +0x4c; retail = PDB);
//   CSpecialAbilitiesBulletTimeDef at PDB - 4: the RadialBlur* block +0xD4..+0xFC and the DamageRadialBlur* block
//   +0x100..+0x128; the world's +0x18 object holds the spell-effect interface at +0x1960 and the radial-blur manager at
//   +0x1964.
// Shape: the hit creature is stored through the intelligent pointer's inline operator= from a CIntelligentPointer
//   (creature) temporary; the spell-effect interface is a named local homed in the dead `creature` slot, the blur
//   manager a named local (edi), the parent a third named local (ebp, loaded before the GetPItem compare).
struct C3DVector { float X, Y, Z; };
class CBaseObject;
class CThing {
public:
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
struct CRadialBlurParams {                      // one RadialBlur* block of the def (0x2c bytes)
    C3DVector ThresholdCol;                     // +0x00
    float     Length;                           // +0x0c
    C3DVector IntensityCol;                     // +0x10
    float     Fade;                             // +0x1c
    float     Range;                            // +0x20
    float     Twist;                            // +0x24
    float     FadeTimeSecs;                     // +0x28
};
struct CSpecialAbilitiesBulletTimeDef {
    unsigned char     _pad00[0xd4];
    CRadialBlurParams RadialBlur;               // +0xd4
    CRadialBlurParams DamageRadialBlur;         // +0x100
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
struct CRadialBlurManager {
    void* AddRadialBlur(long a, long b, const C3DVector& threshold, float length, const C3DVector& intensity,
                        float fade, float range, float twist);   // 0x006B8550
    void FadeOutRadialBlur(float seconds, void* key);            // 0x006B82E0
};
struct CEnvironmentSpellEffectInterface {
    void SetRadialBlurCenterWorldPos(const C3DVector& position, bool immediate);   // 0x00885110
};
struct CWorldEffects {
    unsigned char _pad00[0x1960];
    CEnvironmentSpellEffectInterface* SpellEffects;   // +0x1960
    CRadialBlurManager*               RadialBlurs;    // +0x1964
};
struct CWorld {
    unsigned char  _pad00[0x18];
    CWorldEffects* PEffects;                    // +0x18
};

class CTCBulletTime {
public:
    void*         __vftable;                    // +0x00 (CTCBase)
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad08[0x20 - 0x08];
    CDefPointer<const CSpecialAbilitiesBulletTimeDef> PDef;   // +0x20
    unsigned char _pad24[0x44 - 0x24];
    CIntelligentPointer<CThing> HitCreatureBlur;   // +0x44
    void*         RadialBlurKey;                // +0x4c
    CWorld* GetWorld();                         // 0x00686A70 (CTCBase)
    void PlayCreatureOnHitEffect(CThing* creature);
};

void CTCBulletTime::PlayCreatureOnHitEffect(CThing* creature)
{
    HitCreatureBlur = CIntelligentPointer<CThing>(creature);
    CEnvironmentSpellEffectInterface* spellEffects = GetWorld()->PEffects->SpellEffects;
    CRadialBlurManager* blurs = GetWorld()->PEffects->RadialBlurs;
    CThing* parent = ParentThing;
    if (HitCreatureBlur.GetPItem() == parent) {
        RadialBlurKey = blurs->AddRadialBlur(0, 0, PDef->DamageRadialBlur.ThresholdCol, PDef->DamageRadialBlur.Length,
                                             PDef->DamageRadialBlur.IntensityCol, PDef->DamageRadialBlur.Fade,
                                             PDef->DamageRadialBlur.Range, PDef->DamageRadialBlur.Twist);
        blurs->FadeOutRadialBlur(PDef->DamageRadialBlur.FadeTimeSecs, RadialBlurKey);
    } else {
        RadialBlurKey = blurs->AddRadialBlur(0, 0, PDef->RadialBlur.ThresholdCol, PDef->RadialBlur.Length,
                                             PDef->RadialBlur.IntensityCol, PDef->RadialBlur.Fade,
                                             PDef->RadialBlur.Range, PDef->RadialBlur.Twist);
        blurs->FadeOutRadialBlur(PDef->RadialBlur.FadeTimeSecs, RadialBlurKey);
    }
    spellEffects->SetRadialBlurCenterWorldPos(HitCreatureBlur.GetPItem()->GetPosition(), true);
}
