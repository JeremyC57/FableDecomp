// CThing::SendMessageW(CMessage* message) -- forbid adding components, pass a message to every live thing component, then permit adding components again
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7820, 73 bytes. Callees: CTCBase virtual slot 27 (+0x6c) on each
// component not marked ToBeRemoved. (The `W` is the windows.h SendMessage macro baked into the original name.)
// Shape: AddingTCsPermitted is bit 6 of the +0x93 flag byte (`and cl,0BFh` store, `or byte ptr [..],40h` restore);
// Components {type, component} pairs at retail +0x44/+0x48; the message argument is loaded into ebx after the
// entry test and the last pointer re-read after each call.
struct CTCBase;
struct CMessage;

struct SComponentPair {                              // pair<ETCInterfaceType, CTCBase*>
    long     first;
    CTCBase* second;
};

struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26();
    virtual void HandleMessage(CMessage* message);   // slot 27 (+0x6c)
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
    void SendMessageW(CMessage* message);
};

void CThing::SendMessageW(CMessage* message)
{
    AddingTCsPermitted = false;
    for (SComponentPair* it = m_componentsFirst; it != m_componentsLast; ++it) {
        CTCBase* tc = it->second;
        if (!tc->ToBeRemoved)
            tc->HandleMessage(message);
    }
    AddingTCsPermitted = true;
}
