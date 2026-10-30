// Grade: DIFFER(773v773) -- this sweep: fetching the particle thing before zeroing the emitter makes all three inlined CreateParticle blocks allocate alike (diff 150 -> 24 lines); left: the emitter lives in esi (retail ebx) and its zeroing follows the GetPItem call (retail precedes it); tried thing before the test (779), plain inline (not inlined), pointer particle parameter (same as parked)
// CTCFishingSpot::FrameUpdate() -- while enabled, (re)creates the two ripple particles and the underwater fish particle and attaches them to the spot; otherwise removes them
class CBaseObject;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
    CBaseObject* PItem;
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    inline CIntelligentPointer() {}
    inline CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    inline CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
    T* Get() const { return (T*)GetPItem(); }
};
class CCharString {
public:
    void* Data;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct C3DVector { float X, Y, Z; };
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long PresentBits[9];                       // +0x20
    CInterfaceMap Interfaces;                           // +0x44
    __forceinline unsigned long HasTC(long type) const { return PresentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
class CTCBase {
public:
    virtual void v00();
    CThing* Thing;                                      // +0x04
    unsigned char _pad08[4];
};
class CTCDParticleEmitter : public CTCBase {
public:
    static CThing* __fastcall Create(const CCharString& name, const C3DVector& pos, bool flag);   // 0x006E0970
    void AttachToThing(CThing& thing, long bone, const CCharString& helper, long mode, float time);   // 0x006E0BE0
    void SetAlwaysUpdate(bool always);                  // 0x006E0590
};
class CTCFishingSpot : public CTCBase {
public:
    CIntelligentPointer<CThing> PParticle1;             // +0x0C
    CIntelligentPointer<CThing> PParticle2;             // +0x14
    CIntelligentPointer<CThing> PParticleFish;          // +0x1C
    bool Enabled;                                       // +0x24
    void RemoveParticles();                             // 0x007B01C0
    __forceinline void CreateParticle(CIntelligentPointer<CThing>& particle, const char* name, long bone, float time)
    {
        if (!particle.GetPItem()) {
            {
                CCharString particleName(name);
                particle = CTCDParticleEmitter::Create(particleName, Thing->GetPosition(), false);
            }
            if (particle.GetPItem()) {
                CThing* thing = particle.Get();
                CTCDParticleEmitter* emitter = 0;
                if (thing->HasTC(0x4b))
                    emitter = (CTCDParticleEmitter*)thing->GetTC(0x4b);
                {
                    CCharString helper("");
                    emitter->AttachToThing(*Thing, bone, helper, 0, time);
                }
                emitter->SetAlwaysUpdate(true);
            }
        }
    }
    void FrameUpdate();
};

void CTCFishingSpot::FrameUpdate()
{
    if (Enabled) {
        CreateParticle(PParticle1, "FISHING_SPOT_01", 0x40, 0.0f);
        CreateParticle(PParticle2, "FISHING_SPOT_02", 0x40, 0.0f);
        CreateParticle(PParticleFish, "UNDERWATER_FISH_01", 0x80, -1.0f);
    } else {
        RemoveParticles();
    }
}
