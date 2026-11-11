# agentNF15 header overlay

Full copy of the current repo header, with additions only (diff against `rebuild/include/` removes no lines).

| Header | Opt-in | Rows | What it adds | Evidence |
|---|---|---|---|---|
| `engine/CTCBase.h` | `FABLE_RETAIL_CTCBASE_FLAT_PEEKCOMPONENT` | 0071aa50 (CTCEmoteIcon::SetActiveTimer) | In the flattened (default) branch only: forward `class CMainGameComponent;` and the protected declaration `const CMainGameComponent& PeekComponent() const; // 0x00686D60`. No layout change; the size and offset asserts are untouched. | Ego_r.pdb `?PeekComponent@CTCBase@@IBEABVCMainGameComponent@@XZ` (protected const), which the header's retail branch already declares under `FABLE_RETAIL_CTCBASE_COMPONENT`. The retail body at 0x00686D60 is `mov eax,[0x013B8A1C] ; mov eax,[eax+1Ch] ; ret` (CMainGameComponent::Get()->Component). 0071aa50 is the one PeekComponent caller that sits on the flattened CTCBase branch, so it cannot use the retail-branch opt-in without changing its layout model. |

The other PeekComponent callers (0066b280, 0066b300, 007e1f70, 00800540, 007800b0, 006e1620, 00728ff0, 00745730, 0077cb60,
007b6a80, 007d83c0, 007498e0, 007497c0, 007fbc90 and 0065a28f) declare it with the same Ego_r spelling in their row-local
CTCBase models (those rows have no shared-header CTCBase today), so they need no overlay. Moving them onto
`engine/CTCBase.h` is header debt for a later pass.
