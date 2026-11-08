#!/usr/bin/env python3
"""gen.py [--n N] [--out FILE] [--test FILE]: write the CTCBase::RegisterTCs source (and its behaviour test) from probe/regs.tsv,
the retail statement list tools/regs.py extracts. --n N keeps only the first N registrations (probe builds).
The source header comment is tools/src_header.txt; the test frame is tools/test_head.txt + tools/test_tail.txt."""
import sys, os, re
A = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent884'
args = sys.argv[1:]
def opt(k, d=None): return args[args.index(k)+1] if k in args else d
N = int(opt('--n', '100000')); out = opt('--out', f'{A}/src/004ee23f.cpp'); test = opt('--test')
rows = [l.rstrip('\n').split('\t') for l in open(f'{A}/probe/regs.tsv')][1:]
# the static class setups RegisterTCs calls first (and RegisterModes last): retail address -> (class, function)
STATICS = {
    '0x749d60': ('CTCCreatureGenerator', 'StatRegisterClass', 0x00749D60),
    '0x7330f0': ('CTCCombat', 'StatRegisterClass', 0x007330F0),
    '0x7899c0': ('CTCHeroRangedAttacks', 'StatRegisterClass', 0x007899C0),
    '0x7fffa0': ('CTCHaste', 'StatRegisterClass', 0x007FFFA0),
    '0x6cba00': ('CTCCreatureNavigation', 'StatRegisterClass', 0x006CBA00),
    '0x75c0e0': ('CTCCameraPointDefinitionBase', 'StatRegisterClass', 0x0075C0E0),
    '0x8162f0': ('CHitLocation', 'StatRegisterClass', 0x008162F0),
    '0x802370': ('CTCTargetingPlayer', 'StatRegisterClass', 0x00802370),
    '0x73b130': ('CTCCreatureModeManager', 'RegisterModes', 0x0073B130),
}
NS = {'CHitLocation': 'NHitLocations'}
for c in ['CTCGraphicAppearance', 'CTCGraphicAppearanceAnimatedMesh', 'CTCGraphicAppearanceStaticMesh',
          'CTCGraphicAppearanceStaticMeshFadeOutOnProximity', 'CTCGraphicAppearanceSprite', 'CTCGraphicAppearanceReplaceableMesh',
          'CTCGraphicAppearanceMultipleStaticMeshes']:
    NS[c] = 'NGraphicAppearance'
for r in rows:
    if r[1] == 'TC' and r[2].startswith('CTCShotDefinition'): NS[r[2]] = 'NThingShot'
keep = []; k = 0
for r in rows:
    if r[1] in ('TC', 'DEF'):
        if k >= N: continue
        k += 1
    keep.append(r)
tcs = [r for r in keep if r[1] == 'TC']; defs = [r for r in keep if r[1] == 'DEF']
calls = [STATICS[r[2]] for r in keep if r[1] == 'CALL']
def qual(c): return f'{NS[c]}::{c}' if c in NS else c
tcnames = [r[2] for r in tcs]
statics_of = {}
for cls, fn, va in calls: statics_of.setdefault(cls, []).append((fn, va))

def declarations(w, testing):
    """the classes RegisterTCs names, in registration order (TC classes, then the two setup-only classes, then the defs)"""
    order = list(tcnames) + [c for c in statics_of if c not in tcnames and (testing or c != 'CHitLocation')]
    open_ns = None
    for c in order:
        ns = NS.get(c)
        if ns != open_ns:
            if open_ns: w('}')
            if ns: w(f'namespace {ns} {{')
            open_ns = ns
        r = next((x for x in tcs if x[2] == c), None)
        members = []
        if r:
            members.append(f'TC_REGISTRATION_INTERFACE({c})')
            if r[4] != 'call':
                members.append(f'static unsigned long __fastcall StatGetDriverType() {{ return {int(r[4], 16)}; }}')
        for fn, va in statics_of.get(c, []):
            if not testing: members.append(f'static void __fastcall {fn}();')
            elif fn == 'RegisterModes':
                members.append(f'static void __fastcall {fn}() {{ NoteSetupCall("{c}::{fn}"); ModesSawCount = RegisteredTCs.Count; ModesSawDirty = RegisteredTCs.Dirty; }}')
            else: members.append(f'static void __fastcall {fn}() {{ NoteSetupCall("{c}::{fn}"); }}')
        base = ' : public CTCBase' if (r or c.startswith('CTC')) else ''
        notes = []
        if r:
            notes.append(f'Alloc 0x{int(r[3], 16):08X}')
            if r[5] != 'inl': notes.append(f'name 0x{int(r[5], 16):08X}')
        for fn, va in statics_of.get(c, []): notes.append(f'{fn} 0x{va:08X}')
        line = f'class {c}{base} {{ public: {" ".join(members)} }};'
        w(f'{line}   // {", ".join(notes)}' if not testing else line)
    if open_ns: w('}')
    w('')
    for r in defs:
        line = f'class {r[2]} {{ public: DEF_CLASS_REGISTRATION_INTERFACE({r[2]}) }};'
        w(f'{line}   // Alloc 0x{int(r[3], 16):08X}' if not testing else line)

