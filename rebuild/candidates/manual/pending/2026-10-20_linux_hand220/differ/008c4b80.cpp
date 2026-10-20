// CCombatAction_StrikeResponseScorpionKing::Initialise() -- the scorpion king's hit response: run the generic strike-response Initialise while keeping the body's facing angle, then spawn the definition's "EYE_GET_HIT" special effect at the creature and attach its particle emitter to the creature's EMITTER_EYE helper
// Grade: DIFFER(304v302) -- retail packs the frame into two words (the facing spill and then effectType share [esp+14h], the "EYE_GET_HIT" temporary reuses the def slot word, "EMITTER_EYE" reuses effectType's) and loads the def slot straight into ebp; ours keeps effectType in a third word (all +4 offsets) and goes through eax (`mov eax,[esp+10h] ; cmp ; mov ebp,eax`); tried function-scope, block-scoped facing and nested-scope forms
// Leaf: ?Initialise@CCombatAction_StrikeResponseScorpionKing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4B80, length 302 bytes (plain `ret`, virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8), the body's (+0x60) virtual slots 63 (+0xfc,
//          GetFacingAngleXY) / 66 (+0x108, SetFacingAngleXY), CCombatAction_GenericStrikeResponseBase::Initialise 0x00865F40,
//          CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580 (on a counted copy of the thing's def +0x70), CCharString
//          0x0099EBF0 / ~ 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString 0x00669D30, CThing::GetPosition 0x004C73D0,
//          CreateEffectThing 0x006E0880 (__fastcall), CTCDParticleEmitter::AttachToThing 0x006E0BE0; def release slot 1.
// Model: the landed CCreatureAction_UndeadReturnToEarth::Initialise (0x008EC320) and CTCResurrectionItem::OnItemUsed
//          (0x007F04C0) models (the emitter is the effect thing's cached interface-0x4b component at +0x5c).
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefVirtual {
    virtual ~CDefVirtual();                     // slot 0
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
};
struct CSpecialEffectsDef : CDefBase {
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
template <class T> struct CDefPointer {         // the out-slot GetSubDef fills (counted reference handed over)
    T* m_p;
    CDefPointer() : m_p(0) { }
};
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
struct CDefClassReference {                     // a counted CDefClassBase reference (AddRef / Release)
    CDefClassBase* m_p;
    CDefClassReference(CDefClassBase* p) : m_p(p) { if (m_p) ++m_p->m_refCount; }
    ~CDefClassReference() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    CDefClassBase* operator->() const { return m_p; }
};
struct CPhysicsBody {                           // the thing's physics body at +0x60
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062();
    virtual float GetFacingAngleXY();   // slot 63 (+0xfc)
    virtual void v064(); virtual void v065();
    virtual void SetFacingAngleXY(float angle);   // slot 66 (+0x108)
};
class CTCDParticleEmitter {
public:
    void AttachToThing(class CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];            // +0x20 std::bitset of interface types
    unsigned char  _pad44[0x5c - 0x44];
    CTCDParticleEmitter* PTCDriver;             // +0x5c (the cached interface-0x4b component)
    CPhysicsBody*  m_pBody;                     // +0x60
    unsigned char  _pad64[0x70 - 0x64];
    CDefClassBase* m_pDef;                      // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_flags91;                   // +0x91 bit 0: dead
    CPhysicsBody* GetBody() const { return m_pBody; }
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CCombatAction_GenericStrikeResponseBase {
    virtual void Initialise();                  // 0x00865F40
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
};
struct CCombatAction_StrikeResponseScorpionKing : CCombatAction_GenericStrikeResponseBase {
    virtual void Initialise();
};

void CCombatAction_StrikeResponseScorpionKing::Initialise()
{
    float facing = PCreature.GetPItem()->GetBody()->GetFacingAngleXY();
    CCombatAction_GenericStrikeResponseBase::Initialise();
    PCreature.GetPItem()->GetBody()->SetFacingAngleXY(facing);
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    {
        CDefClassReference def(PCreature.GetPItem()->m_pDef);
        def->GetSubDef(effectsSlot);
    }
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (effects) {
        if (effects->GetEffectFromString("EYE_GET_HIT", effectType)) {
            CThing* effect = CreateEffectThing(effectType, &PCreature.GetPItem()->GetPosition(), false);
            if (effect && !(effect->m_flags91 & 1) && effect->HasTC(0x4b)) {
                CTCDParticleEmitter* emitter = effect->PTCDriver;
                emitter->AttachToThing(PCreature.GetPItem(), 10, "EMITTER_EYE", 0, 0.0f);
            }
        }
        if (--effects->m_refCount == 0)
            effects->Destroy();
    }
}
