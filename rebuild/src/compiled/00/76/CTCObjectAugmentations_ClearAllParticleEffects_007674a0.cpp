// CTCObjectAugmentations::ClearAllParticleEffects() -- kill the projectile trail and every running / attack particle effect, then empty both particle tables
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007674A0, length 507 bytes (+ int3 padding), esp frame, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / ctor @ 0x00A01B10 / dtor @ 0x00A01C10
// (CIntelligentPointer<CThing> vtable 0x01238C6C), CThing::Kill(bool) @ 0x004C9B80, vector<ParticleAndEndPoints>::operator=
// @ 0x0076B1E0, vector<AttackParticleAndEndPoints>::operator= @ 0x0076B320, and the out-of-line STLport __destroy_aux
// instantiations @ 0x0076BD70 / 0x0076BDD0 (first in ecx, last in edx, the __false_type reference pushed; manifest labels
// "_Uninit_copy" / "Vector_Assign_*" are propagated).
// Shape: the inline STLport clear() = erase(begin, end) (copy loop of zero elements, then __destroy_aux, then the finish
// store); the trail reset assigns a CIntelligentPointer(0) temporary; the kill target `v[i].GetPItem()` goes through an
// element operator-> and the vector's operator[] reads _M_start through an inline begin() (`*(begin() + n)`), which gives
// retail's `mov eax,[base] ; push 1 ; lea ecx,[edi+eax]` (raw `*(_M_start + n)` loads straight into ecx); the tag type has a
// user constructor so `FalseType()` stores nothing.
// Layout (local model, as the landed ctor 0x007673C0): retail 12-byte STLport containers, ParticleEffects +0x38,
// AttackParticleEffects +0x44, CurrentRunningAugEffect +0x50, ProjectileTrailEffect +0x65 (pack(1)); inner elements
// ParticleAndEndPoints 0x14 and AttackParticleAndEndPoints 0x18 (ParticleEffect +0x10), field names before +0x10 descriptive.
class CCharString { public: void* PStringData; };
class CThing {
public:
    unsigned char  _pad00[0x91];
    unsigned char  m_flags91;                           // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    void Kill(bool immediate);                          // 0x004C9B80
};
#pragma pack(push, 1)                                   // the PDB puts ProjectileTrailEffect at the odd offset +0x65
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    void SetPItem(CThing* item);                        // 0x00A01B90
    CThing* GetPItem() const;                           // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    T* operator->() const { return (T*)GetPItem(); }
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
struct FalseType { FalseType() { } };                     // STLport __false_type
template <class T> struct CAllocProxy {                 // STLport _STLP_alloc_proxy over an empty allocator
    T _M_data;
};
template <class T> void __fastcall DestroyAux(T* first, T* last, const FalseType&);   // STLport __destroy_aux
template <class T> struct vector {                      // STLport vector: _Vector_base, 12 bytes
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T*> _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    vector& operator=(const vector& other);
    __forceinline void clear() { erase(_M_start, _M_finish); }
    __forceinline T* erase(T* first, T* last)
    {
        T* i = Copy(last, _M_finish, first);
        DestroyAux(i, _M_finish, FalseType());
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
};
struct ParticleAndEndPoints {                           // 0x14 (PDB)
    CCharString StartDummy;                             // +0x00 (descriptive)
    CCharString EndDummy;                               // +0x04
    long        ParticleType;                           // +0x08
    long        Unknown0c;                              // +0x0c
    long        PeriodicFrames;                         // +0x10
};
struct AttackParticleAndEndPoints {                     // 0x18 (PDB)
    CCharString StartDummy;                             // +0x00 (descriptive)
    CCharString EndDummy;                               // +0x04
    long        ParticleType;                           // +0x08
    long        PeriodicFrames;                         // +0x0c
    CIntelligentPointer<CThing> ParticleEffect;         // +0x10
};
// vector<ParticleAndEndPoints>::operator= @ 0x0076B1E0, vector<AttackParticleAndEndPoints>::operator= @ 0x0076B320,
// __destroy_aux over vector<ParticleAndEndPoints> @ 0x0076BD70 and over vector<AttackParticleAndEndPoints> @ 0x0076BDD0.
class CTCObjectAugmentations { public:
    virtual void v00();
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
    CIntelligentPointer<CThing> ProjectileTrailEffect;  // +0x65
    void ClearAllParticleEffects();
};

void CTCObjectAugmentations::ClearAllParticleEffects()
{
    if (IS_THING_ALIVE(ProjectileTrailEffect)) {
        ProjectileTrailEffect->Kill(true);
        ProjectileTrailEffect = CIntelligentPointer<CThing>(0);
    }
    for (long i = 0; i < (long)CurrentRunningAugEffect.size(); ++i) {
        if (IS_THING_ALIVE(CurrentRunningAugEffect[i]))
            CurrentRunningAugEffect[i]->Kill(true);
    }
    vector<AttackParticleAndEndPoints>* end = AttackParticleEffects._M_finish;
    for (vector<AttackParticleAndEndPoints>* it = AttackParticleEffects._M_start; it != end; ++it) {
        for (long j = 0; j < (long)it->size(); ++j) {
            if (IS_THING_ALIVE((*it)[j].ParticleEffect))
                (*it)[j].ParticleEffect->Kill(true);
        }
    }
    ParticleEffects.clear();
    AttackParticleEffects.clear();
}