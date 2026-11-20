# NF144 overlay headers

None. Every NF144 row compiles against the repo headers unchanged (no FABLE_HDR_OVERLAY content; the directory exists only because the
lane passes it). The opt-ins the rows use already exist in rebuild/include: FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER,
FABLE_RETAIL_CTCCREATUREMODEMANAGER (RemoveMode; draft 0x008C0D10 only, blocked), FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR,
FABLE_RETAIL_CPERSISTTRAITS_SLONG_ONLY (NF136). No shared-header member is renamed; pin_asserts.py was not needed.
