# Chunk 470 -- lessons from the authoring agent (fold into GOTCHAS at landing)

## Search tools (rough area)

- Reuse a landed sibling loop and swap only the filter and the result container. That closes or nearly closes the large search rows:
  - the 007A3CA0 loop closed 009793d0;
  - 007FDB10 / 008786E0 / 0080ECC0 / 0091BA40 brought 8 rows to one reload-order diff.
- The E-18 loop-exit spill-reload order gap also covers LEVEL-loop exits (006ce800, 00518340, 007c2d10), not only node loops (0063fd40, 007c5110, 00920200, 0084ba50, 0066a1e0).
  - Inert: the GetMapwhoArea store order (it only reschedules the box), a dead local, the iterator form, the map placement, a named filter reference, a HasComponent helper.
  - Dropping the `typed` temp costs 8 bytes (007c2d10 v2).
  - The order flips between 0084ba50 and 00518340, which differ only in the filter. That suggests a numbering effect: a permuter candidate.
- An `and esp,-8` frame in GetAllThingsInRoughArea comes from the 8-byte CIntelligentPointer local (vptr + PData) built for the push (007c5110, same as the landed 008786e0). The STLport list insert(end(), p) then placement-copies the pointer into the 16-byte node under the placement-new null test.

## Graphics appearance (0077ba40)

- CAGraphicAppearanceComponent::OnAppearanceDraw lives in a second base at +0x0C. Model it as true MI, `class X : public CTCBase, public CAGraphicAppearanceComponent`, so `this` is the component and ParentThing reads `[this-8]`. Use a local CTCBase with a real virtual dtor: engine/CTCBase.h models the vptr as data.
- Two counted-release shapes:
  - a CIVCountedPointer release `mov r,[eax+4] ; mov ecx,eax ; add eax,4 ; dec r ; mov edx,r ; test edx,edx ; mov [eax],r` is a count in a SECOND base (`DecRef(); if (GetRefCount() == 0) Destroy();`, as the landed 0x00410820). It applies to CEngineMeshEffect in 0077ba40 / 007a6760 / 0077bec0.
  - `dec [eax+4] ; jne` is `--RefCount == 0` on a first-member count.
- A by-value `const CRGBColour` result built straight into a call's argument slot (`push ecx ; push esp ; mov ecx,this ; call GetTintColour`) needs CRGBColour with a user default ctor and a user copy ctor.
- An x87 product of two members compiled to the same order (`fld RenderSizeX ; fmul Scale`) for `a*b` and for `b*a`. The brief's "`fld [a]; fmul [b]` is `b*a`" rule did not hold here; retail loads Scale first, cause unknown.
- In a ctor-argument context these forms gave identical code: named locals v temporaries for const-ref arguments, and `S + (E-S)*t` v `(E-S)*t + S`.
- Retail evaluates every call-containing argument into a temporary right to left (bulletTime, alpha, orientation, position, UniqueID) before the first push; ours loads UniqueID / alpha at push time. Next idea: inline accessors (`GetThing()->GetUID()`, `GetAlpha()`).
- The debug theme-channel test (global 0x013756F0) copies the 4-byte CEnginePrimitiveThemeChannelDesc as a dword (`mov eax,[esi+40h] ... test al,al`): a named local copy reproduces it. An inline by-value helper folds to a byte load.
- CEngineGraphic (14 bytes) needs `#pragma pack(2)`, as the landed SetActivePersistenceFlags model, or Alpha moves from +0x5A to +0x5C.

Review at landing:
- Several rough-area differ sources model CThing / CThingSearchTools locally (the landed sibling style) although engine/CThingSearchTools.h exists. differ/0077ba40.cpp models CTCBase locally on purpose: the engine header's vptr-as-data cannot host a derived override.
- Already-landed rows to fix:
  - 0x004C4160 "SPrimitiveHandleHolder_004c4160::~..." is NGraphicAppearance::CPrimitive::~CPrimitive (PDB: Type, PrimitiveHandle, BankIndex).
  - 0x0077B9A0 "CTCGraphicAppearanceStaticMesh vfunc1" is a generic `struct T` under `#pragma pack(1)`. It is OnAppearanceClear (PrimHandle = 0 through the component `this`).
  - 0x0091BA40 is the list instance and 0084ba50 the vector instance of the same GetAllTCsInRoughArea.
- differ/0077ba40.cpp still models CEnginePrimitiveStaticMesh as 0x48 bytes with MeshEffect at +0x38. The ctor 0x0077BD70 shows the PDB layout (76 bytes, MeshEffect +0x3C): fix that before re-grading.
- No row needed more than 4 grades. No catalog row held two functions. 10 large rows were not attempted (STATUS SKIP).
