# mkout15.py: QUEUE_W17.tsv (every input row + outcome), DEFERRED.tsv, LABELS.tsv, RENAMES.tsv for naming wave 15, from
# runs/qout1.tsv (qout.py before/after), the family exclusion files, the leaf skip log and the final index / pairs.
import os, sys, re, json, collections, glob
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); RUNS = os.path.join(W, 'runs')
sys.path.insert(0, os.path.join(W, 'scripts'))
import und
FAMT = {'0099a300': 'bcn', '0099a430': 'bo', '00686830': 'tcb', '00a01c10': 'bip', '00693ef0': 'cab', '0062e900': 'cmb', '00cbd510': 'scb', '00833a10': 'sai'}
FAMWHY = {
 'bcn': 'remaining rows spell ~CBaseClassNonCopyable through free-function / hand-written destructor-sequence stand-ins (AutoTiny*, CBase_dtor views, FableDest*), stand-in classes that are real PDB classes needing a shared base model, or the two rows whose respell moves bytes (excl_bcn.txt)',
 'bo': 'every row models the base through a hand-written "CBaseObjectDtor"-style destructor view or a flattened engine header with an explicit __vftable member (fam_dtor 0 safe edits): the faithful form is a polymorphic CBaseObject base whose destructor the compiler emits -- re-authoring',
 'tcb': '13 rows are pad-shrink models whose respell moves bytes (the derived destructor gains the vptr store `mov dword ptr [esi],offset ??_7X@@6B@`, excl_tcb.txt); the other 8 use stand-in shapes (free helpers, row-namespace CTCBase copies) -- re-authoring',
 'bip': 'remaining rows spell ~CBaseIntelligentPointer through free / scalar-destructor / row-namespace stand-ins (NRemap*, FableHeap_*, Maze*PointerBase, CMember_00a01c10, ...) or member-destroy views; 5 rows move bytes (excl_bip.txt) -- per-row / re-authoring',
 'cab': 'remaining rows are real derived CCreatureAction_* classes modelled with a stand-in base view, free destructor helpers, or rows whose respell moves bytes (excl_cab.txt)',
 'cmb': 'remaining rows model CCombatActionBase as a flattened view with hand-written destructor sequences (4 rows move bytes, excl_cmb.txt: the derived destructor gains a vptr store)',
 'scb': '29 rows destroy the script base through the free AutoTinyCompositeDestructor_00cbd510 shape (no class model): re-authoring as the NScript::CQ_* destructor the compiler emits',
 'sai': 'remaining rows call 0x00833A10 through shapes the member rewrite cannot reach without moving bytes or per-row models',
}
STL = re.compile(r'@std@@|\?\$_Rb_tree|\?\$_List_base|\?\$vector@|\?\$list@')
MEGA = {
 '00410de0': 'mega family (239 rows): row-local std::bitset<274>::test spellings (bitset model per row); needs a shared STLport bitset model -- not reached',
 '00411570': 'mega family (138 rows): CBasicString<char>::Compare (protected static __fastcall) spelled per row; the own row is a special member (fam_leaf skip) -- not reached',
 '00485815': 'mega family (29 rows): CDefClassBase::GetSubDef<CCreatureDef> template spelled per row (CDefPointer models differ) -- not reached',
 '0041e5f2': 'mega family (29 rows): NUISystem::CManager::GetInstance (static __fastcall, namespaced) spelled per row -- not reached',
 '00431102': 'mega family (26 rows): CPersistContext::Transfer<long> template spelled per row -- not reached',
 '0043314a': 'mega family (12 rows): CPersistContext::Transfer<bool> template spelled per row -- not reached',
 '008113c0': 'mega family (23 rows): CTCEntityEvents::AddEvent(NEntityEvents::CEventBase*) with row-local event models -- not reached',
 '006c6990': '14 rows: CTCHeroOpinionDeedLog::PostDeed (enum / reference / pointer parameter models differ per row) -- not reached',
 '01375550': 'mega family (314 rows): the class-static data member CMainGameComponent::ConstantFPS is spelled through row-local globals / class models; a data-symbol respell needs a shared CMainGameComponent model (GATED with PMainGameComponent in wave 14) -- not attempted',
 '00686a70': 'carried from wave 14 (DEFERRED): every row is bespoke (free helpers, qualified calls from non-CTCBase classes, NRemap namespaces)',
 '00a01b50': 'CBaseIntelligentPointer::GetPItem mega family (988 -> 903 rows): the generic caller respell took the rows whose shape is a plain member / free one-argument call; the rest use row-local CIntelligentPointer / CThing_Retail models -- carried',
 '0099eae0': '~CCharString (193 -> 135 rows): remaining rows destroy through free / scalar-destructor stand-ins -- carried',
 '00a242c0': 'remaining rows model CMeshDataBank per row with non-const / long-parameter views (C3DAnimationInfo const return) -- per-row',
 '00c28690': 'CONFLICT, owner decision: ?Free@CHandleBasedMemoryPool@@QAEXJ@Z is already bound to the engine copy 0x00A02B30 (by 0x00A032F0); 0x00C28690 is the second (libpk) copy of the class. One symbol cannot name two retail addresses, so the libpk copy needs its own retail name (namespace / class) before its callers 0x00C26090 / 0x00C265B0 can be respelled; the trial respell (RELOCATION_MATCH) was withdrawn',
 '00440730': 'HOLD (brief): left untouched',
 '00861d00': 'own row: CModeStandBase::OnDeactivate is virtual (UAE) but the row models `this` with engine/CModeStandBase.h, a flattened struct with an explicit __vftable member; the virtual member needs a polymorphic engine header (engine header changes are out of this wave)',
 '00634870': 'own row: CPlayer::ResetModes -- renaming the CPlayer_Methods stand-in to CPlayer collides with the CPlayer engine-header class the row includes (C2011); needs the #define-Fields model -- per-row',
}
GW = 'landed under a stand-in name: the compiler emits ??_G only for a class with an inline virtual destructor that a `new X` / `delete` instantiates; re-authoring (inline ctor + a factory, as the landed precedents) and test edits -- not reached'
LEAFWHY = {}
for l in open(os.path.join(W, 'logs', 'compose_ov_final_leaf_in_leaf_jobs.tsv.log')):
    m = re.match(r'skip (\w{8}) (.*)', l.strip())
    if m: LEAFWHY[m.group(1)] = m.group(2)[:200]
