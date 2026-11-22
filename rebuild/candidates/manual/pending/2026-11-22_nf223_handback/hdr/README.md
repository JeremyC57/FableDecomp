# NF223 header overlay (base copies in ../orig/)

## engine/CGraphicDataBankInit.h
- Add-only, member-specific opt-in `FABLE_RETAIL_CGRAPHICDATABANKINIT_CTOR_DECL_ONLY`: declares `CGraphicDataBankInit()` without its inline
  body. Users: rows 0x0041E3F6 (NUISystem::CManager::CManager) and 0x005BFF07 (NInventory::CTCInventoryBase::CTCInventoryBase). Both are size
  TUs that call the out-of-line ctor 0x00415B80. With the default inline body the TU emits a 48-byte ??0CGraphicDataBankInit@@QAE@XZ copy that is
  NEQ v 0x00415B80 (it stores the constant bytes directly; retail goes through dl / cl). Retail's body stays the #else one.
- Default path is token-identical to the repo copy. The only other change: pin_asserts.py pinned 1 assert
  (`FABLE_STATIC_ASSERT(sizeof(CGraphicDataBankInit) == 0x2c)` -> `FABLE_STATIC_ASSERT_AT(32, ...)`, its repo line).
- Landed includers: rebuild/src/compiled/00/b2/CEngineSurfaceCompositionManager_CEngineSurfaceCompositionManager_00b2cb40.cpp only. It does not
  define the macro, so it preprocesses as before. No other landed row defines the macro. No other agent overlay (auth10) has this header.
- 0x00B2CB40, the only includer, graded with the overlay on and with it off: RELOCATION_MATCH both times (work/b2cb40.tsv).
