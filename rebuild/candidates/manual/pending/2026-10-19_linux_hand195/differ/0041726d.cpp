// Grade: DIFFER(189v189) -- register swap only: retail keeps the package / new server frame in ebx and &CurrentServerFrame in edi, ours swaps them
// CMainGameComponent::UpdateFromEventPackageSet(const CGameEventPackageSet& set) -- run every event package newer than the input frame: process it, update the world and the display engine, advance the frame counters and report the frame/checksum/seed to the network client
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041726D, length 189 bytes (`ret 4`).
// Callees: CGameEventPackageSet::GetNoPackages @ 0x009F1750 and GetPackage(ulong) @ 0x009F1730 (labelled CInputEvent::GetKey /
// CMouse::GetEventFromStore), CMainGameComponent::ProcessEventPackage @ 0x00416670, CWorld::Update(CGameEventPackage*, bool) @ 0x0049DFB0,
// CDisplayEngine::ConfirmFeedbackGameEventPackage @ 0x00434A60, CMainGameComponent::PeekWorldChecksum @ 0x00416392,
// CNetworkClient::Update(long, ulong, ulong) @ 0x004AE9D0, CDisplayEngine::WorldUpdate @ 0x00434F60.
// Layout (Ego_r.pdb CMainGameComponent, retail-identical up to the client): PWorld +0x24, PDisplayEngine +0x28 (scoped_ptr),
// CurrentServerFrame +0x48, InputServerFrame +0x4C, Client +0x13AB8, WorldSeed +0x1613C; the caught-up flag byte at +0x161E1 has no PDB
// counterpart at that offset (the tail of the retail object differs), so it carries a descriptive name. No engine header for the component;
// engine/CGameEventPackageSet.h has no methods for the two out-of-line accessors, so the set is modelled locally.
// Size TU with a frame pointer (`#pragma optimize("y", off)` + `("s", on)`, as the rest of this module): the index lives in [ebp-4]
// (`and [ebp-4],0`), the bool in [ebp-8]; the new server frame is a copied local bound to the Dinkumware `max` (`a < b ? b : a`), its result
// a local (ebx) stored and then passed to the client.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CGameEventPackage {
    long          TimeStamp;                    // +0x00
    unsigned long NoEvents;                     // +0x04
};
struct CGameEventPackageSet {
    unsigned long GetNoPackages() const;                         // 0x009F1750
    CGameEventPackage* GetPackage(unsigned long index) const;    // 0x009F1730
};
struct CWorld {
    void Update(CGameEventPackage* package, bool caughtUp);      // 0x0049DFB0
};
struct CDisplayEngine {
    void ConfirmFeedbackGameEventPackage(CGameEventPackage* package);   // 0x00434A60
    void WorldUpdate();                                                 // 0x00434F60
};
struct CNetworkClient {
    void Update(long frame, unsigned long checksum, unsigned long seed);   // 0x004AE9D0
};
template<class T> struct scoped_ptr {
    T* ptr;
    T* operator->() const { return ptr; }
};
template<class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }
struct CMainGameComponent {
    unsigned char             _pad00[0x24];
    scoped_ptr<CWorld>         PWorld;          // +0x24
    scoped_ptr<CDisplayEngine> PDisplayEngine;  // +0x28
    unsigned char             _pad2c[0x1c];
    long                      CurrentServerFrame;   // +0x48
    long                      InputServerFrame;     // +0x4c
    unsigned char             _pad50[0x13ab8 - 0x50];
    CNetworkClient            Client;               // +0x13ab8
    unsigned char             _pad13ab9[0x1613c - 0x13ab9];
    unsigned long             WorldSeed;            // +0x1613c
    unsigned char             _pad16140[0x161e1 - 0x16140];
    bool                      CaughtUpWithInput;    // +0x161e1
    void ProcessEventPackage(CGameEventPackage* package);   // 0x00416670
    unsigned long PeekWorldChecksum();                      // 0x00416392
    void UpdateFromEventPackageSet(const CGameEventPackageSet& set);
};

void CMainGameComponent::UpdateFromEventPackageSet(const CGameEventPackageSet& set)
{
    for (unsigned long i = 0; i < set.GetNoPackages(); ++i) {
        CGameEventPackage* package = set.GetPackage(i);
        if (package->TimeStamp > InputServerFrame) {
            ProcessEventPackage(package);
            bool caughtUp = false;
            if (InputServerFrame == CurrentServerFrame) {
                caughtUp = true;
                CaughtUpWithInput = true;
            }
            PWorld->Update(package, caughtUp);
            PDisplayEngine->ConfirmFeedbackGameEventPackage(package);
            InputServerFrame = package->TimeStamp;
            long frame = package->TimeStamp;
            long serverFrame = Max(frame, CurrentServerFrame);
            CurrentServerFrame = serverFrame;
            Client.Update(serverFrame, PeekWorldChecksum(), WorldSeed);
            PDisplayEngine->WorldUpdate();
        }
    }
}
