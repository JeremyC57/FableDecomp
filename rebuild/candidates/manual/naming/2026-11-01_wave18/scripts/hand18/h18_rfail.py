# h18_rfail.py: naming wave 18, family rfail -- the four wave 17 [recheck-fail] rows, re-authored name-only with their tests:
#  0x00401F30 GFGetBuildNumber2: CDiskFileWin32 modelled polymorphic (PDB ?Close@CDiskFileWin32@@UAEXXZ, the protected virtual
#    ??1CBaseClassNonCopyable@@MAE@XZ, ctor ??0CDiskFileWin32@@QAE@ABVCWideString@@W4FileAccessType@@K@Z); the explicit destructor
#    call is qualified (CDiskFileWin32::~CDiskFileWin32) so it stays direct, the inlined destructor calls Close directly and
#    stores the compiler's ??_7CDiskFileWin32@@6B@ (0x0122D06C). Test: the vtable spy reads the constructed vptr.
#  0x006A2520 CTCCarrying::SetAllCarriedThingsAsBypassFilter(void*) (PDB QAEXPAX): BypassFilters / Key members, the callee
#    ?ResetAllCarriedThingsAsBypassFilter@CTCCarrying@@QAEXXZ 0x006A23E0 (test spy renamed).
#  0x009E1D00 / 0x009E1D50 GFRoundVX/VYToNearestPixel: CDisplayManager::GetRenderTargetDimensions() returning const C2DExtentsI
#    (PDB ?BVC2DExtentsI@@), CSystemManager::PDisplayManager +0x60, C2DExtentsI {X, Y}; the virtual-screen doubles keep their
#    row-local spellings (VIRTUAL_SCREEN_* gated). Tests follow the model.
# Name-only: the recheck proves the bytes (RELOCATION_MATCH + PASS in the ov_t1 trial).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import put
T = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'texts')
for a in ('00401f30', '006a2520', '009e1d00', '009e1d50'):
    put(a, 'src', open(os.path.join(T, a + '.cpp'), encoding='utf-8', newline='').read())
    put(a, 'tst', open(os.path.join(T, a + '_test.cpp'), encoding='utf-8', newline='').read())
