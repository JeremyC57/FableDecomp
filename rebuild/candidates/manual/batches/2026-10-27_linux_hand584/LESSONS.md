# Chunk 584 lessons

## GOTCHAS one-liners
- A CSpawnedFunc1 functor under `#pragma inline_depth(1)` needs a `__forceinline` ctor. A plain inline ctor stays a call at depth 1, while the member copy of the CScriptThing argument must stay the out-of-line 0x004ABE90 (00E7BD60).
- If the source assigns one local CScriptGameResourceObjectScriptedThingBase to another twice (`cast["A"]=x; cast["B"]=y;`), give the local model an explicit `__forceinline operator=` (CBaseObject::operator= call plus inline counted-pointer assign). Otherwise VC7.1 emits the implicit operator= out of line at the second store (00DA2CA0).
- An inline CScriptThing copy ctor must name `CBaseClass(other)` in its init list. Without it, VC7.1 calls `CBaseClass()` 0x0099A2D0 (00E0E040).
- The chunk-583 "per-exit-site dtor" gap (retail inlines a local's dtor at one return and calls it at a later one) can be beaten with an in-body `#pragma inline_depth(0)` / `return;` / `#pragma inline_depth()` block at the out-of-line exit (00E9BBC0, landed 0x00D55E90 pattern).
- inline_depth is applied per call site, using the setting of the top-level function at that site, and counted through nested inlinees. A depth-2 trivial base dtor (~CPredicateBase) and a depth-2 member dtor therefore cannot be split by pragma (00EF5840).
- 0x00CD23B9 is `CScriptGameResourceObjectScriptedThingBase::IsValid() const` (Ego_r: QBE, `return PImp.Data != 0`). The bindings' IsScripting spellings are invented.
- The VC7.1 RTM std::basic_string has no clear(); in tests, reset with `s = "";`.
- A virtual call on a local object of known type (`me.MoveToThing(...)`) is emitted as a direct call to the slot's function (0x007E7300). Declare the method virtual at its true slot in the local model.

## Review at landing
- Local stand-ins for classes with retail vtables: CBaseObject 0x0129A7D8, CScriptGameResourceObjectBase 0x0126008C,
  CScriptGameResourceObjectScriptedThingBase 0x0127094C, CScriptGameResourceObjectMovieBase 0x01260EF4 (00DA2CA0, 00E68B20,
  00E9BBC0). The 00E9BBC0 and 00DA2CA0 copies add a forceinline operator= and IsValid/Release. CSpawnedFunc1<CV_FishermanScript,
  CScriptThing> vtable 0x012EA164 (00E7BD60). CEntityScriptBinding<CV_FishermanScript> vtable 0x012EA0AC comes from the
  shared header (00E7B4E0).
- Local minimal std::vector (00E0E040), STLport map/_Rb_tree (00DA2CA0, 00E68B20, 00E9BBC0), and CBasicString/CCharString
  (00EC2520). These should move to shared retail headers when they exist.
- NAME_FIXES.tsv: three proven fixes. (1) Respell the landed callers' binding for 0x00E0E040 to the NScript form. (2) Respell
  0x00F3CF50 as AAE. (3) Change 0x00CD23B9 from IsScripting to IsValid QBE; at least two landed rows (0x00D37330 QAE, 0x00CBF9DE QBE) and
  the ScriptResource alias bind it. The three STLport spellings (0x00512630, 0x004676EF, 0x00451583) are proposals, not proofs.
- The vland RELOC_NAME warnings are manifest labels that are wrong (0x00CB8930, 0x006E7AB0, 0x00CDBFB0, 0x00CD23B9), not
  wrong bindings in these rows.
