// CSoundSystem_00c131c0::PlaySoundList(ISoundListSource_00c131c0* source, CSoundRequest_00c131c0* request) -- under the libpk global lock, start every valid entry of the source's list; consecutive started sounds are chained into one sound group (allocated from the pool at +0x2C and numbered from the counter at +0x88C); returns the group id, the lone sound's id, or -1 when nothing started
// Leaf: ?PlaySoundList@CSoundSystem_00c131c0@@QAEJPAVISoundListSource_00c131c0@@PAUCSoundRequest_00c131c0@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C131C0, 332 bytes, __fastcall, `ret 8`, /EHsc frame (unwind 0x012117DC) for the scoped lock.
// Identity: libpk audio code (no PDB / RTTI): every name here is descriptive. The only caller 0x00C13310 builds a request block
//   on its stack and passes it with its own list argument. The body chains the started sounds with the group helpers
//   0x00C29F70 (stores the id at group+4, registers the group through system+0x104 slot 9, then AddMember) and 0x00C29EC0
//   (sound+0x30 = group, sound+0x34 = group->m_first, group->m_first = sound); 0x00C21F20 is `this+0x2C+0x318`'s slot-1
//   allocator. The manifest label "CEnginePrimitive_UpdateParticleTrail" is wrong (propagated label); the reloc bindings
//   for 0x00C12CD0 / 0x00C29EC0 / 0x00C29F70 are conflicting placeholders (NOTES.md).
// Callees: PKCProtectionInstance ctor 0x00C0FFC0 / dtor 0x00C0FFE0 over g_lock_0143bb14 (landed spellings), StartSound 0x00C12CD0
//   (ret 0x10), CSoundGroupPool_00c21f20::Allocate 0x00C21F20, CSoundGroup_00c29ec0::Init 0x00C29F70 (ret 0xC) and AddMember
//   0x00C29EC0 (ret 4); list virtuals slot 5 GetList, then IsValid slot 4 / GetCount slot 5 / GetItem slot 7.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;          // 0x0143BB14

class PKCProtectionInstance { public:               // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                            // 0x00C0FFE0
};

struct CSoundRequest_00c131c0;
class CSoundSystem_00c131c0;
class CSoundGroup_00c29ec0;

struct CSoundInstance_00c131c0 {
    char pad0[0xC];
    long m_id;                                      // +0x0C
    char pad10[0x30 - 0x10];
    CSoundGroup_00c29ec0* m_group;                  // +0x30
    CSoundInstance_00c131c0* m_nextInGroup;         // +0x34
};

class CSoundGroup_00c29ec0 { public:
    void* m_vtbl;
    long m_id;                                      // +0x04
    CSoundInstance_00c131c0* m_first;               // +0x08
    void Init(CSoundSystem_00c131c0* system, long id, CSoundInstance_00c131c0* first);   // 0x00C29F70
    void AddMember(CSoundInstance_00c131c0* sound);                                    // 0x00C29EC0
};

class CSoundGroupPool_00c21f20 { public:
    CSoundGroup_00c29ec0* Allocate();               // 0x00C21F20
};

class ISoundList_00c131c0 { public:
    virtual void Slot00();
    virtual void Slot01();
    virtual void Slot02();
    virtual void Slot03();
    virtual long IsValid(unsigned long index);      // slot 4
    virtual unsigned long GetCount();               // slot 5
    virtual void Slot06();
    virtual void* GetItem(unsigned long index);     // slot 7
};

class ISoundListSource_00c131c0 { public:
    virtual void Slot00();
    virtual void Slot01();
    virtual void Slot02();
    virtual void Slot03();
    virtual void Slot04();
    virtual ISoundList_00c131c0* GetList();         // slot 5
};

class CSoundSystem_00c131c0 { public:
    char pad0[0x2C];
    CSoundGroupPool_00c21f20 m_groupPool;           // +0x2C
    char pad2d[0x88C - 0x2D];
    long m_nextGroupId;                             // +0x88C

    CSoundInstance_00c131c0* StartSound(void* item, void* parent, CSoundRequest_00c131c0* request, long flags);   // 0x00C12CD0
    long PlaySoundList(ISoundListSource_00c131c0* source, CSoundRequest_00c131c0* request);
};

long CSoundSystem_00c131c0::PlaySoundList(ISoundListSource_00c131c0* source, CSoundRequest_00c131c0* request)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    CSoundInstance_00c131c0* previous = 0;
    for (unsigned long i = 0; i < source->GetList()->GetCount(); i++)
    {
        if (!source->GetList()->IsValid(i))
            continue;
        CSoundInstance_00c131c0* sound = StartSound(source->GetList()->GetItem(i), 0, request, 0);
        if (!sound)
            continue;
        if (previous)
        {
            if (!previous->m_group)
            {
                CSoundGroup_00c29ec0* group = m_groupPool.Allocate();
                group->Init(this, m_nextGroupId++, previous);
            }
            previous->m_group->AddMember(sound);
        }
        previous = sound;
    }
    if (!previous)
        return -1;
    if (previous->m_group == 0)
        return previous->m_id;
    return previous->m_group->m_id;
}