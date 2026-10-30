// NThingShot::CTCShotDefinitionMultiArrow::InitAttachedEffects() -- spawn the multi-arrow trail effect at the shooter-side thing and, when it lives, keep it as PEffect and attach its particle emitter to the thing's "weapon_pos_a" helper
// Grade: DIFFER(185v186) -- CDefPointer release without the null test (retail: dec [ecx+4] straight after the reload) takes the residue from 3 bytes to 1; left: VC7.1 hoists a zero register (push ebx/xor ebx) where retail stores and [ebp-4],0 / push 0, and the def/effect slots are swapped
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BDD77, 186 bytes (plain `ret`; frame-pointer size TU, as the landed
// GetShotDamage 0x007BDCFC: optimize("y", off) + ("s", on)).
// Callees: CDefClassBase::GetSubDef_007be7ea(CDefPointer&) @ 0x007BE7EA (the 'CThingMultiArrowShotDef' instantiation; landed
// spelling), CThing::GetPosition const @ 0x004C73D0, CreateEffectThing(long, const C3DVector*, bool) @ 0x006E0880 (__fastcall),
// IS_THING_ALIVE<CThing*>(CThing* const&) @ 0x0040DCE0 (__fastcall), CIntelligentPointer<CThing>(CThing*) @ 0x004A96F0,
// CIntelligentPointer::operator= @ 0x004A9760, ~CBaseIntelligentPointer @ 0x00A01C10, CThing::GetTCDriver<T>(T**) @ 0x004ACB50
// (driver interface bit 75; the landed body's CTCDExperienceOrb argument is a propagated label -- the driver used here is the
// particle emitter), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CTCDParticleEmitter::AttachToThing
// (CThing*, long, const CCharString&, long, float) @ 0x006E0BE0; the def's virtual slot 1. Literal "weapon_pos_a" @ 0x0124374C.
// Layout: Ego_r.pdb CTCShotDefinitionBase with the retail 12-byte vector: PEffect +0x0c, PShotDef +0x38; CThingMultiArrowShotDef
// (def at PDB offsets) MultiArrowTrailEffect +0x44.
// Shape: the two blocks let the temporary and the emitter out pointer share a frame word; the sub def out pointer is zeroed with `and [ebp-4],0` and released at the end without a null test (it is
// dereferenced first); the effect thing is a named local (IS_THING_ALIVE takes it by reference); PEffect is assigned from an
// unnamed CIntelligentPointer temporary whose frame slot the emitter out pointer reuses; the helper name is a named local.
class CThing;
class CBaseObject;
struct C3DVector { float X, Y, Z; };
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { Object->Release(); }
    T* operator->() const { return Object; }
};
struct CThingMultiArrowShotDef : CDefBase {
    unsigned char _pad08[0x44 - 0x08];
    long MultiArrowTrailEffect;                         // +0x44
};
struct CDefClassBase : CDefBase {
    bool GetSubDef_007be7ea(CDefPointer<CThingMultiArrowShotDef>& out);   // 0x007BE7EA
};
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long flags, float scale);   // 0x006E0BE0
};
class CThing {
public:
    const C3DVector& GetPosition() const;               // 0x004C73D0
    template <class T> bool GetTCDriver(T** out);       // 0x004ACB50
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0040DCE0 (CThing*)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    ~CBaseIntelligentPointer();                         // 0x00A01C10
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer(T* item);                       // 0x004A96F0 (CThing)
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760
};
namespace NThingShot {
struct CTCShotDefinitionMultiArrow {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CIntelligentPointer<CThing> PEffect;                // +0x0c
    unsigned char _pad14[0x38 - 0x14];
    CDefClassBase* PShotDef;                            // +0x38 CDefPointer<const CThingShotDef>
    void InitAttachedEffects();
};

void CTCShotDefinitionMultiArrow::InitAttachedEffects()
{
    CDefPointer<CThingMultiArrowShotDef> arrowDef;
    PShotDef->GetSubDef_007be7ea(arrowDef);
    long trailEffect = arrowDef->MultiArrowTrailEffect;
    if (trailEffect) {
        CThing* effect = CreateEffectThing(trailEffect, &ParentThing->GetPosition(), false);
        if (IS_THING_ALIVE(effect)) {
            {
                PEffect = CIntelligentPointer<CThing>(effect);
            }
            {
                CTCDParticleEmitter* emitter = 0;
                effect->GetTCDriver(&emitter);
                CCharString helper("weapon_pos_a", -1);
                emitter->AttachToThing(ParentThing, 0x3c, helper, 0, 0.0f);
            }
        }
    }
}
}
