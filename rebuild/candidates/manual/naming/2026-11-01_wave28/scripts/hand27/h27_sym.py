# h27_sym.py: naming wave 27, family sym (chunk 695 queue rows 0x009E5C30 / 0x009E5350) -- the CSymbolMap binders on the Ego_r.pdb /
#  FableWin.pdb spellings chunk 695 proved: ?UpdateDirty@CSymbolMap@@QBEXXZ 0x009E5C30 (no Finalise in either PDB; 0x009AEF60 /
#  0x009CC530 are in family dm) and ?DeallocateUnusedMemory@CSymbolMap@@QAEXXZ 0x009E5350 (no RemoveUnusedMemory on CSymbolMap in
#  either PDB); 0x009AE5F0's own Leaf takes its PDB access (?RemoveUnusedMemory@CDefinitionManager@@IAEXXZ, bound by 0x009B08C0).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
def note(a, txt, leaf=None):
    p, rel, t = he._rw(a, 'src')
    if 'family sym' in t: return
    L = t.split('\n'); i = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')]
    if leaf:
        if i: L[i[0]] = '// Leaf: ' + leaf
        else: L.insert(1, '// Leaf: ' + leaf); i = [1]
    L.insert((i[0] + 1) if i else 1, '// Naming wave 27 (family sym, chunk 695): ' + txt); he._w(p, '\n'.join(L))
UD = 'CSymbolMap::UpdateDirty() const 0x009E5C30 (Ego_r.pdb + FableWin.pdb\n//   ?UpdateDirty@CSymbolMap@@QBEXXZ); was an invented ?Finalise@CSymbolMap (no Finalise in either PDB).'
a = '009b0470'
he.edit(a, 'src', '    void Finalise();                                   // 0x009E5C30\n',
        '    void UpdateDirty() const;                          // 0x009E5C30 (Ego_r.pdb / FableWin.pdb)\n')
he.edit(a, 'src', '    PSymbolMap->Finalise();', '    PSymbolMap->UpdateDirty();')
note(a, UD)
a = '00ae85f0'
he.edit(a, 'src', '    void Finalise();                                                                // 0x009E5C30\n',
        '    void UpdateDirty() const;                                                       // 0x009E5C30 (Ego_r.pdb / FableWin.pdb)\n')
he.edit(a, 'src', '    SymbolMap.Finalise();', '    SymbolMap.UpdateDirty();')
note(a, UD)
a = '009ae5f0'
he.edit(a, 'src', '    void RemoveUnusedMemory();                     // 0x009E5350\n',
        '    void DeallocateUnusedMemory();                 // 0x009E5350 (Ego_r.pdb / FableWin.pdb)\n')
he.edit(a, 'src', '    PDefinitionSymbolMap->RemoveUnusedMemory();', '    PDefinitionSymbolMap->DeallocateUnusedMemory();')
he.edit(a, 'src', '''    CArray<CInstantiatedDefInfo*> InstantiatedDefs;   // +0xa4
    void RemoveUnusedMemory();
};''', '''    CArray<CInstantiatedDefInfo*> InstantiatedDefs;   // +0xa4
protected:
    void RemoveUnusedMemory();                     // protected (Ego_r.pdb / FableWin.pdb IAE)
};''')
note(a, 'the symbol map\'s call is CSymbolMap::DeallocateUnusedMemory 0x009E5350 (Ego_r.pdb +\n//   FableWin.pdb; was ?RemoveUnusedMemory@CSymbolMap, no such member), and RemoveUnusedMemory itself is protected (IAE, both PDBs;\n//   the spelling 0x009B08C0 binds). Was public (QAE).', '?RemoveUnusedMemory@CDefinitionManager@@IAEXXZ')
