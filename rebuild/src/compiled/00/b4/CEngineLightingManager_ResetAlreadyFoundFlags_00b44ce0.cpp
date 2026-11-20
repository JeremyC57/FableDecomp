// CEngineLightingManager::ResetAlreadyFoundFlags(long minX, long minY, long maxX, long maxY) -- clears the "already found" flag of every local light linked into the given rectangle of the 16x16 light lookup grid
// Leaf: ?ResetAlreadyFoundFlags@CEngineLightingManager@@AAEXJJJJ@Z
// Name fix (NF151, q4500): Ego_r.pdb 0x009B9B40 + FableWin.pdb 0x02CF1B50 ?ResetAlreadyFoundFlags@CEngineLightingManager@@AAEXJJJJ@Z (private);
//   the declaration in this row's existing local model moves under `private:` in place; both landed callers (GetLightsInSphere 0x00B45FB0,
//   GetLightsInArea 0x00B46280) already bind the AAE spelling.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B44CE0, length 107 bytes (112 with padding). No calls.
// Shape: the outer loop runs over the second coordinate (minY..maxY, inclusive `jle`, maxY re-read from its slot), the
// inner over the first (minX..maxX); VC strength-reduces the inner loop into a down-counter (`maxX - minX + 1`) and a
// cell pointer stepping one grid row (0x40 bytes), so the grid is indexed [x][y]; each cell heads a singly linked list of
// CLocalLightElementLink {+4 element, +8 next} and the element's bool at +0x44 is cleared. `this` is spilled into the
// `push ecx` slot (all registers are busy).
// Layout: the PDB puts LightLookupGrid at +0xF8; retail CArray is 12 bytes (PDB 16) and three CArrays precede the grid,
// so it sits at +0xEC here -- the class is modelled locally.
class CEngineLocalLightElement { public:
    unsigned char _pad00[0x44];
    bool          m_alreadyFound;                  // +0x44
};

struct CLocalLightElementLink {
    void*                     m_pUnknown;          // +0x00
    CEngineLocalLightElement* m_pElement;          // +0x04
    CLocalLightElementLink*   m_pNext;             // +0x08
};

class CEngineLightingManager { public:
    unsigned char           _pad000[0xec];
    CLocalLightElementLink* m_lightLookupGrid[16][16];   // +0xEC
private:
    void ResetAlreadyFoundFlags(long minX, long minY, long maxX, long maxY);
};

void CEngineLightingManager::ResetAlreadyFoundFlags(long minX, long minY, long maxX, long maxY)
{
    for (long y = minY; y <= maxY; ++y) {
        for (long x = minX; x <= maxX; ++x) {
            for (CLocalLightElementLink* link = m_lightLookupGrid[x][y]; link; link = link->m_pNext)
                link->m_pElement->m_alreadyFound = false;
        }
    }
}