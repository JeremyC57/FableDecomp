# chunk 801 header overlay

Every change is an opt-in branch: the default path of each header is textually unchanged (checked with `diff` against
rebuild/include on 2026-11-06, the only differences are the blocks below). No landed source defines any of the new macros.

| Header | Macro | What it adds | Evidence | Used by |
|---|---|---|---|---|
| engine/CThingAICreature.h | FABLE_RETAIL_CTHINGAICREATURE_DEBUGRENDER (nested in the retail branch) | the virtual `DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&)` (slot 36, 0x00833C00) and the Ego_r members DebugRenderHandles (std::list<CEnginePrimitiveHandle>, +0x194), MoodSprite +0x198, MoodSpriteCounter +0x19C, PCurrentBrainOutput (opaque +0x1A0), PBrain (CCountedPointer<CAIBrain>, +0x1A8), the tail opaque; includes for list / CCountedPointer / CEnginePrimitiveHandle | vtable 0x0127293C slot 36; `lea edi,[esi+194h]` + clear 0x00834F90 and `cmp [esi+1A8h],0` in 0x00833C00; offsets agree with the landed ~CThingAICreature 0x008320C0; no size change | differ/00833c00.cpp |
| engine/CAIBrain.h | FABLE_RETAIL_CAIBRAIN_STATEGROUPTEXT | declarations of PeekCurrentStateGroupName 0x0088B7D0 / GetCurrentStateGroupDebugText 0x0088B7E0 (+ `class CCharString;` forward declaration outside the branch) | Ego_r + FableWin PDB name sets; bodies read (see NAME_FIXES.tsv); no layout change | differ/00833c00.cpp |
| engine/CIntelligentPointer.h | FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_OUTOFLINE | declaration-only copy ctor so callers call the out-of-line COMDAT 0x00654D58 | FableWin ??0?$CIntelligentPointer@$$CBVCThing@@@@QAE@ABV0@@Z; 0x00960646 calls it for the by-value argument of AIAction_PerformActionTargetFaceAndWait | differ/00960120.cpp |
| stlport/fable_containers.h | FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE | `_List_base::clear()` declared only (else the inline body) | retail calls 0x00834F90 (landed ~CThingAICreature binds the same symbol) | differ/00833c00.cpp |
| stlport/fable_containers.h | FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT | `iterator insert(iterator)` declared only + `push_back()` as `insert(end())` | retail calls 0x007464A0 with the iterator copied into the arg slot (0x00833C74) | differ/00833c00.cpp |

Includers rechecked read-only with this overlay: ~CThingAICreature 0x008320C0 and CThingAICreature ctor 0x00831F80, both
RELOCATION_MATCH PASS (SIBLINGS.tsv). The only matched row of this chunk (0x007B0CD0) uses none of these macros.
None of the parked rows landed, so the coordinator may also choose to defer this overlay until one of them matches.
