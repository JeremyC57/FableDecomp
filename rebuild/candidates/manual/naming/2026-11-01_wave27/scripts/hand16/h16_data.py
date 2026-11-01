# h16_data.py: naming wave 16, family data -- the c674 data symbols in landed sources: CConversationAttitude's static tables
# 0x013B8A38 / 0x013B8A44 (Init 0x004CD670, GetAnimName 0x004CD9B0), ScriptActiveStatuses 0x0143E910 (its dynamic initializer
# 0x01224C6D), and the CrimeText array's `dynamic atexit destructor' 0x01228ED8 (landed as an invented ThrowException).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all, path, _w, R
import rows
# --- 0x004CD670 CConversationAttitude::Init: the two tables are the class's static members (Ego_r dynamic initializer /
#     atexit names, c674); 0x013B8A2C keeps its landed spelling (not in this queue)
a = '004cd670'
edit(a, 'src', '// Strings: every literal', '// Naming wave 16 (c674): the 0x013B8A38 / 0x013B8A44 tables are the static members\n'
     '//   CConversationAttitude::ConversationAttitudeToAnimName / ConversationAttitudeToAnimGroupName (Ego_r.pdb dynamic initializer\n'
     '//   and atexit names; the landed initializers 0x0121A4F0 / 0x0121A580 define them); the landed g_* externs are respelled.\n'
     '// Strings: every literal')
edit(a, 'src', 'extern std::vector<CCharString> g_ConversationAttitudeAnimNames;         // 0x013B8A38 (12, by EConversationAttitude)\n'
     'extern std::vector<CCharString> g_ConversationAttitudeAnimGroupNames;    // 0x013B8A44 (12, by EConversationAttitude)\n', '')
edit(a, 'src', 'class CConversationAttitude {\npublic:\n    static void __fastcall Init();\n',
     'class CConversationAttitude {\npublic:\n    static void __fastcall Init();\n'
     '    static std::vector<CCharString> ConversationAttitudeToAnimName;        // 0x013B8A38 (12, by EConversationAttitude)\n'
     '    static std::vector<CCharString> ConversationAttitudeToAnimGroupName;   // 0x013B8A44 (12, by EConversationAttitude)\n')
edit_all(a, 'src', 'g_ConversationAttitudeAnimNames[', 'ConversationAttitudeToAnimName[', 12)
edit_all(a, 'src', 'g_ConversationAttitudeAnimGroupNames[', 'ConversationAttitudeToAnimGroupName[', 12)
# --- 0x004CD9B0: GetAnimName(EConversationAttitude), the twin of the landed GetAnimGroupName 0x004CD9C0 (copied from it)
src = open(os.path.join(rows.ROOT, 'rebuild/src/compiled', R['004cd9c0']['Source']), encoding='utf-8').read()
body = src[src.index('class CCharString {'):]
body = body.replace('static const CCharString& __fastcall GetAnimGroupName(EConversationAttitude attitude);\n    static std::vector<CCharString> ConversationAttitudeToAnimGroupName;   // 0x013B8A44',
                    'static const CCharString& __fastcall GetAnimName(EConversationAttitude attitude);\n    static std::vector<CCharString> ConversationAttitudeToAnimName;        // 0x013B8A38')
body = body.replace('const CCharString& __fastcall CConversationAttitude::GetAnimGroupName(EConversationAttitude attitude)\n{\n    return ConversationAttitudeToAnimGroupName[attitude];',
                    'const CCharString& __fastcall CConversationAttitude::GetAnimName(EConversationAttitude attitude)\n{\n    return ConversationAttitudeToAnimName[attitude];')
