// CTCWillLightning::MakeRandomBeam(bool) -- fire a temporary main beam from the caster in a random direction: rotate the caster's forward by random angles about its side and up axes, scale it to the area-effect range, offset it from the caster, activate the beam and keep it in the beam list
// Leaf: ?MakeRandomBeam@CTCWillLightning
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A1870, 676 bytes (+ int3 padding), `ret 4` (the bool is unused).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50; the caster physics' right-handed set (virtual +0x120);
//   operator new 0x00BFEA1A; CLightningBeam(CThing*, CThing*) 0x007A0630; the counted-pointer deleter 0x007A4120;
//   CTCBase::GetWorld 0x00686A70; CWorld::GetPRandomSeed 0x0049D810; GFFloatRandom(float, unsigned long*) 0x004978A0
//   (`__fastcall`); C3DVector::Rotate(const C3DVector&, float) 0x00A13C60; CThing::GetPosition 0x004C73D0;
//   CLightningBeam::Activate 0x007A1380; malloc 0x00BFEA0E (list node); operator delete 0x00BFE9BC. 1/360 0x01238E00.
// Layout (retail CTCWillLightning, as the landed InitialiseBeams 0x007A16F0): m_lspDef +0x28, m_lbBeams +0x68,
//   m_cbpPlayer +0x70, BeamsInitialised +0x95, m_fAreaEffect +0x9c, m_fActivationTime +0xa8, m_fDeActivationTime +0xac,
//   m_fTemporaryBeamLife +0xc0; def angle ranges +0xcc / +0xd4; CLightningBeam (0x7c, landed ctor 0x007A0630).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Rotate(const C3DVector& axis, float angle);   // 0x00A13C60
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    const float* pa = &a.X; const float* pb = &b.X;
    return C3DVector((float)(double)(pb[2] * pa[1] - pa[2] * pb[1]), (float)(double)(*(&b.Y - 1) * pa[2] - pa[0] * pb[2]), (float)(double)(pb[1] * pa[0] - pa[1] * *(&b.Y - 1)));
}
class CRightHandedSet { public: C3DVector Up, Forward; };
class CFloatRange { public: float Min, Max; };
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

class CTCPhysicsBase { public:
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
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual const CRightHandedSet& GetRHSet() const;   // slot 72 (+0x120)
};
class CThing { public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    const C3DVector& GetPosition() const;       // 0x004C73D0
    const CRightHandedSet& GetRHSet() const { return PhysicsTC->GetRHSet(); }
};
struct CBaseIntelligentPointer {
    void* _vfptr;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
class CWorld { public:
    unsigned long* GetPRandomSeed();            // 0x0049D810
};

void operator delete(void* p);                  // 0x00BFE9BC
#include <stdlib.h>

class CCPPointerInfo { public:                         // counted-pointer info block
    long Count;                                 // +0
    void (__fastcall* Destroy)(void*);          // +4 per-type deleter, object in ecx
    void* Object;                               // +8
    CCPPointerInfo(void (__fastcall* destroy)(void*), void* object) : Count(1), Destroy(destroy), Object(object) { }
};
template <class T> class CCountedPointer { public:
    T* Data;                                    // +0
    CCPPointerInfo* Info;                       // +4
    static void __fastcall DestroyObject(void* object);   // 0x007A4120
    CCountedPointer(T* data) : Data(data) { Info = data ? new CCPPointerInfo(&DestroyObject, data) : 0; }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->Count; }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->Count == 0) {
                Info->Destroy(Info->Object);
                delete Info;
            }
        }
    }
    T* operator->() const { return Data; }
};

inline void* operator new(unsigned int, void* p) { return p; }
template <class T> struct CRetailList {          // STLport list: the sentinel node pointer only
    struct Node { Node* Next; Node* Prev; T Value; };
    Node* Head;
    void push_back(const T& value)
    {
        Node* position = Head;
        Node* node = (Node*)malloc(sizeof(Node));
        new (&node->Value) T(value);
        Node* prev = position->Prev;
        node->Next = position;
        node->Prev = prev;
        prev->Next = node;
        position->Prev = node;
    }
};

struct CDefNative {
    virtual void v00();
    virtual void Delete() const;
    unsigned long IVRefCount;
};
class CSpecialAbilitiesLightningSpellDef : public CDefNative { public:
    unsigned char _pad08[0xcc - 0x08];
    CFloatRange RandomBeamYawRange;             // +0xcc (descriptive)
    CFloatRange RandomBeamPitchRange;           // +0xd4 (descriptive)
};
struct CDefPointer {
    CSpecialAbilitiesLightningSpellDef* Object;
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                CDefNative* old = Object;
                if (--old->IVRefCount == 0)
                    old->Delete();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
    CSpecialAbilitiesLightningSpellDef* operator->() const { return Object; }
};

