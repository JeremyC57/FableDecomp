// CTCObjectAugmentations::FrameUpdate() -- per-frame augmentation upkeep: plays each not-yet-played init / looping augmentation sound once, (re)creates the projectile trail effect for fire / lightning augmentations, cycles the idle augmentation particle sets once every running effect has died, and touches the attack particle effects
// Grade: DIFFER(1065v1067) -- the two sound-map walks match; from the trail block on the scratch registers rotate (retail eax/ecx/edx for the three trail-effect def loads, ours ecx/edx/eax) and the particle-cycle loop addresses the element as `lea eax,[ecx+edx]` before its field reads where ours folds `[eax+edx+0Ch]`; the attack walk forms `[esi+edi+10h]` where ours forms `[edi+esi+10h]` (tried: CDefPointer operator-> on PDef, an uninitialised effect local with an else-zero: inert)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00766DD0, 1067 bytes, plain `ret`. CTCObjectAugmentations vtable FrameUpdate.
// Callees: CVectorMap LowerBound 0x0040F020 (inline GetTC, types 0x10 / 0x4B), CSoundInfo::PlaySound 0x0066BD00,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 / CBaseIntelligentPointer() 0x00A01B10 / SetPItem 0x00A01B90 /
//   ~CBaseIntelligentPointer 0x00A01C10 (vtable 0x01238C6C = CIntelligentPointer<CThing>), HasCombinationOfAugmentations
//   0x00766050, HasAugmentationOfType 0x007660C0, CThing::GetPosition 0x004C73D0, CreateSpecialEffect 0x006E0880
//   (`__fastcall`, the trailing `push 0` is its bool), CTCDParticleEmitter::AttachToThing 0x006E0BE0,
//   vector<CIntelligentPointer<CThing> >::erase 0x004AC0F0 / resize 0x00739B40, AttemptToStartParticle 0x00766660.
// Shape: the two sound maps are walked with the inlined STLport _M_increment and end() re-read from the member; the
//   trail effect is assigned from a CIntelligentPointer temporary (inline ctor / operator=, out-of-line base dtor);
//   the emitter lookup is `T* tc = 0; if (HasTC) tc = GetTC;`; the attack-effect walk caches the outer end pointer.
// Layout: as the landed 0x007673C0 / 0x00766A60 (PDef +0x0C, InitSounds +0x20, LoopingSounds +0x2C, ParticleEffects
//   +0x38, AttackParticleEffects +0x44, CurrentRunningAugEffect +0x50, CurrentRunningAugEffectIndex +0x5C, CurPeriod +0x60,
//   InStrikeAction +0x64, ProjectileTrailEffect +0x65 under pack(1)); CObjectAugmentationsDef trail effects +0x70/+0x74/+0x78
//   and WeaponTrailAttachmentPoint +0x88 (PDB minus five 4-byte container shrinks); CThing bitset +0x20 (0x10 = word 0
//   bit 16, 0x4B = word 2 bit 11, 0x77 = word 3 bit 23), dead bit +0x91.
struct CTCBase;

struct CCharString { void* PStringData; };
struct C3DVector { float X, Y, Z; };

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 std::bitset of thing-component interface types
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long type, T*)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached);   // 0x006E0880

#pragma pack(push, 1)                                   // the PDB puts ProjectileTrailEffect at the odd offset +0x65
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void*   PData;
    CBaseIntelligentPointer();                          // 0x00A01B10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) { SetPItem(item); }
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

