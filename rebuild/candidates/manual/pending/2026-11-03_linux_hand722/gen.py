# gen.py: write src/<addr>.cpp + test/<addr>_test.cpp for the chunk-722 CEngine forwarders
import os
D = os.path.dirname(os.path.abspath(__file__))
H  = ('CEnginePrimitiveHandle&', 'handle')
HC = ('const CEnginePrimitiveHandle&', 'parentHandle')
P  = ('const CEnginePrimitive&', 'primitive')
MAPR = ('CPrimitiveStatMap&', 'stats')
MAPC = ('const CPrimitiveStatMap&', 'stats')
COL = 'const CRGBFloatColour&'
G = {
 '01436EA0': ('CEngineCamera', 'g_pEngineCamera'),
 '01436E80': ('CEnginePrimitiveRenderer', 'g_pPrimitiveRenderer_01436e80'),
 '01436E70': ('CEnginePrimitiveAttachmentManager', 'g_pPrimitiveAttachmentManager_01436e70'),
 '01436EAC': ('CEngineSurfaceCompositionManager', 'g_pSurfaceComposition_01436eac'),
 '01436E8C': ('CEngineWorldMap', 'g_pEngineWorldMap'),
 '01436E9C': ('CEngineLightingManager', 'g_pEngineLightingManager'),
 '01436E7C': ('CEnginePrimitiveRenderer2D', 'g_pEnginePrimitiveRenderer2D'),
 '01436E98': ('CEngineResourceManager', 'g_pResourceManager'),
 '01436E60': ('CEngineShadowRenderer', 'g_pShadowRenderer_01436e60'),
 '01436E44': ('CEngineWeatherRenderer', 'g_pEngineWeatherRenderer'),
}
# addr, slot, ret, name, const, params, global, callee addr, callee name, callee const, leaf, callee leaf, what
R = [
 ('00b23b80', 5, 'void', 'SetWindow', 0, [('const C2DBoxF*', 'window')], '01436EA0', '00B2FC00', 'SetGameWindow', 0,
  '?SetWindow@CEngine@@UAEXPBVC2DBoxF@@@Z', '?SetGameWindow@CEngineCamera@@QAEXPBVC2DBoxF@@@Z', 'hand the game window box to the engine camera'),
 ('00b23bb0', 20, 'void', 'AddPrimitive', 0, [P], '01436E80', '00B32E90', 'AddPrimitive', 0,
  '?AddPrimitive@CEngine@@UAEXABVCEnginePrimitive@@@Z', '?AddPrimitive@CEnginePrimitiveRenderer@@QAEXABVCEnginePrimitive@@@Z', 'add a one-frame primitive through the primitive renderer'),
 ('00b23be0', 24, 'void', 'SetPrimitiveAnimation', 0, [H, ('const CEnginePrimitiveAnimation&', 'animation')], '01436E80', '00B31E90', 'SetPrimitiveAnimation', 0,
  '?SetPrimitiveAnimation@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitiveAnimation@@@Z', '?SetPrimitiveAnimation@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitiveAnimation@@@Z', 'set a primitive\'s animation through the primitive renderer'),
 ('00b23bf0', 25, 'void', 'SetPrimitivePosition', 0, [H, ('const CEnginePrimitivePosition&', 'position')], '01436E80', '00B31EC0', 'SetPrimitivePosition', 0,
  '?SetPrimitivePosition@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitivePosition@@@Z', '?SetPrimitivePosition@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitivePosition@@@Z', 'set a primitive\'s position through the primitive renderer'),
 ('00b23c20', 26, 'unsigned long', 'AddChildPrimitive', 0, [H, P, ('unsigned long', 'flags')], '01436E80', '00B31EF0', 'AddChildPrimitive', 0,
  '?AddChildPrimitive@CEngine@@UAEKAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K@Z', '?AddChildPrimitive@CEnginePrimitiveRenderer@@QAEKAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K@Z', 'add a child primitive under a handle and return its child index'),
 ('00b23c30', 27, 'void', 'SetChildPrimitive', 0, [H, ('unsigned long', 'childIndex'), ('CEnginePrimitiveHandle&', 'childHandle'), P, ('unsigned long', 'flags')], '01436E80', '00B31F20', 'SetChildPrimitive', 0,
  '?SetChildPrimitive@CEngine@@UAEXAAVCEnginePrimitiveHandle@@K0ABVCEnginePrimitive@@K@Z', '?SetChildPrimitive@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@K0ABVCEnginePrimitive@@K@Z', 'replace a child primitive (five-argument overload, child handle returned)'),
 ('00b23c40', 29, 'void', 'GetChildPrimitiveHandle', 0, [H, ('unsigned long', 'childIndex'), ('CEnginePrimitiveHandle&', 'childHandle')], '01436E80', '00B31F50', 'GetChildPrimitiveHandle', 0,
  '?GetChildPrimitiveHandle@CEngine@@UAEXAAVCEnginePrimitiveHandle@@K0@Z', '?GetChildPrimitiveHandle@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@K0@Z', 'fetch the handle of a child primitive'),
 ('00b23c50', 28, 'void', 'SetChildPrimitive', 0, [H, ('unsigned long', 'childIndex'), P, ('unsigned long', 'flags')], '01436E80', '00B31F70', 'SetChildPrimitive', 0,
  '?SetChildPrimitive@CEngine@@UAEXAAVCEnginePrimitiveHandle@@KABVCEnginePrimitive@@K@Z', '?SetChildPrimitive@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@KABVCEnginePrimitive@@K@Z', 'replace a child primitive (four-argument overload)'),
 ('00b23c60', 30, 'void', 'ClearChildPrimitive', 0, [H, ('unsigned long', 'childIndex')], '01436E80', '00B31FA0', 'ClearChildPrimitive', 0,
  '?ClearChildPrimitive@CEngine@@UAEXAAVCEnginePrimitiveHandle@@K@Z', '?ClearChildPrimitive@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@K@Z', 'clear one child primitive'),
 ('00b23c70', 31, 'void', 'ClearAllChildPrimitives', 0, [H], '01436E80', '00B31FC0', 'ClearAllChildPrimitives', 0,
  '?ClearAllChildPrimitives@CEngine@@UAEXAAVCEnginePrimitiveHandle@@@Z', '?ClearAllChildPrimitives@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@@Z', 'clear every child primitive of a handle'),
 ('00b23c80', 32, 'void', 'SetAttachedPrimitive', 0, [H, P, ('const CEnginePrimitiveAttachmentInfo&', 'attachmentInfo'), HC, ('long', 'attachIndex'), ('unsigned long', 'flags')], '01436E70', '00B3A180', 'SetAttachedPrimitive', 0,
  '?SetAttachedPrimitive@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@ABVCEnginePrimitiveAttachmentInfo@@ABV2@JK@Z', '?SetAttachedPrimitive@CEnginePrimitiveAttachmentManager@@QAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@ABVCEnginePrimitiveAttachmentInfo@@ABV2@JK@Z', 'attach a primitive to a parent primitive through the attachment manager'),
 ('00b23c90', 33, 'void', 'CollectPrimitiveStatsForArea', 1, [MAPR, ('const C2DBoxF&', 'area'), ('EEnginePrimitiveStatIndex', 'statIndex'), ('unsigned long', 'flags')], '01436E80', '00B321B0', 'CollectPrimitiveStatsForArea', 1,
  '?CollectPrimitiveStatsForArea@CEngine@@UBEXAAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@ABVC2DBoxF@@W4EEnginePrimitiveStatIndex@@K@Z', '?CollectPrimitiveStatsForArea@CEnginePrimitiveRenderer@@QBEXAAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@ABVC2DBoxF@@W4EEnginePrimitiveStatIndex@@K@Z', 'collect primitive statistics for an area of the world'),
 ('00b23ca0', 34, 'void', 'ProcessPrimitiveStats', 1, [MAPC, ('EEnginePrimitiveStatIndex', 'statIndex'), ('unsigned long&', 'total'), ('CArray<CCharString>&', 'report')], '01436E80', '00B33390', 'ProcessPrimitiveStats', 1,
  '?ProcessPrimitiveStats@CEngine@@UBEXABV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@W4EEnginePrimitiveStatIndex@@AAKAAV?$CArray@VCCharString@@@@@Z', '?ProcessPrimitiveStats@CEnginePrimitiveRenderer@@QBEXABV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@W4EEnginePrimitiveStatIndex@@AAKAAV?$CArray@VCCharString@@@@@Z', 'turn collected primitive statistics into a total and report lines'),
 ('00b23ce0', 36, 'unsigned long', 'AddCustomMorphTexture', 1, [('CTexture&', 'texture'), ('const CCharString&', 'name')], '01436EAC', '00B2C4E0', 'AddCustomMorphTexture', 0,
  '?AddCustomMorphTexture@CEngine@@UBEKAAVCTexture@@ABVCCharString@@@Z', '?AddCustomMorphTexture@CEngineSurfaceCompositionManager@@QAEKAAVCTexture@@ABVCCharString@@@Z', 'register a custom morph texture with the surface-composition manager'),
 ('00b23db0', 41, 'void', 'CloseStaticMapFile', 0, [], '01436E8C', '00B40000', 'CloseStaticMapFile', 0,
  '?CloseStaticMapFile@CEngine@@UAEXXZ', '?CloseStaticMapFile@CEngineWorldMap@@QAEXXZ', 'close the world map\'s static map file'),
 ('00b23dc0', 52, 'void', 'SetStaticMapFileForUse', 0, [('const CWideString&', 'fileName')], '01436E8C', '00B428E0', 'SetStaticMapFileForUse', 0,
  '?SetStaticMapFileForUse@CEngine@@UAEXABVCWideString@@@Z', '?SetStaticMapFileForUse@CEngineWorldMap@@QAEXABVCWideString@@@Z', 'select the world map\'s static map file'),
 ('00b23e00', 7, 'void', 'SetWorld', 0, [('const CIDrawWorld*', 'world')], '01436E8C', '00B42ED0', 'SetWorld', 0,
  '?SetWorld@CEngine@@UAEXPBVCIDrawWorld@@@Z', '?SetWorld@CEngineWorldMap@@QAEXPBVCIDrawWorld@@@Z', 'hand the game\'s draw-world interface to the world map'),
 ('00b23e10', 42, 'void', 'AddMap', 0, [('long', 'mapIndex'), ('long', 'mapUID')], '01436E8C', '00B42D10', 'AddMap', 0,
  '?AddMap@CEngine@@UAEXJJ@Z', '?AddMap@CEngineWorldMap@@QAEXJJ@Z', 'add a map to the world map'),
 ('00b23e20', 43, 'void', 'RemoveMap', 0, [('long', 'mapIndex')], '01436E8C', '00B42B10', 'DeleteMap', 0,
  '?RemoveMap@CEngine@@UAEXJ@Z', '?DeleteMap@CEngineWorldMap@@QAEXJ@Z', 'delete a map from the world map'),
 ('00b23e30', 44, 'void', 'SetMapAsLoaded', 0, [('long', 'mapIndex')], '01436E8C', '00B401E0', 'OnGameMapLoaded', 0,
  '?SetMapAsLoaded@CEngine@@UAEXJ@Z', '?OnGameMapLoaded@CEngineWorldMap@@QAEXJ@Z', 'tell the world map that the game has loaded a map'),
 ('00b23e40', 47, 'void', 'SetMapVisibility', 0, [('long', 'mapIndex'), ('bool', 'visible')], '01436E8C', '00B42390', 'SetMapVisibility', 0,
  '?SetMapVisibility@CEngine@@UAEXJ_N@Z', '?SetMapVisibility@CEngineWorldMap@@QAEXJ_N@Z', 'show or hide one map'),
 ('00b23e50', 46, 'void', 'SetMapVisibilityAllMaps', 0, [('bool', 'visible')], '01436E8C', '00B423B0', 'SetMapVisibilityAllMaps', 0,
  '?SetMapVisibilityAllMaps@CEngine@@UAEX_N@Z', '?SetMapVisibilityAllMaps@CEngineWorldMap@@QAEX_N@Z', 'show or hide every map'),
 ('00b23e60', 45, 'void', 'SetMapAsUnloaded', 0, [('long', 'mapIndex')], '01436E8C', '00B40230', 'OnGameMapUnloaded', 0,
  '?SetMapAsUnloaded@CEngine@@UAEXJ@Z', '?OnGameMapUnloaded@CEngineWorldMap@@QAEXJ@Z', 'tell the world map that the game has unloaded a map'),
 ('00b23ea0', 66, 'void', 'SetGlobalLightingParameters', 0, [('const C3DVector&', 'lightDirection'), (COL, 'diffuse'), (COL, 'ambient'), (COL, 'backlight')], '01436E9C', '00B47280', 'SetGlobalLightingParameters', 0,
  '?SetGlobalLightingParameters@CEngine@@UAEXABVC3DVector@@ABVCRGBFloatColour@@11@Z', '?SetGlobalLightingParameters@CEngineLightingManager@@QAEXABVC3DVector@@ABVCRGBFloatColour@@11@Z', 'set the global light direction and colours on the lighting manager'),
 ('00b23eb0', 67, 'void', 'Set2DPrimitiveLightingParameters', 0, [('const C3DVector&', 'lightDirection'), (COL, 'diffuse'), (COL, 'ambient'), (COL, 'backlight')], '01436E7C', '00B4A5F0', 'SetLightingParameters', 0,
  '?Set2DPrimitiveLightingParameters@CEngine@@UAEXABVC3DVector@@ABVCRGBFloatColour@@11@Z', '?SetLightingParameters@CEnginePrimitiveRenderer2D@@QAEXABVC3DVector@@ABVCRGBFloatColour@@11@Z', 'set the 2D primitive renderer\'s light direction and colours'),
 ('00b23f40', 69, 'CTexture*', 'AllocateTextureFromEngineResourcePools', 0, [('unsigned long', 'usage'), ('const C2DExtentsI&', 'extents'), ('long', 'mipLevels'), ('const CPixelFormat&', 'format')], '01436E98', '00B3B1F0', 'AllocateExternalTextureFromResourcePools', 0,
  '?AllocateTextureFromEngineResourcePools@CEngine@@UAEPAVCTexture@@KABVC2DExtentsI@@JABVCPixelFormat@@@Z', '?AllocateExternalTextureFromResourcePools@CEngineResourceManager@@QAEPAVCTexture@@KABVC2DExtentsI@@JABVCPixelFormat@@@Z', 'allocate a texture for outside use from the resource manager\'s pools'),
 ('00b23f50', 70, 'void', 'FreeTextureFromEngineResourcePools', 0, [('CTexture*', 'texture')], '01436E98', '00B3B5B0', 'FreeExternalTextureFromResourcePools', 0,
  '?FreeTextureFromEngineResourcePools@CEngine@@UAEXPAVCTexture@@@Z', '?FreeExternalTextureFromResourcePools@CEngineResourceManager@@QAEXPAVCTexture@@@Z', 'return an outside texture to the resource manager\'s pools'),
 ('00b23f70', 74, 'bool', 'DeallocateShadowBuffer', 0, [], '01436E60', '00B4B890', 'XBoxDeallocateShadowBuffer', 0,
  '?DeallocateShadowBuffer@CEngine@@UAE_NXZ', '?XBoxDeallocateShadowBuffer@CEngineShadowRenderer@@QAE_NXZ', 'ask the shadow renderer to free its shadow buffer (an Xbox-only path; the PC callee returns false)'),
 ('00b23f80', 75, 'void', 'ReallocateShadowBuffer', 0, [], '01436E60', '00B4B8A0', 'XBoxReallocateShadowBuffer', 0,
  '?ReallocateShadowBuffer@CEngine@@UAEXXZ', '?XBoxReallocateShadowBuffer@CEngineShadowRenderer@@QAEXXZ', 'ask the shadow renderer to rebuild its shadow buffer (an Xbox-only path; the PC callee is a bare `ret`)'),
 ('00b24010', 80, 'void', 'OverrideRain', 0, [('float', 'amount')], '01436E44', '00B50EE0', 'OverrideRain', 0,
  '?OverrideRain@CEngine@@UAEXM@Z', '?OverrideRain@CEngineWeatherRenderer@@QAEXM@Z', 'override the weather renderer\'s rain amount'),
 ('00b24020', 81, 'void', 'OverrideSnow', 0, [('float', 'amount')], '01436E44', '00B50F40', 'OverrideSnow', 0,
  '?OverrideSnow@CEngine@@UAEXM@Z', '?OverrideSnow@CEngineWeatherRenderer@@QAEXM@Z', 'override the weather renderer\'s snow amount'),
]
CLASSES = ['C2DBoxF','CEnginePrimitive','CEnginePrimitiveHandle','CEnginePrimitiveAnimation','CEnginePrimitivePosition','CEnginePrimitiveAttachmentInfo','CTexture','CCharString','CWideString','CIDrawWorld','C3DVector','CRGBFloatColour','C2DExtentsI','CPixelFormat']
MAPDECL = '''namespace std {                                   // retail STLport 4.6 lives in std (EXTRA_RULES 2026-10-25)
template <class _Tp> struct less;
template <class _Tp> class allocator;
template <class _T1, class _T2> struct pair;
template <class _Key, class _Tp, class _Compare, class _Alloc> class map;
}
typedef std::map<unsigned long, unsigned long, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, unsigned long> > > CPrimitiveStatMap;
'''
def sig(params):
    return ', '.join('%s %s' % p for p in params)
