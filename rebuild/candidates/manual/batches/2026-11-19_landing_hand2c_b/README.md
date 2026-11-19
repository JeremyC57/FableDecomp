# Landing hand2c b (2026-11-19): 3 more not-plain-C++ rows (415 bytes)

vland --land, strict headers: RELOCATION_MATCH + PASS (`logs/vland_land.log`); no flags; overlay merged (three additive opt-ins,
`hdr_README.md`). Payload names normalised to the short form (the agent's payload repeated the class in `name`).

| row | notes |
|---|---|
| 0x0083DB60 CThingPhysicalSwitch::GetCurrentGraphic (125) | `if (Triggered) return PeekDefinition().TriggeredGraphic; return PeekDefinition().UntriggeredGraphic;` on PEEKDEFINITION_SAMETU; opt-in FABLE_RETAIL_CTHINGPHYSICALSWITCH_GETCURRENTGRAPHIC |
| 0x00A5B650 CCachedFile::Copy (184) | shared-header port; opt-in FABLE_RETAIL_CCACHEDFILE_COPY + the repo's STLport back_inserter / out-of-line __copy opt-ins (precedent 0x007CDFE0) |
| 0x00BE2120 CEngineInternalPrimitive2DViewport::ClearChildPrimitive (106) | the landed group twin 0x00BC62D0's body; opt-in FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DVIEWPORT_CLEARCHILD. ORACLE FIX: the scratch batch oracle had cut it to the twin's 97 bytes; retail is 106 (`call; pop esi; pop edi; ret 4`) -- other twin-derived oracle rows may be cut the same way |

hand2c finished with 10 of its 98 rows closed (all landed today); its final notes, open drafts (0x00AC7EB0, 0x008A8260, 0x0063F4B0,
0x00C40590) and the c2 scheduler tools t20.py / sim.py are in `pending/2026-11-18_hand1/hand2c_final/`.
