# agentNF5 header overlay

Full copies of five repo headers (`rebuild/include/engine/<name>.h` as of this pass), each with one `#ifdef`
opt-in added. The default path of every header is unchanged byte-for-byte; nothing outside the new `#ifdef` blocks
moved. Grade with `FABLE_HDR_OVERLAY=<this dir>` (the overlay is searched before `rebuild/include`).

| File | New opt-in | What it adds | Rows that define it |
|---|---|---|---|
| `engine/CTCEntityEvents.h` | `FABLE_RETAIL_CTCENTITYEVENTS_GETEVENT_DECL` | `template<class E,class F> bool GetEvent(E**,const F&) const;` on the flattened (default) class. This is the Ego_r/FableWin public const member template. Declaration only. | 0x0096BAC0, 0x0096BC20 |
| `engine/CDefClassBase.h` | `FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL` | The private default ctor `CDefClassBase();` (FableWin `??0CDefClassBase@@AAE@XZ`, out of line at 0x0042D9E0) plus `friend class CSubDefClassBase;`. Declaration only. It sits in an `#elif` after the existing `FABLE_RETAIL_DEF_CLASS_BASE_CTORS` branch, so the two never combine. | 0x00F336F0, 0x00F33730 |
| `engine/CSubDefClassBase.h` | `FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE` | `CSubDefClassBase() { }` inline in place of the out-of-line declaration (0x0044C0C0). The CConversation TU expands it around the out-of-line CDefClassBase ctor call. Its vtable store is dead and drops out. Pair it with the one above. | 0x00F336F0, 0x00F33730 |
| `engine/CTCScriptedControl.h` | `FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONLIST` | `PActionList` typed as Ego_r has it: `std::list<CCountedPointer<CActionBase> >`. It replaces `unsigned char[4]` with the same size and offset (+0x14). The nested `class CActionBase;` is forward-declared public just before it. The opt-in includes `stlport/fable_containers.h` and `engine/CCountedPointer.h`. It needs `FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE` and `FABLE_RETAIL_STLP_LIST`. | 0x007137D0 |
| `engine/CAIBrain.h` | `FABLE_RETAIL_CAIBRAIN_FORCERESET` | `void ForceReset();` (Ego_r + FableWin `?ForceReset@CAIBrain@@QAEXXZ`, 0x0088BB60). Declaration only. | 0x006D5AB0 |

## Landing notes

- Merge each block into the repo header at the same place. The blocks are self-contained.
- `FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL` and `FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE` model a TU where:
  - CDefClassBase's ctor is out of line;
  - CSubDefClassBase's ctor is inline.

  Other def TUs with that shape can reuse them.
- Under VC7.1/Wine, a `#error` in a header made `cl.exe` spin until the 200 s grader timeout instead of exiting. CDefString.h's `#error` asking for `FABLE_RETAIL_CDEFSTRINGTABLE` did this. A row that "hangs" usually means a missing opt-in define. `g++ -fsyntax-only` shows the `#error` in a second.
