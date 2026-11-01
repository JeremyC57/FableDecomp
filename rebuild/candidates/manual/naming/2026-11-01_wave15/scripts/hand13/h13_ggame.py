# family ggame (hand): GGame 0x013B83D0 ?GGame@@3VCGame@@A and GSystemManager 0x013CA618 ?GSystemManager@@3VCSystemManager@@A
# (ESTABLISHED_SPELLINGS.tsv; Ego_r + FableWin `dynamic initializer for 'GGame'` / 'GSystemManager'). HE_OV=<overlay>
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all, resub
# ---- 0x00401B80 GFUninitialise: the first callee 0x00412F60 is CGame::Uninitialise (Ego_r + FableWin ?Uninitialise@CGame@@QAEXXZ;
#      the thunk tail-jumps to CInitBaseClass::Uninitialise 0x0099A360, which the PDB makes protected, and whose spelling
#      ?Uninitialise@CInitBaseClass@@QAEXXZ is bound to 0x0099A360 by 00487af0)
edit('00401b80', 'src', '// Retail: Fable.exe (TLC, VC7.1) function @ 0x00401B80, 64 bytes. Callees: CInitBaseClass::Uninitialise\n// 0x00412F60 (on global 0x013B83D0),',
     '// Retail: Fable.exe (TLC, VC7.1) function @ 0x00401B80, 64 bytes. Callees: CGame::Uninitialise\n// 0x00412F60 (on GGame 0x013B83D0; tail-jumps to CInitBaseClass::Uninitialise 0x0099A360),')
edit('00401b80', 'src', 'class CInitBaseClass { public:\n    void Uninitialise();                          // 0x00412F60\n};',
     'class CGame { public:\n    void Uninitialise();                          // 0x00412F60 (Ego_r / FableWin ?Uninitialise@CGame@@QAEXXZ)\n};')
edit('00401b80', 'src', 'extern CInitBaseClass g_InitBase;                 // 0x013B83D0',
     "extern CGame GGame;                               // 0x013B83D0 (Ego_r / FableWin dynamic initializer 'GGame')")
edit('00401b80', 'src', '    g_InitBase.Uninitialise();', '    GGame.Uninitialise();')
# ---- 0x004022B0 GFInitialise: the start-up object passed to 0x00413120 is GGame
edit('004022b0', 'src', '// Start-up state object that owns the progress display.\nextern GFInitialiseState g_GFInitialiseState_013B83D0;',
     "// The CGame object (Ego_r / FableWin dynamic initializer 'GGame'); the fable_boot.h callee view of 0x00413120 takes it as\n"
     "// GFInitialiseState (its identity, probably CGame::Initialise, is not settled: naming wave 13 DEFERRED).\n"
     'class CGame;\nextern CGame GGame;                                     // 0x013B83D0')
edit('004022b0', 'src', '            &g_GFInitialiseState_013B83D0) == 1)', '            (GFInitialiseState*)&GGame) == 1)')
# ---- 0x0049DCC0 CWorld::ResetToFrontEnd: both component ctors take CGame& GGame
edit('0049dcc0', 'src', '(RTTI: CGame : CInitBaseClass; no PDB data symbol -- descriptive g_Game)',
     "(RTTI: CGame : CInitBaseClass; Ego_r / FableWin dynamic initializer 'GGame')")
edit('0049dcc0', 'src', 'extern CGame g_Game;                         // 0x013B83D0 (descriptive)',
     'extern CGame GGame;                          // 0x013B83D0')
edit('0049dcc0', 'src', '        frontEnd = new CFrontendGameComponent(g_Game, init);', '        frontEnd = new CFrontendGameComponent(GGame, init);')
edit('0049dcc0', 'src', '        frontEnd = new CNewFrontendGameComponent(g_Game, init);', '        frontEnd = new CNewFrontendGameComponent(GGame, init);')
# ---- 0x01223FF0: `dynamic initializer for 'GSystemManager'` (Ego_r 0x0101AA70 / FableWin 0x0400A7F0); 0x009A7440 / 0x009A5800 are
#      ??0CSystemManager@@QAE@XZ / ??1CSystemManager@@UAE@XZ (catalog + both PDBs)
edit('01223ff0', 'src', '// dynamic initializer for CStatic_009a7440 g_CStatic_009a7440_013ca618 @ 0x013CA618 -- default-constructs it out of line, then registers its destructor with atexit',
     "// dynamic initializer for 'GSystemManager' @ 0x013CA618 -- default-constructs the CSystemManager out of line, then registers its destructor with atexit\n"
     '// Leaf: ??__EGSystemManager@@YAXXZ')
edit('01223ff0', 'src', '// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CStatic_009a7440\n// living at 0x013CA618:',
     "// Compiler-generated `??__E` dynamic initializer for the static-storage CSystemManager GSystemManager (Ego_r / FableWin\n// `dynamic initializer for 'GSystemManager'`; GFGetSystemManager 0x009A4EC0 returns its address) living at 0x013CA618:")
edit('01223ff0', 'src', "(`mov ecx,offset object ; jmp CStatic_009a7440::~CStatic_009a7440`, already landed)", "(`mov ecx,offset object ; jmp CSystemManager::~CSystemManager`, already landed)")
edit('01223ff0', 'src', "exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its\n// real identifier is not recoverable. One of 36 bodies of this shape.",
     "exact function (and the thunk) from the .CRT$XCU table (naming wave 13: the object and class carry their PDB names).\n// One of 36 bodies of this shape.")
edit('01223ff0', 'src', 'struct CStatic_009a7440 {\n    CStatic_009a7440();                 // 0x009A7440\n    ~CStatic_009a7440();                // 0x009A5800, reached through the atexit thunk\n};\n\nCStatic_009a7440 g_CStatic_009a7440_013ca618;',
     'class CSystemManager { public:\n    CSystemManager();                   // 0x009A7440 ??0CSystemManager@@QAE@XZ\n    virtual ~CSystemManager();          // 0x009A5800 ??1CSystemManager@@UAE@XZ, reached through the atexit thunk\n};\n\nCSystemManager GSystemManager;')
# the GFInitialise test links the source object: it defines the start-up object under the new name
edit('004022b0', 'tst', 'GFInitialiseState g_GFInitialiseState_013B83D0;', 'class CGame { public: GFInitialiseState State; };   // GGame 0x013B83D0 (naming wave 13)\nCGame GGame;')
edit('004022b0', 'tst', 'g_progressState != &g_GFInitialiseState_013B83D0)', 'g_progressState != (GFInitialiseState*)&GGame)')
