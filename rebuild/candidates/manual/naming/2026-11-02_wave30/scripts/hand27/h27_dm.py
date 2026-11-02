# h27_dm.py: naming wave 27, family dm (DECISIONS_W26_RESOLVED D5, parameter-type rows) -- CDefinitionManager's two path-list
#  passes on their Ego_r.pdb / FableWin.pdb spellings (identical in both; the callers 0x009AF6E0 / 0x009AF720 / 0x009B08C0 already
#  bind them): ?CreateSymbolsFromPathList@CDefinitionManager@@IAEXABV?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@_N@Z
#  (protected, the flag a bool passed on to ?ParseStringForSymbols@CDefinitionManager@@IAEXPBD_N@Z 0x009ADE90) and
#  ?DoCompilePathList@CDefinitionManager@@IAEXABV?$vector@...@std@@ABVCCharString@@@Z (protected, the def type a const
#  CCharString& passed on to ?CompileString@CDefinitionManager@@IAEXPBDABVCCharString@@@Z 0x009AF1E0, as its own row defines).
#  Retail: both pass-throughs are `mov edx,[esp+0x6c]; push edx` (the same push for a bool, a long or a reference).
#  The own row 0x009ADE90 ParseStringForSymbols takes its PDB access too (IAE; it defined QAE).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
NOTE = '// Naming wave 27 (family dm, DECISIONS_W26 D5): %s'
def note(a, txt, leaf=None):
    p, rel, t = he._rw(a, 'src')
    if 'family dm' in t: return
    L = t.split('\n'); i = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')]
    if leaf:
        if i: L[i[0]] = '// Leaf: ' + leaf
        else: L.insert(1, '// Leaf: ' + leaf); i = [1]
    L.insert((i[0] + 1) if i else 1, NOTE % txt); he._w(p, '\n'.join(L))
a = '009aef60'
he.edit(a, 'src', '''    void ParseStringForSymbols(char* text, long flags);   // 0x009ADE90
    void CreateSymbolsFromPathList(const std::vector<CWideString>& paths, long flags);
};

void CDefinitionManager::CreateSymbolsFromPathList(const std::vector<CWideString>& paths, long flags)''', '''protected:                                              // Ego_r.pdb / FableWin.pdb IAE
    void ParseStringForSymbols(const char* text, bool instantiate);   // 0x009ADE90
    void CreateSymbolsFromPathList(const std::vector<CWideString>& paths, bool instantiate);
};

void CDefinitionManager::CreateSymbolsFromPathList(const std::vector<CWideString>& paths, bool instantiate)''')
he.edit(a, 'src', '            ParseStringForSymbols(buffer, flags);', '            ParseStringForSymbols(buffer, instantiate);')
note(a, 'CreateSymbolsFromPathList is protected with a bool flag, passed on to\n//   the protected ParseStringForSymbols(const char*, bool) 0x009ADE90 (both PDBs). Was public with a long flag (QAE ..J).',
     '?CreateSymbolsFromPathList@CDefinitionManager@@IAEXABV?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@_N@Z')
a = '009af480'
he.edit(a, 'src', '''    void ParseStringForSymbols(char* text, long flags);   // 0x009ADE90
    void CompileString(char* text, long flags);         // 0x009AF1E0
    void DoCompilePathList(const std::vector<CWideString>& paths, long flags);
};

void CDefinitionManager::DoCompilePathList(const std::vector<CWideString>& paths, long flags)''', '''protected:                                              // Ego_r.pdb / FableWin.pdb IAE
    void CompileString(const char* text, const CCharString& defType);   // 0x009AF1E0
    void DoCompilePathList(const std::vector<CWideString>& paths, const CCharString& defType);
};

void CDefinitionManager::DoCompilePathList(const std::vector<CWideString>& paths, const CCharString& defType)''')
he.edit(a, 'src', '            CompileString(buffer, flags);', '            CompileString(buffer, defType);')
note(a, 'DoCompilePathList is protected with a const CCharString& def type,\n//   passed on to the protected CompileString(const char*, const CCharString&) 0x009AF1E0 (both PDBs; its own row\'s spelling).\n//   Was public with a long flag (QAE ..J).',
     '?DoCompilePathList@CDefinitionManager@@IAEXABV?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@ABVCCharString@@@Z')
