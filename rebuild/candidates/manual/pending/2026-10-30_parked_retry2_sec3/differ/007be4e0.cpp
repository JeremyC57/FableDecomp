// NThingShot::CTCShotDefinitionDrainLife::InitAttachedEffects() -- create the shot def's attached continuous effect at the shot, keep it as the shot's effect, attach its particle emitter to the shot at "weapon_pos_a" and seed the emitter's MOVE_DIRECTION with the shot's facing
// Grade: DIFFER(246v256) -- byte twin of 0x007BE3E0 (frame-packing gap: conversion temporary overlapping the later emitter word, edi zero register); CRC callee respelled CCRC::Calc (identical bytes)
// Leaf: InitAttachedEffects
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BE4E0, 256 bytes (plain `ret`). NThingShot::CTCShotDefinitionDrainLife vtable
//   0x012403F4 slot 34 (byte twin of CTCShotDefinitionForcePushBall::InitAttachedEffects 0x007BE3E0, the same TU code;
//   only the call displacements differ). The shot TU is size-optimised with a frame pointer: `optimize("y", off)` + `("s", on)`.
// Callees: CThing::GetPosition() const @ 0x004C73D0, CreateEffectThing(long, const C3DVector*, bool) @ 0x006E0880
//   (`__fastcall`), IS_THING_ALIVE<CThing*> @ 0x0040DCE0 (`__fastcall`, by const reference), CIntelligentPointer(CThing*)
//   @ 0x004A96F0, CIntelligentPointer::operator= @ 0x004A9760, ~CBaseIntelligentPointer @ 0x00A01C10,
//   CThing::GetTCDriver<CTCDParticleEmitter>(CTCDParticleEmitter**) @ 0x004ACB50, CCharString(const char*, long) @ 0x0099EBF0
//   ("weapon_pos_a" @ 0x0124374C), CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float)
//   @ 0x006E0BE0, CRC Calc @ 0x004014A0 ("MOVE_DIRECTION" @ 0x0126E160, 14), the physics body's virtual GetFacing (slot 70,
//   +0x118, C3DVector by hidden pointer), CParticleUpdateParameters ctor @ 0x006E24D0 (out of line in this TU),
//   vector<pair<unsigned long, C3DVector> >::push_back @ 0x0076B120, CTCDParticleEmitter::SetEmitterUpdateParameters
//   @ 0x006E11A0, ~CParticleUpdateParameters @ 0x006E2500, ~CCharString @ 0x0099EAE0.
// Layout: Ego_r.pdb CTCShotDefinitionBase PEffect +0x0C, PShotDef +0x38; CThingShotDef AttachedContinuousEffect +0x48 (PDB
//   0x4C); retail CThing PhysicsTC +0x60.
// Shape: the effect is a named local passed by const reference; the effect assignment goes through the implicit conversion
//   temporary (its frame words reused by the later emitter local); the emitter is a zero-initialised out word; the CRC key is
//   a named local computed before the facing call, and the {key, facing} pair a named local built before the parameter block.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C3DVector { float X, Y, Z; };
struct CCRC {
    static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
namespace NParticleEngine {
struct SVectorParam {                                   // pair<unsigned long, C3DVector>
    unsigned long m_key;
    C3DVector     m_value;
    SVectorParam(const unsigned long& key, const C3DVector& value) : m_key(key), m_value(value) {}
};
struct CVectorParamVector {                             // retail STLport vector (12 bytes)
    SVectorParam* _M_start;
    SVectorParam* _M_finish;
    SVectorParam* _M_end_of_storage;
    void push_back(const SVectorParam& value);          // 0x0076B120
};
struct CVectorParamMap {
    CVectorParamVector Vector;                          // +0x00
    bool               Dirty;                           // +0x0d
};
struct CParticleUpdateParameters {                      // <unsigned long, unsigned long>, 0x24 bytes
    void*           __vftable;                          // +0x00 (CBaseClass root)
    CVectorParamMap VectorParams;                       // +0x04
    unsigned char   UpdateParams[0x10];                 // +0x14
    CParticleUpdateParameters();                        // 0x006E24D0
    ~CParticleUpdateParameters();                       // 0x006E2500
};
}
class CThing;
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
    void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters& parameters);   // 0x006E11A0
};
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetFacing() const;                // slot 70 (+0x118)
};
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    const C3DVector& GetPosition() const;               // 0x004C73D0
    template <class T> bool GetTCDriver(T** out);       // 0x004ACB50 (<CTCDParticleEmitter>)
};
CThing* __fastcall CreateEffectThing(long defIndex, const C3DVector* position, bool flag);   // 0x006E0880
bool __fastcall IS_THING_ALIVE(CThing* const& thing);  // 0x0040DCE0
struct CBaseIntelligentPointer {
    void* m_pItem;
    void* m_pInfo;
    ~CBaseIntelligentPointer();                         // 0x00A01C10
};
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {
    CIntelligentPointer_CThing(CThing* thing);          // 0x004A96F0
    CIntelligentPointer_CThing& operator=(const CIntelligentPointer_CThing& other);   // 0x004A9760
};
struct CThingShotDef {
    unsigned char _pad00[0x48];
    long          AttachedContinuousEffect;             // +0x48 (PDB 0x4c)
};
namespace NThingShot {
struct CTCShotDefinitionDrainLife {
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x4];
    CIntelligentPointer_CThing PEffect;                 // +0x0c
    unsigned char _pad14[0x38 - 0x14];
    const CThingShotDef* PShotDef;                      // +0x38 CDefPointer
    void InitAttachedEffects();
};

void CTCShotDefinitionDrainLife::InitAttachedEffects()
{
    long effectDef = PShotDef->AttachedContinuousEffect;
    if (effectDef == 0)
        return;
    CThing* effect = CreateEffectThing(effectDef, &ParentThing->GetPosition(), false);
    if (!IS_THING_ALIVE(effect))
        return;
    PEffect = effect;
    {
    CTCDParticleEmitter* emitter = 0;
    effect->GetTCDriver(&emitter);
    CCharString helper("weapon_pos_a", -1);
    emitter->AttachToThing(ParentThing, 0x3c, helper, 0, 0.0f);
    unsigned long key = CCRC::Calc(0, "MOVE_DIRECTION", 14);
    NParticleEngine::SVectorParam moveDirection(key, ParentThing->PhysicsTC->GetFacing());
    NParticleEngine::CParticleUpdateParameters parameters;
    parameters.VectorParams.Vector.push_back(moveDirection);
    emitter->SetEmitterUpdateParameters(parameters);
    }
}
}
