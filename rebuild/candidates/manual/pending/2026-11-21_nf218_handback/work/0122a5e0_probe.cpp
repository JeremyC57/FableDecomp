// CCharString::$E_NullString() -- dynamic initializer of CCharString::NullString: the inline default constructor (null string data, one more live string), then its destructor is registered with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01223EC0, 22 bytes, in `.text$yc`, reached from the .CRT$XCU table, plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb `dynamic initializer for 'CCharString::NullString'` (FableWin 0x0400A4F0, right
//   after the LOCAL_HEAP_* initializers); the registered atexit thunk 0x0122A5E0 (`dynamic atexit destructor for
//   'CCharString::NullString'`) runs the inline ~CCharString on 0x013BD804 (CCharString::UnassignString 0x0099E9B0, then
//   one live string fewer). 0x013BD804 is read as the empty string by CQuestManager::GetRegisteredQuestFromIndex
//   0x004AF3C0 and CGameScriptInterface::GetRegionName 0x0088E340.
// Label: the manifest's "sub_01223ec0" is a gap-scan placeholder; the landed atexit thunk 0x0122A5E0 carries a
//   propagated `_Cons_val<...CParticleMorphs::CEntry...>` label.
// Globals: the live CCharString count 0x013BD800 (no PDB name; the landed spelling g_CCharStringInstanceCount is kept),
//   CCharString::NullString 0x013BD804. Callee: _atexit 0x004012BC.
// Shape: the count is defined in this TU with a constant 0 initializer ahead of NullString, so VC7.1 folds the inline
//   `++count` into `mov [count],1`; the null PStringData is static data (folded). With the count only declared extern
//   the initializer is `mov edx,[count] ; inc edx ; mov [count],edx` instead.
class CCharStringData;

long g_CCharStringInstanceCount = 0;                     // 0x013BD800

class CCharString {
public:
    CCharStringData* PStringData;                         // +0x00
    CCharString() : PStringData(0) { ++g_CCharStringInstanceCount; }
    ~CCharString() { UnassignString(); --g_CCharStringInstanceCount; }
    static CCharString NullString;                        // 0x013BD804
protected:
    void UnassignString();                                // 0x0099E9B0 ?UnassignString@CCharString@@IAEXXZ
};

CCharString CCharString::NullString;