a = '009ade90'
he.edit(a, 'src', '''    void ParseEnumForSymbols(CStringParser& parser);   // 0x009ACEE0
    void ParseStringForSymbols(const char* text, bool instantiate);
};''', '''    void ParseEnumForSymbols(CStringParser& parser);   // 0x009ACEE0
protected:
    void ParseStringForSymbols(const char* text, bool instantiate);   // protected (Ego_r.pdb / FableWin.pdb IAE)
};''')
note(a, 'ParseStringForSymbols is protected in both PDBs (IAE). Was public (QAE).', '?ParseStringForSymbols@CDefinitionManager@@IAEXPBD_N@Z')

# 0x009E5C30 is ?UpdateDirty@CSymbolMap@@QBEXXZ (Ego_r.pdb + FableWin.pdb; chunk 695 landed its own row); no Finalise in either PDB.
#  0x009AEF60's two callees are the CDefinitionManager members PSymbolMap / PDefinitionSymbolMap (Ego_r CCountedPointer<CSymbolMap>,
#  retail +0x58 / +0x60: the pointer word of each), and 0x009CC530 CBankFile::CreateSymbolMap names the same callee.
a = '009aef60'
he.edit(a, 'src', '''struct CSymbolTable { void Finalise(); };              // 0x009E5C30
''', '''class CSymbolMap { public: void UpdateDirty() const; };   // 0x009E5C30 (Ego_r.pdb / FableWin.pdb ?UpdateDirty@CSymbolMap@@QBEXXZ)
''')
he.edit(a, 'src', '''    CSymbolTable* Symbols58;                            // +0x58
    unsigned char _pad05c[4];
    CSymbolTable* Symbols60;                            // +0x60''', '''    CSymbolMap*   PSymbolMap;                           // +0x58 CCountedPointer<CSymbolMap> PSymbolMap (its pointer word)
    unsigned char _pad05c[4];
    CSymbolMap*   PDefinitionSymbolMap;                 // +0x60 CCountedPointer<CSymbolMap> PDefinitionSymbolMap''')
he.edit(a, 'src', '''    Symbols58->Finalise();
    Symbols60->Finalise();''', '''    PSymbolMap->UpdateDirty();
    PDefinitionSymbolMap->UpdateDirty();''')
p, rel, t = he._rw(a, 'src')
if 'UpdateDirty 0x009E5C30' not in t:
    he._w(p, t.replace('//   the protected ParseStringForSymbols(const char*, bool) 0x009ADE90 (both PDBs). Was public with a long flag (QAE ..J).',
        '//   the protected ParseStringForSymbols(const char*, bool) 0x009ADE90 (both PDBs). Was public with a long flag (QAE ..J).\n'
        '//   The two symbol maps (Ego_r PSymbolMap / PDefinitionSymbolMap) end with CSymbolMap::UpdateDirty 0x009E5C30 (both PDBs,\n'
        '//   const; chunk 695), which this row had spelled ?Finalise@CSymbolTable (no Finalise in either PDB).', 1))
a = '009cc530'
he.edit(a, 'src', '    void Finalise(); //009E5C30\n', '    void UpdateDirty() const;   // 0x009E5C30 (Ego_r.pdb / FableWin.pdb ?UpdateDirty@CSymbolMap@@QBEXXZ)\n')
he.edit(a, 'src', '    map.Finalise();', '    map.UpdateDirty();')
p, rel, t = he._rw(a, 'src')
if 'family dm' not in t:
    L = t.split('\n'); L.insert(2, '// Naming wave 27 (family dm, chunk 695): 0x009E5C30 is CSymbolMap::UpdateDirty() const (Ego_r.pdb + FableWin.pdb); was\n//   an invented ?Finalise@CSymbolMap (no Finalise in either PDB).'); he._w(p, '\n'.join(L))
