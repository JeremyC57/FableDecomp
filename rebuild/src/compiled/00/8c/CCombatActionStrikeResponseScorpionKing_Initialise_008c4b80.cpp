// CCombatAction_StrikeResponseScorpionKing::Initialise() -- the scorpion king's hit response: run the generic strike-response Initialise while keeping the body's facing angle, then spawn the definition's "EYE_GET_HIT" special effect at the creature and attach its particle emitter to the creature's EMITTER_EYE helper
// Leaf: ?Initialise@CCombatAction_StrikeResponseScorpionKing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4B80, length 302 bytes (plain `ret`, virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8), the body's (+0x60) virtual slots 63 (+0xfc,
//          GetFacingAngleXY) / 66 (+0x108, SetFacingAngleXY), CCombatAction_GenericStrikeResponseBase::Initialise 0x00865F40,
//          CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580 (on a counted copy of the thing's def +0x70), CCharString
//          0x0099EBF0 / ~ 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString 0x00669D30, CThing::GetPos 0x004C73D0,
//          CreateEffectThing 0x006E0880 (__fastcall), CTCDParticleEmitter::AttachToThing 0x006E0BE0; def release slot 1.
// Model: the landed CCreatureAction_UndeadReturnToEarth::Initialise (0x008EC320) and CTCResurrectionItem::OnItemUsed
//          (0x007F04C0) models (the emitter is the effect thing's cached interface-0x4b component at +0x5c).
// Shape: GetSubDef and GetEffectFromString are same-TU noinline stand-in bodies that write only their out slots, so the
//          def slot and effectType do not escape: the frame packs into two words (the facing spill and effectType share one,
//          the "EYE_GET_HIT" temporary reuses the def slot word, "EMITTER_EYE" reuses effectType's).
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
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
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail): it writes the out type only, so VC7.1 knows the effect type's address does
// not escape the call.
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
template <class T> class CDefPointer { public:         // the out-slot GetSubDef fills (counted reference handed over)
    T* m_p;
    CDefPointer() : m_p(0) { }
};
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)
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
    const C3DVector& GetPos() const;       // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CCombatAction_GenericStrikeResponseBase { public:
    virtual void Initialise();                  // 0x00865F40
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
};
class CCombatAction_StrikeResponseScorpionKing : public CCombatAction_GenericStrikeResponseBase { public:
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
            CThing* effect = CreateEffectThing(effectType, &PCreature.GetPItem()->GetPos(), false);
            if (effect && !(effect->m_flags91 & 1) && effect->HasTC(0x4b)) {
                CTCDParticleEmitter* emitter = effect->PTCDriver;
                emitter->AttachToThing(PCreature.GetPItem(), 10, "EMITTER_EYE", 0, 0.0f);
            }
        }
        if (--effects->m_refCount == 0)
            effects->Destroy();
    }
}