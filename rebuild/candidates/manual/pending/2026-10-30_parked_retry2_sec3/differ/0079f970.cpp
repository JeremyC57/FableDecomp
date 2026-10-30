// Grade: DIFFER(1136v1052) 188 difflines (wave 2 sec3: first reconstruction from the disassembly; was the 8-byte stub, 348 lines)
// Tried: g1 UpdateDestination as a plain inline + user operator= (648v1052/387: not inlined, fstp straight to members);
//   g2 __forceinline UpdateDestination + implicit C3DVector assignment (this file, 1136v1052/188: the whole switch shape,
//   both lerps and the nested alive checks of cases 3/4 line up); g3 one appearance fetch + `emitter = 0; if (HasTC)`
//   (836v1052/213: retail's `xor ebp,ebp` but VC dropped one inlined UpdateDestination; source in the STATUS note).
// Next: retail tail-merges case 1 into case 3's `lea eax,[esi+34h]` copy (we duplicate the copy), loads the appearance
//   once (`mov ecx,[eax+64h]` kept for the call) and keeps the emitter in ebp (xor before the HasTC test); try the g3
//   appearance local alone, then a `const C3DVector* src` shared copy for cases 1/3.
// CTCWillLightning::CLightningBeam::UpdatePositions() -- refresh the beam's start and end points: the start follows the
//   particle emitter when the home thing is alive; the end point by state (1 start, 2 grow toward the destination over
//   the activation time, 3/4 the refreshed destination, 5 shrink back over the deactivation time)
// Leaf: UpdatePositions
// Name: Ego_r.pdb ?UpdatePositions@CLightningBeam@CTCWillLightning@@QAEXXZ.
// Layout: Ego_r.pdb CLightningBeam with the STLport 4-byte list (PDB 12): m_lChildren +0x4C, m_bTemporary +0x50,
//   m_fActivationTime +0x54, m_fDeActivationTime +0x58 (the ctor 0x007A0630 stores bytes +0x50/+0x51/+0x52).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap LowerBound 0x0040F020,
//   CTCDParticleEmitter::GetEmitterPosition 0x006E0620, IS_THING_ALIVE 0x004CC340 (__fastcall), CThing::GetPos
//   0x004C73D0, NGraphicAppearance::CTCGraphicAppearance::GetHelperPoint 0x004C0560 on g_helperFocal_013bac48.
class CTCBase;
class CBaseObject;
class CBaseObjectPointer;
class CCharString { public: void* PData; };
extern const CCharString g_helperFocal_013bac48;

class C3DVector {
public:
    float X, Y, Z;
    __forceinline C3DVector() { }
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    __forceinline C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    __forceinline C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    __forceinline C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    __forceinline C3DVector operator/(float f) const { float r = 1.0f / f; return C3DVector(X * r, Y * r, Z * r); }
};

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                       // 0x00A01B50
private:
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                // 0x0040F020
};
namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector& pos, float time) const; // 0x004C0560
};
}
class CTCDParticleEmitter {
public:
    C3DVector GetEmitterPosition() const;                // 0x006E0620
};
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                      // +0x20
    CInterfaceMap m_interfaces;                          // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance; // +0x64 (PDB +0x74)
    unsigned char _pad68[0x91 - 0x68];
    unsigned char m_flags91;                             // +0x91 bit 0: dead
    const C3DVector& GetPos() const;                     // 0x004C73D0
    NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance() const { return PTCGraphicAppearance; }
    unsigned long HasTC(long t) const { return m_presentBits[t >> 5] & (1UL << (t & 31)); }
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
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<CThing>& p);   // 0x004CC340

class CLightningBeam {
public:
    long                        m_lsState;               // +0x00 ELightningState
    CIntelligentPointer<CThing> m_tpParticleHandle;      // +0x04
    CIntelligentPointer<CThing> m_tcpHome;               // +0x0C
    CIntelligentPointer<CThing> m_tcpTarget;             // +0x14
    C3DVector                   m_vStartPos;             // +0x1C
    C3DVector                   m_vEndPos;               // +0x28
    C3DVector                   m_vDestination;          // +0x34
    float                       m_fDamagePerHit;         // +0x40
    float                       m_fTime;                 // +0x44
    float                       m_fTimeBase;             // +0x48
    void*                       m_lChildren;             // +0x4C (STLport list node)
    bool                        m_bTemporary;            // +0x50
    bool                        m_bMainBeam;             // +0x51
    bool                        m_bRightHand;            // +0x52
    float                       m_fActivationTime;       // +0x54
    float                       m_fDeActivationTime;     // +0x58

    __forceinline void UpdateDestination()
    {
        if (!m_bTemporary && IS_THING_ALIVE(m_tcpHome) && IS_THING_ALIVE(m_tcpTarget)) {
            C3DVector pos = m_tcpTarget.GetPItem()->GetPos();
            if (m_tcpTarget.GetPItem()->GetTCGraphicAppearance())
                m_tcpTarget.GetPItem()->GetTCGraphicAppearance()->GetHelperPoint(g_helperFocal_013bac48, 0, 0, pos, 1.0f);
            m_vDestination = pos;
        }
    }
    void UpdatePositions();
};

void CLightningBeam::UpdatePositions()
{
    C3DVector pos(0.0f, 0.0f, 0.0f);
    if (m_tpParticleHandle.GetPItem() && !(m_tpParticleHandle.GetPItem()->m_flags91 & 1)) {
        CThing* particle = m_tpParticleHandle.GetPItem();
        CTCDParticleEmitter* emitter = particle->HasTC(0x4b) ? particle->GetTC<CTCDParticleEmitter>(0x4b) : 0;
        pos = emitter->GetEmitterPosition();
    }
    if (m_tcpHome.GetPItem() && !(m_tcpHome.GetPItem()->m_flags91 & 1))
        m_vStartPos = pos;

    switch (m_lsState) {
    case 1:
        m_vEndPos = m_vStartPos;
        break;
    case 2:
        UpdateDestination();
        m_vEndPos = (m_vDestination - m_vStartPos) * (m_fTime - m_fTimeBase) / m_fActivationTime + m_vStartPos;
        break;
    case 3:
        if (!m_bTemporary && IS_THING_ALIVE(m_tcpHome) && IS_THING_ALIVE(m_tcpTarget))
            UpdateDestination();
        m_vEndPos = m_vDestination;
        break;
    case 4:
        if (!m_bTemporary && IS_THING_ALIVE(m_tcpTarget) && IS_THING_ALIVE(m_tcpHome))
            UpdateDestination();
        m_vEndPos = m_vDestination;
        break;
    case 5:
        m_vEndPos = (m_vDestination - m_vStartPos) * (m_fDeActivationTime - (m_fTime - m_fTimeBase)) / m_fDeActivationTime + m_vStartPos;
        break;
    }
}
