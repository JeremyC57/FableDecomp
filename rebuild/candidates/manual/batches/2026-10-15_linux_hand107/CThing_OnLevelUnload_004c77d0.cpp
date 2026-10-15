// CThing::OnLevelUnload() -- mark the thing as unloading with its level, forbid adding components, tell every live component the level is unloading, then permit adding components again
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C77D0, 67 bytes. CThing vtable 0x01239ABC slot 43.
// Callees: CTCBase virtual slot 15 (+0x3c) on each component not marked ToBeRemoved.
// Shape: the +0x93 flag byte (PDB +0xa3) holds LevelIsUnloading (bit 4) and AddingTCsPermitted (bit 6): both
// stores merge into one load/`and 0BFh`/`or 10h`/store; the restore after the loop is `or byte ptr [..],40h`.
// Components {type, component} pairs at retail +0x44/+0x48, last pointer re-read after each call.
struct CTCBase;

struct SComponentPair {                              // pair<ETCInterfaceType, CTCBase*>
    long     first;
    CTCBase* second;
};

struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14();
    virtual void OnLevelUnload();                    // slot 15 (+0x3c)
    void* ParentThing;                               // +0x04
    bool  ToBeRemoved;                               // +0x08
};

struct CThing {
    void*           __vftable;                       // +0x00
    unsigned char   _pad04[0x44 - 0x04];
    SComponentPair* m_componentsFirst;               // +0x44 Components
    SComponentPair* m_componentsLast;                // +0x48
    unsigned char   _pad4c[0x93 - 0x4c];
    unsigned char   NotPausedDuringCutscenes : 1;    // +0x93 bit 0 (PDB +0xa3)
    unsigned char   DeathPersistent : 1;             // bit 1
    unsigned char   KillOnLevelUnload : 1;           // bit 2
    unsigned char   ActivatedScript : 1;             // bit 3
    unsigned char   LevelIsUnloading : 1;            // bit 4
    unsigned char   IsAffectedByBulletTimeOverride : 1; // bit 5
    unsigned char   AddingTCsPermitted : 1;          // bit 6
    void OnLevelUnload();
};

void CThing::OnLevelUnload()
{
    LevelIsUnloading = true;
    AddingTCsPermitted = false;
    for (SComponentPair* it = m_componentsFirst; it != m_componentsLast; ++it) {
        CTCBase* tc = it->second;
        if (!tc->ToBeRemoved)
            tc->OnLevelUnload();
    }
    AddingTCsPermitted = true;
}
