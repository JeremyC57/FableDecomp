// CPhysicsMesh::GetNavigationLevelAt(const C3DVector& position) -- the level number of the first navigation-level record (16 bytes: level, containment volume) whose volume contains the position, else 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A73D10, length 88 bytes.
// Callees: the record volume's containment test @ 0x00A2E710 (`__thiscall`, one `const C3DVector&`,
// bool result; the manifest label `_Cons_val<...>` is a propagated template name).
// Shape: the record array lives at +0x54/+0x58 (count = byte difference `sar 4`); the count is
// tested for zero before the counted loop (whose own `jle` entry test survives); the array start
// is re-read from `this` every iteration (the call may change it; `this` spilled to the `push ecx`
// slot under register pressure), while the found record is a reference local so its level is read
// back through the same pointer after the call (`mov eax,[esi]`).
struct C3DVector { float x, y, z; };
struct CContainmentVolume_a2e710 {
    unsigned char _body[0xc];
    bool IsContaining(const C3DVector& position) const;   // 0x00A2E710
};
struct SNavigationLevel {                          // pair<long, C3DMeshVolume>, 16 bytes
    long                       m_level;            // +0x00
    CContainmentVolume_a2e710  m_volume;           // +0x04
};

struct CPhysicsMesh {
    unsigned char     _pad00[0x54];
    SNavigationLevel* m_pLevelsFirst;              // +0x54 (NavigationContainmentVolumes, PDB +0x68)
    SNavigationLevel* m_pLevelsLast;               // +0x58
    long GetNavigationLevelAt(const C3DVector& position) const;
};

long CPhysicsMesh::GetNavigationLevelAt(const C3DVector& position) const
{
    long count = m_pLevelsLast - m_pLevelsFirst;
    if (count != 0) {
        for (long i = 0; i < count; ++i) {
            const SNavigationLevel& level = m_pLevelsFirst[i];
            if (level.m_volume.IsContaining(position))
                return level.m_level;
        }
    }
    return 0;
}
