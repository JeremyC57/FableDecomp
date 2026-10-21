// CTCWillLightning::~CTCWillLightning() -- clear the beam list, send every live hand effect (right, then left) to the end of its particle timeline, then destroy the members and chain to ~CTCBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A07F0, length 339 bytes (plain `ret`; vtable 0x0126C724).
// Callees: list<CCountedPointer<CLightningBeam>>::clear @ 0x007A3910 (manifest "remove_if<...>" is propagated);
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / ~CBaseIntelligentPointer @ 0x00A01C10;
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetTC on interface 0x4B, the particle emitter);
//   CTCDParticleEmitter::EnterTimelineEnd @ 0x006E05F0; the CRT `free` thunk @ 0x00BFEA14 (STLport list node and the
//   timer's vector storage); CTCBase::~CTCBase @ 0x00686830 (manifest "~CChunkedFileChunk" is propagated).
// Layout: engine/CTCWillLightning.h has the PDB offsets; retail is smaller after the timer (its STLport vector is 12
//   bytes, the list 4): PItemRemoved +0x11, m_lspDef +0x28, m_tTimer +0x2C (0x34 bytes: an intelligent pointer at +0x08,
//   a vector at +0x18), m_fTime +0x60, m_fTimeBase +0x64, m_lbBeams +0x68, m_lsState +0x6C, m_cbpPlayer +0x70,
//   m_tpLeftHandEffect +0x78, m_tpRightHandEffect +0x80, PTargetOverride +0x88 (landed Deactivate 0x007A0310 uses the
//   same retail offsets). Local model for that reason.
// Shape: `CTCDParticleEmitter* emitter = 0;` before the GetPItem call gives `xor edi,edi`; the CThing dead flag is bit 0
//   of +0x91; the two hand-effect members stay in ebx/ebp for their destructor calls; the def release is the pointee's
//   inline Release() followed by `Object = 0`.
class CTCBase;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    bool          m_bDead : 1;                  // +0x91 bit 0
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

struct CTCDParticleEmitter {
    void EnterTimelineEnd();                    // 0x006E05F0
};

struct CBaseIntelligentPointer {
    void* _vfptr;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};

extern "C" void __cdecl free(void* block);      // 0x00BFEA14 (CRT import thunk)

struct CBeamNode;
struct CBeamList {                              // STLport list<CCountedPointer<CLightningBeam>>: the sentinel pointer
    CBeamNode* _M_node;
    void clear();                               // 0x007A3910
    ~CBeamList()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};

struct CLongVector {                            // STLport vector of a trivial element
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    ~CLongVector()
    {
        if (_M_start)
            free(_M_start);
    }
};

struct CWillButtonHelperTimer {                 // retail 0x34 bytes
    unsigned char _pad00[0x08];
    CBaseIntelligentPointer m_pThing;           // +0x08
    unsigned char _pad10[0x08];
    CLongVector m_values;                       // +0x18
    unsigned char _pad24[0x10];
};

struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
struct CLightningSpellDefPointer {              // CDefPointer<const CSpecialAbilitiesLightningSpellDef>
    CDefPointee* Object;
    ~CLightningSpellDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};

class CTCBase {
public:
    virtual ~CTCBase();                         // 0x00686830
    CThing* ParentThing;                        // +0x04
    unsigned char _pad_0x08[0x4];
};

#pragma pack(push, 1)
struct CTCWillLightning : CTCBase {             // vtable 0x0126C724
    long  WeaponState;                          // +0x0c
    bool  LeftHandAnim;                         // +0x10
    CBaseIntelligentPointer PItemRemoved;       // +0x11
    unsigned char _pad_0x19[0x3];
    long  AIHoldingDownTimer;                   // +0x1c
    long  PrimaryHandEffect;                    // +0x20
    long  SecondaryHandEffect;                  // +0x24
    CLightningSpellDefPointer m_lspDef;         // +0x28
    CWillButtonHelperTimer m_tTimer;            // +0x2c
    float m_fTime;                              // +0x60
    float m_fTimeBase;                          // +0x64
    CBeamList m_lbBeams;                        // +0x68
    long  m_lsState;                            // +0x6c
    CBaseIntelligentPointer m_cbpPlayer;        // +0x70
    CBaseIntelligentPointer m_tpLeftHandEffect; // +0x78
    CBaseIntelligentPointer m_tpRightHandEffect;// +0x80
    CBaseIntelligentPointer PTargetOverride;    // +0x88
    virtual ~CTCWillLightning();
};
#pragma pack(pop)

CTCWillLightning::~CTCWillLightning()
{
    m_lbBeams.clear();
    if (m_tpRightHandEffect.GetPItem() && !m_tpRightHandEffect.GetPItem()->m_bDead) {
        CTCDParticleEmitter* emitter = 0;
        CThing* effect = m_tpRightHandEffect.GetPItem();
        if (effect->HasTC(0x4b))
            emitter = effect->GetTC<CTCDParticleEmitter>(0x4b);
        emitter->EnterTimelineEnd();
    }
    if (m_tpLeftHandEffect.GetPItem() && !m_tpLeftHandEffect.GetPItem()->m_bDead) {
        CTCDParticleEmitter* emitter = 0;
        CThing* effect = m_tpLeftHandEffect.GetPItem();
        if (effect->HasTC(0x4b))
            emitter = effect->GetTC<CTCDParticleEmitter>(0x4b);
        emitter->EnterTimelineEnd();
    }
}
