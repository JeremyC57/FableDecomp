# 2026-11-10 landing aw -- agentNF3: 68 queued name fixes verified, 155 landed rows respelled

Source: `naming/2026-11-10_agentNF3/` (QUEUE.tsv = queue tags land1011ao / ap / ar / as; STATUS / NOTES / LESSONS / RENAMES).
All 68 queue entries held against two sources (58 applied in full, 10 partly). 156 rows re-authored; vland --recheck-landed: 8 MATCH
+ 148 RELOCATION_MATCH. 0x008916A0 (CGameScriptInterface::SheatheHeroWeapons, an invented-stand-in row) passed its grade but its old
test stubs the callee under the old name (LINK_FAIL): reverted to HEAD, debt kept. 26 rows renamed with rerow.py (CSoundEmitterLH /
CASoundManager bank lookups, the vector<T>::resize COMDATs 0x004ED04D / 0x004EDA32 / 0x004EDF0A, destructors 0x004F9CFC /
0x00478722 / 0x00430300, scalar deleting destructors 0x00477460 / 0x00478706, 0x00868860 = CHitParameters::IsMeleeStrike() const, ...).
Headers (3-way merged onto HEAD, base 20a7e548f; hdrcheck2 default=same): CDefinitionManager GETDEFCLASSINDEX, CASoundManager
REGISTEREDBANKS / GETPSOUNDBANK_LONG, CBulletTimeManager SLOWDOWNMULTIPLIER, CCardPositionsDef DTOR_NOVTABLE.
reloc_bindings.tsv fixed by hand (logs/bind_*.txt): 97 stale spellings removed, 70 added. Six one-symbol-two-targets pairs surfaced
by the recheck are pre-existing (rows whose spelling did not change here; the table had hidden them under the first caller):
??_7CIntelligentPointer@@6B@ / ??_7CIntelligentPointerCreature@@6B@ used as the vtable of several CIntelligentPointer<T> instances
(local models; agentNF4 is rewriting those rows), ??1CCardPositionsDef@@UAE@XZ named at both 0x00C10940 and 0x004F9CFC (the manifest
names both ~CCardPositionsDef: one address one name -- settle), ?g_AnimComponentPrototypes@@3UCProtoMap@@A at 0x013B8C28 and
0x013B8C2C inside 0x005DD090 (a member accessed through the wrong base). Queued land1011av.
Left (NOTES.md): ~290 call sites in stand-in rows -- CBaseIntelligentPointer ctor/dtor (~123, agentNF4 now), GetCurrentMode (57),
CParentDefClassBase ctor/dtor (38, agentNF4 now), AddTC (21), WriteSLONG (32), PutThingInCarrySlot (8), ReadWithSrcChunkOverflow (3), Create (3).
