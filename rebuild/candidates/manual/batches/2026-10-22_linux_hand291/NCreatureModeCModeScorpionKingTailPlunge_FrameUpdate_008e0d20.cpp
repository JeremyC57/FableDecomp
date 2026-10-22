// NCreatureMode::CModeScorpionKingTailPlunge::FrameUpdate() -- base mode frame update; count the recovery timer down; at the
//   spike-decision frame lock the target position (the live target, pulled in to 6 units when closer than 6 in 2D; else 8 units
//   ahead of the creature) and reset the dust counter; at the spike frame Spike() and kill the dust trail; once recovered and the
//   animation has finished (empty queue or flag bit 1) Recover(); between the two frames slide the dust trail emitter one
//   frame-step from the tail towards the target
// Leaf: ?FrameUpdate@CModeScorpionKingTailPlunge
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E0D20, 953 bytes (`ret`).
// Callees: CModeBase::FrameUpdate 0x0073AC50, CVectorMap::LowerBound 0x0040F020 (inline GetTC 0x5a, unchecked miss path),
//   CTCAnimationComplex::GetAnimation 0x0070B460, CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPosition 0x004C73D0,
//   CThing::Kill 0x004C9B80, Spike 0x008DA0F0, Recover 0x008DF510 (makes TAIL_DUST_EFFECT), driver slots 30 (MoveTo) / 70 (GetForward).
// Data: the FastInvSqrt mantissa table 0x013CE558, floats 6.0 0x0125A2A0, 3.0 0x0122DED4, 0.5 0x0122F59C, 8.0 0x0125916C, 1.0 0x0122DED8.
// Layout: Ego_r.pdb CModeScorpionKingTailPlunge (packed, as the landed Spike 0x008DA0F0 / OnActivate 0x008D9E30); thing driver +0x60,
//   dead flag +0x91 bit 0; CAnimateThing queue begin/end +0x04/+0x08, flag byte +0x51.
// Shape: the near-target pull-in normalises the delta IN PLACE (forceinline Normalise, so the direction reuses delta's frame words);
//   the dust step is `delta / (float)frames` through an inline operator/ that multiplies by 1/s (fst + fmul st(2)); the dust
//   emitter's driver is read through a forceinline accessor so it lands in ecx before the push.
#include <math.h>
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    __forceinline void Normalise() { float inv = FastInvSqrt(X * X + Y * Y + Z * Z); X *= inv; Y *= inv; Z *= inv; }
    __forceinline C3DVector operator/(float s) const { return *this * (1.0f / s); }
    float GetLength2D() const { return sqrtf(X * X + Y * Y); }
};
__forceinline C3DVector ScaleRef(const C3DVector& v, const float& s) { return C3DVector(s * v.X, s * v.Y, s * v.Z); }
struct CTCDriverBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void MoveTo(const C3DVector& position);   // slot 30 (+0x78)
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetForward();   // slot 70 (+0x118)
};
struct CAnimateThing {
    void* _vt;
    void* AnimBegin;                            // +0x04
    void* AnimEnd;                              // +0x08
    unsigned char _pad0c[0x51 - 0x0c];
    unsigned char Flags51;                      // +0x51 bit 1
};
struct CTCAnimationComplex {
    CAnimateThing* GetAnimation();              // 0x0070B460
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CTCDriverBase* PTCDriver;                   // +0x60
    unsigned char  _pad64[0x91 - 0x64];
    unsigned char  m_flags91;                   // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    void Kill(bool immediately);
    __forceinline CTCDriverBase* GetDriver() const { return PTCDriver; }                // 0x004C9B80
    __forceinline bool HasTC(long interfaceType) const
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
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CThing* GetPItem() const;                         // 0x00A01B50
};
struct CIntelligentPointerThing : CBaseIntelligentPointer { };

struct CModeBase {
    virtual ~CModeBase();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x14];
    void FrameUpdate();                         // 0x0073AC50
};
namespace NCreatureMode {
#pragma pack(push, 1)
struct CModeScorpionKingTailPlunge : CModeBase {
    unsigned char       _pad1c[0x68 - 0x1c];
    long                RecoveryCount;          // +0x68
    long                DustEffectCounter;      // +0x6c
    long                SpikeTime;              // +0x70
    long                SpikeDecisionTime;      // +0x74
    unsigned char       Filter[0xc];            // +0x78
    void*               PCreature;              // +0x84
    bool                bRecovering;            // +0x88
    C3DVector           TargetPos;              // +0x89
    C3DVector           TailPos;                // +0x95
    CIntelligentPointerThing PTarget;           // +0xa1
    CIntelligentPointerThing PTailEmitter;      // +0xa9
    CIntelligentPointerThing PDustTrailEmitter; // +0xb1
    void FrameUpdate();
    void Spike();                               // 0x008DA0F0
    void Recover();                             // 0x008DF510
};
#pragma pack(pop)

void CModeScorpionKingTailPlunge::FrameUpdate()
{
    CModeBase::FrameUpdate();
    CTCAnimationComplex* animComplex = 0;
    if (ParentThing->HasTC(0x5a))
        animComplex = ParentThing->GetTC<CTCAnimationComplex>(0x5a);
    CAnimateThing* animation = animComplex->GetAnimation();
    if (RecoveryCount)
        --RecoveryCount;
    if (RecoveryCount == SpikeDecisionTime) {
        if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1)) {
            TargetPos = PTarget.GetPItem()->GetPosition();
            C3DVector delta = TargetPos - ParentThing->GetPosition();
            if (delta.GetLength2D() < 6.0f) {
                delta.Normalise();
                TargetPos = ParentThing->GetPosition() + delta * 6.0f;
            }
        } else {
            TargetPos = ParentThing->GetPosition() + ParentThing->PTCDriver->GetForward() * 8.0f;
        }
        DustEffectCounter = 0;
    } else if (RecoveryCount == SpikeTime) {
        Spike();
        if (PDustTrailEmitter.GetPItem() && !(PDustTrailEmitter.GetPItem()->m_flags91 & 1))
            PDustTrailEmitter.GetPItem()->Kill(true);
    } else if (RecoveryCount <= 0) {
        if (animation->AnimBegin == animation->AnimEnd || (animation->Flags51 & 2)) {
            Recover();
            return;
        }
    }
    if (RecoveryCount < SpikeDecisionTime && RecoveryCount > SpikeTime &&
        PDustTrailEmitter.GetPItem() && !(PDustTrailEmitter.GetPItem()->m_flags91 & 1)) {
        C3DVector step = (TargetPos - TailPos) / (float)(SpikeDecisionTime - SpikeTime);
        C3DVector position = PDustTrailEmitter.GetPItem()->GetPosition() + step;
        PDustTrailEmitter.GetPItem()->GetDriver()->MoveTo(position);
    }
}
}
