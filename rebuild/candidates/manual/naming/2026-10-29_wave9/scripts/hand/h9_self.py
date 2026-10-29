# wave 9 self-name fixes of defining rows (Ego_r.pdb + FableWin.pdb spellings; code unchanged: access, an enum return, a
# const& parameter that passes the same pointer)
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all
# 0x0097DB70 ?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z
edit('0097db70', 'src', '\nclass CAIStateGroupBase\n{ public:', '\nenum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2 };\n\nclass CAIStateGroupBase\n{ public:')
edit('0097db70', 'src', '    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced);\n};',
     'protected:\n    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& actionRef, bool stopWhenReplaced);\n};')
edit('0097db70', 'src', 'long CAIStateGroupBase::AIAction_PerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced)\n{\n',
     'EAIActionReturn CAIStateGroupBase::AIAction_PerformActionAndWait(const CCreatureActionBase& actionRef, bool stopWhenReplaced)\n{\n    CCreatureActionBase* action = (CCreatureActionBase*)&actionRef;\n')
edit_all('0097db70', 'src', '        return 1;\n', '        return AI_ACTION_STOPPING;\n', 3)
edit('0097db70', 'src', '    return 2;\n}', '    return AI_ACTION_DONE;\n}')
# 0x004C9840 ?RemoveTC@CThing@@QAEXW4ETCInterfaceType@@@Z: the unqualified caller 00839080 (fam_rtc9 shape); the defining row
# itself is DEFERRED (its inline find(const long&) copies an enum argument: DIFFER 216v213)
edit('00839080', 'src', '    void RemoveTC(long interfaceType);          // 0x004C9840', '    void RemoveTC(ETCInterfaceType interfaceType);   // 0x004C9840')
edit('00839080', 'src', '        RemoveTC(0x2a);', '        RemoveTC((ETCInterfaceType)0x2a);')
