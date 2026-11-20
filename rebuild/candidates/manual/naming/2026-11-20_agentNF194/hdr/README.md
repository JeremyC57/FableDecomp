# NF194 header overlay (2026-11-20)

Bases are in ../orig/ (copied from the repo working tree at start). Opt-ins only; with no NF194 macro defined each header is
token-identical to its orig/ copy.

## engine/CCountedPointer.h (base: rebuild/include/engine/CCountedPointer.h)
- `FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY`: declares `ShareData(T*, CCPPointerInfo*)` without its in-class body
  (the body is moved under `#else`, unchanged). Users: the 35 q5992 rows (CEntityScriptBinding<T>::CreateActiveEntityScript),
  whose retail calls 0x00CE1040 out of line.
- pin_asserts.py: pinned 0 (the one FABLE_STATIC_ASSERT, sizeof(CCPPointerInfo), sits above the opt-in and did not move).
- Header users: 8343 landed rows include engine/CCountedPointer.h; none defines the new macro (grep of rebuild/ finds no use),
  so the default path they compile is unchanged.
