// CSoundEventManager::CanStartEvent(CParticle* source, long a, long b, long c, long force, unsigned long priority) -- look the (source, a, b, c) key up in the listening list: no match -> 1; an inaudible match is stopped -> 1; an audible match needs (Pending or force) and priority >= its own to be reset to full level (-> 1), else 0
// Grade: DIFFER(242v242) -- block layout only: retail falls from the audible test's `jb` into ResetLevel and shares one return-1 epilogue (Stop ends in `jmp`), with the return-0 epilogue last; ours either inverts to `jae` and puts the return-0 epilogue inline (early-return forms, this body) or duplicates the return-1 epilogue after Stop (else-if / nested positive forms). Instructions identical otherwise
// Leaf: ?CanStartEvent@CSoundEventManager@@QAEHPAUCParticle@@JJJJK@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C11230, 242 bytes, thiscall `ret 18h`; EH-framed (the key, state 0).
// Identity (descriptive; libpk has no PDB or RTTI): called once, from the CSoundEventManager body 0x00C12940 (this in
//   ebx; a zero result skips the start); it uses the landed CSoundEvent methods IsAudible 0x00C20C10, Stop 0x00C21620 and
//   ResetLevel 0x00C20DF0, the event's Priority (+0x14) and Pending (+0x28) words and the manager's listening list at
//   +0x1F4 (slot 17 = Find by key, as CSoundEventManager::ReleaseTarget 0x00C12140 uses its registry). The key is the
//   16-byte particle handle (zeroing ctor 0x00C25C50, Bind 0x00C25C70 add-refs the source, Release 0x00C25CB0 drops it,
//   empty out-of-line dtor 0x00C25CA0). The catalog's "CTCHero_OnActionInterrupt" (and the callee labels C3DGroup2,
//   GetPMeshAnims, CTCStealth::OnCreate, $E2, InputEvent_ValidateForContext, CTCInventory::OnCreate,
//   Thiscall_ActionLightningStateHandler) are propagated game labels.
struct CSoundEventManager;

struct CParticle;                                    // the referenced object (16-bit count at +0x1E)

struct CParticleHandle {                             // 16 bytes
    CParticle* m_pParticle;                          // +0x00
    long       m_a;                                  // +0x04
    long       m_b;                                  // +0x08
    long       m_c;                                  // +0x0c
    CParticleHandle();                               // 0x00C25C50 (zeroes the four words)
    ~CParticleHandle();                              // 0x00C25CA0 (empty)
    void Bind(CParticle* particle, long a, long b, long c);   // 0x00C25C70
    void Release();                                  // 0x00C25CB0
};

struct CSoundEvent {
    void*         __vftable;                         // +0x00
    unsigned char _pad_0x04[0x10];
    long          Priority;                          // +0x14
    unsigned char _pad_0x18[0x10];
    long          Pending;                           // +0x28
    bool IsAudible(CSoundEventManager* manager);     // 0x00C20C10
    void Stop(CSoundEventManager* manager);          // 0x00C21620
    void ResetLevel(CSoundEventManager* manager);    // 0x00C20DF0
};

struct CSoundEventList {                             // embedded polymorphic event registry
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16();
    virtual CSoundEvent* Find(CParticleHandle* key); // slot 17 (+0x44)
};

struct CSoundEventManager {
    unsigned char   _pad_0x000[0x1F4];
    CSoundEventList m_listening;                     // +0x1F4
    int CanStartEvent(CParticle* source, long a, long b, long c, long force, unsigned long priority);
};

int CSoundEventManager::CanStartEvent(CParticle* source, long a, long b, long c, long force, unsigned long priority)
{
    CParticleHandle key;
    key.Bind(source, a, b, c);
    CSoundEvent* event = m_listening.Find(&key);
    key.Release();
    if (event) {
        if (!event->IsAudible(this)) {
            event->Stop(this);
        } else {
            if (event->Pending == 0 && force == 0)
                return 0;
            if (priority < (unsigned long)event->Priority)
                return 0;
            event->ResetLevel(this);
        }
    }
    return 1;
}