assert 'GetAnimGroupName' not in body and body.count('ConversationAttitudeToAnimName') == 2, body
t = '''// CConversationAttitude::GetAnimName(EConversationAttitude attitude) -- the talk animation name of a conversation attitude (an index into the static name table)
// Leaf: ?GetAnimName@CConversationAttitude@@SIABVCCharString@@W4EConversationAttitude@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD9B0, 9 bytes: mov eax,[0x013B8A38] ; lea eax,[eax+ecx*4] ; ret.
// Identity (two sources): (1) the table it reads, 0x013B8A38, is CConversationAttitude::ConversationAttitudeToAnimName (chunk c674:
//   Ego_r.pdb dynamic initializer / atexit names, the landed initializer 0x0121A4F0); (2) Ego_r.pdb's donor run
//   GetAnimName(EAttitude) 0x004EB240 / GetAnimName(EConversationAttitude) 0x004EB250 / GetAnimGroupName(EConversationAttitude)
//   0x004EB260 = retail ... / 0x004CD9B0 / 0x004CD9C0 (the landed GetAnimGroupName twin reads 0x013B8A44). Static __fastcall:
//   the attitude arrives in ecx and the result is a const CCharString&.
// Naming wave 16: landed as the free __fastcall GetAnimGroupName(int) over `void** g_AnimGroupNameTable` (catalog
//   CConversationAttitude::GetAnimGroupName -- the 0x004CD9C0 name, wrong here); re-authored as the 0x004CD9C0 twin
//   (name-only: the bytes are unchanged).
''' + body
p, rel = path('004cd9b0', 'src')
if open(p, encoding='utf-8', newline='').read() == t: print('already 004cd9b0')
else: _w(p, t); print('ok 004cd9b0 src')
# --- 0x01224C6D: the dynamic initializer of ScriptActiveStatuses (map<CCharString, EScriptActiveStatus>, c674)
a = '01224c6d'
edit(a, 'src', 'class CUnprovenMapKey_00CD3640;class CUnprovenMapValue_00CD3640;\n', 'class CCharString;enum EScriptActiveStatus{};\n')
edit(a, 'src', 'std::map<CUnprovenMapKey_00CD3640,CUnprovenMapValue_00CD3640> g_UnprovenMap_0143E910;',
     'std::map<CCharString,EScriptActiveStatus> ScriptActiveStatuses;')
edit(a, 'src', 'typedef char ActualMapSize[sizeof(g_UnprovenMap_0143E910)==12?1:-1];', 'typedef char ActualMapSize[sizeof(ScriptActiveStatuses)==12?1:-1];')
edit(a, 'src', '// Layout: real tree header', '// Naming wave 16 (c674): the global is ?ScriptActiveStatuses@@3V?$map@VCCharString@@W4EScriptActiveStatus@@...@A (SetScriptActiveStatus\n'
     '//   0x00CBFAB8 / GetScriptActiveStatus 0x00CBF9CF index it by script name); the CUnproven key / value stand-ins are respelled\n'
     '//   CCharString / EScriptActiveStatus (incomplete / empty enum: no layout touched; name-only, bytes unchanged).\n// Layout: real tree header')
# --- 0x01228ED8: `anonymous namespace'::`dynamic atexit destructor for 'CrimeText'' -- the CrimeText TU (landed initializer
#     0x0121A7CA) emits it; the source is that row's, graded on _$E2
src = open(os.path.join(rows.ROOT, 'rebuild/src/compiled', R['0121a7ca']['Source']), encoding='utf-8').read()
code = src[src.index('#pragma optimize("s", on)'):]
t = '''// `anonymous namespace'::`dynamic atexit destructor for 'CrimeText'' -- destroy the ten CCharStrings of CrimeText[10] through the vector destructor iterator
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01228ED8, 20 bytes: push ~CCharString ; push 10 ; push 4 ; push 0x013B8B10 ;
//   call `vector destructor iterator' ; ret.
// Identity (proven, chunk c674): the one atexit destructor the CrimeText initializer 0x0121A7CA registers (its _atexit push);
//   the ten consecutive CCharStrings 0x013B8B10..0x013B8B34 are the anonymous-namespace array CrimeText[10] (Ego_r.pdb initializer
//   order DEFAULT_DEATH_RECOVERY_MARKER_NAME -> CrimeText -> CrimeTotalText; see the 0x0121A7CA row).
// Callees: `vector destructor iterator' 0x004012F0-family (??_I, __stdcall: array, 4, 10, dtor), CCharString::~CCharString 0x0099EAE0.
// Shape: the same TU as the landed initializer 0x0121A7CA (its source, verbatim below): the compiler emits this _$E2 with the
//   _$E1 initializer; size-optimised TU.
// Naming wave 16: landed as the invented CDebugManager::ThrowException (FableThrowException_1228ed8 calling a stdcall
//   FableThrow_1228ed8 with immediate arguments); re-authored as the compiler-generated atexit destructor (name-only).
''' + code
p, rel = path('01228ed8', 'src')
if open(p, encoding='utf-8', newline='').read() == t: print('already 01228ed8')
else: _w(p, t); print('ok 01228ed8 src')
