# h19_c680.py: naming wave 19, family c680 -- the chunk 680 rows that had no mangled final in waves 17/18 (BRIEF_W19 scope 3), proven here:
#   cross: 0x00C25860 is ?Cross@C3DVector@@QBE?BV1@ABV1@@Z (FableWin.pdb name set 0x01D8E950 -- the only C3DVector cross-product member in either
#          PDB -- and the body: `this` x other through the hidden result pointer, QBE, `ret 8`); the own row (landed as the descriptive
#          CrossProduct) and its test are respelled, the file is renamed (RENAMES.tsv);
#   theme: 0x0063A390 is ?GetThemeSizeZAt@CWorldMap@@QBEMABVC3DVector@@@Z (Ego_r.pdb donor order GetGroundSizeZAt 006E7EC0 / GetThemeSizeZAt
#          006E7F50 / GetWaterSizeZAt 006E7FE0 = retail 0x0063A300 / 0x0063A390 / 0x0063A420, 0x90 apart in both; the twin bodies differ
#          only in the CMap callee: 0x0081EA50 CMap::GetGroundSizeZAt for the ground row, 0x008200B0 for the theme row, which is therefore
#          ?GetThemeSizeZAt@CMap@@IBEMABVC3DVector@@@Z -- Ego_r 006796E0 / 0067B2D0 in the same order); own row + test respelled (file renamed),
#          the vtable-view caller 0x00912700 names the function GetThemeSizeZAt (its CWorldMapMethods view stays: per-row);
#   exit:  0x009C4F20 CFileInstaller::ExitThread is volatile (chunk 680 Retail-volatile evidence in the landed 0x009C3590 / 0x009C3660 /
#          0x009C5690: the discarded read `mov al,[eax+7Ah]` before the store at 0x009C3608); the member is not read in this body (bytes hold);
#   dwv:   0x00C73770 (landed as CDefString::SetDefaultString, a propagated label) is the one-time guard around the GameCODA DetectWindowsVersion
#          0x00C73660 (?DetectWindowsVersion@@YAXXZ, no arguments): `if (!flag) DetectWindowsVersion(); return self;` with ecx in / eax out --
#          re-authored with the callee's real signature (byte-identical) under a descriptive name; the flag 0x0143CCC0 keeps the landed callee
#          row's bound spelling g_defStringInit (one address, one name: respelling it is a separate decision).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_c680.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())
# cross
a = '00c25860'
if a not in _X:
    he.edit(a, 'src', '// C3DVector::CrossProduct(const C3DVector& other) const -- returns this x other through the hidden result pointer\n',
            '// C3DVector::Cross(const C3DVector& other) const -- returns this x other through the hidden result pointer\n')
    he.edit(a, 'src', '// Leaf: ?CrossProduct@C3DVector\n', '// Leaf: ?Cross@C3DVector@@QBE?BV1@ABV1@@Z\n// Name: FableWin.pdb ?Cross@C3DVector@@QBE?BV1@ABV1@@Z (0x01D8E950; the only C3DVector cross-product member of either PDB; the body is the\n//   cross product with the hidden result pointer, const this, `ret 8`). Naming wave 19: landed as the descriptive CrossProduct.\n')
    he.resub(a, 'src', r'C3DVector CrossProduct\(const C3DVector& other\) const;', 'const C3DVector Cross(const C3DVector& other) const;')
    he.resub(a, 'src', r'C3DVector C3DVector::CrossProduct\(const C3DVector& other\) const', 'const C3DVector C3DVector::Cross(const C3DVector& other) const')
    he.resub(a, 'tst', r'\bCrossProduct\b', 'Cross')
    he.resub(a, 'tst', r'(?<!const )C3DVector Cross\(const C3DVector& other\) const;', 'const C3DVector Cross(const C3DVector& other) const;')
    he.resub(a, 'tst', r'(?<!const )C3DVector C3DVector::Cross\(const C3DVector& other\) const', 'const C3DVector C3DVector::Cross(const C3DVector& other) const')