Q = [l.rstrip('\n').split('\t') for l in open(os.path.join(RUNS, 'qout1.tsv'))][1:]
RAW = [l.rstrip('\n').split('\t') + [''] * 5 for l in open(os.path.join(W, 'QUEUE_W17_input.tsv'))]
FAMROWS = {}
for f in ('bcn', 'bip', 'cab', 'cmb', 'scb', 'sai', 'anong', 'dec', 'newline', 'w14def', 'queue', 'leaf'):
    for a in open(os.path.join(RUNS, f + '.rows.txt')):
        FAMROWS.setdefault(a.strip(), []).append(f)
def outcome(r):
    i, ch, t, fin, wb, wa, own, ofb, ofa, rem = r
    wb, wa = int(wb), int(wa)
    if t == '00440730': return 'hold'
    changed = wa < wb or (ofb == 'False' and ofa == 'True')
    full = wa == 0 and (own == 'n' or ofa == 'True')
    if changed: return 'respelled' if full else 'respelled-partial'
    if full: return 'already-correct'
    return 'deferred'
def why(r):
    i, ch, t, fin, wb, wa, own, ofb, ofa, rem = r
    if t in MEGA: return MEGA[t]
    if t in FAMT: return 'family %s: %s' % (FAMT[t], FAMWHY[FAMT[t]])
    parts = []
    if own == 'Y' and ofa != 'True':
        if fin.startswith('??_G'): parts.append('own row ' + GW)
        elif t in LEAFWHY: parts.append('own row not respelled (fam_leaf: %s)%s' % (LEAFWHY[t], '; STLport instantiation landed under a stand-in class / namespace, needs re-authoring as the real STLport member' if 'namespaced class std::' in LEAFWHY[t] else ''))
        elif STL.search(fin): parts.append('own row: STLport instantiation landed under a stand-in class / free function; re-authoring as the real STLport template member')
        else: parts.append('own row: landed under a stand-in shape (free function / Methods class / row-local namespace) the leaf respell cannot rename in place -- re-authoring')
    if int(wa):
        parts.append('%d row(s) still name a non-final spelling (%s%s): row-local stand-in models the generic caller respell (fam_q) did not rewrite (namespaced / free / per-row class views) -- per-row' % (int(wa), ' '.join(rem.split()[:6]), ' ...' if len(rem.split()) > 6 else ''))
    return '; '.join(parts) or 'no edit applied'