template <class T> struct CAllocProxy { T _M_data; };   // STLport _STLP_alloc_proxy over an empty allocator
template <class T> struct vector {                      // STLport vector: _Vector_base, 12 bytes
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T*> _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
struct CIntelligentPointerVector : vector< CIntelligentPointer<CThing> > {
    void erase(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last);   // 0x004AC0F0
    void resize(unsigned long count);                   // 0x00739B40
};

struct ParticleAndEndPoints {                           // 0x14 (PDB)
    CCharString StartDummy;                             // +0x00 (descriptive, AttemptToStartParticle's argument order)
    CCharString EndDummy;                               // +0x04
    long        ParticleType;                           // +0x08
    long        EffectFrames;                           // +0x0c (passed as the start frame count; name descriptive)
    long        PeriodicFrames;                         // +0x10
};
struct AttackParticleAndEndPoints {                     // 0x18 (PDB)
    CCharString StartDummy;                             // +0x00
    CCharString EndDummy;                               // +0x04
    long        ParticleType;                           // +0x08
    long        PeriodicFrames;                         // +0x0c
    CIntelligentPointer<CThing> ParticleEffect;         // +0x10
};

struct CObjectAugmentationSoundSet {                    // 0x0c
    bool          Played;                               // +0x00
    CCharString   Sound;                                // +0x04
    unsigned long Handle;                               // +0x08
};
struct SSoundMapNode {                                  // STLport _Rb_tree_node<pair<const EObjectAugmentationType, CObjectAugmentationSoundSet> >
    long           _M_color;
    SSoundMapNode* _M_parent;
    SSoundMapNode* _M_left;
    SSoundMapNode* _M_right;
    long           first;                               // +0x10 EObjectAugmentationType
    CObjectAugmentationSoundSet second;                 // +0x14
};
struct CSoundSetMap {                                   // STLport map (12 bytes)
    SSoundMapNode* _M_header;
    unsigned long  _M_node_count;
    unsigned char  _M_key_compare;
};
static __forceinline SSoundMapNode* _M_increment(SSoundMapNode* node)
{
    if (node->_M_right != 0) {
        node = node->_M_right;
        while (node->_M_left != 0)
            node = node->_M_left;
    } else {
        SSoundMapNode* parent = node->_M_parent;
        while (node == parent->_M_right) {
            node = parent;
            parent = parent->_M_parent;
        }
        if (node->_M_right != parent)
            node = parent;
    }
    return node;
}

struct CTCSoundInfo {                                   // interface 0x10
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00 (CSoundInfo)
};
struct CTCDParticleEmitter {                            // interface 0x4B
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CObjectAugmentationsDef {
    unsigned char _pad00[0x70];
    long          FireProjectileTrailEffect;            // +0x70
    long          LightningProjectileTrailEffect;       // +0x74
    long          FireAndLightningProjectileTrailEffect;   // +0x78
    unsigned char _pad7c[0x88 - 0x7c];
    CCharString   WeaponTrailAttachmentPoint;           // +0x88
};

struct CTCObjectAugmentations {
    virtual ~CTCObjectAugmentations();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    bool          ToBeRemoved;                          // +0x08
    bool          IsInGlobalUpdateTCs;                  // +0x09
    const CObjectAugmentationsDef* PDef;                // +0x0c CDefPointer<const CObjectAugmentationsDef>
    bool          LoadedFromSaveGame;                   // +0x10
    unsigned char Augmentations[0xc];                   // +0x14
    CSoundSetMap  InitSounds;                           // +0x20
    CSoundSetMap  LoopingSounds;                        // +0x2c
    vector< vector<ParticleAndEndPoints> > ParticleEffects;               // +0x38
    vector< vector<AttackParticleAndEndPoints> > AttackParticleEffects;   // +0x44
    CIntelligentPointerVector CurrentRunningAugEffect;  // +0x50
    long          CurrentRunningAugEffectIndex;         // +0x5c
    long          CurPeriod;                            // +0x60
    bool          InStrikeAction;                       // +0x64
    CIntelligentPointer<CThing> ProjectileTrailEffect;  // +0x65
    bool HasCombinationOfAugmentations(long types) const;   // 0x00766050
    bool HasAugmentationOfType(long type) const;        // 0x007660C0
    void AttemptToStartParticle(CIntelligentPointer<CThing>& effect, long particleType, long periodicFrames,
                                const CCharString& startDummy, const CCharString& endDummy);   // 0x00766660
    void FrameUpdate();
};

void CTCObjectAugmentations::FrameUpdate()
{
    for (SSoundMapNode* node = InitSounds._M_header->_M_left; node != InitSounds._M_header; node = _M_increment(node)) {
        if (!node->second.Played) {
            CThing* thing = ParentThing;
            if (thing->HasTC(0x10)) {
                node->second.Handle = thing->GetTC(0x10, (CTCSoundInfo*)0)->PlaySound(node->second.Sound, 0, 0, 0, -1);
                node->second.Played = true;
            }
        }
    }
    for (SSoundMapNode* node = LoopingSounds._M_header->_M_left; node != LoopingSounds._M_header; node = _M_increment(node)) {
        if (!node->second.Played) {
            CThing* thing = ParentThing;
            if (thing->HasTC(0x10)) {
                node->second.Handle = thing->GetTC(0x10, (CTCSoundInfo*)0)->PlaySound(node->second.Sound, 0, 0, 0, -1);
                node->second.Played = true;
            }
        }
    }
    if (ParentThing->HasTC(0x77) && !IS_THING_ALIVE(ProjectileTrailEffect)) {
        long effect = 0;
        if (HasCombinationOfAugmentations(0xc))
            effect = PDef->FireAndLightningProjectileTrailEffect;
        else if (HasAugmentationOfType(4))
            effect = PDef->FireProjectileTrailEffect;
        else if (HasAugmentationOfType(8))
            effect = PDef->LightningProjectileTrailEffect;
        if (effect) {
            ProjectileTrailEffect = CIntelligentPointer<CThing>(CreateSpecialEffect(effect, ParentThing->GetPosition(), false));
            if (IS_THING_ALIVE(ProjectileTrailEffect)) {
                CTCDParticleEmitter* emitter = 0;
                CThing* trail = ProjectileTrailEffect.GetPItem();
                if (trail->HasTC(0x4b))
                    emitter = trail->GetTC(0x4b, (CTCDParticleEmitter*)0);
                emitter->AttachToThing(ParentThing, 0x28, PDef->WeaponTrailAttachmentPoint, 0, 0.0f);
            }
        }
    }
    if (!InStrikeAction) {
        bool allDead = true;
        for (long i = 0; i < (long)CurrentRunningAugEffect.size(); ++i) {
            if (IS_THING_ALIVE(CurrentRunningAugEffect[i]))
                allDead = false;
        }
        if (allDead) {
            if (CurPeriod > 0)
                --CurPeriod;
            else {
                long count = (long)ParticleEffects.size();
                if (count > 0) {
                    CurrentRunningAugEffectIndex = (CurrentRunningAugEffectIndex + 1) % count;
                    CurPeriod = ParticleEffects[CurrentRunningAugEffectIndex][0].PeriodicFrames;
                    CurrentRunningAugEffect.erase(CurrentRunningAugEffect._M_start, CurrentRunningAugEffect._M_finish);
                    long particles = (long)ParticleEffects[CurrentRunningAugEffectIndex].size();
                    CurrentRunningAugEffect.resize(particles);
                    for (long j = 0; j < particles; ++j) {
                        ParticleAndEndPoints& particle = ParticleEffects[CurrentRunningAugEffectIndex][j];
                        AttemptToStartParticle(CurrentRunningAugEffect[j], particle.ParticleType, particle.EffectFrames,
                                               particle.StartDummy, particle.EndDummy);
                    }
                }
            }
        }
    }
    vector<AttackParticleAndEndPoints>* attack = AttackParticleEffects.begin();
    vector<AttackParticleAndEndPoints>* end = AttackParticleEffects.end();
    for (; attack != end; ++attack) {
        for (long j = 0; j < (long)attack->size(); ++j) {
            CIntelligentPointer<CThing>& effect = (*attack)[j].ParticleEffect;
            if (effect.GetPItem() != 0)
                effect.GetPItem();
        }
    }
}
