// CGraphicBankManager::ProcessLoading(bool flag, double time) -- two passes over the banks (sentinel at +4, bank at the node's +0x38): first each bank's pending asynchronous updates (0x009D68B0), then each bank's loading queue processed with the flag and the time (0x009FC410)
// Leaf: ?ProcessLoading@CGraphicBankManager@@QAEX_NN@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F8230, length 96 bytes.
// Callees: CBankFileAsync::AsyncUpdatePending @ 0x009D68B0, CResourceBank::ProcessLoadingQueue(bool, double) @ 0x009FC410 (on the
// bank's CResourceBank base, +0x164). Shape: two sentinel walks, the flag in ebx, the double re-read from the argument each iteration
// and pushed as two words, `ret 0Ch`.
// Name (round 7, NF53): Ego_r.pdb ?ProcessLoading@CGraphicBankManager@@QAEX_NN@Z (0x00B4B690), ?AsyncUpdatePending@CBankFileAsync@@QAEXXZ
//   (Ego_r 0x00B2ABD0 / FableWin 0x02FF6B50) and ?ProcessLoadingQueue@CResourceBank@@QAEX_NN@Z (engine/CResourceBank.h); the landed
//   CEngineResourceManager::ProcessLoading 0x00B3B7F0 calls this with (bool, double). Was a local CGraphicDataBank_009f8200 /
//   CResourceList_009f8230 model (StepLoading / ProcessLoading(long, double)); now engine/CGraphicDataBank.h (CBankFileAsync +
//   CResourceBank bases) with the overlay opt-in FABLE_RETAIL_CBANKFILEASYNC_ASYNCUPDATEPENDING. CGraphicBankManager has no shared
//   class body (engine/CSystemManager.h names it only); its bank list stays a local node view.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEASYNC_ASYNCUPDATEPENDING
#include "fable_string.h"
#include "engine/CGraphicDataBank.h"

struct SBankNode_009f8200 {                        // a list node: next, prev, the payload
    SBankNode_009f8200* m_pNext;
    SBankNode_009f8200* m_pPrev;
    unsigned char       _pad08[0x30];
    CGraphicDataBank*   m_pBank;                   // +0x38 (after the entry record)
};
class CGraphicBankManager { public:
    unsigned char       _pad[4];
    SBankNode_009f8200* m_pHead;                   // +0x04 (the sentinel)
    void ProcessLoading(bool flag, double time);
};

void CGraphicBankManager::ProcessLoading(bool flag, double time)
{
    SBankNode_009f8200* node;
    for (node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext)
        node->m_pBank->AsyncUpdatePending();
    for (node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext)
        node->m_pBank->ProcessLoadingQueue(flag, time);
}
