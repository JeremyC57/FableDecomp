// CTCChest::CTCChest(CThing*) -- CTCContainerRewardHero(thing), own vtable, the open/close helper reset (closed, unit open/close times), ChestOpen cleared, the open-particle intelligent pointer constructed, then the parent thing's cutscene pause behaviour set to 2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007508D0, length 88 bytes (+ int3 padding).
// Callees: CTCContainerRewardHero::CTCContainerRewardHero(CThing*) @ 0x0074E6B0 (the manifest label
// "CTCActivationTrigger" is propagated: it installs vtable 0x01269424 = CTCContainerRewardHero),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (then the CIntelligentPointer vtable
// 0x01238C6C), CThing::SetCutscenePauseBehaviour @ 0x004C78A0. Vtable 0x0126957C = CTCChest.
// Layout = Ego_r.pdb CTCChest less 4 bytes (the retail CTCContainer vector is 12 bytes, so
// CTCContainerRewardHero is 0x1c): ChestOpener (COpenCloseHelper, 0x14 bytes) +0x1c, ChestOpen +0x30,
// POpenParticle +0x31 -- CIntelligentPointer is declared under pack(1) (alignment 1), which is why it
// sits unaligned right after the bool, as the PDB also records (+0x35 there).
// Shape: COpenCloseHelper is a member object with an inline constructor (all its stores follow the
// vtable store; the two 1.0f floats share ecx, the zeroes share eax); the pointer member is addressed
// once in edi; the cutscene call is the constructor body.
struct CThing {
    void SetCutscenePauseBehaviour(long behaviour);         // 0x004C78A0
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item pointer
    virtual void v00();
    void* m_pItem;                                          // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
};
struct CIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
#pragma pack(pop)

struct CTCContainerRewardHero_Calls {                       // retail 0x1c bytes (vtable 0x01269424)
    virtual void v00();
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    bool          ToBeRemoved;                              // +0x08
    bool          IsInGlobalUpdateTCs;                      // +0x09
    unsigned char _pad0a[2];
    void*         ContainerItems[3];                        // +0x0c CTCContainer: vector<pair<long, CCharString>>
    bool          Enabled;                                  // +0x18
    bool          CreateObjectsThatTimeOut;                 // +0x19
    CTCContainerRewardHero_Calls(CThing* thing);            // 0x0074E6B0
};

struct COpenCloseHelper {                                   // 0x14 bytes
    bool  Opened;                                           // +0x00
    bool  Closed;                                           // +0x01
    bool  Opening;                                          // +0x02
    bool  Closing;                                          // +0x03
    float OpenCloseTransitionAmount;                        // +0x04
    float LastOpenCloseTransitionAmount;                    // +0x08
    float TotalTimeToOpen;                                  // +0x0c
    float TotalTimeToClose;                                 // +0x10
    COpenCloseHelper()
    {
        Reset();
        TotalTimeToOpen = 1.0f;
        TotalTimeToClose = 1.0f;
    }
    void Reset()                                            // closed, no transition in progress
    {
        Opened = false;
        Closed = true;
        OpenCloseTransitionAmount = LastOpenCloseTransitionAmount = 0.0f;
        Opening = false;
        Closing = false;
    }
};

struct CTCChest : CTCContainerRewardHero_Calls {            // vtable 0x0126957C
    COpenCloseHelper    ChestOpener;                        // +0x1c
    bool                ChestOpen;                          // +0x30
    CIntelligentPointer POpenParticle;                      // +0x31
    virtual void v00();
    CTCChest(CThing* thing);
};

CTCChest::CTCChest(CThing* thing)
    : CTCContainerRewardHero_Calls(thing)
    , ChestOpener()
    , ChestOpen(false)
    , POpenParticle()
{
    ParentThing->SetCutscenePauseBehaviour(2);
}