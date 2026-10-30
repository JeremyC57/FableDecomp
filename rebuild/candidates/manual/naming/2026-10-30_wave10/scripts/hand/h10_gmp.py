# sub-wave 1c (hand part): 0x00449970 ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ (Ego_r 0x004450F0, FableWin 0x01A596C0) and
# 0x00449700 ?GetMainPlayerNumber@CPlayerManager@@QBEJXZ (Ego_r 0x00444EE0, FableWin 0x01A592C0; already bound from 00446220)
# in the rows the fam_gmp pattern cannot take (cast call sites, the CPlayer_Methods carrier, CPlayer_Retail with GetCurrentMode).
# Code unchanged: a reference return passes the same address; const on an out-of-line call changes nothing.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all
# 004887c0 (CPlayer::SetupInitialPlayerMode through the CPlayer_Methods carrier)
edit('004887c0', 'src', 'struct CPlayerManager_Retail {\n    long GetMainPlayerID() const;                      // 0x00449700\n    CPlayer* GetMainPlayer();                          // 0x00449970\n};',
     'class CPlayerManager { public:\n    long GetMainPlayerNumber() const;                  // 0x00449700\n    CPlayer& GetMainPlayer();                          // 0x00449970\n};')
edit('004887c0', 'src', '((CPlayerManager_Retail*)PlayerManager)->GetMainPlayerID()', '((CPlayerManager*)PlayerManager)->GetMainPlayerNumber()')
edit('004887c0', 'src', 'spirit->SetMaster(((CPlayerManager_Retail*)PlayerManager)->GetMainPlayer());', 'spirit->SetMaster(&((CPlayerManager*)PlayerManager)->GetMainPlayer());')
edit('004887c0', 'src', 'CPlayerManager::GetMainPlayerID 0x00449700', 'CPlayerManager::GetMainPlayerNumber 0x00449700')
# 0048a070 (CPlayer::InitCharacterAs through the carrier)
edit('0048a070', 'src', 'struct CPlayer_Methods;\nstruct CPlayerManager_Retail {\n    long GetMainPlayerID() const;                      // 0x00449700\n    CPlayer_Methods* GetMainPlayer();                  // 0x00449970\n};',
     'class CPlayerManager { public:\n    long GetMainPlayerNumber() const;                  // 0x00449700\n    CPlayer& GetMainPlayer();                          // 0x00449970\n};')
edit('0048a070', 'src', '((CPlayerManager_Retail*)PlayerManager)->GetMainPlayerID()', '((CPlayerManager*)PlayerManager)->GetMainPlayerNumber()')
edit_all('0048a070', 'src', '((CPlayerManager_Retail*)PlayerManager)->GetMainPlayer()->ControlledCreaturePtr()',
         'static_cast<CPlayer_Methods&>(((CPlayerManager*)PlayerManager)->GetMainPlayer()).ControlledCreaturePtr()', 2)
edit('0048a070', 'src', 'CPlayerManager::GetMainPlayerID 0x00449700', 'CPlayerManager::GetMainPlayerNumber 0x00449700')
# 0088e980 (CGameScriptInterface::IsHeroControlledByPlayer): also CPlayer::GetCurrentMode() const returning EPlayerMode
# (Ego_r 0x00554600, FableWin 0x01F5A750 ?GetCurrentMode@CPlayer@@QBE?AW4EPlayerMode@@XZ; bound to 0x00633BE0 from 00629b10)
edit('0088e980', 'src', 'struct CPlayerManager_Retail;\n', 'class CPlayerManager;\n')
edit('0088e980', 'src', 'struct CPlayer_Retail {\n    long GetCurrentMode();                         // 0x00633BE0\n};\nstruct CPlayerManager_Retail {\n    CPlayer_Retail* GetPlayer();                   // 0x00449970\n};',
     'enum EPlayerMode { };\nclass CPlayer { public:\n    EPlayerMode GetCurrentMode() const;            // 0x00633BE0\n};\nclass CPlayerManager { public:\n    CPlayer& GetMainPlayer();                      // 0x00449970\n};')
edit('0088e980', 'src', '    CPlayerManager_Retail*  PlayerManager;                   // +0x14', '    CPlayerManager*         PlayerManager;                   // +0x14')
edit('0088e980', 'src', 'switch (PlayerManager->GetPlayer()->GetCurrentMode()) {', 'switch (PlayerManager->GetMainPlayer().GetCurrentMode()) {')
edit('0088e980', 'src', '// Callees: the player getter @ 0x00449970 (on PlayerManager; the label "Catch@0040d959" is propagated),', '// Callees: CPlayerManager::GetMainPlayer @ 0x00449970 (on PlayerManager),')
# 0088e150 (CGameScriptInterface::RespawnHero): GetMainPlayerNumber is const on CPlayerManager
edit('0088e150', 'src', 'struct CPlayerManager_Retail {\n    long GetMainPlayerNumber();                   // 0x00449700\n};', 'class CPlayerManager { public:\n    long GetMainPlayerNumber() const;             // 0x00449700\n};')
edit('0088e150', 'src', '    CPlayerManager_Retail* PlayerManager;         // +0x14', '    CPlayerManager*        PlayerManager;         // +0x14')
