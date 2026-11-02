# mkq18_notes.py (exec'd by mkq17.py): per-target outcome overrides / notes, and skipsum() -- the fam_q skip reasons of a target.
import glob as _g
ABASE = dict((r[1], r) for r in A)
def skipsum(t):
    ls = sorted(_g.glob(os.path.join(W, 'logs', 'q_*_%s.log' % t)) + _g.glob(os.path.join(W, 'logs', 'compose_ov_final_*_q%s.log' % t)), key=os.path.getmtime)
    if not ls:
        a = ABASE.get(t)
        if a and a[5] and a[6].startswith('N'):
            return 'not driven this wave: the own row defines a stand-in (%s) -- own-row re-author needed before the callers' % a[6][2:].split()[0][:90]
        return 'not driven this wave: the remaining rows name per-row stand-in spellings (vtable / data / free-function views) -- per-row'
    c = collections.Counter()
    for l in open(ls[-1]):
        p = l.split()
        if len(p) >= 4 and p[0] == 'skip':
            k = ' '.join(p[2:4]).rstrip(':')
            k = {'free stand-in': 'free-function stand-in (no class to move)', 'no calls': 'no call site found (address bound through a view / data)',
                 'call of': 'qualified / indirect call', 'non-public': 'non-public member reached from an unrelated class'}.get(' '.join(p[2:4]), k)
            if p[2] == 'moved' or p[2] == 'inplace': k = 'compile error after moving %s' % p[3]
            if p[2].isdigit(): k = 'stand-in %s (definition / signature mismatch)' % p[3]
            if p[3] == 'defined': k = 'row defines its own %s model' % p[2]
            c[k] += 1
    a = ABASE.get(t)
    own = ('; the own row defines a stand-in (%s) -- own-row re-author needed' % a[6][2:].split()[0][:90]) if a and a[5] and a[6].startswith('N') else ''
    if not c: return ('callers respelled' + own) if own else 'fam_q: every driven row edited; the rows left are outside its reach (re-authoring)'
    return 'fam_q skips: ' + '; '.join('%d %s' % (n, k) for k, n in c.most_common(3)) + ' -- per-row re-modelling' + own
