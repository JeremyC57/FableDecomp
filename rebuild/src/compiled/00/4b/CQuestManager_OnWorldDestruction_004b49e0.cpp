// CQuestManager::OnWorldDestruction() -- deactivate every quest, drop all boasts (stamping the boast frame) and mark the world as destructed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B49E0, length 41 bytes.
// Callees: CQuestManager::DeactivateAllQuests @ 0x004B4450, list<CBoast>::clear @ 0x004B8C00 (x2; manifest
// "CGuiControlTreePane::SortTreeRecursively" is propagated), GFGetCurrentFrame @ 0x0049D870 (returns the frame
// counter @ 0x013B89BC; its "GFGetPreMainMemoryUsed" label is propagated).
// Identity: FableWin.pdb ?OnWorldDestruction@CQuestManager@@QAEXXZ. The middle is CQuestManager::RemoveAllBoasts
// (landed @ 0x004AFF80: the same two clears and the frame store) inlined.
// Layout (retail): FrameBoastsLastUpdated +0x64, Boasts +0x68, AcceptedBoasts +0x6c (lists of one head pointer),
// WorldDestructed +0x8e (PDB +0xa0/+0xa4/+0xb0/+0xea).
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};               // 0x0049D870

struct CBoastList {                                // list<CBoast>: one head-node pointer
    void* m_pHead;
    void  clear();                                 // 0x004B8C00
};

class CQuestManager { public:
    unsigned char _pad_0x00[0x64];
    unsigned long FrameBoastsLastUpdated;          // +0x64
    CBoastList    Boasts;                          // +0x68
    CBoastList    AcceptedBoasts;                  // +0x6c
    unsigned char _pad_0x70[0x1e];
    bool          WorldDestructed;                 // +0x8e
    void DeactivateAllQuests();                    // 0x004B4450
    void RemoveAllBoasts()                         // 0x004AFF80 out of line; inlined here
    {
        Boasts.clear();
        AcceptedBoasts.clear();
        FrameBoastsLastUpdated = CWorld::GetFrame();
    }
    void OnWorldDestruction();
};

void CQuestManager::OnWorldDestruction()
{
    DeactivateAllQuests();
    RemoveAllBoasts();
    WorldDestructed = true;
}