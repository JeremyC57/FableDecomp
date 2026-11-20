# NF126 lessons (one line each)
- A row whose local CModeBase is the only stand-in on the path moves onto the shared nested CModeBase in one grade with a mechanical transform (tools/mb.py); include engine/ETCInterfaceType.h first (the header's StatGetInterfaceType needs TCI_ENTITY_MODE_MANAGER).
- Before a self leaf change, scan the exe for referencers: 0x008DBB30 had one (0x008DC6C0, a qualified base call), so both had to move together.
- A local CThingCreatureBase that does not derive from CThing blocks the CModeBase move: ParentThing becomes CThing&, so only an unrelated-class cast reaches it; port CThingCreatureBase first.
- A by-pointer member read where retail goes through an inline getter (`mov eax,[x+3Ch] ; mov ecx,eax`) needs the getter (CWorld::GetMeshBank), not the member.
- For a cmp operand-order residue after a named count local, try the mirrored compare with the local on the left before anything else (0x00B20FD0).
- Removing a local model block by regex can take neighbouring declarations with it (a template declaration, an include); diff the transform before grading.
