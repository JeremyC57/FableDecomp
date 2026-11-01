# h16_stlvec.py: naming wave 16, family stl -- 0x009C2F50 std::vector<CQuickDrawTriInfo>::~vector re-authored from its descriptive
# stand-in (CRetailVector_SOcclusionEntry_009c2f50) on the real STLport 4.6 text the landed ~CRenderManager2D 0x009C2FD0 row compiles
# (its std block and element classes copied verbatim), explicitly instantiated for the proven element type.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import path, _w, R
import rows
src = open(os.path.join(rows.ROOT, 'rebuild/src/compiled', R['009c2fd0']['Source']), encoding='utf-8').read()
i = src.index('extern "C" void __cdecl free(void* block);'); j = src.index('class CTVertexRHWColSpecTex1')
STD = src[i:j]
assert STD.count('namespace std {') == 1 and 'class CQuickDrawTriInfo' in STD
H = '''// std::vector<CQuickDrawTriInfo, std::allocator<CQuickDrawTriInfo> >::~vector() -- destroy every 60-byte element (its texture, then
//   its string), then free the storage
// Leaf: ??1?$vector@VCQuickDrawTriInfo@@V?$allocator@VCQuickDrawTriInfo@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C2F50, 58 bytes, thiscall.
// Identity (proven, NAME_FIXES_QUEUE): called by ~CRenderManager2D 0x009C2FD0 on its three vector<CQuickDrawTriInfo> members (+0x3E7C /
//   +0x3E88 / +0x3E94; Ego_r CRenderManager2D member types); body = STLport ~vector (_Destroy over [start, finish): ~CTexture ->
//   CTexture::Uninitialise at +0x18, ~CWideString at +0x14), then ~_Vector_base frees a non-null start.
// Callees: CTexture::Uninitialise 0x009F9F70, CWideString::~CWideString 0x0099B510, free 0x00BFEA14.
// Shape: the real STLport 4.6 text and the CQuickDrawTriInfo model (Ego_r member names) copied from the landed ~CRenderManager2D
//   0x009C2FD0 row (whose TU emits this COMDAT), explicitly instantiated for the proven element type.
// Naming wave 16: landed as the descriptive CRetailVector_SOcclusionEntry_009c2f50::~CRetailVector_SOcclusionEntry_009c2f50 (manifest
//   label CCombatWheel::ResetRings is propagated); re-authored on the proven STLport instantiation (name-only: the bytes are unchanged).
'''
TAIL = 'template class std::vector<CQuickDrawTriInfo>;\n'
p, rel = path('009c2f50', 'src')
t = H + STD + TAIL
if open(p, encoding='utf-8', newline='').read() == t: print('already 009c2f50')
else: _w(p, t); print('ok 009c2f50 src')
