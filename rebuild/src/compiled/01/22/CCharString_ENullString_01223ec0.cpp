// CCharString::$E_NullString() -- dynamic initializer of CCharString::NullString: the inline default constructor (null string data, one more live string), then its destructor is registered with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01223EC0, 22 bytes, in `.text$yc`, reached from the .CRT$XCU table, plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb `dynamic initializer for 'CCharString::NullString'` (FableWin 0x0400A4F0, right
//   after the LOCAL_HEAP_* initializers); the registered atexit thunk 0x0122A5E0 (`dynamic atexit destructor for
//   'CCharString::NullString'`) runs the inline ~CCharString on 0x013BD804 (CCharString::UnassignString 0x0099E9B0, then
//   one live string fewer). 0x013BD804 is read as the empty string by CQuestManager::GetRegisteredQuestFromIndex
//   0x004AF3C0 and CGameScriptInterface::GetRegionName 0x0088E340. Ego_d.pdb PUB32 ?NullString@CCharString@@2V1@A (public static).
// Label: the manifest's "sub_01223ec0" is a gap-scan placeholder. The atexit thunk 0x0122A5E0 is the same TU's $E2 (its row
//   defines NullString too: the static initializer / atexit pair, EXTRA_RULES 2026-11-20).
// Globals: the live CCharString count 0x013BD800 (the landed spelling g_CCharStringInstanceCount is kept; Ego_d.pdb PUB32
//   ?NoStrings@CCharString@@1JA, fable_string.h's protected NoStrings, is queued for it), CCharString::NullString 0x013BD804.
//   Callee: _atexit 0x004012BC.
// Shape: the count is defined in this TU with a constant 0 initializer ahead of NullString, so VC7.1 folds the inline
//   `++count` into `mov [count],1`; the null PStringData is static data (folded). With the count only declared extern
//   the initializer is `mov edx,[count] ; inc edx ; mov [count],edx` instead.
// Inferred: the inline default constructor and destructor bodies (0x0099E4B0 / 0x0099EAE0 are their out-of-line copies,
//   and this object's pick-any copies equal them).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_NULLSTRING
#include "fable_string.h"

long g_CCharStringInstanceCount = 0;                     // 0x013BD800

inline CCharString::CCharString() : PStringData(0) { ++g_CCharStringInstanceCount; }
inline CCharString::~CCharString() { UnassignString(); --g_CCharStringInstanceCount; }

CCharString CCharString::NullString;                     // 0x013BD804
