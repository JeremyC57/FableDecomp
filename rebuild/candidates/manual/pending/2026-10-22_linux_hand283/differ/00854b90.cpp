// CCreatureAction_SpiritAttack::FrameUpdate() -- base frame update; while the target and the lightning emitter are alive, run the emitter's LIGHTNING_START at the creature's coop-spirit focal point (raised 0.8) and LIGHTNING_END at the target's helper point (or centre)
// Grade: DIFFER(568v566) -- the LIGHTNING_END key: retail keeps it in esi and forwards it into the second push_back's in-place copy (loads hoisted above the stores); ours stores it straight into the pair and copies the 16 bytes member by member from memory (pair copy ctor, C3DVector copy ctor, std::pair template, by-value key and unnamed-key forms all tried)
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00854B90, length 566 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CCreatureActionBase::FrameUpdate @ 0x00694020, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CVectorMap::LowerBound @ 0x0040F020 (emitter driver 0x4B, coop spirit 0xC7), CTCCoopSpirit::GetFocalPos
//          @ 0x0066FFD0, CTCGraphicAppearance::GetHelperPointOrCentre(float) @ 0x004C1060, CRC Calc @ 0x004014A0
//          ("LIGHTNING_START" 15 / "LIGHTNING_END" 13), CBaseClass ctor @ 0x0099A2D0, STLport vector
//          _M_insert_overflow @ 0x0076ADA0, CParticleUpdateParameters update @ 0x0076C750,
//          CTCDParticleEmitter::SetEmitterUpdateParameters @ 0x006E11A0, ~CParticleUpdateParameters @ 0x006E2500.
// Layout: Ego_r.pdb CCreatureAction_SpiritAttack less 0xc: PTarget +0xa8, PParticleEmitter +0xb0; CParticleUpdateParameters
//   as the landed CTCShotDefinitionBase::UpdateEffects 0x007C2480 model (VectorParams map +0x04, Dirty +0x11).
// Shape: the two {crc, position} pairs are named locals built after their keys; the vector-map add is an inline
//   push_back (STLport: in-place copy or _M_insert_overflow; the first add folds to the overflow call on the fresh
//   vector) followed by the dirty flag.
class CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC
struct CBaseClass {
    virtual void v00();
    CBaseClass();                                       // 0x0099A2D0
};
inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type {};
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
    CVectorParamVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    void _M_insert_overflow(SVectorParam* position, const SVectorParam& x, const __true_type&, unsigned int n,
                            bool atEnd);                // 0x0076ADA0
    __forceinline void push_back(const SVectorParam& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) SVectorParam(x);
            ++_M_finish;
        } else {
            __true_type trivial;
            _M_insert_overflow(_M_finish, x, trivial, 1, true);
        }
    }
};
struct CUpdateParamVector {
    SUpdateParam* _M_start;
    SUpdateParam* _M_finish;
    SUpdateParam* _M_end_of_storage;
    CUpdateParamVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CCompare {};
struct CVectorParamMap {
    CVectorParamVector Vector;                          // +0x00
    CCompare           Compare;                         // +0x0c
    bool               Dirty;                           // +0x0d
    CVectorParamMap() : Dirty(false) {}
    __forceinline void push_back(const SVectorParam& x) { Vector.push_back(x); Dirty = true; }
};
struct CUpdateParamMap {
    CUpdateParamVector Vector;                          // +0x00
    CCompare           Compare;                         // +0x0c
    bool               Dirty;                           // +0x0d
    CUpdateParamMap() : Dirty(false) {}
};
struct CParticleUpdateParameters : CBaseClass {         // <unsigned long, unsigned long>, vtable 0x0125FFDC
    CVectorParamMap VectorParams;                       // +0x04
    CUpdateParamMap UpdateParams;                       // +0x14
    CParticleUpdateParameters() {}
    virtual ~CParticleUpdateParameters();               // 0x006E2500
    void Update();                                      // 0x0076C750
};
}
struct CTCDParticleEmitter {
    void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters& parameters);   // 0x006E11A0
};
struct CTCCoopSpirit {
    C3DVector GetFocalPos();                            // 0x0066FFD0
};
struct CTCGraphicAppearance {
    C3DVector GetHelperPointOrCentre(float t) const;    // 0x004C1060
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];                   // +0x20
    CInterfaceMap   m_interfaces;                       // +0x44
    unsigned char   _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* GraphicAppearance;            // +0x64
    unsigned char   _pad68[0x91 - 0x68];
    unsigned char   m_flags91;                          // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
struct CBaseIntelligentPointer {
    void*   __vftable;                                  // +0x00
    CThing* m_pItem;                                    // +0x04
    CThing* GetPItem() const;                           // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCreatureActionBase {
    virtual void FrameUpdate();                         // 0x00694020
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                  // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
};
struct CCreatureAction_SpiritAttack : CCreatureActionBase {
    CBaseIntelligentPointer PTarget;                    // +0xa8
    CBaseIntelligentPointer PParticleEmitter;           // +0xb0
    virtual void FrameUpdate();
};

void CCreatureAction_SpiritAttack::FrameUpdate()
{
    CCreatureActionBase::FrameUpdate();
    if (IS_THING_ALIVE(PTarget) && IS_THING_ALIVE(PParticleEmitter)) {
        CThing* target = PTarget.GetPItem();
        CTCDParticleEmitter* emitter = 0;
        CThing* effect = PParticleEmitter.GetPItem();
        if (effect->HasTC(0x4b))
            emitter = (CTCDParticleEmitter*)effect->FindTC(0x4b);
        CTCCoopSpirit* spirit = 0;
        CThing* creature = PCreature.GetPItem();
        if (creature->HasTC(0xc7))
            spirit = (CTCCoopSpirit*)creature->FindTC(0xc7);
        C3DVector start = spirit->GetFocalPos();
        start.Z += 0.8f;
        C3DVector end = target->GraphicAppearance->GetHelperPointOrCentre(1.0f);
        unsigned long startKey = Calc_004014a0(0, "LIGHTNING_START", 15);
        NParticleEngine::SVectorParam startParam(startKey, start);
        unsigned long endKey = Calc_004014a0(0, "LIGHTNING_END", 13);
        NParticleEngine::SVectorParam endParam(endKey, end);
        NParticleEngine::CParticleUpdateParameters parameters;
        parameters.VectorParams.push_back(startParam);
        parameters.VectorParams.push_back(endParam);
        parameters.Update();
        emitter->SetEmitterUpdateParameters(parameters);
    }
}
