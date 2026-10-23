// Grade: DIFFER(700v696) -- 160 difflines: this/effect in edi/esi (retail esi/edi) and the end particle kept in ebx (retail spills it to the dead particleType slot and keeps the start emitter in ebx); `or byte ptr [ebp+0x41],2` read-modify-write where ours loads/ors/stores (the GOTCHAS 0x713740 lane gap; plain byte, bool and dword bitfields, a member reference and #pragma optimize("s") all tried). The x87 distance/ratio, the extern 1e-4 constant through a max() reference and both GetInterface lookups already match.
// CTCObjectAugmentations::AttemptToStartParticle(CIntelligentPointer<CThing>& effect, long particleType, long endParticleType, const CCharString& startDummy, const CCharString& endDummy) -- reset the effect; when both dummies resolve (helper point, else dummy object) on the owner's appearance and the particle type is set, create the effect and an end-point particle at the owner, make the effect's emitter interpolate towards the end emitter scaled by the dummies' distance against the spline lengths, attach the effect at the start dummy and kill the end particle
// Leaf: AttemptToStartParticle
// Retail: Fable.exe (TLC, VC7.1) @ 0x00766660, 693 bytes, `ret 14h`.
#include <math.h>
struct CCharString { void* PStringData; };
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline float Length() const { return sqrtf(X * X + Y * Y + Z * Z); }
};
struct CRightHandedSet { C3DVector Forward, Up; };

class CThing;
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

namespace NParticleEngine {
class CParticleEmitter {
public:
    unsigned char _pad00[0x41];
    unsigned char Flags41;                      // +0x41
    unsigned char _pad42[2];
    unsigned char Flags44_0 : 4;                // +0x44
    unsigned char Flags44_4 : 1;
    float GetApproximateTotalSplineLength();    // 0x00AED050
    void InterpolateTo(CParticleEmitter* target, float t);   // 0x00AED600
};
}
using NParticleEngine::CParticleEmitter;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void* GetPItem() const;                     // 0x00A01B50
    void SetPItem(void* item);                  // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C (CThing)
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        void* item = other.CBaseIntelligentPointer::GetPItem();
        SetPItem(item);
        return *this;
    }
};

class CTCDParticleEmitter {
public:
    unsigned char _pad00[0x14];
    CIntelligentPointer<CParticleEmitter> PEmitter;   // +0x14
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
    static CThing* __fastcall Create(long particleIndex, const C3DVector& position, bool flag);   // 0x006E0880
};

class CTCGraphicAppearance {
public:
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* pos, float scale);   // 0x004C0560
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* pos, CRightHandedSet* rh, float scale);   // 0x004C08C0
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* PTCGraphicAppearance; // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char m_flags91;                    // +0x91 bit 0 dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    void Kill(bool immediate);                  // 0x004C9B80
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

extern const float g_MinSplineRange;            // 0x0129BA3C (1.0e-4f)
template <class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }

struct CTCObjectAugmentations {
    virtual ~CTCObjectAugmentations();
    CThing* ParentThing;                        // +0x04
    void AttemptToStartParticle(CIntelligentPointer<CThing>& effect, long particleType, long endParticleType,
                                const CCharString& startDummy, const CCharString& endDummy);
};

void CTCObjectAugmentations::AttemptToStartParticle(CIntelligentPointer<CThing>& effect, long particleType, long endParticleType,
                                                    const CCharString& startDummy, const CCharString& endDummy)
{
    effect = CIntelligentPointer<CThing>(0);
    C3DVector startPos;
    C3DVector endPos;
    CRightHandedSet rh;
    if (!ParentThing->PTCGraphicAppearance->GetHelperPoint(startDummy, 0, 0, &startPos, 1.0f) &&
        !ParentThing->PTCGraphicAppearance->GetDummyObject(startDummy, 0, 0, &startPos, &rh, 1.0f))
        return;
    if (!ParentThing->PTCGraphicAppearance->GetHelperPoint(endDummy, 0, 0, &endPos, 1.0f) &&
        !ParentThing->PTCGraphicAppearance->GetDummyObject(endDummy, 0, 0, &endPos, &rh, 1.0f))
        return;
    if (particleType <= 0)
        return;
    effect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(particleType, ParentThing->GetPosition(), true));
    CThing* endParticle = CTCDParticleEmitter::Create(endParticleType, ParentThing->GetPosition(), true);
    if (effect.GetPItem() && !(effect.GetPItem()->m_flags91 & 1) && endParticle && !(endParticle->m_flags91 & 1)) {
        CTCDParticleEmitter* startEmitter = 0;
        CTCDParticleEmitter* endEmitter = 0;
        effect.GetPItem()->GetInterface(&startEmitter, 0x4b);
        endParticle->GetInterface(&endEmitter, 0x4b);
        CParticleEmitter* startSystem = startEmitter->PEmitter.GetPItem();
        CParticleEmitter* endSystem = endEmitter->PEmitter.GetPItem();

        endSystem->Flags41 |= 2;
        float distance = (startPos - endPos).Length();
        float endLength = endSystem->GetApproximateTotalSplineLength();
        float startLength = startSystem->GetApproximateTotalSplineLength();
        float range = endLength - startLength;
        startSystem->InterpolateTo(endSystem, (distance - startLength) / Max(range, g_MinSplineRange) * 2.0f);
        startSystem->Flags44_4 = 1;
        startEmitter->AttachToThing(ParentThing, 10, startDummy, 0, 0.0f);
        endParticle->Kill(true);
    }
}
