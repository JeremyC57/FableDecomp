# wave 8 self-name fixes B: defining rows respelled to the Ego_r.pdb + FableWin.pdb access / const form, with their callers.
#   0049dd50 ?CopyFileInternal@CWorld@@IBEXABVCWideString@@0@Z (protected)       callers 004a0180 004a0ae0 004a0cd0 (CWorld members)
#   004a00e0 ?SaveHeroStateInternal@CWorld@@IAE_NABVCWideString@@_N@Z (protected) callers 004a0180 004a05c0 (CWorld members)
#   004b00c0 ?IsQuestRegistered@CQuestManager@@QBE_NABVCCharString@@@Z (const)   caller 00892f50
#   0063dfd0 ?UpdateAllEdges@CWorldMap@@IAEXXZ (protected)                        callers 004ff080 004ff440 (CWorldMap members)
#   006cb620 ?IsAbleToNavigateFromPosToPos@CTCCreatureNavigation@@QBE_NABVC3DVector@@0M@Z (const, references)
#            callers 0074a260 008c1600 (const); 006ea770 00970930 (pointer -> reference: the same pointers are pushed)
#   00a01920 ?GetSoundIndexFromSymbol@CASoundBank@@QBEJABVCCharString@@@Z (const) callers 006e2a50 0065c781
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all
def prot(a, k, line):
    edit(a, k, line, 'protected:\n' + line + 'public:\n')
# CopyFileInternal / SaveHeroStateInternal
prot('0049dd50', 'src', '    void CopyFileInternal(const CWideString& source, const CWideString& destination) const;\n')
prot('004a0180', 'src', '    void CopyFileInternal(const CWideString& source, const CWideString& destination) const;   // 0x0049DD50\n'
                        '    bool SaveHeroStateInternal(const CWideString& name, bool b);                            // 0x004A00E0\n')
prot('004a0ae0', 'src', '    void CopyFileInternal(const CWideString& source, const CWideString& destination) const;   // 0x0049DD50\n')
prot('004a0cd0', 'src', '    void CopyFileInternal(const CWideString& source, const CWideString& destination) const;   // 0x0049DD50\n')
prot('004a00e0', 'src', '    bool SaveHeroStateInternal(const CWideString& name, bool b);\n')
prot('004a05c0', 'src', '    bool SaveHeroStateInternal(const CWideString& name, bool flag);   // 0x004A00E0\n')
# IsQuestRegistered
edit('004b00c0', 'src', '    bool IsQuestRegistered(const CCharString& name);\n', '    bool IsQuestRegistered(const CCharString& name) const;\n')
edit('004b00c0', 'src', 'bool CQuestManager::IsQuestRegistered(const CCharString& name)\n', 'bool CQuestManager::IsQuestRegistered(const CCharString& name) const\n')
edit('004b00c0', 'tst', '    bool IsQuestRegistered(const CCharString& name);\n', '    bool IsQuestRegistered(const CCharString& name) const;\n')
edit('004b00c0', 'tst', 'bool CQuestManager::IsQuestRegistered(const CCharString& name)\n', 'bool CQuestManager::IsQuestRegistered(const CCharString& name) const\n')
edit('00892f50', 'src', '    bool IsQuestRegistered(const CCharString& questName);\n', '    bool IsQuestRegistered(const CCharString& questName) const;\n')
edit('00892f50', 'tst', '    bool IsQuestRegistered(const CCharString& questName);\n', '    bool IsQuestRegistered(const CCharString& questName) const;\n')
edit('00892f50', 'tst', 'bool CQuestManager::IsQuestRegistered(const CCharString& questName)\n', 'bool CQuestManager::IsQuestRegistered(const CCharString& questName) const\n')
# UpdateAllEdges
prot('0063dfd0', 'src', '    void UpdateAllEdges();\n')
prot('004ff080', 'src', '    void UpdateAllEdges();                                   // 0x0063DFD0\n')
prot('004ff440', 'src', '    void UpdateAllEdges();                      // 0x0063DFD0\n')
# IsAbleToNavigateFromPosToPos
edit('006cb620', 'src', '    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float proximity);\n',
     '    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float proximity) const;\n')
