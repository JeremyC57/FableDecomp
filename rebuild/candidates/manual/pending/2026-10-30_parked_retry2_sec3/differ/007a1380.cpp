// Grade: DIFFER(864v870) -- same-TU LowerBound stand-in dropped (declaration only, identical bytes with the ternary GetInterface) and the CRC callee respelled CCRC::Calc (static __fastcall, 0x004014A0); residue: retail spills the emitter to a frame word shared with the LowerBound key (reloaded for SetEmitterUpdateParameters) and keeps both CRC keys in edi/ebx; ours keeps the emitter in ebx with a 4-byte larger frame
// CTCWillLightning::CLightningBeam::Activate() -- start a beam
// Leaf: Activate
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A1380, 870 bytes (+ int3 padding).
struct C3DVector { float X, Y, Z; };
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() {} };
struct CCharString {
    void* PStringData;
    ~CCharString();                                   // 0x0099EAE0
};
struct CDefString { long Index; };
struct CDefStringTable {
    CCharString GetString(CDefString name);           // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;              // 0x013CA828
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    const C3DVector& GetPosition() const;       // 0x004C73D0             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

struct CCRC {
    static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
};
CThing* __fastcall CreateParticleEffect(long type, const C3DVector& position);   // 0x0072AB70
extern long g_LightningBeamEffect;               // 0x013BAE10
extern long g_LightningMainBeamEffect;           // 0x013BAE14
extern CCharString g_RightHandAttachmentName;    // 0x013BAC30
extern CCharString g_LeftHandAttachmentName;     // 0x013BAC34
extern CCharString g_FocusEyeAttachmentName;     // 0x013BAC48 "focus_eye_01"
namespace NParticleEngine {
struct SVectorParam {                            // pair<unsigned long, C3DVector>
    unsigned long m_key;
    C3DVector     m_value;
    SVectorParam(const unsigned long& key, const C3DVector& value) : m_key(key), m_value(value) {}
};
struct CVectorParamVector {                      // retail STLport vector (12 bytes)
    SVectorParam* _M_start;
    SVectorParam* _M_finish;
    SVectorParam* _M_end_of_storage;
    void _M_insert_overflow(SVectorParam* pos, const SVectorParam& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x0076ADA0
    __forceinline void push_back(const SVectorParam& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) SVectorParam(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CVectorParamMap {
    CVectorParamVector Vector;                   // +0x00
    char               Compare;                  // +0x0c
    bool               Dirty;                    // +0x0d
    __forceinline void insert(const SVectorParam& x) { Vector.push_back(x); Dirty = true; }
};
struct CParticleUpdateParameters {               // <unsigned long, unsigned long>, vtable 0x0125FFDC
    void*           vptr;
    CVectorParamMap VectorParams;                // +0x04
    unsigned char   UpdateParams[0x10];          // +0x14
    CParticleUpdateParameters();                 // 0x006E24D0
    ~CParticleUpdateParameters();                // 0x006E2500
    void Finalise();                             // 0x0076C750 (descriptive name)
};
}
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, long flags);   // 0x006E0BE0
    C3DVector GetEmitterPosition() const;        // 0x006E0620 (descriptive name)
    void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters& parameters);   // 0x006E11A0
};
struct CTCWillLightning {
    void ResumeForCutscene();                    // 0x007A0400
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                               // +4
    CBaseIntelligentPointer();                   // 0x00A01B10
    ~CBaseIntelligentPointer();                  // 0x00A01C10
    CThing* GetPItem() const;                    // 0x00A01B50
    void SetPItem(CThing* item);                 // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x004CC340 (out-of-line copy)
struct CLightningBeam {
    long                    m_lsState;           // +0x00 ELightningState
    CIntelligentPointer<CThing> m_tpParticleHandle;   // +0x04
    CIntelligentPointer<CThing> m_tcpHome;       // +0x0c
    CIntelligentPointer<CThing> m_tcpTarget;     // +0x14
    C3DVector               m_vStartPos;         // +0x1c
    C3DVector               m_vEndPos;           // +0x28
    C3DVector               m_vDestination;      // +0x34
    float                   m_fDamagePerHit;     // +0x40
    float                   m_fTime;             // +0x44
    float                   m_fTimeBase;         // +0x48
    void*                   m_lChildren;         // +0x4c (STLport list node)
    bool                    m_bTemporary;        // +0x50
    bool                    m_bMainBeam;         // +0x51
    bool                    m_bRightHand;        // +0x52
    float                   m_fActivationTime;   // +0x54
    float                   m_fDeActivationTime; // +0x58
    float                   m_fLife;             // +0x5c
    float                   m_fDecapitationTime; // +0x60
    float                   m_fDecapitationClock;   // +0x64
    float                   m_fDelay;            // +0x68
    void Activate();
};

void CLightningBeam::Activate()
{
    if (m_lsState == 0 || m_lsState == 5) {
        if (m_fDelay == 0.0f) {
            m_fTimeBase = m_fTime;
            m_lsState = 2;
            CTCWillLightning* lightning;
            if (IS_THING_ALIVE(m_tcpHome) && m_tcpHome.GetPItem()->GetInterface(&lightning, 0x94))
                lightning->ResumeForCutscene();
        } else {
            m_fTimeBase = m_fTime;
            m_lsState = 1;
        }
        if (IS_THING_ALIVE(m_tcpHome)) {
            if (!IS_THING_ALIVE(m_tpParticleHandle)) {
                CThing* effect;
                if (m_bMainBeam)
                    effect = CreateParticleEffect(g_LightningMainBeamEffect, m_tcpHome.GetPItem()->GetPosition());
                else
                    effect = CreateParticleEffect(g_LightningBeamEffect, m_tcpHome.GetPItem()->GetPosition());
                m_tpParticleHandle = CIntelligentPointer<CThing>(effect);
                if (IS_THING_ALIVE(m_tpParticleHandle)) {
                    CTCDParticleEmitter* emitter = 0;
                    m_tpParticleHandle.GetPItem()->GetInterface(&emitter, 0x4b);
                    if (m_bMainBeam) {
                        if (m_bRightHand)
                            emitter->AttachToThing(m_tcpHome.GetPItem(), 10, g_RightHandAttachmentName, 0, 0);
                        else
                            emitter->AttachToThing(m_tcpHome.GetPItem(), 10, g_LeftHandAttachmentName, 0, 0);
                    } else {
                        emitter->AttachToThing(m_tcpHome.GetPItem(), 12, g_FocusEyeAttachmentName, 0, 0);
                    }
                    m_vStartPos = emitter->GetEmitterPosition();
                    if (IS_THING_ALIVE<CIntelligentPointer<CThing> >(m_tcpTarget))
                        m_vEndPos = m_tcpTarget.GetPItem()->GetPosition();
                    NParticleEngine::SVectorParam start(CCRC::Calc(0, "LIGHTNING_START", 15), m_vStartPos);
                    NParticleEngine::SVectorParam end(CCRC::Calc(0, "LIGHTNING_END", 13), m_vStartPos);
                    NParticleEngine::CParticleUpdateParameters parameters;
                    parameters.VectorParams.insert(start);
                    parameters.VectorParams.insert(end);
                    parameters.Finalise();
                    emitter->SetEmitterUpdateParameters(parameters);
                }
            }
        }
    }
}
