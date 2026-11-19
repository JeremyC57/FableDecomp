# ESCALATE 0x008EEC70 Mass::Collide -- DIFFER(1129v1129), 2 hunks

Constraint: the block after `if(bounce_impulse<0.0f)return false;` is one scheduler region of 81 tuples ending
`... fstp fstp sub mov` (dag.sh: REGION 83 incl. two None). Retail's region holds `sub esp,8` but not the following
`mov eax,esp` (0x008EEF2E -> retail 0x008EEFA8), i.e. retail has exactly one extra tuple in that region (or 0x269 PAREN).
Ego_d locals match the draft exactly; GetVel / ApplyImpulse bodies MATCH at 0x008EEA30 / 0x008EEA80.
Measured (region sizes unchanged 83/83/17/7/83/55/16/40 for all): parens around Dot result / Dot arg / ApplyImpulse arg /
GetVel body, -(c.X), (c.Y), named r in Right and GetVel, Right(const&) (worse), Right non-inline / non-fastcall,
C2DVector opt-ins NAMED_RESULT_SUB/MUL, ADD_COPY_RHS_X (worse), FORCEINLINE_CTOR (same).
Every C2DVector operator used in this region also runs in regions 1-2 (both exactly at the 81 cap), so a header-level
tuple would move those boundaries too. Needed: the source of one non-emitting tuple local to lines 39-41.
