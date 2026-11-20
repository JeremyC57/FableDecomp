# NF153 header overlay

No overlay headers. hdr/ is empty and orig/ is empty: no shared header was copied or edited, and no new opt-in was added.
pin_asserts.py was not needed: no FABLE_STATIC_ASSERT moved.

Existing shared-header opt-ins switched on in rows (all already in rebuild/include; every FABLE_RETAIL_* used in rows/ was checked
to exist there):
- 0x007B1EB0: FABLE_RETAIL_CTCBASE + FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER (engine/CTCBase.h retail branch, PeekPlayerManager() const
  0x00686D50); engine/CPlayerManager.h default branch.
- 0x005AF64E: FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER (engine/CMainGameComponent.h, inline GetPlayerManager()).
- 0x004496B0 (+ test), 0x00687C90 (+ test): FABLE_RETAIL_PLAYER (engine/CPlayer.h retail branch); 0x004496B0 also engine/EPlayerMode.h.
- 0x00957F20: FABLE_RETAIL_CCHARSTRING / CCLASS / DEF_CLASS_BASE / CTCBASE / CTHINGPHYSICAL / CTHINGGAMEOBJECT / CTHINGCREATUREBASE
  (the chain the self row 0x00487DD0 uses) + engine/CThingPlayerCreature.h.
- 0x005E3EA0: engine/EPlayerMode.h. 0x008902B0: engine/CPlayerManager.h default branch.
Header users: no header changed, so no landed includer is affected.
