// CTCObjectAugmentations::FireCombatAugEffect() -- enter the strike action: kill and drop the running augmentation effects, then try to start every attack particle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00766A60, length 403 bytes (+ int3 padding), esp frame, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90, CThing::Kill(bool) @ 0x004C9B80, the
// element virtual destructor (slot 0 with 0), CTCObjectAugmentations::AttemptToStartParticle @ 0x00766660.
// Shape: InStrikeAction is set first; the running effects are killed by index (signed size), then cleared with the inline
// STLport erase(begin, end): a forced-inline copy loop (operator= names the item) and an inline destroy loop of virtual
// destructor calls; the attack table walks a named reference to each inner vector. operator[] goes through an inline
// begin() (`*(begin() + n)`), which gives retail's `lea ecx,[edi+eax]` for the kill target.
// Layout (local model, as the landed ctor 0x007673C0): AttackParticleEffects +0x44, CurrentRunningAugEffect +0x50,
// InStrikeAction +0x64; AttackParticleAndEndPoints 0x18 (PDB) with ParticleEffect +0x10; fields before it descriptive
// (start/end dummy names +0x00/+0x04, particle type +0x08, periodic frames +0x0c, the argument order of AttemptToStartParticle).
struct CCharString { void* PStringData; };
class CThing {
public:
    unsigned char  _pad00[0x91];
    unsigned char  m_flags91;                           // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    void Kill(bool immediate);                          // 0x004C9B80
};
#pragma pack(push, 1)                                   // the PDB puts ProjectileTrailEffect at the odd offset +0x65
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    void*   PData;
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    virtual ~CIntelligentPointer();
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
#pragma pack(pop)
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
template <class T> struct CAllocProxy {                 // STLport _STLP_alloc_proxy over an empty allocator
    T _M_data;
};
template <class T> struct vector {                      // STLport vector: _Vector_base, 12 bytes
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T*> _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    __forceinline void clear() { erase(_M_start, _M_finish); }
    __forceinline T* erase(T* first, T* last)
    {
        T* i = Copy(last, _M_finish, first);
        Destroy(i, _M_finish);
        _M_finish = i;
        return first;
    }
    static __forceinline T* Copy(T* first, T* last, T* result)
    {
        for (long n = last - first; n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    static __forceinline void Destroy(T* first, T* last)
    {
        for (; first != last; ++first)
            first->~T();
    }
};
struct ParticleAndEndPoints;
struct AttackParticleAndEndPoints {                     // 0x18 (PDB)
    CCharString StartDummy;                             // +0x00 (descriptive)
    CCharString EndDummy;                               // +0x04
    long        ParticleType;                           // +0x08
    long        PeriodicFrames;                         // +0x0c
    CIntelligentPointer<CThing> ParticleEffect;         // +0x10
};
struct CTCObjectAugmentations {
    virtual ~CTCObjectAugmentations();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    bool          ToBeRemoved;                          // +0x08
    bool          IsInGlobalUpdateTCs;                  // +0x09
    void*         PDef;                                 // +0x0c
    bool          LoadedFromSaveGame;                   // +0x10
    unsigned char Augmentations[0xc];                   // +0x14
    unsigned char InitSounds[0xc];                      // +0x20
    unsigned char LoopingSounds[0xc];                   // +0x2c
    vector< vector<ParticleAndEndPoints> > ParticleEffects;               // +0x38
    vector< vector<AttackParticleAndEndPoints> > AttackParticleEffects;   // +0x44
    vector< CIntelligentPointer<CThing> > CurrentRunningAugEffect;        // +0x50
    long          CurrentRunningAugEffectIndex;         // +0x5c
    long          CurPeriod;                            // +0x60
    bool          InStrikeAction;                       // +0x64
    void AttemptToStartParticle(CIntelligentPointer<CThing>& effect, long particleType, long periodicFrames,
                                const CCharString& startDummy, const CCharString& endDummy);   // 0x00766660
    void FireCombatAugEffect();
};

void CTCObjectAugmentations::FireCombatAugEffect()
{
    InStrikeAction = true;
    for (long i = 0; i < (long)CurrentRunningAugEffect.size(); ++i) {
        if (IS_THING_ALIVE(CurrentRunningAugEffect[i]))
            CurrentRunningAugEffect[i].GetPItem()->Kill(true);
    }
    CurrentRunningAugEffect.clear();
    for (long i = 0; i < (long)AttackParticleEffects.size(); ++i) {
        vector<AttackParticleAndEndPoints>& particles = AttackParticleEffects[i];
        for (long j = 0; j < (long)particles.size(); ++j) {
            AttackParticleAndEndPoints& particle = particles[j];
            AttemptToStartParticle(particle.ParticleEffect, particle.ParticleType, particle.PeriodicFrames,
                                   particle.StartDummy, particle.EndDummy);
        }
    }
}