qo = open(os.path.join(W, 'QUEUE_W17.tsv'), 'w')
qo.write('chunk\taddress\tcorrect_symbol\tevidence\twrong_spellings_seen\toutcome\twrong_rows_before\twrong_rows_after\town_row_final_after\tnote\n')
cnt = collections.Counter(); D = []
for r, raw in zip(Q, RAW):
    o = outcome(r); cnt[o] += 1
    i, ch, t, fin, wb, wa, own, ofb, ofa, rem = r
    note = ''
    if o in ('respelled-partial', 'deferred', 'hold'): note = why(r)
    elif o == 'respelled':
        fams = sorted(set(f for a in rem.split() for f in FAMROWS.get(a, ())))
        note = 'naming wave 15: no row names a non-final spelling; own row %s' % ('emits the final' if ofa == 'True' else ('n/a' if own == 'n' else 'unchanged'))
    else: note = 'no row names a non-final spelling; own row %s' % ('emits the final' if ofa == 'True' else ('n/a' if own == 'n' else 'unchanged'))
    qo.write('\t'.join(raw[:5] + [o, wb, wa, {'True': 'Y', 'False': 'n'}[ofa] if own == 'Y' else '-', note]) + '\n')
    if o in ('respelled-partial', 'deferred', 'hold'):
        D.append((rem or (t if own == 'Y' else ''), t, fin, o, note))
qo.close()
# DEFERRED.tsv: one line per queue target left open (deduplicated) + the family rows whose respell moved bytes
dd = open(os.path.join(W, 'DEFERRED.tsv'), 'w'); dd.write('row\ttarget\tfinal_symbol\tstill_emitted\treason\n')
seen = set()
for rows_, t, fin, o, note in D:
    if (t, fin) in seen: continue
    seen.add((t, fin))
    dd.write('%s\t%s\t%s\t%s\t%s\n' % (' '.join(rows_.split()[:40]) + (' ...' if len(rows_.split()) > 40 else ''), t, fin, 'row-local spellings (queue wrong_spellings_seen)' if o != 'hold' else '-', ('[%s] ' % o) + note))
CAUSE = {
 'bcn': {'00446be0': 'same length, bytes differ after the respell (not analysed further)',
         '00995df0': '+21 bytes after the respell (the row-local derived class becomes polymorphic; not analysed further)'},
 'bip': 'same length, a member offset moves: e.g. 0x006DF7C0 retail `lea ecx,[esi+0B5h]` -> built `lea ecx,[esi+0B8h]` (the member at +0xB5 lands at +0xB8 in the respelled model; the other four rows are the same length, not analysed per row)',
 'cab': '+6 bytes: the derived destructor gains the vptr store `mov dword ptr [esi],offset ??_7X@@6B@` once ~CCreatureActionBase is virtual (retail stores none)',
 'cmb': '+9..+11 bytes after the respell (not analysed per row; the cab / tcb rows of the same shape gain the derived vptr store)',
 'tcb': '+6 (38v32 / 53v47) or +27 (62v35) bytes: pad-shrink model; the derived destructor gains `mov dword ptr [esi],offset ??_7X@@6B@` (e.g. 0x004DB004 C3DAnimationBlendState) once ~CTCBase is virtual',
}
FIN = {'bcn': '??1CBaseClassNonCopyable@@MAE@XZ', 'bip': '??1CBaseIntelligentPointer@@UAE@XZ', 'cab': '??1CCreatureActionBase@@UAE@XZ', 'cmb': '??1CCombatActionBase@@UAE@XZ', 'tcb': '??1CTCBase@@UAE@XZ'}
TGT = {'bcn': '0099a300', 'bip': '00a01c10', 'cab': '00693ef0', 'cmb': '0062e900', 'tcb': '00686830'}
for f in ('bcn', 'bip', 'cab', 'cmb', 'tcb'):
    for l in open(os.path.join(RUNS, 'excl_%s.txt' % f)):
        q = l.split()
        if not q: continue
        c = CAUSE[f].get(q[0], '') if isinstance(CAUSE[f], dict) else CAUSE[f]
        dd.write('%s\t%s\t%s\t%s\t[bytes-move, family %s] recheck %s %s: %s\n' % (q[0], TGT[f], FIN[f], 'the row keeps its current spelling', f, q[1], q[2] if len(q) > 2 else '', c))
dd.close()
# RENAMES.tsv: no file renames this wave (sources keep their paths; build_candidates.ps1 unchanged)
open(os.path.join(W, 'RENAMES.tsv'), 'w').write('address\told_name\tnew_name\told_module\tnew_module\told_source\tnew_source\told_test\tnew_test\tevidence\n')
print(cnt)
