# h27_acc.py: naming wave 27, family acc (DECISIONS_W26_RESOLVED D5, access-only) -- the own Leafs wave 26 touched name-only that
#  keep a pre-wave access or const qualifier the PDB spells otherwise (Ego_r.pdb + FableWin.pdb, identical in both), with their
#  same-class callers: 0x0071CE80 GetTattooNames QBE -> QAE (no landed caller), 0x007FECB0 UpdateBlockedActionPoints QAE -> IAE
#  (caller 0x007FEDE0 GetFreeActionPoint), 0x005FC620 FindTeleports QAE -> AAE (caller 0x006116E0 OpenInventory). The
#  0x009A7720 / 0x00A60AA0 / 0x0098A9A0 access respells are in families fdosw / pxs.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
NOTE = '// Naming wave 27 (family acc, DECISIONS_W26 D5): %s'
def note(a, txt, leaf=None):
    p, rel, t = he._rw(a, 'src')
    if 'family acc' in t: return
    L = t.split('\n'); i = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')]
    if leaf:
        if i: L[i[0]] = '// Leaf: ' + leaf
        else: L.insert(1, '// Leaf: ' + leaf); i = [1]
    L.insert((i[0] + 1) if i else 1, NOTE % txt); he._w(p, '\n'.join(L))
a = '0071ce80'
he.edit(a, 'src', '    void GetTattooNames(std::vector<CWideString>& names) const;\n};',
        '    void GetTattooNames(std::vector<CWideString>& names);   // non-const (Ego_r.pdb / FableWin.pdb QAE)\n};')
he.edit(a, 'src', 'void CTCHeroMorph::GetTattooNames(std::vector<CWideString>& names) const\n', 'void CTCHeroMorph::GetTattooNames(std::vector<CWideString>& names)\n')
note(a, 'GetTattooNames is non-const in both PDBs (QAE). Was const (QBE).',
     '?GetTattooNames@CTCHeroMorph@@QAEXAAV?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@@Z')
a = '007fecb0'
he.edit(a, 'src', '''    CIntelligentPointer<CThing> PThing;             // +0x2c
    void UpdateBlockedActionPoints();
};''', '''    CIntelligentPointer<CThing> PThing;             // +0x2c
protected:
    void UpdateBlockedActionPoints();               // protected (Ego_r.pdb / FableWin.pdb IAE)
};''')
note(a, 'UpdateBlockedActionPoints is protected in both PDBs (IAE). Was public (QAE).', '?UpdateBlockedActionPoints@CTCUserActionPointBase@@IAEXXZ')
a = '007fede0'
he.edit(a, 'src', '''    void UpdateBlockedActionPoints();                        // 0x007FECB0
    bool GetFreeActionPoint(''', '''protected:
    void UpdateBlockedActionPoints();                        // 0x007FECB0 (protected: Ego_r.pdb / FableWin.pdb IAE)
public:
    bool GetFreeActionPoint(''')
note(a, 'the callee UpdateBlockedActionPoints 0x007FECB0 is protected in both PDBs (IAE).')
a = '005fc620'
he.edit(a, 'src', '''    long          NumberOfRegions;                    // +0x23c
    void FindTeleports();
};''', '''    long          NumberOfRegions;                    // +0x23c
private:
    void FindTeleports();                             // private (Ego_r.pdb / FableWin.pdb AAE)
};''')
note(a, 'FindTeleports is private in both PDBs (AAE). Was public (QAE).', '?FindTeleports@CTCInventoryMap@@AAEXXZ')
a = '006116e0'
he.edit(a, 'src', '''    void FindTeleports();                               // 0x005FC620
    NUISystem::CComponent* BuildMapMenu''', '''private:
    void FindTeleports();                               // 0x005FC620 (private: Ego_r.pdb / FableWin.pdb AAE)
public:
    NUISystem::CComponent* BuildMapMenu''')
note(a, 'the callee FindTeleports 0x005FC620 is private in both PDBs (AAE).')