struct CLightningBeam;
typedef CRetailList<CCountedPointer<CLightningBeam> > CBeamList;
struct CLightningBeam {
    long State;                                 // +0x00
    CBaseIntelligentPointer ParticleHandle;     // +0x04
    CBaseIntelligentPointer Home;               // +0x0c
    CBaseIntelligentPointer Target;             // +0x14
    C3DVector Start, End, Destination;          // +0x1c
    float DamagePerHit, Time, TimeBase;         // +0x40
    CBeamList Children;                         // +0x4c
    bool Temporary;                             // +0x50
    bool MainBeam;                              // +0x51
    bool RightHand;                             // +0x52
    float ActivationTime;                       // +0x54
    float DeactivationTime;                     // +0x58
    float Life;                                 // +0x5c
    float DecapitationTime;                     // +0x60
    float DecapitationClock;                    // +0x64
    float Delay;                                // +0x68
    long OnhitEveryFrame;                       // +0x6c
    long FrameCount;                            // +0x70
    CDefPointer PDef;                           // +0x74
    bool SuccessfulHit;                         // +0x78
    CLightningBeam(CThing* home, CThing* target);   // 0x007A0630
    void Activate();                            // 0x007A1380
};

class CTCWillLightning { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x0c - 0x08];
    long  WeaponState;                          // +0x0c
    bool  LeftHandAnim;                         // +0x10
    unsigned char _pad11[0x28 - 0x11];
    CDefPointer m_lspDef;                       // +0x28
    unsigned char _pad2c[0x68 - 0x2c];
    CBeamList m_lbBeams;                        // +0x68
    long  m_lsState;                            // +0x6c
    CBaseIntelligentPointer m_cbpPlayer;        // +0x70
    unsigned char _pad78[0x95 - 0x78];
    bool  BeamsInitialised;                     // +0x95
    unsigned char _pad96[0x9c - 0x96];
    float m_fAreaEffect;                        // +0x9c
    float m_fDamagePerHit;                      // +0xa0
    float m_fSubBeamDamagePerHit;               // +0xa4
    float m_fActivationTime;                    // +0xa8
    float m_fDeActivationTime;                  // +0xac
    long  m_nMinMainBeams;                      // +0xb0
    long  m_nMaxMainBeams;                      // +0xb4
    long  m_nMinSubBeams;                       // +0xb8
    long  m_nMaxSubBeams;                       // +0xbc
    float m_fTemporaryBeamLife;                 // +0xc0
    CWorld* GetWorld();                         // 0x00686A70 (CTCBase)
    float GetRandomInRange(CFloatRange range) { return GFFloatRandom(range.Max - range.Min, GetWorld()->GetPRandomSeed()) + range.Min; }
    bool IsRightHanded() const
    {
        switch (WeaponState) {
        case 0:
            return !LeftHandAnim;
        case 1:
            return false;
        case 2:
            return true;
        }
        return true;
    }
    void MakeRandomBeam(bool unused);
};

void CTCWillLightning::MakeRandomBeam(bool unused)
{
    BeamsInitialised = false;
    CRightHandedSet set = m_cbpPlayer.GetPItem()->GetRHSet();
    CCountedPointer<CLightningBeam> beam(new CLightningBeam(m_cbpPlayer.GetPItem(), 0));
    beam->ActivationTime = m_fActivationTime;
    beam->DeactivationTime = m_fDeActivationTime;
    beam->PDef = m_lspDef;
    beam->Destination = set.Forward;
    float yaw = GetRandomInRange(m_lspDef->RandomBeamYawRange);
    C3DVector side = CrossProduct(set.Forward, set.Up);
    beam->Destination.Rotate(side, yaw * (1.0f / 360.0f));
    float pitch = GetRandomInRange(m_lspDef->RandomBeamPitchRange);
    beam->Destination.Rotate(set.Up, pitch * (1.0f / 360.0f));
    C3DVector& destination = beam->Destination;
    destination *= m_fAreaEffect;
    destination += m_cbpPlayer.GetPItem()->GetPosition();
    beam->Life = m_fTemporaryBeamLife;
    beam->Temporary = true;
    beam->MainBeam = true;
    beam->RightHand = IsRightHanded();
    beam->Activate();
    m_lbBeams.push_back(beam);
}