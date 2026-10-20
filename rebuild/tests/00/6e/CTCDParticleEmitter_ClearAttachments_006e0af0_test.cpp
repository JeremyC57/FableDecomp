// Behaviour test for CTCDParticleEmitter::ClearAttachments (0x006E0AF0): a living attached thing with interface 0xd8 gets
// the emitter's thing as dependent; the attachment is then cleared (pointer null, name "", index -1, flags 1) and only the
// documented bits change.
#include <cstdio>
#include <cstring>
static int g_temps = 0;
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CThing {
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char Flags91;                              // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    template <class T> bool GetInterface(T** ppOut, long interfaceType)
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
struct CTCDependentThings {                             // interface 0xd8
    CThing* Added; int Adds;
    void AddDependentThing(CThing* thing) { Added = thing; ++Adds; }
};
struct CCharString {
    void* PStringData;
    CCharString& operator=(const char* text) { PStringData = (void*)text; return *this; }
};
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer() : m_pItem(0) { ++g_temps; }
    ~CBaseIntelligentPointer() { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00() { }
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
inline bool IS_THING_ALIVE(const CIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
struct CTCDParticleEmitter {
    virtual void v00() { }
    CThing* ParentThing;
    CTCDParticleEmitter() : PAttachedToThing(0) { }
    CIntelligentPointer PAttachedToThing;               // +0x1c
    unsigned long AttachmentFlags;                      // +0x24
    CCharString AttachmentPosName;                      // +0x28
    long AttachmentPosIndex;                            // +0x2c
    bool WaitingForParticleToLoad : 1;                  // +0x44 bit 0
    bool ForceParticleToLoad : 1;
    bool IndependantObject : 1;
    bool ParticleCreationDisabled : 1;                  // bit 3
    bool ResetEmitterPosition : 1;                      // bit 4
    bool ResetEmitterOrientation : 1;                   // bit 5
    bool AttachedToCamera : 1;                          // bit 6
    bool Moving : 1;                                    // bit 7
    bool AttachedToThing : 1;                           // +0x45 bit 0
    bool AlwaysUpdate : 1;
    bool Asleep : 1;
    void ClearAttachments();
};

void CTCDParticleEmitter::ClearAttachments()
{
    if (IS_THING_ALIVE(PAttachedToThing)) {
        CTCDependentThings* dependents;
        if (PAttachedToThing.GetPItem()->GetInterface(&dependents, 0xd8))
            dependents->AddDependentThing(ParentThing);
    }
    AttachmentFlags = 1;
    ResetEmitterPosition = true;
    ResetEmitterOrientation = true;
    Moving = false;
    PAttachedToThing = CIntelligentPointer(0);
    AttachmentPosName = "";
    AttachmentPosIndex = -1;
    ParticleCreationDisabled = false;
    AttachedToCamera = false;
    AttachedToThing = false;
}

int main()
{
    int bad = 0;
    CTCDependentThings deps; deps.Added = 0; deps.Adds = 0;
    CThing attached; for (int i = 0; i < 9; ++i) attached.m_presentBits[i] = 0; attached.Flags91 = 0;
    CInterfaceMap::SEntry e[1] = { { 0xd8, (CTCBase*)&deps } };
    attached.m_interfaces.first = e; attached.m_interfaces.last = e + 1; attached.m_presentBits[6] = 1UL << 24;
    CThing me;
    CTCDParticleEmitter p; p.ParentThing = &me; p.PAttachedToThing.SetPItem(&attached);
    p.AttachmentFlags = 5; p.AttachmentPosName.PStringData = (void*)"HAND"; p.AttachmentPosIndex = 3;
    p.WaitingForParticleToLoad = 1; p.ForceParticleToLoad = 1; p.IndependantObject = 1; p.ParticleCreationDisabled = 1;
    p.ResetEmitterPosition = 0; p.ResetEmitterOrientation = 0; p.AttachedToCamera = 1; p.Moving = 1;
    p.AttachedToThing = 1; p.AlwaysUpdate = 1; p.Asleep = 1;
    p.ClearAttachments();
    if (deps.Adds != 1 || deps.Added != &me || p.PAttachedToThing.GetPItem() || p.AttachmentFlags != 1
        || std::strcmp((const char*)p.AttachmentPosName.PStringData, "") || p.AttachmentPosIndex != -1) bad |= 1;
    if (!p.WaitingForParticleToLoad || !p.ForceParticleToLoad || !p.IndependantObject || p.ParticleCreationDisabled
        || !p.ResetEmitterPosition || !p.ResetEmitterOrientation || p.AttachedToCamera || p.Moving
        || p.AttachedToThing || !p.AlwaysUpdate || !p.Asleep) bad |= 2;
    attached.Flags91 = 1; p.PAttachedToThing.SetPItem(&attached);
    p.ClearAttachments();                            // dead: no dependent
    if (deps.Adds != 1 || p.PAttachedToThing.GetPItem()) bad |= 4;
    attached.Flags91 = 0; attached.m_presentBits[6] = 0; p.PAttachedToThing.SetPItem(&attached);
    p.ClearAttachments();                            // no 0xd8 component
    if (deps.Adds != 1) bad |= 8;
    if (bad) { std::printf("006e0af0_TEST FAIL\n"); return 1; }
    std::printf("006e0af0_TEST PASS\n");
    return 0;
}