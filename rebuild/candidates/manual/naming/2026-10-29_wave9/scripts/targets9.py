# wave 9 settlements and what was applied (DECISIONS_W9.tsv merges these with the queue / wave-8 open rows)
_B = 'Ego_r.pdb + FableWin.pdb (exact, both lists); '
FINAL = {
    '0099efb0': '??4CCharString@@QAEABV0@ABV0@@Z',
    '012c32f0': '??_7CIsThingAlive@NScript@@6B@',
}
CONF = {
    '012c32f0': 'vftable (not in the PDB name lists): chunk 571 RTTI / slot-0 evidence (tail-jump to CScriptThing::IsAlive) + Ego_r.pdb class '
                'NScript::CIsThingAlive (class key V, virtual dtor ??1CIsThingAlive@NScript@@UAE@XZ = 0x00E241B0, proven); after cita9 the '
                'final recheck pairs ??_7CIsThingAlive@NScript@@6B@ with 0x012C32F0 in the edited rows',
    '0099efb0': _B + 'Ego_r 0x00AFA810 / FableWin 0x02F61830 return const CCharString& (opeq pass 2026-10-28; chunks 529/531/538/571/574). '
                'NAME_FIXES_QUEUE row 522 (chunk 522: ??4CCharString@@QAEAAV0@ABV0@@Z, non-const) is WRONG. Catalog-wide state before wave 9: '
                '2,045 files already declare the const& form, 619 rows emit it; 21 bound wrong spellings came from 21 rows (row-local '
                'namespaces CombatDebug / N009b77b0 / N00cb68c0 / N00cb93e0 / N00cbac70 / N00cbb4d0 / NParticleEngine / X_004fe630 / '
                'X_007565e0 / X_007adbf0 / X_00e54990 / std, and stand-ins MemberA / SymbolString / Assign / AttachToScript / CopyContents / '
                'Sub::M / SubObj::copyFrom / StrDest::ctor); all respelled (h9_opeq). A catalog sweep of every indexed object for ??4CCharString* ''found 3 more rows with unbound namespaced spellings (0052b9d0 N0052b9d0, 0052bff0 X_0052bff0, 00569550 N00569550), hoisted too ''(own recheck: 3 RELOCATION_MATCH PASS). One emitter is left: 00404720 declares the non-const ??4CCharString@@QAEAAV0@ABV0@@Z ''(unbound) and grades FAIL at baseline, so it is DEFERRED (one-line fix with its repair). No unindexed row source declares a non-const form',
    '00c28690': 'chunk 520 row (misfiled in the 566-574 queue): ?Free@CHandleBasedMemoryPool@@QAEXJ@Z is already bound to 0x00A02B30 (the '
                'engine copy); 0x00C28690 is the libpk copy of the same function: one mangled name cannot bind two retail addresses',
    '004045c0': 'Ego_r and FableWin carry BOTH ??$Transfer@_N@ (bool) and ??$Transfer@C@ (signed char); the 58 caller rows model the members '
                'as signed char (PDB members are bool): which instance 0x004045C0 is (or an ICF fold of both) needs the member types settled first',
}
FAM = {   # address: family / hand script that respelled rows for it in wave 9
    '0099efb0': 'h9_opeq', '0097db70': 'aia9 + h9_self', '0097e200': 'aia9 + acc9', '00e241b0': 'cita9', '012c32f0': 'cita9',
    '00410be0': 'xfl9', '00497890': 'gror9', '004c9840': 'rtc9 + h9_self', '0049d810': 'h9_ren + gws9', '004bfac0': 'nga9',
    '008fcf80': 'h9_ren', '00405f00': 'h9_ren', '0051ed80': 'h9_ren', '00438570': 'h9_ren',
}
