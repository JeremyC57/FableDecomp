# 2026-11-10 landing ba -- agentNF5: name-fix wave over the 125-address queue (Codex pass 18 fixes + earlier queue)

Source: `naming/2026-11-10_agentNF5/` (QUEUE / STATUS / NOTES / LESSONS / RENAMES / NAME_FIXES).
133 rows re-authored; vland --recheck-landed: 133 wins (126 RELOCATION_MATCH, 7 MATCH), tests PASS (g++ and VC7.1 /Od).
- Verdicts: 27 applied, 82 partly (verified; some binder rows still spell them wrong, per-entry counts in STATUS.tsv), 16 rejected.
- 100 call-site rows in seven groups (GetWorldSeed, RemoveMode, GetDeedFromHandle, AddMessage, AIAction_LookAtPos,
  AIAction_MoveToPos, GetString); 33 renames with rerow.py (IsSpaceInInventoryForItem, GetPrioritisedAbilityDefs on the shared
  headers, GetDef<CCombatTypeDef>, CPersistTraitsSLONG::TransferIn, CConversation ctor + vector<CConversation>::resize,
  ~CThingComponentSet, AddAction, ForceAIReset ...).
- Wide text: the five `_W` queue spellings were rejected for their `G` twins (owner decision 2026-10-29: `unsigned short`, no
  `/Zc:wchar_t`). 15 landed rows that still carry `/Zc:wchar_t` are queued (land1011ba).
- Rejected also: 0x00BFEA30 (__chkstk / __alloca_probe are one routine), 0x013B8C28 (real name
  CAnimationEntry::RegisteredComponentsByType), 0x00C10940 (not ~CCardPositionsDef; unidentified dtor, vtable 0x012B263C), seven held
  (VIRTUAL_SCREEN_* owner gate, CRegisteredBank x3 single-source, 0x008E8A10 anon-namespace hash, 0x00452B4D overload arity).
Headers (opt-ins, base 9a7c6bffb, hdrcheck2 default=same): CTCEntityEvents GETEVENT_DECL, CDefClassBase DEF_CLASS_BASE_CTOR_DECL,
CSubDefClassBase CTOR_INLINE, CTCScriptedControl ACTIONLIST, CAIBrain FORCERESET.
reloc_bindings.tsv: rows' bindings recomputed from the vland objects (rebind.py), 258 lines out, 248 in; bindings_multi_check at baseline.
Left: large binder groups still on stand-in models (CCharString copy/dtor/const char* ctor 73/61/43, CBaseIntelligentPointer
GetPItem 72, CVectorMap LowerBound 40+30, PMainGameComponent 31, PeekDefinitionManager 20, AddAction 20, AddTC 18, TransferIn 11);
seven rows still on local models (NOTES.md). Queued land1011ba: 8 new name fixes + the /Zc:wchar_t debt line.
