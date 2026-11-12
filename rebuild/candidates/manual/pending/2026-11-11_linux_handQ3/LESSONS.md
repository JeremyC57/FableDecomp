# Chunk Q3 lessons (GOTCHAS-ready one-liners)
- A script Main whose two branches end in the same recall tail: retail emits the tail once and jumps to it -- spell the second path with `goto done;` (0x00CDD80B, NScript::CGlobal_TeleportToHeroGuildScript::Main); duplicating the code or an inline helper both miss (1006v1012 / 788v1012).
- 0x0076EB00 CTCWeaponTrail::InitialiseTrails: retail keeps the four literal push_backs inline but calls the loop push_back -- /d2inlT65 (band T65..T67) reproduces the split.
- CParticleUpdateParameters locals need FABLE_RETAIL_CVECTORMAP_CTOR (the ctor zeroes the Dirty bytes, stores at 0x008E2337); without it 1011v1019.
- CModeGuildmasterPowerUpSpeed::FrameUpdate (0x008E24F0) is CModeGuildmasterPowerUpShield::FrameUpdate (0x008E20F0) plus `CTCBoss* boss; ParentThing.GetTC(&boss);` right after `--Timer;` -- author one, clone the other first try.
- VC7.1 3-case switch with `case 0:` folded into `default:` emits a dec chain; give `case 0:` its own body to get retail's jump table (0x007CCF50, table 0x007CD380).
- `if (!a() || !b()) return;` vs two separate `if (!x) return;` changes block layout but not retail's duplicated failure epilogue (0x007CD031) -- still open.
