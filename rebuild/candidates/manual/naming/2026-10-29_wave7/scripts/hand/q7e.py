# wave 7 hand edits, part e: w6 DEFERRED GetMainPlayer rows with both overloads (fam_gmp6 skips) and 00773b20.
#   CPlayerManager (Ego_r.pdb + FableWin.pdb): GetMainPlayer() const 0x00449960 -> const CPlayer&, GetMainPlayer() 0x00449970 ->
#   CPlayer&, GetMainPlayerNumber() const 0x00449700.  CThing (Ego_r.pdb, retail = Ego_r - 0x1BA0, protected): PeekPlayerManager()
#   const 0x004C79F0 -> const CPlayerManager&, GetPlayerManager() 0x004C7A00 -> CPlayerManager&.  CTCBase: PeekComponent() const
#   0x00686D60 -> const CMainGameComponent& (Ego_r CMainGameComponent +0x1C PPlayerManager), PeekPlayerManager() const 0x00686D50.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
from he7 import edit_n

F = '006cfc00'   # CTCWife::OnGreet: GetPlayerManager().GetMainPlayer() (non-const, 0x00449970)
edit(F, 'src', '    CPlayer* GetMainPlayer() const;             // 0x00449960 (const)\n    CPlayer* GetMainPlayer();                   // 0x00449970\n',
     '    const CPlayer& GetMainPlayer() const;       // 0x00449960 (Ego_r.pdb)\n    CPlayer& GetMainPlayer();                   // 0x00449970 (Ego_r.pdb)\n')
edit(F, 'src', 'GetPlayerManager().GetMainPlayer()->GetPControlledCreature();', 'GetPlayerManager().GetMainPlayer().GetPControlledCreature();')

F = '006d0020'   # CTCWife::Died: g_pMainGameComponent->PlayerManager->GetMainPlayer() (non-const, 0x00449970)
edit(F, 'src', '    CPlayer* GetMainPlayer() const;             // 0x00449960 (const)\n    CPlayer* GetMainPlayer();                   // 0x00449970\n',
     '    const CPlayer& GetMainPlayer() const;       // 0x00449960 (Ego_r.pdb)\n    CPlayer& GetMainPlayer();                   // 0x00449970 (Ego_r.pdb)\n')
edit(F, 'src', 'PlayerManager->GetMainPlayer()->GetPControlledCreature();', 'PlayerManager->GetMainPlayer().GetPControlledCreature();')

F = '007e62d0'   # CTCOracleMinigame::Quit: non-const through GetPlayerManager(), const through the component's const pointer
edit(F, 'src', '    CPlayer* GetMainPlayer();                   // 0x00449970\n    CPlayer* GetMainPlayer() const;             // 0x00449960 (byte twin)\n',
     '    CPlayer& GetMainPlayer();                   // 0x00449970 (Ego_r.pdb)\n    const CPlayer& GetMainPlayer() const;       // 0x00449960 (Ego_r.pdb; byte twin)\n')
edit(F, 'src', '    CPlayer* player = GetPlayerManager().GetMainPlayer();\n', '    CPlayer* player = &GetPlayerManager().GetMainPlayer();\n')
edit(F, 'src', 'PlayerManager->GetMainPlayer()->GetPControlledCreature();', 'PlayerManager->GetMainPlayer().GetPControlledCreature();')

F = '0082db00'   # CThingBuilding::LockAllDoors: CThing::PeekPlayerManager / GetPlayerManager (protected, references)
edit(F, 'src', '    const CPlayer* GetMainPlayer() const;            // 0x00449960\n    CPlayer* GetMainPlayer();                        // 0x00449970\n',
     '    const CPlayer& GetMainPlayer() const;            // 0x00449960 (Ego_r.pdb)\n    CPlayer& GetMainPlayer();                        // 0x00449970 (Ego_r.pdb)\n')
edit(F, 'src', '    const CPlayerManager* GetPlayerManager() const;  // 0x004C79F0\n    CPlayerManager* GetPlayerManager();              // 0x004C7A00\n',
     'protected:                                           // Ego_r.pdb CThing protected accessors\n'
     '    const CPlayerManager& PeekPlayerManager() const; // 0x004C79F0\n    CPlayerManager& GetPlayerManager();              // 0x004C7A00\npublic:\n')
edit(F, 'src', 'static_cast<const CThingBuilding*>(this)->GetPlayerManager()->GetMainPlayer()->PeekPControlledCreature();',
     'static_cast<const CThingBuilding*>(this)->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();')
edit(F, 'src', 'GetPlayerManager()->GetMainPlayer()->GetPControlledCreature();', 'GetPlayerManager().GetMainPlayer().GetPControlledCreature();')

F = '00773b20'   # CTCShop::OpenShoppingInterface: PeekComponent() const 0x00686D60, PeekPlayerManager() const 0x00686D50
edit(F, 'src', '// Callees: CTCBase::GetGameSubsystem @ 0x00686D60 (its +0x1c member is the player manager here)',
     '// Callees: CTCBase::PeekComponent @ 0x00686D60 (CMainGameComponent +0x1c PPlayerManager, Ego_r.pdb)')
edit(F, 'src', 'CMemoryBuffer::Copy @ 0x009F1760, CTCBase::GetPlayerManager const\n// @ 0x00686D50,',
     'CMemoryBuffer::Copy @ 0x009F1760, CTCBase::PeekPlayerManager const\n// @ 0x00686D50,')
edit(F, 'src', '    CPlayer* GetMainPlayer() const;                     // 0x00449960\n    long GetMainPlayerNumber();                         // 0x00449700\n',
     '    const CPlayer& GetMainPlayer() const;               // 0x00449960 (Ego_r.pdb)\n    long GetMainPlayerNumber() const;                   // 0x00449700 (Ego_r.pdb)\n')
edit(F, 'src', 'struct CGameSubsystem {\n    unsigned char _pad00[0x1c];\n    CPlayerManager* PlayerManager;                      // +0x1c\n};\n', '')
edit(F, 'src', 'class CMainGameComponent { public:\n    void AddGameEvent(const CGameEvent& event);         // 0x0041649C\n};\n',
     'class CMainGameComponent { public:\n    unsigned char _pad00[0x1c];\n    CPlayerManager* PPlayerManager;                     // +0x1c (Ego_r.pdb scoped_ptr)\n'
     '    void AddGameEvent(const CGameEvent& event);         // 0x0041649C\n};\n')
edit(F, 'src', '    CPlayerManager* GetPlayerManager() const;           // 0x00686D50\n    CGameSubsystem* GetGameSubsystem();                 // 0x00686D60\n',
     'protected:                                          // Ego_r.pdb CTCBase protected accessors\n'
     '    const CPlayerManager& PeekPlayerManager() const;    // 0x00686D50\n    const CMainGameComponent& PeekComponent() const;    // 0x00686D60\n')
edit(F, 'src', 'CThing* hero = GetGameSubsystem()->PlayerManager->GetMainPlayer()->GetPControlledCreature();',
     'CThing* hero = PeekComponent().PPlayerManager->GetMainPlayer().GetPControlledCreature();')
edit(F, 'src', 'event.Player = (signed char)GetPlayerManager()->GetMainPlayerNumber();', 'event.Player = (signed char)PeekPlayerManager().GetMainPlayerNumber();')
