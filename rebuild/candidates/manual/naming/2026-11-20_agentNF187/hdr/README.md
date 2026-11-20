# NF187 header overlay

All opt-ins are `#ifdef` / `#elif` additions; the default path of every overlaid header is token-identical to the repo copy (orig/).

## stlport/fable_containers.h (base: orig/stlport/fable_containers.h = repo working tree at start)
- `FABLE_RETAIL_STLP_LIST_DTOR_DECL_ONLY` (new `#elif` branch of the list destructor block, after `FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST`):
  `~list();` declared only, so a TU that defines a static list calls the landed out-of-line copy and emits no pick-any copy. Used by
  0x01228E44 and 0x0121A6AC. pin_asserts.py: pinned 0.
- Landed includers: 2859 rows include stlport/fable_containers.h or fable_map.h; none defines the new macro, so none changes.
