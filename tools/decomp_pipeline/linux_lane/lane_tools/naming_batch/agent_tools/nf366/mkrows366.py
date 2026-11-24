# mkrows366.py: writes rows/<addr>.cpp (forward-declaration form, the landed 0x00428B10 shape) and var/<addr>_inc.cpp (the T's shared
# header included instead, for the four Ts that have one) for NF366's seven @DeleteIfSet@4 respells. Comment text is frozen here.
import os
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
R = {
 '00447660': dict(T='const CInputTypeBase', M='$$CBVCInputTypeBase', fwd='class CInputTypeBase;', hdr=None,
   reset='0x00447D90', why='CCountedPointer<const CInputTypeBase>::Reset 0x00447D90, called at 0x00447216 with a fresh CInputTypeXboxPadButtonEvent / LeftStickEvent / RightStickEvent (RTTI vtables 0x012317A8 / 0x012317CC / 0x012317F0, each deriving CInputTypeBase); FableWin.pdb also names Reset(const CInputTypeBase*), and Ego_r.pdb types CGamePlayerInterface::Controls / *Buttons as CCountedPointer<const CInputTypeBase>'),
 '00a28370': dict(T='CMeshDataBankEntry', M='VCMeshDataBankEntry', fwd='class CMeshDataBankEntry;', hdr='engine/CMeshDataBankEntry.h', hdrdefs=[],
   reset='0x00A29D20', why='CCountedPointer<CMeshDataBankEntry>::Reset 0x00A29D20, called by CMeshDataBank::NotifyEntryAdded 0x00A26D40 (+0x2B0) with a fresh C3DAnimationInfo (RTTI vtable 0x0129CD64, base CMeshDataBankEntry); FableWin.pdb also names Reset(CMeshDataBankEntry*), and Ego_r.pdb types CMeshDataBank::DynamicInfoBuffer as CArray<CCountedPointer<CMeshDataBankEntry>>'),
 '00a281e0': dict(T='C3DMesh2', M='VC3DMesh2', fwd='class C3DMesh2;', hdr='engine/C3DMesh2.h', hdrdefs=[],
   reset='0x00A299C0', why='CCountedPointer<C3DMesh2>::Reset 0x00A299C0, called twice by C3DMeshLODInfo::BuildMesh 0x00A24520 with a fresh C3DMesh2 (constructor 0x00A89190, RTTI vtable 0x0129DEF0); FableWin.pdb also names Reset(C3DMesh2*), the reloc_bindings line witnessed by 0x00A24520 binds that Reset at 0x00A299C0, and Ego_r.pdb types C3DMeshLODInfo::Mesh as CCountedPointer<C3DMesh2>'),
 '00aa4310': dict(T='QuadricOptimiserInternals::COptimisedMesh', M='VCOptimisedMesh@QuadricOptimiserInternals', fwd='namespace QuadricOptimiserInternals { class COptimisedMesh; }', hdr=None,
   reset='0x00AA4460', why='CCountedPointer<QuadricOptimiserInternals::COptimisedMesh>::Reset 0x00AA4460, called by C3DQuadricBasedMeshOptimiser::BeginOptimisation 0x00AA4210 (+0x88) with a fresh COptimisedMesh (constructor 0x00ACE600, RTTI vtable 0x0129F05C .?AVCOptimisedMesh@QuadricOptimiserInternals@@); FableWin.pdb also names Reset(QuadricOptimiserInternals::COptimisedMesh*)'),
 '00b3e080': dict(T='CLoadingMemoryFailureHandler', M='VCLoadingMemoryFailureHandler', fwd='class CLoadingMemoryFailureHandler;', hdr=None,
   reset='0x00B3E510', why='CCountedPointer<CLoadingMemoryFailureHandler>::Reset 0x00B3E510, called by CEngineResourceManager::AllocatePools 0x00B3B240 (+0x247) with a fresh CLoadingMemoryFailureHandler (constructor 0x009D5EB0, RTTI vtable 0x0129B8C4); FableWin.pdb also names Reset(CLoadingMemoryFailureHandler*), and Ego_r.pdb types CEngineResourceManager::LoadingMemoryFailureHandler as that pointer'),
 '00cd42e0': dict(T='CScriptDefinitionManager', M='VCScriptDefinitionManager', fwd='class CScriptDefinitionManager;', hdr='engine/CScriptDefinitionManager.h', hdrdefs=['FABLE_RETAIL_CCHARSTRING', 'FABLE_RETAIL_CDEFSTRINGTABLE', 'FABLE_RETAIL_DEF_CLASS_BASE', 'FABLE_RETAIL_CRESOURCE', 'FABLE_RETAIL_CRESOURCELIST', 'FABLE_RETAIL_CDEFINITIONMANAGER'], hdrinc=['fable_string.h'],
   reset='0x00CD43A0', why='CCountedPointer<CScriptDefinitionManager>::Reset 0x00CD43A0, whose only caller is the 12-byte CScriptDefinitionManager::Set 0x00CD3F40 (FableWin.pdb + Ego_r.pdb ?Set@CScriptDefinitionManager@@SIXPAV1@@Z, Ego_r 12 bytes: `push ecx; mov ecx, offset PClass; call Reset; ret`), called by CScriptInfoManager::Init 0x00CB5D80 with a fresh CScriptDefinitionManager (constructor 0x00CD3F00, vtable 0x012C2648 without RTTI); FableWin.pdb also names Reset(CScriptDefinitionManager*) and the static CScriptDefinitionManager::PClass (0x0143E920)'),
 '00f38920': dict(T='CRenderTarget', M='VCRenderTarget', fwd='class CRenderTarget;', hdr='engine/CRenderTarget.h', hdrdefs=['FABLE_RETAIL_CSURFACE', 'FABLE_RETAIL_CSURFACE_GETD3DSURFACE'],
   reset='0x00F39CA0', why='CCountedPointer<CRenderTarget>::Reset 0x00F39CA0, called by CSurfaceCompositor::CSurfaceCompositor 0x00F38990 with a fresh CRenderTarget (RTTI vtable 0x0129C800); FableWin.pdb also names Reset(CRenderTarget*), and Ego_r.pdb types CSurfaceCompositor::RenderTarget as CCountedPointer<CRenderTarget>'),
}
import textwrap
def para(first, text):
    w = textwrap.wrap(text, width=140, initial_indent=first, subsequent_indent='//   ', break_long_words=False, break_on_hyphens=False)
    return w
