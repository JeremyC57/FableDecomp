# fam_gmp2 test follow-up: seven behaviour tests link the row object and defined the row's old stand-in callee; the row now
# calls ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ (0x00449970), so the test's fake moves to that member (same `this`,
# same returned address; the PassPattern lines are unchanged).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, resub
SUB = (r'extern "C" void\* __fastcall Sub_ResolveA\(void\* self\)\n\{\n    (g_\w+) = self;\n    return \(void\*\)0xAAAA;\n\}',
       'class CPlayer;\nclass CPlayerManager { public: CPlayer& GetMainPlayer(); };   // 0x00449970, called by the row\n'
       'CPlayer& CPlayerManager::GetMainPlayer()\n{\n    \\1 = this;\n    return *(CPlayer*)0xAAAA;\n}')
for a in ('00899c90', '00899d50', '00899db0', '0089a070', '0089a0e0'):
    resub(a, 'test', SUB[0], SUB[1], 1)
edit('008916a0', 'test', 'CPlayer*\nCGSISheatheHeroWeapons_LookupSource::BeginLookup()\n{\n    g_ObservedLookupSource = this;\n    RecordEvent(1);\n    return g_LookupResult;\n}',
     'class CPlayerManager { public: CPlayer& GetMainPlayer(); };   // 0x00449970: the row\'s inline BeginLookup forwards here\n'
     'CPlayer&\nCPlayerManager::GetMainPlayer()\n{\n    g_ObservedLookupSource = (CGSISheatheHeroWeapons_LookupSource*)this;\n    RecordEvent(1);\n    return *g_LookupResult;\n}')
edit('00890b50', 'test', 'void* __fastcall helper_449970(void* that)\n{\n    // return our fake object; verify \'that\' is the expected pointer\n    return g_obj;\n}',
     'class CPlayer;\nclass CPlayerManager { public: CPlayer& GetMainPlayer(); };   // 0x00449970, called by the row\n'
     'CPlayer& CPlayerManager::GetMainPlayer()\n{\n    // return our fake object\n    return *(CPlayer*)g_obj;\n}')
