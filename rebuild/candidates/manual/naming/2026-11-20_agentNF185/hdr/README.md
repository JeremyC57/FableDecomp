# NF185 header overlay (2026-11-20)

All overlay headers are copies of the CURRENT repo header (base copies in ../orig/, byte-equal to rebuild/include at copy time) with
`#ifdef` opt-ins only; the default path (no macro defined) is token-identical to the repo header.

## engine/CGameScriptInterface.h
- Opt-in `FABLE_RETAIL_CGAMESCRIPTINTERFACE_SCRIPTTIMERS_MAP` (FABLE_RETAIL_NSCRIPT model only): `ScriptTimers` (+0x3c) is the real
  STLport `std::map<long, long>` (Ego_r.pdb CGameScriptInterface::ScriptTimers) instead of the opaque `unsigned char[0xc]`; the
  includer includes stlport/fable_map.h first. Used by rows/006e76e0.cpp (SetTimer).
- pin_asserts.py: pinned 0 (the asserts already carry fixed FABLE_STATIC_ASSERT_AT line numbers; none moved name).
- Landed includers: 1721 files under rebuild/src/compiled include engine/CGameScriptInterface.h; none defines the new macro (grep), so the
  opt-in changes none of them. No sibling agent overlay (NF179/181/182/183/184) touches ScriptTimers.

## engine/CScriptConversation.h
- Opt-in `FABLE_RETAIL_CSCRIPTCONVERSATION_ADDTOCONVERSATION`: forward declarations of CThing / CCharString and the declarations of
  AddPersonToConversation(CThing&) 0x006E5790 and AddLineToConversation(const CThing&, const CThing*, const CCharString&, bool)
  0x006E58A0 (both PDBs). Used by rows/006e5800.cpp and rows/006e5950.cpp.
- pin_asserts.py: pinned 10 (the ten FABLE_STATIC_ASSERT lines became FABLE_STATIC_ASSERT_AT(30..39, ...), their original lines).
- Landed includers: 6 files; none defines the new macro, so the opt-in changes none of them (default path token-identical
  apart from the pinned assert spelling).
