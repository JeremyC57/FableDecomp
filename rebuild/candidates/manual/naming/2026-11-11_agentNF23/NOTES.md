# agentNF23 notes: name-fix queue land1011cx–da (39 entries)

## Summary
- Rows are under `rows/`. Every changed row grades RELOCATION_MATCH and its test passes with the catalog PassPattern (see STATUS.tsv).
- There is one header overlay, `hdr/engine/CTCPhysicsStandard.h` with `FABLE_RETAIL_CTCPHYSICSSTANDARD_FLUSHONGROUND` (see `hdr/README.md`).
- RENAMES section 2 lists **caller rows** as `address` (the landed row whose binding changed), with its old and new symbol for the respelled callee.

## Entry notes
- **cx 0x00749880**: the `_unattributed::AssignTarget_00749880` stand-in becomes `CTCNoise::SetCreator(const CThing&)` over `engine/CTCNoise.h`.
- **cy 0x0063AB10**: binders are 0x00723A10, 0x006AFF00 and 0x008751D0.
  - 0x00723A10 itself was a free stand-in. It is now `CTCPhysicsStandard::SetRHSetAsFlushOnGround` (Ego_r 0x0073A6D0 / FableWin 0x02527020; header opt-in).
  - Its 0x00686D20 binding is now `CTCBase::PeekWorldMap`.
- **cz 0x00658D5D**: the claim is rejected. The function is `IsSociableTime`, not `IsCurfewTime` (REJECTED.tsv).
  - The 5 binders were respelled to the proven `?IsSociableTime@CTCVillage@@QBE_NXZ`.
  - Not fixed (optional extra): the binder `?IsCurfewTime_00658db3@CTCVillage@@QAE_NXZ` at caller 0x0065C781 is the real IsCurfewTime 0x00658DB3, but it is mis-spelled.
- **cz 0x00810FC0 GetPHitBy**: 5 binders were respelled. 0x0094E480's stand-in `CEventHitByMethods` became `NEntityEvents::CEventHitBase : CEventBase`, which also fixes 0x00810F00 `IsKnockDown`.
  - Gap: rows 0x008D1890 / 0x008D29C0 / 0x008D2BB0 / 0x008D2D90 / 0x008D2F80. Including `NEntityEvents_CEventHitBase.h` pulls `CHitParameters.h` → `stlport/fable_map.h`, which clashes with the rows' local `std::pair`, and Wine cl hung (timeout). 1 grade was spent on 0x008D29C0; the row was not kept.
  - Gap: 0x00733020 (brief: "GetHomeBuilding"). Its catalog identity `CThingFilter_HasBuildingAsHome` is doubtful: the body calls GetPHitBy on its argument, and no landed row references 0x00733020. Left for an identity pass.
- **da _M_erase (QAE→AAE and placeholder types)**:
  - Self rows: 0x004C4490, 0x00696900, 0x00869BB0, 0x009B40F0 and 0x009EF6E0 changed only QAE→AAE (`private:`).
  - The 8 placeholder groups were regenerated over agentFAM07's proven types, each self row together with its placeholder member rows (`clear`, `~_Rb_tree`, `erase`).
  - The 10 FreeTree stand-in self rows were rebuilt as STLport `_Rb_tree::_M_erase` explicit instantiations.
  - 0x0098FD40 and 0x0070ADB0 were already correct.
- **~vector rows 0x00F25B70 / 0x00F26E20 / 0x00F26F70**: renamed. Their binders already bind the PDB spelling.
  - 0x00F25B70 keeps a local CArenaWaveDef, as FAM07 did, because there is no CArenaWaveDef header (it is built over CArenaRoundDef.h).
- **0x004855A3 `_List_base<NUISystem::CList*>::clear`**: the self row was renamed.
  - Gap: the 0x013B8A98 GPreviousSelected binders (0x0047D78B, 0x0047E07F, 0x0053C9BC, 0x0057770A, 0x005778CA, 0x00587847, 0x00587A41, 0x00589D30, 0x0059C8AE, 0x0059E2C7, 0x0059F304) use stand-in globals (CGlobalList, CNodeList, CInventoryCamera, CListRegistry) with local NUISystem models that clash with NUISystem_CList.h.
