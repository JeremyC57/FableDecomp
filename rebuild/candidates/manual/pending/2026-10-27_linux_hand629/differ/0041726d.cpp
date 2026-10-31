// CMainGameComponent::UpdateFromEventPackageSet(const CGameEventPackageSet* set) -- run every event package newer than the input frame: process it, update the world and the display engine, advance the frame counters and report the frame/checksum/seed to the network client
// Grade: DIFFER(189v189) -- register swap only: retail keeps the package (then the new server frame) in ebx and &CurrentServerFrame in edi, ours swaps them (same instructions otherwise)
// Leaf: ?UpdateFromEventPackageSet@CMainGameComponent@@IAEXPBVCGameEventPackageSet@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041726D, length 189 bytes (`ret 4`).
// Identity: Ego_r.pdb ?UpdateFromEventPackageSet@CMainGameComponent@@IAEXPBVCGameEventPackageSet@@@Z (protected, the set by pointer).
// Callees (Ego_r.pdb spellings): CGameEventPackageSet::GetNoPackages() const 0x009F1750 and GetPackage(unsigned long) const
//   0x009F1730 (catalog labels CInputEvent::GetKey / CMouse::GetEventFromStore are ICF-propagated),
//   CMainGameComponent::ProcessEventPackage(const CGameEventPackage*) 0x00416670 (protected), CWorld::Update(const
//   CGameEventPackage&, bool) 0x0049DFB0, CDisplayEngine::ConfirmFeedbackGameEventPackage(const CGameEventPackage*) 0x00434A60,
//   CMainGameComponent::PeekWorldChecksum() const 0x00416392, CNetworkClient::Update(long, unsigned long, unsigned long)
//   0x004AE9D0, CDisplayEngine::WorldUpdate() 0x00434F60.
// Layout (Ego_r.pdb CMainGameComponent, retail-identical up to the client): PWorld +0x24, PDisplayEngine +0x28 (scoped_ptr),
//   CurrentServerFrame +0x48, InputServerFrame +0x4C, Client +0x13AB8, WorldSeed +0x1613C; the caught-up flag byte at +0x161E1
//   has no PDB counterpart at that offset (the tail of the retail object differs), so it carries a descriptive name. No engine
//   header for the component; engine/CGameEventPackageSet.h has no methods for the two out-of-line accessors.
// Size TU with a frame pointer (optimize "y" off + "s" on, as the rest of this module): the index lives in [ebp-4]
//   (`and [ebp-4],0`), the bool in [ebp-8]; the new server frame is a copied local bound to the Dinkumware-style `max`
//   (`a < b ? b : a`), its result a local (ebx) stored and then passed to the client.
// Tried (sweep): every inline ternary accessor as two returns. Chunk 629: the PDB signatures (set by pointer, const package
//   pointers, CWorld::Update by reference), a `long&` local for CurrentServerFrame, the package pointer declared outside the
//   loop, `InputServerFrame < package->TimeStamp`, the Max result stored straight into the member (190) -- the swap stays.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CGameEventPackage { public:
    long          TimeStamp;                    // +0x00
    unsigned long NoEvents;                     // +0x04
};
class CGameEventPackageSet { public:
    unsigned long GetNoPackages() const;                             // 0x009F1750
    const CGameEventPackage* GetPackage(unsigned long index) const;  // 0x009F1730
};
class CWorld { public:
    void Update(const CGameEventPackage& package, bool caughtUp);   // 0x0049DFB0
};
class CDisplayEngine { public:
    void ConfirmFeedbackGameEventPackage(const CGameEventPackage* package);   // 0x00434A60
    void WorldUpdate();                                                 // 0x00434F60
};
class CNetworkClient { public:
    void Update(long frame, unsigned long checksum, unsigned long seed);   // 0x004AE9D0
};
template<class T> struct scoped_ptr {
    T* ptr;
    T* operator->() const { return ptr; }
};
template<class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }
class CMainGameComponent { public:
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
    unsigned long PeekWorldChecksum() const;                // 0x00416392
protected:
    void ProcessEventPackage(const CGameEventPackage* package);   // 0x00416670
    void UpdateFromEventPackageSet(const CGameEventPackageSet* set);
};

void CMainGameComponent::UpdateFromEventPackageSet(const CGameEventPackageSet* set)
{
    for (unsigned long i = 0; i < set->GetNoPackages(); ++i) {
        const CGameEventPackage* package = set->GetPackage(i);
        if (package->TimeStamp > InputServerFrame) {
            ProcessEventPackage(package);
            bool caughtUp = false;
            if (InputServerFrame == CurrentServerFrame) {
                caughtUp = true;
                CaughtUpWithInput = true;
            }
            PWorld->Update(*package, caughtUp);
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
