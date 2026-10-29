# wave 8 self-name fixes A: defining rows whose own symbol differs from the Ego_r.pdb + FableWin.pdb spelling by access, const
# or pointer->reference only (code bytes unchanged: access and const do not touch codegen, a reference passes the same pointer).
#   007aef10 ?CountEffects@CTCBoss@@QAEJXZ (non-const)            007f61b0 ?AddThingToNotAttack@CTCTargetingAI@@QAEXAAVCThing@@@Z
#   008e5270 ?UpdateIsMoving@CTCPhysicsStandard@@IAEXXZ           008e5670 ?PerformForceUpdates@CTCPhysicsStandard@@IAEXXZ
#   008f6fc0 ?ZeroAllUnusedThemePaletteEntries@CMap@@IAEXXZ       008fa120 ?ClearData@LSocket@LHNetworkLib@@AAEXXZ
#   0097fa00 ?AIAction_MinionIdle@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ
#   00a48ac0 ?WriteToFile@CSpeechFrame@@QBEXAAVCDataOutputStream@@@Z
#   00ab5d00 ??0CMouseDX@@QAE@AAVCInputManagerDX@@_N@Z (+ callee ??0CMouse@@QAE@AAVCInputManager@@_N@Z, defined so by 00a66f20)
#   00ab64e0 ??0CKeyboardDX@@QAE@AAVCInputManagerDX@@_N@Z (+ callee ??0CKeyboard@@QAE@AAVCInputManager@@_N@Z, defined so by 00a66930)
#   00ab7850 ??0CJoystickDX@@QAE@AAVCInputManagerDX@@_N@Z (+ callee ??0CJoystick@@QAE@AAVCInputManager@@_N@Z)
#   009e43b0 ??0CJoystick@@QAE@AAVCInputManager@@_N@Z
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all
P = '\nprotected:\n'
edit('007aef10', 'src', '    long CountEffects() const;', '    long CountEffects();')
edit('007aef10', 'src', 'long CTCBoss::CountEffects() const\n', 'long CTCBoss::CountEffects()\n')
edit('007f61b0', 'src', '    void AddThingToNotAttack(CThing* thing);', '    void AddThingToNotAttack(CThing& thing);')
edit('007f61b0', 'src', 'void CTCTargetingAI::AddThingToNotAttack(CThing* thing)\n{\n    if (IsThingAttackable(thing)) {\n        CIntelligentPointer pointer(thing);',
     'void CTCTargetingAI::AddThingToNotAttack(CThing& thing)\n{\n    if (IsThingAttackable(&thing)) {\n        CIntelligentPointer pointer(&thing);')
edit('008e5270', 'src', '    bool Impassable : 1;                        //       bit 7\n    void UpdateIsMoving();\n};',
     '    bool Impassable : 1;                        //       bit 7\nprotected:\n    void UpdateIsMoving();\n};')
edit('008e5670', 'src', 'struct CTCPhysicsStandard : CTCPhysicsBase {\n    void PerformForceUpdates();\n};',
     'struct CTCPhysicsStandard : CTCPhysicsBase {\nprotected:\n    void PerformForceUpdates();\n};')
edit('008f6fc0', 'src', '    CGameMapGrid*              m_pGameMapGrid;     // +0x1a7c\n    void ZeroAllUnusedThemePaletteEntries();\n};',
     '    CGameMapGrid*              m_pGameMapGrid;     // +0x1a7c\nprotected:\n    void ZeroAllUnusedThemePaletteEntries();\n};')
