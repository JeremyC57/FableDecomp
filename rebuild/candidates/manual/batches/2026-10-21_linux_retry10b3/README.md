# Retry 10b3 (2026-10-21)

Retry B, 6 rows landed (WINS 6): 005bc6d6 (float re-pushed through the FPU = `double a = alpha; f((float)a);`), 009fbd00 CRenderTarget ??_G (no vptr store = implicit dtor, inlined into ??_G once the TU also calls the dtor directly), 00c202f0 (the 0x012B3444 object is a MEMBER of the base), 00caf480 + 00caefb0 (base null check survives only when the Release is in a smart-pointer member's inline dtor), 004c82a0 (dead frame byte = `const bool& persistent = GameStatePersisted;`).
