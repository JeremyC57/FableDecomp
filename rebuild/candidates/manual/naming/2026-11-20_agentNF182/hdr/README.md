# NF182 overlay headers (add-only #ifdef opt-ins; base copies in ../orig/)

## engine/CTCFlashOnHit.h (copy of rebuild/include/engine/CTCFlashOnHit.h)
- Adds `FABLE_RETAIL_CTCFLASHONHIT_DTOR` (inside the class, after the existing VSLOTS block): the inline constructor
  `CTCFlashOnHit(CThing&)` in the form of its landed row 0x004D6292, and declarations of `virtual ~CTCFlashOnHit()` (0x007E3420),
  `FrameUpdate` (slot 9), `GetInterfaceType` (slot 21), `GetClassTypeName` (slot 22). Used by rows 0x004D62D1 and 0x004D6292.
- Without the macro the header is token-identical to the repo copy except the pin below.
- pin_asserts.py pinned 1: `FABLE_STATIC_ASSERT(sizeof(CTCFlashOnHit)==0x1c)` -> `FABLE_STATIC_ASSERT_AT(19, ...)` (its repo line).
- Landed includers (grep `CTCFlashOnHit.h` in rebuild/src/compiled): 0x00667D30, 0x004D62CE. Neither defines
  FABLE_RETAIL_CTCFLASHONHIT_DTOR, so the opt-in changes none of them.

## engine/CSimpleMemoryPool.h (copy of rebuild/include/engine/CSimpleMemoryPool.h)
- Adds `FABLE_RETAIL_CSIMPLEMEMORYPOOL_FLAT_CTOR` in the FLATTENED (default) branch: a declaration of
  `CSimpleMemoryPool(unsigned long, unsigned long)` (0x00A5C570, ??0CSimpleMemoryPool@@QAE@KK@Z, as the retail branch declares it).
  Used by row 0x00B260B0. Without the macro the header is token-identical to the repo copy.
- pin_asserts.py: nothing new to pin (the header's asserts were already FABLE_STATIC_ASSERT_AT).
- Landed includers: 28 rows in rebuild/src/compiled; none defines the new macro, so the opt-in changes none of them.

## engine/CMaintainedEvent.h (copy of rebuild/include/engine/CMaintainedEvent.h)
- Adds `FABLE_RETAIL_CMAINTAINEDEVENT_CTOR`: a declaration of `CMaintainedEvent()` (0x009E45B0) inside the class. Used by row 0x009E45B0.
  Without the macro the header is token-identical (the class's closing `};` moved to its own line; tokens unchanged). No asserts.
- Landed includers: 10 rows (grep CMaintainedEvent.h in rebuild/src/compiled); none defines the new macro, so none changes.

## (held, not in hdr/) libpk/LHAudioEventPosition.h
- The opt-in `FABLE_RETAIL_CSOUNDEVENT_CTOR_DTOR` (declarations of CSoundEvent() 0x00C21BA0 / ~CSoundEvent() 0x00C207F0) serves only the
  HELD draft work/00c239b0.cpp, so it sits in work/held_hdr/libpk/ (base copy work/held_orig/libpk/), not in the hand-back overlay.
