# Landing hand4b b4 (2026-11-19): C3DAnimation2::InitialiseAnimationInstance (0x00A50C20, 611 bytes) -- one Inferred block scope, owner review

vland --land, strict headers: RELOCATION_MATCH + PASS. New opt-ins (`hdr_README.md`): C3DAnimationStats.h TIMINGEVENTS (TimingEvents as
multimap<CCharString,float>, stats members protected; retail copies them member by member), C3DAnimation2.h INSTANCELIST
(list<CIntelligentPointer<C3DAnimation2> >), CSimpleMemoryPool.h RESET (Reset 0x00A5C790 / CalculateMemoryAllocatedByClient 0x00A5C850 declarations).
All 46 landed users of those headers rechecked. `// Inferred:` the scratch-buffer part sits in its own block after the instance registration
(retail frame 8: `matches` shares the CIntelligentPointer temporary's frame word, 0x00A50D0C = 0x00A50C77's slot); with Ego_d's function-scope
locals (newer revision) the frame is 0xC. Name fix: 0x009F49A0 is ?Unlock@CScratchBuffer@@QAEXXZ (catalog `_global::SetMemberConst_009f49a0`).
