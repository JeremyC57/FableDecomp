g1: the row-text optimize("s")/("gt") toggles removed; the row-local inline CScriptThing::operator= marked noinline instead; hypothesis: the leaf still calls it and its copy expands ReleaseData (retail 74 B).
result g1: DIFFER(1090v1223); the ??4CScriptThing copy becomes EQ (74 B at 0x008AB980) but the leaf loses 133 bytes: Main's own inline
  decisions need operator= to be a plain (size-built) inline. The copy and the leaf want opposite settings: a link-order case.
HEAD witness search: 157 landed rows define CScriptThing::operator= inline, none with FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (437
  rows use that opt-in, none of them define the assignment). So no landed object emits the 51-byte copy: no debt path.