edit('006cb620', 'src', 'bool CTCCreatureNavigation::IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float proximity)\n',
     'bool CTCCreatureNavigation::IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float proximity) const\n')
edit('006cb620', 'tst', '    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float proximity);\n',
     '    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float proximity) const;\n')
edit('006cb620', 'tst', 'bool CTCCreatureNavigation::IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float proximity)\n',
     'bool CTCCreatureNavigation::IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float proximity) const\n')
for a in ('0074a260', '008c1600'):
    edit(a, 'src', '    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float tolerance);   // 0x006CB620',
         '    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float tolerance) const;   // 0x006CB620')
edit('006ea770', 'src', '    bool IsAbleToNavigateFromPosToPos(const C3DVector* from, const C3DVector* to, float tolerance);   // 0x006CB620',
     '    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float tolerance) const;   // 0x006CB620')
edit('006ea770', 'src', 'navigation->IsAbleToNavigateFromPosToPos(&creature.GetPos(), &candidate, 0.5f)',
     'navigation->IsAbleToNavigateFromPosToPos(creature.GetPos(), candidate, 0.5f)')
edit('006ea770', 'tst', '    bool IsAbleToNavigateFromPosToPos(const C3DVector* from, const C3DVector* to, float tolerance);   // 0x006CB620',
     '    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float tolerance) const;   // 0x006CB620')
edit('006ea770', 'tst', 'navigation->IsAbleToNavigateFromPosToPos(&creature.GetPosition(), &candidate, 0.5f)',
     'navigation->IsAbleToNavigateFromPosToPos(creature.GetPosition(), candidate, 0.5f)')
edit('006ea770', 'tst', 'bool CTCCreatureNavigation::IsAbleToNavigateFromPosToPos(const C3DVector* from, const C3DVector* to, float tolerance)\n{\n'
     '    if (g_navCalls < 16) g_navTo[g_navCalls] = *to;\n    g_navFrom = from;',
     'bool CTCCreatureNavigation::IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float tolerance) const\n{\n'
     '    if (g_navCalls < 16) g_navTo[g_navCalls] = to;\n    g_navFrom = &from;')
edit('00970930', 'src', '    bool IsAbleToNavigateFromPosToPos(const C3DVector* from, const C3DVector* to, float tolerance);   // 0x006CB620',
     '    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float tolerance) const;   // 0x006CB620')
edit('00970930', 'src', 'navigation->IsAbleToNavigateFromPosToPos(&GetCreature()->GetPos(), &result, 2.0f)',
     'navigation->IsAbleToNavigateFromPosToPos(GetCreature()->GetPos(), result, 2.0f)')
# GetSoundIndexFromSymbol
edit('00a01920', 'src', '    long GetSoundIndexFromSymbol(const CCharString& symbol);\n', '    long GetSoundIndexFromSymbol(const CCharString& symbol) const;\n')
edit('00a01920', 'src', 'long CASoundBank::GetSoundIndexFromSymbol(const CCharString& symbol)\n', 'long CASoundBank::GetSoundIndexFromSymbol(const CCharString& symbol) const\n')
edit('006e2a50', 'src', '    long GetSoundIndexFromSymbol(const CCharString& symbol);    // 0x00A01920', '    long GetSoundIndexFromSymbol(const CCharString& symbol) const;    // 0x00A01920')
edit('006e2a50', 'tst', '    long GetSoundIndexFromSymbol(const CCharString& symbol);    // 0x00A01920', '    long GetSoundIndexFromSymbol(const CCharString& symbol) const;    // 0x00A01920')
edit('006e2a50', 'tst', 'long CASoundBank::GetSoundIndexFromSymbol(const CCharString& s) {', 'long CASoundBank::GetSoundIndexFromSymbol(const CCharString& s) const {')
edit('0065c781', 'src', '    long GetSoundIndexFromSymbol(const CCharString& symbol);   // 0x00A01920', '    long GetSoundIndexFromSymbol(const CCharString& symbol) const;   // 0x00A01920')
