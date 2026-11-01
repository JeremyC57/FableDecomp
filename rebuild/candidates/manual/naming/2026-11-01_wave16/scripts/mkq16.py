# mkq16.py: QUEUE_W16.tsv (every QUEUE_W16_input.tsv row + outcome) and DEFERRED.tsv, from runs/qscan0.tsv (baseline index) /
# runs/qscan1.tsv (index after ov_final), runs/final.rows.txt and the per-target notes below.
# outcome: respelled (edited rows, no row left naming a wrong bound spelling, own row defines the final) / respelled-partial
# (edited, some rows left) / deferred (nothing edited, rows left) / already-correct (nothing to respell) / hold.
import os, re, collections
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); RUNS = os.path.join(W, 'runs')
f = lambda p: [l.rstrip('\n').split('\t') for l in open(p)][1:]
A = f(os.path.join(RUNS, 'qscan0.tsv')); B = f(os.path.join(RUNS, 'qscan1.tsv'))
Q = [l.rstrip('\n').split('\t') + [''] * 5 for l in open(os.path.join(W, 'QUEUE_W16_input.tsv'))]
FR = set(l.strip() for l in open(os.path.join(RUNS, 'final.rows.txt')) if l.strip())
FAM = {}
for fam in ['gdt', 'qctor', 'stl', 'own', 'data', 'sp', 'queue', 'leaf', 'w15d']:
    p = os.path.join(RUNS, fam + '.rows.txt')
    if os.path.exists(p):
        for l in open(p):
            if l.strip(): FAM.setdefault(l.strip(), []).append(fam)
def wrows(s):
    r = set()
    for part in s.split(' | '):
        if ': ' in part: r |= set(x for x in part.split(': ', 1)[1].split() if re.match(r'^[0-9a-f]{8}$', x))
    return r
