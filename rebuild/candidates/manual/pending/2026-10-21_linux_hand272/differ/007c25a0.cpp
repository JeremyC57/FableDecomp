// CTCShotDefinitionBase::InitAttachedEffects() -- create the shot def's attached continuous effect at the shot, keep it as the shot's effect thing, attach its particle emitter to the shot (at the SHOT_CREATION_DUMMY dummy, else the weapon_pos_a helper, else the default attach type; end-timeline flag 0x10) and hand it the shot's direction as MOVE_DIRECTION
// Grade: DIFFER(452v452) -- same length; frame 8 bytes larger (sub esp,0x50 v 0x48): retail lets the uninitialised emitter (read on the no-driver path), the CIntelligentPointer temporary and the GetDirection result share one frame word, ours gives the emitter its own slot. Tried named pointer block, inline getter, early declaration.
// Retail: Fable.exe @ 0x007C25A0, 452 bytes (plain `ret`).
// Callees: CThing::GetPosition 0x004C73D0, CreateEffectThing 0x006E0880 (__fastcall), CBaseIntelligentPointer ctor 0x00A01B10 /
//   SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C),
//   CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / operator=(const CCharString&) 0x0099EFB0 /
//   operator=(const char*) 0x0099EFE0 / ~ 0x0099EAE0, CThingAppearance::HasDummyObject 0x004BE2C0 / HasHelperPoint 0x004BE170,
//   CTCDParticleEmitter::AttachToThing 0x006E0BE0, CRC Calc 0x004014A0, physics virtual slot 70 (+0x118),
//   CBaseClass ctor 0x0099A2D0, push_back 0x0076B120, SetEmitterUpdateParameters 0x006E11A0, ~CParticleUpdateParameters 0x006E2500.
// Layout: as the landed UpdateEffects 0x007C2480 (same TU); PShotDef +0x38 (CThingShotDef AttachedContinuousEffect +0x48,
//   UseEndTimeLineOnContinousEffect +0x50, PDB minus 4); CThing appearance +0x64; g_CCharString_013bacb4 = "SHOT_CREATION_DUMMY".
class CTCBase;
class CThing;
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC
extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C (1e-4f)
struct CBaseClass {                                     // vtable 0x01231710
    virtual void v00();
    CBaseClass();                                       // 0x0099A2D0
};
namespace NParticleEngine {
struct SVectorParam {                                   // pair<unsigned long, C3DVector>
    unsigned long m_key;
    C3DVector     m_value;
    SVectorParam(const unsigned long& key, const C3DVector& value) : m_key(key), m_value(value) {}
};
struct SUpdateParam;
struct CVectorParamVector {                             // retail STLport vector (12 bytes)
    SVectorParam* _M_start;
    SVectorParam* _M_finish;
    SVectorParam* _M_end_of_storage;
    __forceinline CVectorParamVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    void push_back(const SVectorParam& value);          // 0x0076B120
};
struct CUpdateParamVector {
    SUpdateParam* _M_start;
    SUpdateParam* _M_finish;
    SUpdateParam* _M_end_of_storage;
    __forceinline CUpdateParamVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CCompare {};
struct CVectorParamMap {
    CVectorParamVector Vector;                          // +0x00
    CCompare           Compare;                         // +0x0c
    bool               Dirty;                           // +0x0d
    __forceinline CVectorParamMap() : Dirty(false) {}
};
struct CUpdateParamMap {
    CUpdateParamVector Vector;                          // +0x00
    CCompare           Compare;                         // +0x0c
    bool               Dirty;                           // +0x0d
    __forceinline CUpdateParamMap() : Dirty(false) {}
};
struct CParticleUpdateParameters : CBaseClass {         // <unsigned long, unsigned long>, vtable 0x0125FFDC
    CVectorParamMap VectorParams;                       // +0x04
    CUpdateParamMap UpdateParams;                       // +0x14
    __forceinline CParticleUpdateParameters() {}
    virtual ~CParticleUpdateParameters();               // 0x006E2500
};
}
struct CCharString {
    void* m_data;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
extern CCharString g_CCharString_013bacb4;              // 0x013BACB4 "SHOT_CREATION_DUMMY"
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, long flags);   // 0x006E0BE0
    void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters& parameters);   // 0x006E11A0
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
struct CThingAppearance {
    bool HasHelperPoint(const CCharString& name);       // 0x004BE170
    bool HasDummyObject(const CCharString& name);       // 0x004BE2C0
};
struct CTCPhysics {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual C3DVector GetDirection();                   // slot 70 (+0x118)
};
struct CThingShotDef {
    unsigned char _pad00[0x48];
    long          AttachedContinuousEffect;             // +0x48
    long          AttachedContinuousSound;              // +0x4c
    bool          UseEndTimeLineOnContinousEffect;      // +0x50
};
class CThing {
public:
    const C3DVector& GetPosition() const;               // 0x004C73D0
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];                   // +0x20
    unsigned char   _pad44[0x5c - 0x44];
    void*           PTCDriver;                          // +0x5c
    struct CTCPhysics* PhysicsTC;                       // +0x60
    struct CThingAppearance* Appearance;                // +0x64
    unsigned char   _pad68[0x91 - 0x68];
    unsigned char   m_flags91;                          // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
};
struct CBaseIntelligentPointer {
    void*   __vftable;                                  // +0x00
    CThing* m_pItem;                                    // +0x04
    long    m_link;                                     // +0x08
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCShotDefinitionBase {
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x4];
    CBaseIntelligentPointer PEffect;                    // +0x0c
    unsigned char _pad18[0x20];
    const CThingShotDef* PShotDef;                      // +0x38 (CDefPointer)
    void InitAttachedEffects();
};

void CTCShotDefinitionBase::InitAttachedEffects()
{
    long effectType = PShotDef->AttachedContinuousEffect;
    if (!effectType)
        return;
    CThing* effect = CreateEffectThing(effectType, &ParentThing->GetPosition(), false);
    if (!effect || (effect->m_flags91 & 1))
        return;
    {
        CIntelligentPointerThing effectPointer(effect);
        PEffect.SetPItem(effectPointer.GetPItem());
    }
    CTCDParticleEmitter* emitter;
    if (effect->HasTC(0x4b))
        emitter = (CTCDParticleEmitter*)effect->PTCDriver;
    CCharString helperName;
    CThingAppearance* appearance = ParentThing->Appearance;
    long attachType;
    if (appearance->HasDummyObject(g_CCharString_013bacb4)) {
        helperName = g_CCharString_013bacb4;
        attachType = 10;
    } else if (appearance->HasHelperPoint("weapon_pos_a")) {
        helperName = "weapon_pos_a";
        attachType = 12;
    } else
        attachType = 0x28;
    if (PShotDef->UseEndTimeLineOnContinousEffect)
        attachType |= 0x10;
    emitter->AttachToThing(ParentThing, attachType, helperName, 0, 0);
    unsigned long key = Calc_004014a0(0, "MOVE_DIRECTION", 14);
    NParticleEngine::SVectorParam moveDirection(key, ParentThing->PhysicsTC->GetDirection());
    NParticleEngine::CParticleUpdateParameters parameters;
    parameters.VectorParams.Vector.push_back(moveDirection);
    emitter->SetEmitterUpdateParameters(parameters);
}