# theme
a = '0063a390'
if a not in _X:
    he.edit(a, 'src', '// CWorldMap::GetGroundSizeZAt(const C3DVector&) -- height query: forwards to CMap::GetInterpolatedSizeZAt of the loaded map holding the position, else 0\n',
            '// CWorldMap::GetThemeSizeZAt(const C3DVector&) -- theme height query: forwards to CMap::GetThemeSizeZAt of the loaded map holding the position, else 0\n'
            '// Leaf: ?GetThemeSizeZAt@CWorldMap@@QBEMABVC3DVector@@@Z\n'
            '// Name (naming wave 19): Ego_r.pdb donor order GetGroundSizeZAt 0x006E7EC0 / GetThemeSizeZAt 0x006E7F50 / GetWaterSizeZAt 0x006E7FE0 = retail 0x0063A300 /\n'
            '//   0x0063A390 / 0x0063A420 (0x90 apart in both); the callee 0x008200B0 is then ?GetThemeSizeZAt@CMap@@IBEMABVC3DVector@@@Z (Ego_r 0x0067B2D0, after\n'
            '//   CMap::GetGroundSizeZAt 0x006796E0 = retail 0x0081EA50 of the twin row). Landed as the propagated GetGroundSizeZAt label.\n')
    he.resub(a, 'src', r'float GetInterpolatedSizeZAt\(const C3DVector& localPos\) const; // 0x008200B0', 'float GetThemeSizeZAt(const C3DVector& localPos) const;        // 0x008200B0 (Ego_r.pdb ?GetThemeSizeZAt@CMap@@IBEMABVC3DVector@@@Z; protected in the PDB, reached here through the row\'s public CMap model)')
    he.resub(a, 'src', r'theMap\.GetInterpolatedSizeZAt\(local\)', 'theMap.GetThemeSizeZAt(local)')
    he.resub(a, 'src', r'float GetGroundSizeZAt\(const C3DVector& pos\) const;\n\};', 'float GetThemeSizeZAt(const C3DVector& pos) const;\n};')
    he.resub(a, 'src', r'float CWorldMap::GetGroundSizeZAt\(const C3DVector& pos\) const', 'float CWorldMap::GetThemeSizeZAt(const C3DVector& pos) const')
    he.resub(a, 'src', r'CMap::GetInterpolatedSizeZAt 0x008200B0', 'CMap::GetThemeSizeZAt 0x008200B0')
    he.resub(a, 'tst', r'\bGetGroundSizeZAt\b', 'GetThemeSizeZAt')
    he.resub(a, 'tst', r'\bGetInterpolatedSizeZAt\b', 'GetThemeSizeZAt')
a = '00912700'
if a not in _X:
    he.edit(a, 'src', 'float GetGroundSizeZAt(const C3DVector&);}; //slot16=004FC0C0,0063A390', 'float GetThemeSizeZAt(const C3DVector&) const;}; //slot16=004FC0C0, 0063A390 = Ego_r.pdb ?GetThemeSizeZAt@CWorldMap@@QBEMABVC3DVector@@@Z (naming wave 19; the view class stays)')
    he.edit(a, 'src', 'PeekWorldMap()->GetGroundSizeZAt(*out)', 'PeekWorldMap()->GetThemeSizeZAt(*out)')
# exit
a = '009c4f20'
if a not in _X:
    he.edit(a, 'src', '    bool                                         ExitThread;\n', '    volatile bool                                ExitThread;             // +0x7a (volatile: see the Retail-volatile line)\n')
    he.resub(a, 'src', r'(// Retail-volatile: PCompletedRead[^\n]*\n)(?!// Retail-volatile: ExitThread)', r'\1// Retail-volatile: ExitThread (+0x7A) is written by the main thread (shutdown / SetDirtyDiscError) and re-read by the installer thread: 0x009C35FC `mov al,[eax+7Ah]` then 0x009C3608 `mov byte ptr [eax+7Ah],1` (the discarded read, chunk 680; the landed 0x009C3590 / 0x009C3660 / 0x009C5690 carry the qualifier); not read in this body.\n', 1)
# dwv
a = '00c73770'
if a not in _X:
    he.put(a, 'src', '''// EnsureWindowsVersionDetected_00c73770(self) -- GameCODA one-time guard: run the Windows-version probe DetectWindowsVersion() 0x00C73660 unless its
//   done flag is set, then return the object handed in (ecx in, eax out: a libpk init helper that passes its object through)
// Leaf: ?EnsureWindowsVersionDetected_00c73770@@YIPAXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C73770, 21 bytes, __fastcall, plain `ret`. Descriptive name (libpk / GameCODA, no PDB): the landed
//   CDefString::SetDefaultString label is a propagation; the callee is the landed ?DetectWindowsVersion@@YAXXZ (no arguments: the ecx the
//   caller still holds is this helper's own argument). Naming wave 19: re-authored with the callee's real signature (byte-identical).
extern "C" unsigned char g_defStringInit;   // 0x0143CCC0: the probe's done flag (the landed 0x00C73660 row's bound spelling; really "version probe done")
void __cdecl DetectWindowsVersion();        // 0x00C73660

void* __fastcall EnsureWindowsVersionDetected_00c73770(void* self)
{
    if (!g_defStringInit)
        DetectWindowsVersion();
    return self;
}
''')
    he.put(a, 'tst', '''// behaviour test of EnsureWindowsVersionDetected_00c73770 0x00C73770: the probe runs once while the flag is clear, never once it is set; the
// object is returned unchanged. Self-contained model (the lane links the test alone).
#include <stdio.h>
unsigned char g_defStringInit;
static int g_probes = 0;
void __cdecl DetectWindowsVersion() { ++g_probes; }
void* __fastcall EnsureWindowsVersionDetected_00c73770(void* self)
{
    if (!g_defStringInit)
        DetectWindowsVersion();
    return self;
}
int main()
{
    int a;
    g_defStringInit = 0;
    if (EnsureWindowsVersionDetected_00c73770(&a) != &a || g_probes != 1) { printf("FAIL1\\n"); return 1; }
    g_defStringInit = 1;
    if (EnsureWindowsVersionDetected_00c73770(&a) != &a || g_probes != 1) { printf("FAIL2\\n"); return 1; }
    printf("OK_00c73770\\n");
    return 0;
}
''')