N = {
 '00c28690': ('deferred', 'libpk CHandleBasedMemoryPool::Free (second copy): Ego_r / FableWin carry one CHandleBasedMemoryPool class only (258 names searched, no second class or namespace), and ?Free@CHandleBasedMemoryPool@@QAEXJ@Z is bound to the engine copy 0x00A02B30 -- one mangled symbol cannot link twice: owner decision (carried from wave 15)'),
 '0143bac4': ('deferred', 'descriptive per c670 (PFrameMemoryPool stays descriptive); 0x00BEB540 still names its row-local ?g_beb540@@3PAUG@@A -- the respell needs that row\'s struct G model re-typed to CSimpleMemoryPool*: per-row, not reached'),
 '013b89fc': ('deferred', 'CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (a CCountedPointer<CQuestManager>): ~60 rows read it through 210 row-local global spellings (CGSI*_Manager pointers, typed CQuestManager* / CGameScriptInterface*); the faithful respell needs a shared singleton + CCountedPointer model per row (the pointer is read through the counted pointer, not a raw global) -- not reached'),
 '00404720': ('respelled-partial', 'fam_sp: the 33 callers respelled to ??0CStringParser@@QAE@ABVCCharString@@P6IX0@Z0J@Z (__fastcall callback taking const CCharString&); own row 0x00404720 deferred: its HEAD behaviour test already FAILs (pre-existing, recheck RELOCATION_MATCH FAIL both at HEAD and with the respell); 0x009AE150 deferred: its TErrorCallback typedef is shared with the CPersistContext ctor (conversion error C2664 when respelled)'),
 '006edc50': (None, '13 rows hit parameter-type compile errors (CThingCreatureBase& / pointer models differ per row) -- per-row'),
 '008edff0': (None, 'own row not landed; the 3 callers (0x007A09B0 0x007C6760 0x007C87F0) model the ctor as (long, CThing*): respelling to (EHeroAbility, CThing&) needs the enum and a reference at each call site (compile errors in fam_q) -- per-row'),
 '0099a300': (None, '~CBaseClassNonCopyable family: rows reach it through free / destructor-view stand-ins (fam_dtor territory, wave 15 bcn remainder) -- re-authoring'),
 '0099a2e0': (None, '~CBaseClass family: rows reach it through ??1CBase / destructor-view stand-ins (fam_dtor territory) -- re-authoring'),
 '00f35b40': (None, '~CEntityScriptBase family: rows use AutoTinyCompositeDestructor / ~CFrontEndScreen stand-ins (wave 15 scb remainder) -- re-authoring as the compiler-emitted NScript destructors'),
 '00aa75c0': (None, '~CChunkedFileChunk: callers use BaseOnDie / ??1 QAE stand-ins (UAE in Ego_r: needs the polymorphic chunk model) -- re-authoring'),
 '0099a410': (None, '~CInitBaseClass: own row and callers are immediate-vtable / Release_ stand-in models; the faithful QAE destructor needs re-authoring of the own row'),
 '009fc550': (None, 'own row and callers model the destructor as Destroy / BaseDestructor views (immediate-vtable models) -- re-authoring'),
 '00ac1ee0': (None, 'own row respelled (CIndexBuffer::Uninitialise, flat model + new test); the 4 callers use CIndexBuffer_Retail / CIndexBufferNative / Release stand-in classes (fam_q compile errors) -- per-row'),
 '009d5f80': (None, 'own row already correct; the 2 callers (0x00B3ECC0 CBankFile_Retail, 0x0050B420 CBase_009d5f80) model the base ctor through stand-in classes -- per-row'),
 '00a5c600': (None, 'own row respelled (private ClearMemoryBlocks); 0x0122AB10/30/50 (AutoTinyFieldMethodsTarget::First atexit stubs) and 0x00987F10 (@sub_A5C600@4) need class models for a private member -- per-row'),
 '0099a330': (None, 'own row and 12 callers respelled (IsValid const); 0x004C5EA0 / 0x0083C690 / 0x0083C800 / 0x0083CA90 call it as CPlayer::IsInitialised on a CPlayer view (the real callee is the CInitBaseClass member: a class-model change) -- per-row'),
 '00692910': (None, 'own row respelled (protected const PeekDefinitionManager); the remaining callers are not derived from CCreatureActionBase (protected access path: the call is inlined through a derived class) -- per-row'),
 '00692c10': (None, 'protected member: the callers are not derived from CCreatureActionBase (fam_q access errors) -- per-row'),
 '004bfac0': (None, 'CTCGraphicAppearance lives in namespace NGraphicAppearance in Ego_r; the callers model it in the global namespace (namespace move per row) -- per-row'),
 '00510d40': (None, 'own row re-authored on STLport _List_base<CIntelligentPointer<CThingAICreature>>::clear; 4 callers (CAIFollowerList / CCreatureList / CWitnessList clear stand-ins, @base_510d40@4) hit compile errors in fam_q -- per-row'),
 '008f7860': (None, 'the only wrong caller 0x00865C20 is a multi-target row (several stand-in spellings; fam_q skip) -- per-row'),
 '008f7950': (None, 'own row is a heavily invented model (free CreateDustEffectForCreature helper); caller 0x00865C20 multi-target -- re-authoring'),
 '00b866a0': (None, 'own ??_G row done; caller 0x00B86510 binds the address through a virtual Override_00b866a0 stand-in slot -- per-row'),
 '00a555c0': (None, 'own ??_G row done; caller 0x00A55400 binds the address through a virtual Override_00a555c0 stand-in slot -- per-row'),
 '00f37030': (None, 'own row already correct; 0x00B5ECE0 respelled; 0x00B6D600 declares the parameter as void* (PAX) instead of long -- per-row'),
 '00bd9ce0': (None, '0x00BDA070 respelled; 0x00B684F0 calls it through a CEngineLandscapeMemoryManagerCallView view -- per-row'),
 '009918f0': (None, 'descriptive per c670 (ClearBanks_009918f0 stays descriptive); 0x0042EBB6 respelled; 0x004175E5 still calls it as the qualified CSoundManager::Shutdown (fam_q skip: qualified call)'),
 '0121a460': ('already-correct', 'the _$E1 initializer already defines the static member (landed under the proven spelling)'),
 '0121a4f0': ('already-correct', 'the _$E1 initializer already defines ?ConversationAttitudeToAnimName@CConversationAttitude@@2V...@A'),
 '0121a580': ('already-correct', 'the _$E1 initializer already defines ?ConversationAttitudeToAnimGroupName@CConversationAttitude@@2V...@A'),
 '0121a7ca': ('already-correct', 'the CrimeText TU initializer already names ?CrimeText@?A0x...@@3PAVCCharString@@A (anonymous namespace; the tools match ignoring the TU hash)'),
 '013b8b10': ('respelled', '0x01228ED8 (the CrimeText atexit array dtor) re-authored as the CrimeText TU (??_I vector dtor iterator on the anonymous-namespace CrimeText); the data symbol stays bound under the lane hash ?A0xbfacb866 (hash-normalised match, owner policy 2026-11-01): no binding change'),
 '00c057d0': ('already-correct', '_tr_flush_block is the correct zlib cdecl decoration of ?_tr_flush_block (C linkage); the landed row already spells it'),
 '00a04160': ('already-correct', 'GetChecksum: the landed source already defines the proven spelling'),
 '013b8a38': ('respelled', '0x004CD670 now names the static member ConversationAttitudeToAnimName; 0x004CD9B0 re-authored as ?GetAnimName@CConversationAttitude@@SIABVCCharString@@W4EConversationAttitude@@@Z (the catalog label GetAnimGroupName is wrong: it reads 0x013B8A38, the twin 0x004CD9C0 reads 0x013B8A44)'),
 '013b8a44': ('respelled', '0x004CD670 now names the static member ConversationAttitudeToAnimGroupName'),
 '0143e910': ('respelled', '0x00CBFAB8 SetScriptActiveStatus and 0x01224C6D (the map initializer) now name ?ScriptActiveStatuses@@3V?$map@VCCharString@@W4EScriptActiveStatus@@...@A'),
 '009aa780': ('respelled', 'own row: the STLport _Rb_tree text kept, its CUnprovenKey/Value/KeyOfValue placeholders replaced by map<CCharString, NBankFileManager::CContainedBank> (the caller 0x009AC7C0 already names the final). The placeholder spelling stays bound (KEEP) for the 4 sibling rows of the same map (0x009AACA0 clear, 0x009AB6B0 ctor, 0x009AC0F0 / 0x009AC130 CUnprovenContainer_009aa780): outside the queue, a follow-up'),
}
out = [['chunk', 'address', 'correct_symbol', 'evidence', 'wrong_spellings_seen', 'outcome', 'rows_edited', 'rows_left', 'detail']]
dfr = [['row', 'target', 'final_symbol', 'still_emitted', 'reason']]
cnt = collections.Counter()
for q, a, b in zip(Q, A, B):
    t = a[1]; own = a[5]
    w0, w1 = wrows(a[9]), wrows(b[9])
    n0 = set(a[7].split()) - {'...'}; n1 = set(b[7].split()) - {'...'}
    ed = sorted(x for x in ({own} | w0 | (n1 - n0)) if x and x in FR)
    left = set(w1)
    if own and b[6] != 'Y' and not b[6].startswith('err'): left.add(own)
    if t in ('0121a460', '0121a4f0', '0121a580', '0121a7ca', '01228ed8', '00c057d0'): left.discard(own)
    o = ('respelled' if not left else 'respelled-partial') if ed else ('already-correct' if not left else 'deferred')
    note = ''
    if t == '00440730': o, note = 'hold', 'owner HOLD (brief)'
    if t in N:
        oo, note = N[t]
        if oo: o = oo
        if o == 'respelled-partial' and t == '00404720': left |= {'00404720', '009ae150'}
    if not note:
        if o == 'respelled': note = 'respelled and rechecked: ' + ', '.join('%s (%s)' % (x, '+'.join(FAM.get(x, ['?']))) for x in ed[:12])
        elif o == 'already-correct': note = 'no landed row names a wrong spelling of the target' + ('' if own else ' (own row not landed)')
    cnt[o] += 1
    out.append(q[:5] + [o, ' '.join(ed), ' '.join(sorted(left)), note])
    if o in ('deferred', 'respelled-partial'):
        dfr.append([' '.join(sorted(left)) or '-', t, a[2], 'row-local spellings (queue wrong_spellings_seen)', '[%s] %s' % (o, note)])
open(os.path.join(W, 'QUEUE_W16.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
EXTRA = os.path.join(RUNS, 'deferred_extra.tsv')
QT = set(r[1] for r in dfr[1:]) | set(a[1] for a in A if a[1] in N or True)
if os.path.exists(EXTRA): dfr += [r for r in (l.rstrip('\n').split('\t') for l in open(EXTRA) if l.strip()) if r[1] not in QT or r[0].startswith('009aaca0')]
open(os.path.join(W, 'DEFERRED.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in dfr))
print(dict(cnt), 'deferred rows', len(dfr) - 1)
