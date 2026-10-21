# Chunk 248 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Quest-script GSI calls: generate the CGameScriptInterface model from rtti-vtables.tsv vtable 0x01260F0C plus the manifest labels at each slot. Many labels are mangled, so they carry the exact parameter types. Fill unused slots with `virtual void vNNN();` (the generator used here is probe/mkgsi.py).
- Script wait macros: `WAIT_UNTIL(cond)` is `while (!(cond)) { NewScriptFrame(); if (term) return; } if (term) return;`. `WAIT_UNTIL(!x)` (written as `while (!(!x))`) gives retail's `test al,al ; setne bl`, while `while (!x)` gives `neg ; sbb ; inc` (0x00E75EF0, 0x00ED2A50).
- When retail builds a spawn's region "" string AFTER operator new and pushes the ctor's eax, use an inline wrapper `SpawnFunction(CSpawnedFuncBase* f, const char* region = "") { AddSpawnedFunction(f, CCharString(region)); }`. A plain const-ref conversion swaps the two string slots (0x00DF4E00, 0x00F01EB0, 0x00F018B0). A function that spawns ITSELF needs `__forceinline` on the CSpawnedFunc ctor and the wrapper, or VC emits them out of line (0x00D6BE20).
- A component that retail loads straight into ecx (`mov ecx,[eax+4]`) is HasTC + the ternary GetTC. A miss arm that loads a dead frame word is an uninitialised component pointer (`T* p; if (HasTC(k)) p = GetTC<T>(k); p->F();`) (0x00635840, 0x004884D0).
- An if/else whose arms both end with the same `Array[param] = true` store, written ONCE after the if/else, keeps the parameter in a callee-saved register from entry and frees its argument slot for string locals. An early `return` in one arm adds a frame word and reloads the parameter (0x00DAEA70).
- A by-value C3DVertexBlend2-style pair copied as `const T& src = v[i]; a = src.X; b = src.Y;` reproduces retail's dead `lea` of the element. Its 8-byte by-value temporaries give the `and esp,-8` aligned frame on their own (0x00AE37D0).
- A `char[12]` CScriptThing body is 1-aligned and shifts the next member (+0x99 instead of +0x9C). Model it as `void* _body[3]` (0x00DA0C40).
- A by-value CScriptThing parameter with exactly two exits: retail expands the inline dtor at both. Plain inline expands only one, and `virtual __forceinline ~CScriptThing() {}` expands both. The counted pointer then needs `Info = 0; Data = 0;` to get retail's +0x20-before-+0x24 zero stores (0x00DA2860). Register rotation in those tails is still open.
- A shared `return false` / found-store tail that retail places after the FIRST predecessor (later tests jump back) is placed after the LAST one by both RTM and QFE (0x00BFC600, 0x009E3640). Goto labels, && chains and optimize("s") do not move it.
- The PlayerGui stat-update helper is `UpdateStat(float value, float rate, float steps, float a, float b, CStatBar* bar)` (0x00437AE0). Retail PlayerGui offsets: stat bars +0x2CC/+0x2D4/+0x2DC/+0x2E4/+0x2EC, AbilityButtonsAlpha +0x260, QuickAccessButtonsAlpha +0x26C, OldDrawGui +0x291, PDrawStatUpdates +0x1A8, PDrawQuestInfo +0x1C8. CPlayer UsingFreeCamera/Tracking/TrackingYZ sit at +0x220/+0x221/+0x222.
- `Transfer(CCharString&)` with a non-const reference accepts a helper's by-value result temporary (VC7.1 extension) and pushes its address straight from eax (0x00AE8810).
- A switch over a closed enum with no case 1 leaves a 0 entry in the jump table (0x00F274D0 table at 0x00F27638).

Review at landing:
- Rows needing more than 4 grades: 00daea70 (3), 004884d0 (4), 00d26a50 (2), 00e75ef0 (2), 00a25bd0 (2), 00ae37d0 (3). All are within the 5-per-strategy budget.
- Local base models next to an existing engine header:
  - 0043a380, 00439f20, 004884d0: retail-offset views of CPlayerGui/CPlayer, because retail sits 8/0x14 bytes below the PDB. The Layout comments give the evidence.
  - 00a25bd0: CResourceBase restates engine/CResource.h with its virtual interface, because the header is data-only.
- Nested class: 00d9d4f0's line 1 is `CQ_HobbeToothContestScript::CQ_HobbeToothWhisper::Init`, so finish.py takes that module name; the catalog says CQ_HobbeToothWhisper.
- Leaf lines are set where line 1 differs from the catalog name or the TU has several candidates: 00e824e0 (KeepVariablesUpToDate), 00ed2a50 (WatchForRegionLeaving), 00f274d0 (Transfer), 00a25bd0 (BeginLoading), 00d9d4f0 (Init), parked 00de81b0 (CheckVictimHit).
- Catalog rows holding several functions: 006babd0 and 007b5470 (SKIP).
- No errors noticed in already-landed sources.