EDX = {}
_CFPS = '0057cb7e 006624a0 006666a0 00697710 006cec90 006d8580 0075aec0 008c6db0 00908da0 0091a390 0094fe30 0094feb0'
N = {
 '01375550': (None, 'respelled through the inline static accessor ?GetConstantFPS@CMainGameComponent@@SIJXZ (the PDB way into the protected static ?ConstantFPS@CMainGameComponent@@1JA); 12 rows DEFERRED: retail reads 0x01375550 as an integer-FPU memory operand (fidiv / fild) from non-member functions (%s) -- VC7.1 copies the inlined by-value getter to a temp (DIFFER, e.g. 64v51 at 0x00697710), so retail read the static directly there: friend / public evidence is missing (coordinator decision: a `2JA` public form would hold every row)' % _CFPS),
 '013b86a0': (None, 'respelled through the inline static ?Get@CMainGameComponent@@SIPAV1@XZ (protected static PMainGameComponent, established 1PAV1@A); the rows left name per-row stand-in classes (CEngineCalls / CEngine_Retail typed views) or have no extern declaration -- per-row'),
 '013b89fc': (None, 'respelled through ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (PClass is the private 0V static; 0x00892F10 test defines it through the template model); rows left: QuestManagerNative / CQuestMgr struct views whose member names clash with the real class -- per-row'),
 '0143e8f8': (None, 'respelled as ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (established); rows left: typed GameCalls / no extern / C2440 init of the row-local view -- per-row'),
 '013b8790': (None, 'respelled through ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (private 0V PClass; 16 CGameScriptInterface tests define it through the template model); rows left name per-row stand-in classes whose members clash with CPlayerGui (Draw2DText views) -- per-row'),
 '013b8a1c': (None, 'respelled through ?GetPCurrentThingManager@CThing@@SIPAVCThingManager@@XZ (private static CThing::PCurrentThingManager); rows left: stand-in structs with their own CThing model (nested X_006c8780) -- per-row'),
 '00410de0': (None, 'family bitset: the row-local stand-in class (CPresenceBits / CTCBitsRemaining / InterfaceBits ...) is std::bitset<274> (STLport 4.6 <bitset>; the empty stand-ins keep their empty layout, the 9-word ones the _M_w words) and its Test / test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z; 0x0047DB73 left (the stand-in member Do is used elsewhere)'),
 '0129ba3c': (None, 'family eps: `extern const float DELTA_NOTIONAL_ZERO;` (?DELTA_NOTIONAL_ZERO@@3MB) in every TU that spelled EPSILON, plus the 0x008754A0 test that defines it; 0x00751E80 left (two extern declarations)'),
 '013b8390': (None, 'family gpdm: ?GPDisplayManager@@3PAVCDisplayManager@@A (was PDisplayManager; same type) + the 0x004022B0 test'),
 '013ce558': (None, 'family isqrt: ?GFastOneOverSqrtTable@@3PAKA (was g_InvSqrtMantissaTable; the 3PAIA rows are unsigned int views -- per-row)'),
 '00c28690': ('deferred', 'libpk CHandleBasedMemoryPool::Free (second copy): one mangled symbol cannot link twice; owner decision (carried from waves 15-17)'),
 '00c73660': ('deferred', 'descriptive GameCODA DetectWindowsVersion (no PDB): the caller 0x00C73770 names SetDefaultString_helper with the wrong signature -- the caller needs a re-authoring of its call (signature change), not a name-only respell'),
 '009c4f20': ('deferred', 'CFileInstaller::ExitThread volatile: a member-type change of the own row (not name-only; the chunk 680 evidence says Retail-volatile) -- re-authoring of 0x009C4F20'),
 '00c25860': ('deferred', '?Cross@C3DVector@@QBE?AV1@ABV1@@Z is marked probable, not proven: the two-source rule holds the respell'),
 '0063a300': ('deferred', 'GetThemeSizeZAt is one of 0x0063A300 / 0x0063A390 (the two carry the same label): prove which before respelling'),
 '0143bb58': ('deferred', 'the CODA context global has three spellings in the tree and no settled name (?CODA@@3VCCodaContext@@A is the ChatGPT pass 10 proposal, GameCODA SDK name unverified): coordinator / owner decision'),
 '0143bb48': ('deferred', 'CODA_Error: the two rows model it as a long / void* (g_lastError / g_CurrentInstance); the ?CODA_Error@@3HA int form is the pass 10 proposal, unverified against the GameCODA SDK'),
 '01238c8c': ('deferred', 'the CScriptThing vtable ??_7CScriptThing@@6B@: 30+ rows store it through row-local CEvent_<addr> models of the inline ~CScriptThing -- a per-row CScriptThing class model (re-authoring, the virtual slot order)'),
 '00caf680': ('deferred', 'DirectShow CDeferredCommand ctor (strmbase): the placeholder class is the SDK class; the vendor source model is a re-authoring (not name-only)'),
 '00440730': ('hold', 'owner HOLD (brief)'),
# carried wave-17 notes (the targets' state did not change in wave 18)
 '0040f020': (None, 'wave 17: ' + 'CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound: own row already correct; the callers use free / table stand-ins (EntVec_LowerBound, MazeTable::Find, JackTable::FindEntry) of different template instantiations -- per-row'),
 '005baa41': ('deferred', 'template ??_G?$CDefPointer@$$CBVCAppearanceModifierDef@@@@QAEPAXI@Z: the own row models the class as a CUnproven placeholder; the ??_G emitter (empty-ctor pattern) does not cover template classes -- re-authoring'),
 '00c052c0': ('already-correct', 'zlib C-linkage static: the landed row already emits _build_tree (catalog name build_tree is the undecorated spelling)'),
 '00ca20a0': ('already-correct', 'libvorbis C static accumulate_fit: the landed row already defines it'),
 '00ca3f60': ('already-correct', 'libvorbis _Newton_Raphson@12: the landed row already defines it'),
 '00ad3a10': (None, 'wave 17: ' + 'own row already correct; 00513700 / 00ae7880 name the set<long> copy ctor (an ICF-shared instantiation: KEEP); 00ad77f0 (set<unsigned long> U-class model), 00ad62b0 (CIndexList@N00ad62b0), 00ad7e80 (RetailSet@N00ad7e80), 00ad6ae0 need per-row STLport set re-modelling'),
 '00514c10': (None, 'wave 17: ' + 'own row already correct (iterator traits fixed in 0x004FE7F0); 00a385a0 calls it through a set::insert stand-in with a U-class allocator and 00ad3940 through ?helper2@CTCLook -- per-row STLport set re-modelling'),
 '00947780': ('deferred', 'GetAllLeafThings<CThingBuilding, vector<CThing*>> reads PeekTypeList whose const std::list<CClass*>& return needs the STLport list model in the row (the row models a CThingList struct) -- re-authoring'),
 '008fd000': ('deferred', '31 rows read PeekThingSearchTools through stand-in classes / a non-const pointer return; the PDB spelling (IBE, const CThingSearchTools& return) needs the const CThingSearchTools API model per row -- re-authoring'),
 '008fcfd0': (None, 'wave 17: ' + 'fam_q17 (protected member reached from classes derived from CAIStateGroupBase at offset 0, RTTI): rows whose class derives in the row text respelled; 13 rows use stand-in class names that are not the row\'s RTTI class (...Methods / BaseR / Seed / Adapter), plus 0x0094EF70 (free GetObj) and 0x0095B020 (reference spelling) -- per-row'),
 '00686d60': ('deferred', 'protected PeekComponent: 17 rows read it through per-class getters (GetDefinitionManager / GetFrameSource / GetGameSubsystem1C ...) from classes that do not derive from CTCBase in the row text, or with a different return model -- per-row'),
}
