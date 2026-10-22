// CDisplayManager::CalculateD3DPresentationInterval(const CPresentationIntervalDesc& desc) -- map a presentation-interval description to the D3DPRESENT_INTERVAL_* flag: immediate when asked (or for an unknown count), else ONE/TWO/THREE/FOUR by VSyncCount (0 counts as one)
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BE2D0, 84 bytes incl. the 5-entry jump table, `ret 4`. Called from
//   CDisplayManager's device-creation path (0x009BF90E, 0x009BF91F, 0x009BFCDA) with `this` in ecx.
// Callees: none.
// Shape: `cmp byte ptr [eax],0 ; jne immediate`, then the VSyncCount switch (`cmp eax,4 ; ja` + table, cases 0 and 1
//   share the ONE arm); every arm returns its constant directly (`mov eax,K ; ret 4`). `this` is unused.
//   CPresentationIntervalDesc as in the landed ChangePresentationInterval 0x009BFE70 (PresentImmediate +0,
//   PresentImmediateIfSyncMissed +1, VSyncCount +4). An earlier catalog cut of this row (wave 25, chunk 199) held two more
//   bodies; the row is now exactly this function and its table.
struct CPresentationIntervalDesc {
    bool PresentImmediate;                         // +0x00
    bool PresentImmediateIfSyncMissed;             // +0x01
    long VSyncCount;                               // +0x04
};

enum {
    D3DPRESENT_INTERVAL_ONE       = 0x00000001,
    D3DPRESENT_INTERVAL_TWO       = 0x00000002,
    D3DPRESENT_INTERVAL_THREE     = 0x00000004,
    D3DPRESENT_INTERVAL_FOUR      = 0x00000008,
    D3DPRESENT_INTERVAL_IMMEDIATE = 0x80000000
};

class CDisplayManager {
public:
    unsigned long CalculateD3DPresentationInterval(const CPresentationIntervalDesc& desc);
};

unsigned long CDisplayManager::CalculateD3DPresentationInterval(const CPresentationIntervalDesc& desc)
{
    if (!desc.PresentImmediate) {
        switch (desc.VSyncCount) {
        case 0:
        case 1:
            return D3DPRESENT_INTERVAL_ONE;
        case 2:
            return D3DPRESENT_INTERVAL_TWO;
        case 3:
            return D3DPRESENT_INTERVAL_THREE;
        case 4:
            return D3DPRESENT_INTERVAL_FOUR;
        }
    }
    return D3DPRESENT_INTERVAL_IMMEDIATE;
}