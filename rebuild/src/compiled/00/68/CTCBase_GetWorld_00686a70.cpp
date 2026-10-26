// CTCBase::GetWorld() const -- the world through the global thing manager (CThingManager::World, retail +0x24)
// Leaf: ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686A70, 9 bytes: mov eax,[0x013B8A1C] ; mov eax,[eax+0x24] ; ret (this unused).
// Name: Ego_r.pdb lists ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ right before ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ; the
//   identical twin at 0x00686A80 is PeekWorld (its results only reach const CWorld members), while this one's results
//   reach non-const CWorld members (HandleMoveHeroToRegionGameEvent, TeleportHeroToHSP). Callers are CTC* component
//   methods. The landed free-function shape ("CViewBase::m_pDisplayEngine") was a propagated label.
// Data: 0x013B8A1C is the CThingManager pointer (descriptive name; no PDB name); World at retail +0x24 (Ego_r +0x30).
class CWorld;

class CThingManager {
public:
    unsigned char _unknown[0x24];
    CWorld* World;                                    // +0x24
};

extern CThingManager* g_pThingManager;                // 0x013B8A1C

class CTCBase {
protected:
    CWorld& GetWorld() const;
};

CWorld& CTCBase::GetWorld() const
{
    return *g_pThingManager->World;
}
