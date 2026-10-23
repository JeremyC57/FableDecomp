// CTCCreatureGenerator::FrameUpdate() -- per-frame generator tick: bail when generation is disabled or the generator is destroyed (a fully degraded generator destroys itself and clears its target flag), register the living hero as a triggerer, finish a pending initialisation, show debug appearance, prune dead created creatures / null triggerers (adjusting the global generated count), update undead and triggers, then run the random particle and random sound timers
// Grade: DIFFER(1220v1202) -- frame-slot placement (type keys / temps at +0xc in retail, ours +0x10..0x18), def release `dec [edi+4]` with no null test (ours load/dec/store + test), push edi placement; best draft (no const def ptr) was 1204v1202 with an out-of-line ~CDefPointer
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074C130, 1216 bytes, no EH frame.
// Callees: CTCBase::GetWorldConst 0x00686A80 / GetWorld 0x00686A70 / GetPRandomSeed 0x00686DA0, CWorld::IsCreatureGenerationEnabled
// 0x0049EA40, LowerBound 0x0040F020 / const 0x004365B0, CTCDegradableBase::IsFullyDegraded 0x008EC900, CWorld::GetCurrentPlayer
// 0x0049D9D0, GetPItem 0x00A01B50, AddTriggerer 0x0074AC80, InitialiseCreatureGenerator 0x0074B770, CThing::AddTC 0x004C9D60,
// GetSubDef 0x0042B0A2, list::remove 0x0065DE8A, __find 0x0074CEE0, remove_copy 0x0074CE40, vector::erase 0x0074D3E0,
// UpdateUndead 0x0074ACE0, CWorld::GetFrame 0x0049D870, UpdateCreatureTrigger 0x0074BAA0, IS_THING_ALIVE 0x004CC340,
// CThing::Kill 0x004C9B80, GFRandom 0x004CDAE0, CThing::GetPosition 0x004C73D0, CreateEffectThing 0x006E0880,
// CIntelligentPointer<CThing>(CThing*) 0x004A96F0 / operator= 0x004A9760, AttachToThing 0x006E0BE0,
// CDefString::operator!= 0x005FA740 / operator CCharString 0x00415DD0, PlaySound 0x0066BD00.
// Layout (retail, as the landed UpdateCreatureTrigger model): CreatedCreatures list +0x18, CreatureTriggerers vector
// <CIntelligentPointer<CThingCreatureBase>> +0x1c, Initialised +0x4d, ReadyToInitialise +0x4e, UndeadTriggered +0x88,
// Destroyed +0x89, PDef +0x8c, PRandomParticle +0x90, FramesUntilRandomEffect +0x98, FramesUntilRandomSoundEffect +0x9c.
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
struct CDefString {
    long Index;
    bool operator!=(const char* text) const;                 // 0x005FA740
    operator CCharString() const;                            // 0x00415DD0
};
extern long g_GeneratedCreatureCount;                        // 0x013BAD54
extern bool g_bShowGenerators;                               // 0x013BAD58
extern long g_FramesPerSecond;                               // 0x01375550
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0