edit('008fa120', 'src', '    void ClearData();\n', 'private:\n    void ClearData();\npublic:\n')
edit('0097fa00', 'src', '    EAIActionReturn AIAction_MinionIdle();\n', 'protected:\n    EAIActionReturn AIAction_MinionIdle();\npublic:\n')
edit('00a48ac0', 'src', '    void WriteToFile(CDataOutputStream* stream) const;', '    void WriteToFile(CDataOutputStream& stream) const;')
edit_all('00a48ac0', 'src', '    stream->', '    stream.')
edit('00a48ac0', 'src', 'void CSpeechFrame::WriteToFile(CDataOutputStream* stream) const', 'void CSpeechFrame::WriteToFile(CDataOutputStream& stream) const')
# CMouseDX
edit('00ab5d00', 'src', '// CMouseDX::CMouseDX(CInputManagerDX*, bool)', '// CMouseDX::CMouseDX(CInputManagerDX&, bool)')
edit('00ab5d00', 'src', '\nclass CInputManagerDX;\n', '\nclass CInputManager { public: };\nclass CInputManagerDX : public CInputManager { public: };\n')
edit('00ab5d00', 'src', '    CMouse(CInputManagerDX* manager, bool exclusiveMode);   // 0x00A66F20', '    CMouse(CInputManager& manager, bool exclusiveMode);     // 0x00A66F20')
edit('00ab5d00', 'src', '    CMouseDX(CInputManagerDX* manager, bool exclusiveMode);', '    CMouseDX(CInputManagerDX& manager, bool exclusiveMode);')
edit('00ab5d00', 'src', 'CMouseDX::CMouseDX(CInputManagerDX* manager, bool exclusiveMode)\n    : CMouse(manager, exclusiveMode)\n    , InputManagerDX(manager)',
     'CMouseDX::CMouseDX(CInputManagerDX& manager, bool exclusiveMode)\n    : CMouse(manager, exclusiveMode)\n    , InputManagerDX(&manager)')
# CKeyboardDX
edit('00ab64e0', 'src', '// CKeyboardDX::CKeyboardDX(CInputManagerDX* manager, bool exclusive)', '// CKeyboardDX::CKeyboardDX(CInputManagerDX& manager, bool exclusive)')
edit('00ab64e0', 'src', '    CKeyboard(CInputManager* manager, bool exclusive);   // 0x00A66930', '    CKeyboard(CInputManager& manager, bool exclusive);   // 0x00A66930')
edit('00ab64e0', 'src', '    CKeyboardDX(CInputManagerDX* manager, bool exclusive);', '    CKeyboardDX(CInputManagerDX& manager, bool exclusive);')
edit('00ab64e0', 'src', 'CKeyboardDX::CKeyboardDX(CInputManagerDX* manager, bool exclusive)\n    : CKeyboard(manager, exclusive), InputManagerDX(manager)',
     'CKeyboardDX::CKeyboardDX(CInputManagerDX& manager, bool exclusive)\n    : CKeyboard(manager, exclusive), InputManagerDX(&manager)')
edit('00ab64e0', 'src', '    if (manager->DInput->CreateDevice(', '    if (manager.DInput->CreateDevice(')
# CJoystickDX
edit('00ab7850', 'src', '    CJoystick(CInputManager* inputManager, bool exclusiveMode);   // 0x009E43B0', '    CJoystick(CInputManager& inputManager, bool exclusiveMode);   // 0x009E43B0')
edit('00ab7850', 'src', '    CJoystickDX(CInputManagerDX* inputManager, bool exclusiveMode);', '    CJoystickDX(CInputManagerDX& inputManager, bool exclusiveMode);')
edit('00ab7850', 'src', 'CJoystickDX::CJoystickDX(CInputManagerDX* inputManager, bool exclusiveMode)\n    : CJoystick((CInputManager*)inputManager, exclusiveMode)\n{\n    InputManagerDX = inputManager;',
     'CJoystickDX::CJoystickDX(CInputManagerDX& inputManager, bool exclusiveMode)\n    : CJoystick((CInputManager&)inputManager, exclusiveMode)\n{\n    InputManagerDX = &inputManager;')
# CJoystick
edit('009e43b0', 'src', '// CJoystick::CJoystick(CInputManager* inputManager, bool exclusiveMode)', '// CJoystick::CJoystick(CInputManager& inputManager, bool exclusiveMode)')
edit('009e43b0', 'src', '    CJoystick(CInputManager* inputManager, bool exclusiveMode);\n', '    CJoystick(CInputManager& inputManager, bool exclusiveMode);\n')
edit('009e43b0', 'src', 'CJoystick::CJoystick(CInputManager* inputManager, bool exclusiveMode)\n    : InputManager(inputManager)',
     'CJoystick::CJoystick(CInputManager& inputManager, bool exclusiveMode)\n    : InputManager(&inputManager)')
edit('00ab7850', 'src', '// CJoystickDX::CJoystickDX(CInputManagerDX* inputManager, bool exclusiveMode)', '// CJoystickDX::CJoystickDX(CInputManagerDX& inputManager, bool exclusiveMode)')