def body(w):
    w('void __fastcall CTCBase::RegisterTCs()')
    w('{')
    w('    RegisteredTCs.clear();')
    w('')
    w('    // the component classes\' own static setup')
    for r in keep:
        if r[1] == 'CALL' and STATICS[r[2]][1] != 'RegisterModes':
            cls, fn, va = STATICS[r[2]]
            w(f'    {qual(cls)}::{fn}();')
    w('')
    w('    // every component class, each followed by the definition classes that belong with it')
    for r in keep:
        if r[1] == 'TC': w(f'    REGISTER_TC({qual(r[2])});')
        elif r[1] == 'DEF': w(f'    REGISTER_DEF_CLASS({r[2]});')
    w('')
    w('    CTCCreatureModeManager::RegisterModes();')
    w('    RegisteredTCs.Update();')
    w('}')

MACROS = r'''// One component registration: the class-type name keys the registry entry and is stored in it too.
#define REGISTER_TC(TCClass) \
    { \
        CCharString name = TCClass::StatGetClassTypeName(); \
        RegisteredTCs.Insert(name, CRegisteredTCInfo(TCClass::Alloc, TCClass::StatGetDriverType(), name)); \
    }

// One definition-class registration with the game's definition manager, under the class's own name.
#define REGISTER_DEF_CLASS(DefClass) \
    CGameDefinitionManager::Get().AddDefClass(CDefClassInit(#DefClass, DefClass::Alloc))'''

IFACE = r'''// The registration interface of a thing-component class (declared in each class in the game's headers): its allocator and its
// class-type name. VC7.1 expands StatGetClassTypeName at the first 41 registrations and calls its out-of-line copy after that.
#define TC_REGISTRATION_INTERFACE(TCClass) \
    static CTCBase* __fastcall Alloc(CThing& owner); \
    static CCharString __fastcall StatGetClassTypeName() { return #TCClass; }
// The registration interface of a definition class: its allocator.
#define DEF_CLASS_REGISTRATION_INTERFACE(DefClass) \
    static CDefClassBase* __fastcall Alloc();'''

if not test:
    L = []; w = L.append
    w(open(f'{A}/tools/src_header.txt').read().rstrip('\n'))
    w('#pragma optimize("s", on)')
    for d in ['FABLE_RETAIL_CTCBASE', 'FABLE_RETAIL_CTCBASE_REGISTRY', 'FABLE_RETAIL_CCHARSTRING', 'FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE',
              'FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE', 'FABLE_RETAIL_CVECTORMAP_SORT', 'FABLE_RETAIL_STLP_SORT_FASTCALL',
              'FABLE_RETAIL_DEF_CLASS_BASE', 'FABLE_RETAIL_CRESOURCELIST', 'FABLE_RETAIL_CDEFINITIONMANAGER',
              'FABLE_RETAIL_CDEFINITIONMANAGER_ADDDEFCLASS', 'FABLE_RETAIL_CGAMEDEFINITIONMANAGER', 'FABLE_RETAIL_CHITLOCATION_STATREGISTERCLASS']:
        w(f'#define {d}')
    w('#include "engine/CTCBase.h"')
    w('#include "engine/CGameDefinitionManager.h"')
    w('#include "engine/CDefClassInit.h"')
    w('#include "engine/NHitLocations_CHitLocation.h"')
    w('')
    w('class CThing;')
    w('class CDefClassBase;')
    w('')
    w(IFACE)
    w('')
    w('// The classes RegisterTCs names, in registration order: thing-component classes (drivers return their own driver type, the rest')
    w('// inherit CTCBase::StatGetDriverType\'s 0), CTCCameraPointDefinitionBase (static setup only; NHitLocations::CHitLocation comes from')
    w('// its engine header), then the definition classes. Only the registration interface is declared, no layout and no intermediate')
    w('// bases: in retail (RTTI) the components derive from CTCBase directly (208) or through CTCActionUseBase, CTCDriverBase and the')
    w('// like, and the definition classes from CSubDefClassBase (100), CParentDefClassBase (10) or CTavernGameDef (1).')
    declarations(w, False)
    w('')
    w(MACROS)
    w('')
    body(w)
    open(out, 'w').write('\n'.join(L) + '\n')
    print(out, len(L), 'lines', len(tcs), 'TC', len(defs), 'DEF')
else:
    L = []; w = L.append
    w(open(f'{A}/tools/test_head.txt').read().rstrip('\n'))
    declarations(w, True)
    w('')
    w(MACROS)
    w('')
    body(w)
    w('')
    w('// The retail table (Fable.exe 0x004EE23F, in source order): component classes with their driver types, definition classes,')
    w('// setup calls.')
    w('struct SExpectedTC { const char* Name; unsigned long DriverType; };')
    w('static const SExpectedTC ExpectedTCs[] = {')
    for r in tcs: w(f'    {{ "{r[2]}", {0 if r[4] == "call" else int(r[4], 16)} }},')
    w('};')
    w('struct SExpectedDef { const char* Name; int TCsBefore; };')
    w('static const SExpectedDef ExpectedDefs[] = {')
    ntc = 0
    for r in keep:
        if r[1] == 'TC': ntc += 1
        elif r[1] == 'DEF': w(f'    {{ "{r[2]}", {ntc} }},')
    w('};')
    w('static const char* const ExpectedSetupCalls[] = {')
    for cls, fn, va in calls:
        if fn != 'RegisterModes': w(f'    "{cls}::{fn}",')
    w('    "CTCCreatureModeManager::RegisterModes",')
    w('};')
    w(open(f'{A}/tools/test_tail.txt').read().rstrip('\n'))
    open(test, 'w').write('\n'.join(L) + '\n')
    print(test, len(L), 'lines')