struct CThing;
struct CBaseIntelligentPointer {
    CBaseIntelligentPointer();                               // 0x00A01B10
    ~CBaseIntelligentPointer();                              // 0x00A01C10
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                                // 0x00A01B50
    void SetPItem(CThing* item);                             // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {          // vtable 0x01238C6C
    __forceinline CIntelligentPointerThing(int) { SetPItem(0); }
    virtual void v00();
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {       // vtable 0x0123E7D8
    __forceinline CIntelligentPointerCreature(int) { SetPItem(0); }
    virtual void v00();
};
struct CIntelligentPointerThingOut : CBaseIntelligentPointer {
    CIntelligentPointerThingOut(CThing* item);               // 0x004A96F0
    CIntelligentPointerThingOut& operator=(const CIntelligentPointerThingOut& other);   // 0x004A9760
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointerThingOut& p);   // 0x004CC340

struct CThingList {
    void* _M_node;
    __forceinline long size() const
    {
        long n = 0;
        for (void* p = *(void**)_M_node; p != _M_node; p = *(void**)p)
            ++n;
        return n;
    }
    void remove(const CIntelligentPointerThing& value);      // 0x0065DE8A
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };
typedef CIntelligentPointerCreature* CTriggerIter;
CTriggerIter __fastcall find_impl(CTriggerIter first, CTriggerIter last, const CIntelligentPointerCreature& value,
                                  const random_access_iterator_tag& tag);   // 0x0074CEE0
CTriggerIter __fastcall remove_copy_impl(CTriggerIter first, CTriggerIter last, CTriggerIter result,
                                         const CIntelligentPointerCreature& value);   // 0x0074CE40
__forceinline CTriggerIter remove_impl(CTriggerIter first, CTriggerIter last, const CIntelligentPointerCreature& value)
{
    first = find_impl(first, last, value, random_access_iterator_tag());
    if (first == last)
        return first;
    CTriggerIter next = first;
    return remove_copy_impl(++next, last, first, value);
}
struct CTriggererVector {
    CTriggerIter _M_start;
    CTriggerIter _M_finish;
    CTriggerIter _M_end_of_storage;
    CTriggerIter erase(CTriggerIter first, CTriggerIter last);   // 0x0074D3E0
    CTriggerIter begin() { return _M_start; }
    CTriggerIter end() { return _M_finish; }
};

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
    const SEntry* LowerBound(const long* type) const;        // 0x004365B0
};
#pragma pack(push, 1)
struct SAppearanceBlock { long a, b, c; short d; };          // 14 bytes
#pragma pack(pop)
struct CThingDef {
    virtual void v00();
    virtual void Destroy() const;                            // slot 1
    long RefCount;                                           // +0x04
    __forceinline void Release() { if (--RefCount == 0) Destroy(); }
    unsigned char _pad08[0x25 - 0x08];
    SAppearanceBlock Appearance;                             // +0x25
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer()
    {
        if (Object) {
            T* object = Object;
            if (--const_cast<CThingDef*>(object)->RefCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<const CThingDef>* out) const;       // 0x0042B0A2
};
struct CTCGraphicAppearance {
    unsigned char _pad00[0x44];
    SAppearanceBlock Appearance;                             // +0x44
};
struct CTCDegradableBase { bool IsFullyDegraded() const; };  // 0x008EC900
struct CTCTargetable { unsigned char _pad00[0x10]; bool Enabled; };   // interface 9, +0x10
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CTCSound {
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};
struct CTCParamsBase;
struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetDrawable(bool drawable);                 // slot 28 (+0x70)
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* m_pAppearance;                     // +0x64
    unsigned char _pad68[0x70 - 0x68];
    CDefClassBase* m_pDef;                                   // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                                 // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    void Kill(bool immediately);                             // 0x004C9B80
    CTCBase* AddTC(const CCharString& name, bool persist, CTCParamsBase* params);   // 0x004C9D60
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline T* GetTCConst(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
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
CThing* __fastcall CreateEffectThing(long effectIndex, const C3DVector& pos, bool persistent);   // 0x006E0880
struct CThingCreatureBase : CThing { };
struct CPlayer {
    unsigned char _pad00[0x34];
    CBaseIntelligentPointer PlayerCharacter;                 // +0x34
    CBaseIntelligentPointer& GetPlayerCharacter() { return PlayerCharacter; }
};
struct CWorld {
    CPlayer* GetCurrentPlayer() const;                       // 0x0049D9D0
    long GetFrame() const;                                   // 0x0049D870
    bool IsCreatureGenerationEnabled(const CThing& thing) const;   // 0x0049EA40
};
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    const long* begin() const { return _M_start; }
};
struct CCreatureGeneratorDef {
    unsigned char _pad00[0x28];
    CLongVector RandomEffects;                               // +0x28
    CDefString RandomSound;                                  // +0x34
    long RandomEffectInterval;                               // +0x38
    long RandomSoundInterval;                                // +0x3c
};
struct CTCCreatureGenerator {
    virtual void v00();
    CThing*            ParentThing;                          // +0x04
    unsigned char      _pad08[0x18 - 0x08];
    CThingList         CreatedCreatures;                     // +0x18
    CTriggererVector   CreatureTriggerers;                   // +0x1c
    unsigned char      _pad28[0x4d - 0x28];
    bool               Initialised;                          // +0x4d
    bool               ReadyToInitialise;                    // +0x4e
    unsigned char      _pad4f[0x88 - 0x4f];
    bool               UndeadTriggered;                      // +0x88
    bool               Destroyed;                            // +0x89
    unsigned char      _pad8a[0x8c - 0x8a];
    const CCreatureGeneratorDef* PDef;                       // +0x8c
    CIntelligentPointerThingOut PRandomParticle;             // +0x90
    long               FramesUntilRandomEffect;              // +0x98
    long               FramesUntilRandomSoundEffect;         // +0x9c
    const CWorld* GetWorldConst() const;                     // 0x00686A80
    CWorld* GetWorld();                                      // 0x00686A70
    unsigned long* GetPRandomSeed();                         // 0x00686DA0
    void AddTriggerer(CThing* creature);                     // 0x0074AC80
    void InitialiseCreatureGenerator(bool force);            // 0x0074B770
    void UpdateUndead();                                     // 0x0074ACE0
    void UpdateCreatureTrigger(long frame);                  // 0x0074BAA0
    void FrameUpdate();
};

void CTCCreatureGenerator::FrameUpdate()
{
    if (!GetWorldConst()->IsCreatureGenerationEnabled(*ParentThing))
        return;
    if (Destroyed)
        return;
    if (ParentThing->HasTC(0x67) && ParentThing->GetTCConst<CTCDegradableBase>(0x67)->IsFullyDegraded()) {
        Destroyed = true;
        if (ParentThing->HasTC(9))
            ParentThing->GetTC<CTCTargetable>(9)->Enabled = false;
    }
    if (Destroyed)
        return;
    CThing* hero = GetWorld()->GetCurrentPlayer()->GetPlayerCharacter().GetPItem();
    if (hero && !(hero->m_flags91 & 1))
        AddTriggerer(hero);
    if (ReadyToInitialise && !Initialised)
        InitialiseCreatureGenerator(false);
    if (g_bShowGenerators && ParentThing->m_pAppearance == 0) {
        ParentThing->AddTC("CTCGraphicAppearance", false, 0);
        ParentThing->AddTC("CTCGraphicAppearanceStaticMesh", false, 0);
        CDefPointer<const CThingDef> def;
        ParentThing->m_pDef->GetSubDef(&def);
        ParentThing->m_pAppearance->Appearance = def->Appearance;
        ParentThing->SetDrawable(true);
    }
    long before = CreatedCreatures.size();
    CreatedCreatures.remove(CIntelligentPointerThing(0));
    CreatureTriggerers.erase(remove_impl(CreatureTriggerers.begin(), CreatureTriggerers.end(), CIntelligentPointerCreature(0)),
                             CreatureTriggerers.end());
    g_GeneratedCreatureCount += CreatedCreatures.size() - before;
    if (UndeadTriggered)
        UpdateUndead();
    if (Initialised)
        UpdateCreatureTrigger(GetWorld()->GetFrame());
    if (PDef) {
        if (!PDef->RandomEffects.empty() && --FramesUntilRandomEffect <= 0) {
            if (IS_THING_ALIVE(PRandomParticle))
                PRandomParticle.GetPItem()->Kill(true);
            unsigned long count = PDef->RandomEffects.size();
            long index = GFRandom(count, GetPRandomSeed());
            PRandomParticle = CreateEffectThing(*(PDef->RandomEffects.begin() + index), ParentThing->GetPosition(), false);
            if (PRandomParticle.GetPItem()) {
                CTCDParticleEmitter* emitter;
                PRandomParticle.GetPItem()->GetInterface(&emitter, 0x4b);
                emitter->AttachToThing(ParentThing, 0x20, "", 0, 0.0f);
            }
            FramesUntilRandomEffect = PDef->RandomEffectInterval * g_FramesPerSecond;
        }
        if (PDef->RandomSound != "" && --FramesUntilRandomSoundEffect <= 0) {
            CTCSound* sound;
            if (ParentThing->GetInterface(&sound, 0x10))
                sound->PlaySound(PDef->RandomSound, 1, 0, 0, -1);
            FramesUntilRandomSoundEffect = PDef->RandomSoundInterval * g_FramesPerSecond;
        }
    }
}