def text(a, r, inc):
    sym = '?DeleteData@?$CCountedPointer@%s@@@@SIXPAX@Z' % r['M']
    L = para('// ', 'CCountedPointer<%s>::DeleteData(void* data) -- the counted pointer\'s per-type deleter: null-guarded virtual destroy (vtable slot 0, flag arg 1), i.e. `delete (T*)data` through the scalar deleting destructor.' % r['T'])
    L += ['// Leaf: ' + sym, '// __fastcall pointer=ecx; the callee is the object\'s own slot 0.']
    L += para('// ', 'Names (NF366, 2026-11-24; was the behavioural extern "C" @DeleteIfSet@4, catalog sub_%s): FableWin.pdb %s (static __fastcall void(void*)); retail stores this address as the count block\'s DeleteFunc (`mov dword ptr [eax+4], 0x%s`) in %s.' % (a, sym, a.upper(), r['why']))
    L += para('// ', 'Respelled in place in the 0x00428B10 form: an explicit specialization of engine/CCountedPointer.h\'s member (a strong definition, as the landed extern "C" function was) with the landed body over its local slot-0 model; T is only %s. FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY (the landed NF222 / NF282 opt-in) keeps the member declaration-only: with the header\'s in-class body VC7.1 emits no symbol for the specialization (NF360 measurement).' % ('declared by its shared header' if inc else 'forward-declared'))
    for d in (r.get('hdrdefs') or []) if inc else []: L.append('#define ' + d)
    L += ['#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY', '#include "engine/CCountedPointer.h"', '']
    L += (['#include "%s"' % x for x in r.get('hdrinc', [])] + ['#include "%s"' % r['hdr']] if inc else [r['fwd']])
    L += [ '', 'struct Obj { virtual void Destroy(int flags); };', '', 'template <>',
          'void FABLE_FASTCALL CCountedPointer<%s>::DeleteData(void* data)' % r['T'], '{', '    if (data)', '        static_cast<Obj*>(data)->Destroy(1);', '}', '']
    return '\n'.join(L)
os.makedirs(A + '/rows', exist_ok=True); os.makedirs(A + '/var', exist_ok=True)
# Adopted texts (NF366): rows/<a>.cpp = the include form where T has a shared header (measured OTHER_SAME, .drectve equal), else the
# forward-declaration form; var/<a>_inc.cpp and var/<a>_fwd.cpp keep both measured forms.
for a, r in R.items():
    open('%s/rows/%s.cpp' % (A, a), 'w').write(text(a, r, bool(r['hdr'])))
    if r['hdr']:
        open('%s/var/%s_inc.cpp' % (A, a), 'w').write(text(a, r, True)); open('%s/var/%s_fwd.cpp' % (A, a), 'w').write(text(a, r, False))
print('ok')
