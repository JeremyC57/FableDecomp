// CThingCreatureBase::AddToCreatureInteraction(const CCountedPointer<CCreatureInteraction>& interaction) -- take the interaction and register this creature with it
// Retail: Fable.exe (TLC, VC7.1) @ 0x00662550, 96 bytes (`ret 4`).
// Callees: the count block's destroy function (+4, object at +8 in ecx), operator delete @ 0x00BFE9BC,
//   CCreatureInteraction::AddCreature(CThingCreatureBase*) @ 0x008631D0 (manifest "CreatureInteraction_InsertAfter":
//   a list insert on the interaction's creature list at +0 that stores the pointer by value).
// Layout: PDB CCountedPointer<CCreatureInteraction> PCreatureInteraction is at +0x150 in Ego_r.pdb; retail keeps it at
//   +0x12c (object) / +0x130 (count block), 0x24 lower, so the class is modelled locally.
// Shape: the counted pointer's inline operator= (both source words read into locals first, then the self test on the block, release
//   re-reading the member block, object then block stores, count up), then the member call through the object.
void __cdecl operator delete(void* block);              // 0x00BFE9BC

struct SCountBlock {
    long m_references;                                  // +0
    void (__fastcall* m_pfnDestroy)(void*);             // +4 per-type deleter, object in ecx
    void* m_pObject;                                    // +8
};
template <class T>
class CCountedPointer { public:
    T* Data;                                            // +0
    SCountBlock* m_pCount;                              // +4
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        SCountBlock* count = other.m_pCount;
        if (m_pCount != count) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    operator delete(m_pCount);
                }
            }
            Data = data;
            m_pCount = count;
            if (count) ++count->m_references;
        }
        return *this;
    }
    T* operator->() const { return Data; }
};

class CThingCreatureBase;
class CCreatureInteraction { public:
    void AddCreature(CThingCreatureBase* creature);     // 0x008631D0
};

class CThingCreatureBase { public:
    unsigned char _pad[0x12c];
    CCountedPointer<CCreatureInteraction> PCreatureInteraction;     // +0x12c
    void AddToCreatureInteraction(const CCountedPointer<CCreatureInteraction>& interaction);
};

void CThingCreatureBase::AddToCreatureInteraction(const CCountedPointer<CCreatureInteraction>& interaction)
{
    PCreatureInteraction = interaction;
    PCreatureInteraction->AddCreature(this);
}