def write(row):
    a, slot, ret, name, const, params, g, caddr, cname, cconst, leaf, cleaf, what = row
    gcls, gname = G[g]
    text = ' '.join(p[0] for p in params) + ' ' + ret
    used = [c for c in CLASSES if c in text.replace('const ', ' ')]
    s = []
    s.append('// CEngine::%s(%s)%s -- %s (a CIEngine interface forwarder)' % (name, sig(params), ' const' if const else '', what))
    s.append('// Leaf: %s' % leaf)
    s.append('// Flags: none')
    s.append('// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)')
    s.append('// Retail: Fable.exe (TLC, VC7.1) function @ 0x%s, length 11 bytes: `mov ecx,[0x%s] ; jmp 0x%s`.' % (a.upper(), g, caddr))
    s.append('// Identity: slot %d of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry' % slot)
    s.append('//   %s (the virtual CIEngine method, `this` never read).' % leaf)
    s.append('// Callee: %s::%s @ 0x%s, Ego_r + FableWin %s;' % (gcls, cname, caddr, cleaf))
    nargs = len(params)
    s.append('//   its `ret %s` pops the same %d stack argument%s, so VC7.1 passes them through untouched and tail-jumps.' % (('%xh' % (4*nargs)) if nargs else '' , nargs, '' if nargs == 1 else 's'))
    s.append('// Global: the engine component pointer 0x%s (%s*) under its landed spelling %s; it has no PDB data symbol' % (g, gcls, gname))
    s.append('//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).')
    s.append('// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot')
    s.append('//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).')
    for c in used: s.append('class %s;' % c)
    if 'CArray' in text: s.append('template <class T> class CArray;')
    if 'EEnginePrimitiveStatIndex' in text: s.append('enum EEnginePrimitiveStatIndex { };')
    if 'CPrimitiveStatMap' in text: s.append(MAPDECL.rstrip('\n'))
    s.append('')
    s.append('class %s { public:' % gcls)
    s.append('    %s %s(%s)%s;   // 0x%s' % (ret, cname, sig(params), ' const' if cconst else '', caddr))
    s.append('};')
    s.append('extern %s* %s;   // 0x%s' % (gcls, gname, g))
    s.append('')
    s.append('class CEngine { public:                      // vtable 0x012A0F3C, slot %d' % slot)
    s.append('    virtual %s %s(%s)%s;' % (ret, name, sig(params), ' const' if const else ''))
    s.append('};')
    s.append('')
    s.append('%s CEngine::%s(%s)%s' % (ret, name, sig(params), ' const' if const else ''))
    s.append('{')
    s.append('    %s%s->%s(%s);' % ('' if ret == 'void' else 'return ', gname, cname, ', '.join(p[1] for p in params)))
    s.append('}')
    open('%s/src/%s.cpp' % (D, a), 'w').write('\n'.join(s) + '\n')
    # ---- behaviour test
    t = []
    t.append('// Behaviour test for 0x%s CEngine::%s: the forwarder must call %s::%s on the published component' % (a.upper(), name, gcls, cname))
    t.append('// pointer with every argument unchanged and return the callee\'s result.')
    t.append('#include <cstdio>')
    for c in used: t.append('class %s { public: int v; };' % c)
    if 'CArray' in text: t.append('template <class T> class CArray { public: int n; };')
    if 'EEnginePrimitiveStatIndex' in text: t.append('enum EEnginePrimitiveStatIndex { STAT_INDEX_THREE = 3 };')
    if 'CPrimitiveStatMap' in text: t.append('class CPrimitiveStatMap { public: int n; };   // stands in for the STLport map (only its address travels)')
    t.append('static int gCalls = 0;')
    t.append('static const void* gThis = 0;')
    t.append('static const void* gPtr[8];')
    t.append('static double gVal[8];')
    t.append('class %s { public:' % gcls)
    t.append('    %s %s(%s)%s' % (ret, cname, sig(params), ' const' if cconst else ''))
    t.append('    {')
    t.append('        ++gCalls; gThis = this;')
    for i, (ty, nm) in enumerate(params):
        if '&' in ty: t.append('        gPtr[%d] = &%s;' % (i, nm))
        elif '*' in ty: t.append('        gPtr[%d] = %s;' % (i, nm))
        else: t.append('        gVal[%d] = (double)%s;' % (i, nm))
    if ret == 'unsigned long': t.append('        return 0x1234u;')
    elif ret == 'bool': t.append('        return true;')
    elif ret == 'CTexture*': t.append('        static CTexture made; return &made;')
    t.append('    }')
    t.append('};')
    t.append('static %s gComponent;' % gcls)
    t.append('%s* %s = &gComponent;' % (gcls, gname))
    t.append('class CEngine { public:')
    t.append('    virtual %s %s(%s)%s;' % (ret, name, sig(params), ' const' if const else ''))
    t.append('};')
    t.append('%s CEngine::%s(%s)%s' % (ret, name, sig(params), ' const' if const else ''))
    t.append('{')
    t.append('    %s%s->%s(%s);' % ('' if ret == 'void' else 'return ', gname, cname, ', '.join(p[1] for p in params)))
    t.append('}')
    t.append('int main()')
    t.append('{')
    args = []; checks = []
    for i, (ty, nm) in enumerate(params):
        base = ty.replace('const ', '').replace('&', '').replace('*', '').strip()
        if '&' in ty or '*' in ty:
            if base in ('unsigned long',):
                t.append('    unsigned long o%d = 5;' % i)
            else:
                t.append('    %s o%d;' % (base, i))
            args.append(('o%d' % i) if '&' in ty else ('&o%d' % i))
            checks.append('gPtr[%d] != &o%d' % (i, i))
        elif ty == 'bool':
            args.append('true'); checks.append('gVal[%d] != 1.0' % i)
        elif ty == 'float':
            args.append('2.5f'); checks.append('(gVal[%d] < 2.49 || gVal[%d] > 2.51)' % (i, i))
        elif ty == 'EEnginePrimitiveStatIndex':
            args.append('STAT_INDEX_THREE'); checks.append('gVal[%d] != 3.0' % i)
        elif ty == 'long':
            args.append('%d' % (-7 - i)); checks.append('gVal[%d] != %d.0' % (i, -7 - i))
        else:
            args.append('%du' % (40 + i)); checks.append('gVal[%d] != %d.0' % (i, 40 + i))
    t.append('    CEngine engine;')
    call = 'engine.%s(%s)' % (name, ', '.join(args))
    if ret == 'void': t.append('    %s;' % call); rc = None
    elif ret == 'CTexture*': t.append('    CTexture* r = %s;' % call); rc = 'r == 0'
    elif ret == 'bool': t.append('    bool r = %s;' % call); rc = '!r'
    else: t.append('    unsigned long r = %s;' % call); rc = 'r != 0x1234u'
    cond = ['gCalls != 1', 'gThis != &gComponent'] + checks + ([rc] if rc else [])
    t.append('    if (%s) {' % (' || '.join(cond)))
    t.append('        std::printf("%s_TEST FAIL\\n");' % a)
    t.append('        return 1;')
    t.append('    }')
    t.append('    std::printf("%s_TEST PASS\\n");' % a)
    t.append('    return 0;')
    t.append('}')
    open('%s/test/%s_test.cpp' % (D, a), 'w').write('\n'.join(t) + '\n')
for r in R: write(r)
print(len(R))
