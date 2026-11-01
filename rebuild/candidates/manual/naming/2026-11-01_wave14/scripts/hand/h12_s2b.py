# sub-wave 2b (hand): PDB-proven callee respells on landed chunk rows. HE_OV=<overlay>
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all, resub
# ---- 0x0091EA70 ?IsAbleToFire@CAIStateGroup_CreatureRangedCombat2@@ABE_NXZ (Ego_r + FableWin: private const; chunk 633)
edit('0091eb60', 'src', '    bool IsAbleToFire();                            // 0x0091EA70\n    bool IsRunnable();\n};',
     'private:\n    bool IsAbleToFire() const;                      // 0x0091EA70 (Ego_r.pdb: private const)\npublic:\n    bool IsRunnable();\n};')
# ---- 0x0073ABA0 ?RemoveMode@CTCCreatureModeManager@@QAEXK@Z (Ego_r + FableWin; the landed definition's leaf)
edit('0093afc0', 'src', '    void RemoveMode(long type);                 // 0x0073ABA0', '    void RemoveMode(unsigned long type);        // 0x0073ABA0 (Ego_r.pdb: unsigned long)')
edit('0090ca60', 'src', 'struct CTCModes {                               // interface 0x31\n    void RemoveMode(unsigned long mode);        // 0x0073ABA0',
     'class CTCCreatureModeManager { public:         // interface 0x31 (Ego_r.pdb class name)\n    void RemoveMode(unsigned long mode);        // 0x0073ABA0')
edit_all('0090ca60', 'src', 'CTCModes* modes;', 'CTCCreatureModeManager* modes;')
resub('0090ca60', 'src', r'\(CTCModes\s*\*\)', '(CTCCreatureModeManager*)')
edit('0090ca60', 'src', 'CTCModes::RemoveMode @ 0x0073ABA0', 'CTCCreatureModeManager::RemoveMode @ 0x0073ABA0')
edit('0090ca60', 'src', '0x31 = CTCModes bit 17', '0x31 = CTCCreatureModeManager bit 17')
edit('008df3a0', 'src', 'struct CTCCreatureModeManager {                 // interface type 0x31\n    void RemoveMode(unsigned long mode);        // 0x0073ABA0\n};',
     '}   // namespace NCreatureMode -- CTCCreatureModeManager is a global class (Ego_r.pdb ?RemoveMode@CTCCreatureModeManager@@QAEXK@Z)\n'
     'class CTCCreatureModeManager { public:          // interface type 0x31\n    void RemoveMode(unsigned long mode);        // 0x0073ABA0\n};\nnamespace NCreatureMode {')
edit('008da890', 'src', 'manager->RemoveMode((this->*static_cast<ModeIdSlot*>(VTable)[1].OwnType)());',
     '((CTCCreatureModeManager*)manager)->RemoveMode((this->*static_cast<ModeIdSlot*>(VTable)[1].OwnType)());')
edit('008da890', 'src', 'struct ModeManager {ModeInterface&GetCurrentMode()const;void RemoveMode(unsigned long);};',
     'struct ModeManager {ModeInterface&GetCurrentMode()const;};\nclass CTCCreatureModeManager { public: void RemoveMode(unsigned long); };   // 0x0073ABA0 (Ego_r.pdb)')
# ---- CModeBase virtual slots 5/6 are OnActivate / OnDeactivate (vtable 0x012673CC slot 5 = 0x0073AAC0, the landed
#      ?OnActivate@CModeBase@CTCCreatureModeManager@@UAEXXZ row; Ego_r + FableWin declare OnActivate, OnDeactivate in that order)
edit('0073aba0', 'src', '    virtual void OnResume();                            // slot 5 (+0x14)\n    virtual void OnLeave();                             // slot 6 (+0x18)',
     '    virtual void OnActivate();                          // slot 5 (+0x14) 0x0073AAC0\n    virtual void OnDeactivate();                        // slot 6 (+0x18) 0x00739E60')
edit('0073aba0', 'src', '        PModes.front()->OnLeave();', '        PModes.front()->OnDeactivate();')
edit('0073aba0', 'src', '        PModes.front()->OnResume();', '        PModes.front()->OnActivate();')
edit('0073aba0', 'src', '6 OnLeave (+0x18, as the landed ClearModes 0x0073AE70) and 5 (+0x14, the\n// resume hook; named descriptively)',
     '6 OnDeactivate (+0x18, as the landed ClearModes 0x0073AE70) and 5 OnActivate (+0x14,\n// vtable 0x012673CC: the landed 0x0073AAC0)')
edit('0073ae70', 'src', '    virtual void OnLeave();                        // slot 6 (+0x18)', '    virtual void OnDeactivate();                   // slot 6 (+0x18) 0x00739E60 (vtable 0x012673CC)')
edit('0073ae70', 'src', '        m_modes.m_pHead->m_pNext->m_pMode->OnLeave();', '        m_modes.m_pHead->m_pNext->m_pMode->OnDeactivate();')
# ---- 0x0088F620 calls 0x00991620 with a bool (retail stores the argument as a byte at +0x26): the landed definer's spelling
#      ?func_00991620@CFlaggedObject_Retail@@QAEX_N@Z (placeholder, no PDB name; one address one name)
edit('0088f620', 'src', 'struct CSoundMgr {\n    void** vtbl;\n    void SetMute(int muted);\n};',
     'struct CSoundMgr {\n    void** vtbl;\n};\nclass CFlaggedObject_Retail { public: void func_00991620(bool muted); };   // 0x00991620 (landed definer spelling; bool parameter)')
edit('0088f620', 'src', '        mgr->SetMute(1);', '        ((CFlaggedObject_Retail*)mgr)->func_00991620(true);')
edit('0088f620', 'src', '    g_soundMgr->SetMute(0);', '    ((CFlaggedObject_Retail*)g_soundMgr)->func_00991620(false);')
