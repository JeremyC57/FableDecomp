# item 3 (0x0143E8F8, hand part): the row whose timer-manager view has no address comment
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, resub
edit('00cd4470', 'src', 'struct GMgr {\n', 'class CGameScriptInterface { public:                // view of the script interface vtable (slot 88 at +0x160)\n')
edit('00cd4470', 'src', 'extern GMgr *g_timerMgr_cd4470;', 'extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8 (wave-10 decision)')
edit('00cd4470', 'src', 'g_timerMgr_cd4470->Release(m_first);', 'g_pGameScriptInterface->Release(m_first);')
