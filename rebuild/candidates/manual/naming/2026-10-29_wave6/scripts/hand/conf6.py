# wave 6 RELOC_CONFLICT settlements, hand edits (exact single replacements; HE_OV=<overlay>).
#   0053b7f0 ~CList: RTTI .?AVCList@NUISystem@@ direct base is CChangingStateComponent (rtti_bases.tsv: CList :
#     CChangingStateComponent : CComponent), and 0x0052CCF0 is ~CChangingStateComponent (chunk 561: CSprite's base dtor,
#     the tail jump of ~CSprite 0x0041AB40). The row called it as ~CComponent, binding ??1CComponent@NUISystem@@UAE@XZ
#     to 0x0052CCF0 against 0x00532D90 (the real ~CComponent, landed row 00532d90). An empty CChangingStateComponent
#     layer between CComponent and CList adds no member or vptr (same layout, same code).
#   0059de76 UpdateSellActionsList: 0x0042BED4 / 0x0042CD84 are CDeletion() / CDeletion(const CDeletion&) (landed rows
#     0042bed4 / 0042cd84; Ego_r.pdb CDeletion {EDeletionMethod Method; list<CComponent*> AssociatedParents}, 8 bytes)
#     and slot 62 (+0xF8) is CComponent::SetDeletion(CDeletion) (?SetDeletion@CComponent@NUISystem@@UAEXVCDeletion@2@@Z,
#     NUISystem::CManager 0x0041E922). The row modelled them as CAction (binding ??0CAction@NUISystem@@QAE@XZ to
#     0x0042BED4 against the real CAction ctor 0x0042BE50).
#   00594f94 CFrontEndManager::CreateComponent: the getter it calls is 0x0041E5F2 = NUISystem::CManager::GetInstance
#     (?GetInstance@CManager@NUISystem@@SIPAV12@XZ, DECISIONS proven); the row declared it as a CFrontEndManager static
#     (SA) and bound ?GetInstance@CFrontEndManager@NUISystem@@SAPAV12@XZ to 0x0041E5F2 (CFrontEndManager's own getter is
#     0x00595582, SI). CreateComponent takes const CCharString& in both PDBs
#     (?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z 0x0041DB1D and
#     ?CreateComponent@CFrontEndManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@@Z, this row).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
for k in ('src', 'tst'):
    edit('0053b7f0', k, '// the states vector dtor 0x0053CD46, ~CComponent 0x0052CCF0.\n',
         '// the states vector dtor 0x0053CD46, ~CChangingStateComponent 0x0052CCF0 (RTTI: CList : CChangingStateComponent : CComponent).\n')
    edit('0053b7f0', k, '    ~CComponent();                                 // 0x0052CCF0\n', '')
    edit('0053b7f0', k, 'class CList : public CComponent {\n',
         'class CChangingStateComponent : public CComponent {   // RTTI direct base of CList\npublic:\n'
         '    ~CChangingStateComponent();                    // 0x0052CCF0\n};\nclass CList : public CChangingStateComponent {\n')
edit('0053b7f0', 'tst', 'CComponent::~CComponent() { Log(9); ++g_componentDtor; }\n',
     'CChangingStateComponent::~CChangingStateComponent() { Log(9); ++g_componentDtor; }\n')
# 0059de76
edit('0059de76', 'src', '0x0059DE04 loop: CAction() 0x0042BED4, list push_back 0x0042B6B8, the copy ctor\n// 0x0042CD84 for the by-value argument, slot 62 (+0xF8),',
     '0x0059DE04 loop: CDeletion() 0x0042BED4, list push_back 0x0042B6B8, the copy ctor\n// CDeletion(const CDeletion&) 0x0042CD84 for the by-value argument, slot 62 (+0xF8) CComponent::SetDeletion,')
edit('0059de76', 'src', 'enum EActionType { ACTION_TYPE_3 = 3 };\n', 'enum EDeletionMethod { DELETION_METHOD_3 = 3 };\n')
edit('0059de76', 'src', 'class CAction {                                      // retail 8 bytes\npublic:\n'
     '    EActionType                   ActionType;        // +0x00\n'
     '    std::list<CComponent*>        UIActionParams;    // +0x04 (see the header: the element is the map\'s key type here)\n'
     '    CAction();                                       // 0x0042BED4\n'
     '    CAction(const CAction& other);                   // 0x0042CD84\n',
     'class CDeletion {                                    // retail 8 bytes (Ego_r.pdb NUISystem::CDeletion)\npublic:\n'
     '    EDeletionMethod               Method;            // +0x00\n'
     '    std::list<CComponent*>        AssociatedParents; // +0x04 (see the header: the element is the map\'s key type here)\n'
     '    CDeletion();                                     // 0x0042BED4\n'
     '    CDeletion(const CDeletion& other);               // 0x0042CD84\n')
edit('0059de76', 'src', '    virtual void SetAction(CAction action);          // slot 62 (+0xF8) (name descriptive)\n',
     '    virtual void SetDeletion(CDeletion deletion);    // slot 62 (+0xF8)\n')
edit('0059de76', 'src', '        NUISystem::CAction action;\n        action.ActionType = NUISystem::ACTION_TYPE_3;\n'
     '        action.UIActionParams.push_back(it->first);\n        it->second->SetAction(action);\n',
     '        NUISystem::CDeletion deletion;\n        deletion.Method = NUISystem::DELETION_METHOD_3;\n'
     '        deletion.AssociatedParents.push_back(it->first);\n        it->second->SetDeletion(deletion);\n')
# 00594f94
for k in ('src', 'tst'):
    edit('00594f94', k, '    CComponent* CreateComponent(CCharString* definition, bool frontEnd);\n};\n',
         '    static CManager* __fastcall GetInstance();      // 0x0041E5F2\n'
         '    CComponent* CreateComponent(const CCharString& definition, bool frontEnd);   // 0x0041DB1D\n};\n')
    edit('00594f94', k, '    static CFrontEndManager* GetInstance();\n    CComponent* CreateComponent(CCharString* definition);\n',
         '    CComponent* CreateComponent(const CCharString& definition);\n')
edit('00594f94', 'src', 'CComponent* CFrontEndManager::CreateComponent(CCharString* definition)\n{\n'
     '    return GetInstance()->CManager::CreateComponent(definition, true);\n',
     'CComponent* CFrontEndManager::CreateComponent(const CCharString& definition)\n{\n'
     '    return CManager::GetInstance()->CreateComponent(definition, true);\n')
edit('00594f94', 'src', '// Ghidra confirms this is a one-argument __thiscall wrapper around the base\n// manager factory.',
     '// A one-argument __thiscall wrapper around the base manager factory: it calls\n// NUISystem::CManager::GetInstance 0x0041E5F2 (not its own getter 0x00595582), then\n// CManager::CreateComponent(const CCharString&, true) 0x0041DB1D.')
edit('00594f94', 'tst', 'NUISystem::CFrontEndManager*\nNUISystem::CFrontEndManager::GetInstance()\n',
     'NUISystem::CManager* __fastcall\nNUISystem::CManager::GetInstance()\n')
edit('00594f94', 'tst', '    CCharString* definition,\n    bool frontEnd)\n{\n    ++g_createCalls;\n    g_definition = definition;\n',
     '    const CCharString& definition,\n    bool frontEnd)\n{\n    ++g_createCalls;\n    g_definition = &definition;\n')
edit('00594f94', 'tst', '        g_manager.CreateComponent(&definition);\n', '        g_manager.CreateComponent(definition);\n')
edit('00594f94', 'tst', 'static CCharString* g_definition;\n', 'static const CCharString* g_definition;\n')