- **0x013BAE04 AccuracyOverride**: only busy rows bind it (0x007899C0, 0x0078A390), and they already use the correct spelling.
- **0x00A01C10 ~CBaseIntelligentPointer (virtual)**: 16 binders were respelled through `engine/CIntelligentPointer.h`.
  - 12 CCreatureActionBase-derived member-dtor rows. An explicit member dtor call must be qualified (`m.CBaseIntelligentPointer::~CBaseIntelligentPointer()`) to stay direct.
  - 0x00646709, 0x007699C0 and 0x0076BDD0.
  - 0x00440730 was typed by its catalog/PDB element `CMiniMapDisplay::CMarkerInfo` (self-row rename).
  - Left:
    - **CBaseIntelligentPointerCtor stand-in rows**: 0x0090FB60, 0x00933C10, 0x00937D20, 0x00937F70, 0x0094E6B0, 0x00956B00, 0x009575C0, 0x00957F20, 0x0095E350, 0x00967180, 0x0096E540 and 0x0096F180. A second local class models the same pointer, so this needs a merge into the header class.
    - **Namespaced local models**: 0x006A2E20, 0x006A3300, 0x0080DF90 and 0x008D74B0 (NCreatureMode). Retyping changes their own instantiation leaves, so these need STLport `pair`/heap re-modelling.
    - **Non-template local `CIntelligentPointer` rows**: 0x004DCD2B, 0x004E1437, 0x006592E2, 0x006C1BE0 and 0x006E1E60.
    - **Address-suffixed stand-ins** (`CMember_00a01c10` / `CValue_00a01c10`, `Dtor_00a01c10`, `ForwardTarget_00a01c10`, `Release_00a01c10`, `DestroyElement_00a01c10`): the owning class is not proven.
    - **Rows whose local "CCharString" / "SubA" dtor is really 0x00A01C10** (0x008FECEC, 0x0090010E, 0x00918150): a mis-typed member, outside a name fix.
- **0x00A01B90 SetPItem(CBaseObject*)**: in 19 rows that keep a global-scope local `CBaseIntelligentPointer`, the parameter was retyped to the PDB `CBaseObject*` and the arguments cast, as `CIntelligentPointer<T>::SetPointer` does.
  - Left:
    - **0x00883180**: its landed source grades DIFFER(32v31) unchanged in this lane, so it cannot be verified.
    - **CBaseIntelligentPointerCtor rows**: as above.
    - **Namespaced models**: 0x007FDEB0, 0x007FDF60, 0x00807B70, 0x00809D60, 0x008C0900, 0x006E1080, 0x008110A0, 0x0084AE00 / AE80 / AF30 / B010, 0x008AC3F0, 0x00982320, 0x00737AD0, 0x00748D80, 0x0074CE40, 0x00739550, 0x006A23E0, 0x00961880 and the FableCopy/FableFill/FablePush 0x006A3xxx family.
    - **0x00970F30**: CThingRetail, with the model class under another name.
- **0x004AD520 push_back**: the self row is correct. Its binders reach it through stand-in vector classes, so this is a gap.
- **Stand-in `_Rb_tree`/map models left unchanged**:
  - 0x004C5DA0, 0x00532D90, 0x006301A0 (CRbTree), 0x00869020 (CArmourMap), 0x009B02E0 (CRetailMap), 0x00B3B8E0 (CExternalTextureMap) and 0x00B93DF0 (CBoneConfigTree).
  - 0x0070A570 is a flattened map; restructuring it also moves its find/insert bindings.
  - The `CRetailTree_erase*` dtor rows and the CTCCarryable_OnKill/EraseRange rows: ~map vs ~_Rb_tree identity is ambiguous.

## NF22 open items
None were taken; none was proven in the time available. They are still open:
- 0x006ED9C0 is a second DeregisterCombatant.
- 0x00445BA0 / 0x00445BC0.
- 0x0099BDB0 SetSubstring vs GetSubString.
- 0x0129BA3C has two spellings.

## Header debt
- `CTCPhysicsStandard.h`: the `SetRHSetAsFlushOnGround` declaration should become unconditional once 0x00723A10 lands.
- `CIntelligentPointer.h`: the CBaseIntelligentPointerCtor stand-ins show that rows need the conversion ctor (`CIntelligentPointer(T*)`) and the COPY/ASSIGN opt-ins together. A combined opt-in would let those rows drop their local models.
