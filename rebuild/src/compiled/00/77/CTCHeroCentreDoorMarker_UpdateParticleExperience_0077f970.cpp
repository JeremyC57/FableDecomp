// CTCHeroCentreDoorMarker::UpdateParticleExperience() -- keep the "EXPERIENCE_SPEND" particle on the door marker in step with the world's experience-spending switch: kill it when spending is disabled, create and attach it when spending is enabled and no live particle exists
// Leaf: UpdateParticleExperience
//
// Retail 0x0077F970, 395 bytes (+5 int3 padding). Called from CTCHeroCentreDoorMarker::FrameUpdate @ 0x0077FB00 for door type 1.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCBase::GetWorld @ 0x00686A80 (twice, one per arm), CThing::Kill(bool)
// @ 0x004C9B80, CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10, CCharString(const char*, long)
// @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CThing::GetPosition @ 0x004C73D0, the `__fastcall` effect factory
// Create(const CCharString&, const C3DVector&, bool) @ 0x006E0970, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020
// (inline GetTC for the particle emitter, interface 0x4b), CTCDParticleEmitter::AttachToThing @ 0x006E0BE0.
// Shape: two arms on IS_THING_ALIVE(PParticle); `PParticle = CIntelligentPointer<CThing>(x);` builds/assigns/destroys a frame
// temporary (no user dtor on CIntelligentPointer); `T* emitter = 0; if (HasTC) emitter = GetTC;` gives the `xor edi,edi` preload,
// HasTC returns the raw masked word (`test ch,8` on +0x28).
// Layout (packed, landed 0x0077F5C0): PParticle +0x15; retail CWorld ExperienceSpendingEnabled +0xd6 (PDB +0xe2, three 16-byte
// PDB vectors are 12 bytes in retail); CThing interface bitset +0x20, interface map +0x44, dead bit 0 of +0x91.
class C3DVector { public: float X, Y, Z; };
class CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                             // +0x91 bit 0
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void Kill(bool b);                                  // 0x004C9B80
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
CThing* __fastcall Create(const CCharString& defName, const C3DVector& position, bool flag);   // 0x006E0970
class CTCDParticleEmitter {
public:
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    virtual void v00();
    void* PData;
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { SetPItem(o.GetPItem()); return *this; }
    virtual void v00();
};
template <class P> inline bool IS_THING_ALIVE(const P& p) { return p.GetPItem() && !p.GetPItem()->Dead; }

class CWorld { public:
    unsigned char _pad00[0xd6];
    bool ExperienceSpendingEnabled;                     // +0xd6
    bool SavingEnabled;                                 // +0xd7
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CWorld* GetWorld() const;                           // 0x00686A80
};
#pragma pack(push, 1)
class CTCHeroCentreDoorMarker : public CTCBase {        // vtable 0x0126B044
public:
    float Radius;                                       // +0x0c
    long  DoorType;                                     // +0x10
    bool  HeroInsideRadius;                             // +0x14
    CIntelligentPointer<CThing> PParticle;              // +0x15
    CIntelligentPointer<CThing> PHeroCentre;            // +0x1d
    void UpdateParticleExperience();
};
#pragma pack(pop)

void CTCHeroCentreDoorMarker::UpdateParticleExperience()
{
    if (IS_THING_ALIVE(PParticle)) {
        if (!GetWorld()->ExperienceSpendingEnabled) {
            PParticle.GetPItem()->Kill(true);
            PParticle = CIntelligentPointer<CThing>(0);
        }
    } else if (GetWorld()->ExperienceSpendingEnabled) {
        PParticle = CIntelligentPointer<CThing>(Create("EXPERIENCE_SPEND", ParentThing->GetPosition(), false));
        if (IS_THING_ALIVE(PParticle)) {
            CTCDParticleEmitter* emitter = 0;
            CThing* particle = PParticle.GetPItem();
            if (particle->HasTC(0x4b))
                emitter = particle->GetTC<CTCDParticleEmitter>(0x4b);
            emitter->AttachToThing(ParentThing, 0x20, "", 0, 0.0f);
        }
    }
}