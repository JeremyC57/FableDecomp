# Landing hand4e b2 (2026-11-19): std::for_each (0x00754D80), std::__pop_heap_aux (0x0076A590), CStringParser::SkipPastNewline (0x009B9D70)

All three rows pass strict vland --land: MATCH / RELOCATION_MATCH / MATCH + PASS, no flags.

**0x00754D80** is std::for_each<pair<float,const CAnimationEntry*>*, anonymous CGetBestTurningAnim>. The catalog's _For_each label is VC10's spelling.

**0x0076A590** is std::__pop_heap_aux over pair<unsigned long,C3DVector>, with the PDB comparator CParticleUpdateCCompareVector<unsigned long>. It uses shared
opt-ins only. NAME_FIXES also respells callee 0x00769D50, which has two inconsistent landed stand-in bindings.

**0x009B9D70** needed a new nested opt-in, FABLE_RETAIL_CSTRINGPARSER_MOVESTRINGPOS_EGOD, in engine/CStringParser.h.
- The existing bodies are unchanged under `#else`, and the header's asserts are already `_AT`-pinned.
- Its CalcInQuotes also MATCHes retail's own copy at 0x009B9570.

**Header-user recheck:** CStringParser.h, 78 / 78 good. The NThingMap_CThingMap.h overlay copy was identical to the repo (b1).
