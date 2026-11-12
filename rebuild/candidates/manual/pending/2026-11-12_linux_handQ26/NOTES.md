# chunkQ26 notes

Rows reached 8/8, grades 33 (ATTEMPTS.log; 6 of them SRC_FAIL compile fixes while porting), probes 0.

## Matched
00846dd0 CCreatureAction_PerformExpression::FinishAction -- RELOCATION_MATCH (default flags), VC7.1 and g++ tests PASS, regraded
with LANDVERIFY_STRICT_HEADERS=1 (clean: the only local class is NEntityEvents::CEventExpressionPerformedTo, which has no engine
header). Ported onto the Q25 sibling (0x00847520) framework. What fixed the draft's ebp/ebx residue was identities, not register work:
- the def-name compares call vtable slot 10 = CDefClassBase::GetInstantiationName (the draft had an invented GetName);
- 0x006925C0 is GetFramesRunning (Q25), passed as the event's duration and the Extended action's frame count;
- +0x1C9 bit 2 is CThingAICreature::IsRespondingToFollowAndWait (Ego_r, inline; new overlay opt-in);
- +0x6C bit tests are CThing::GetTypeFlags() (bits 0x10 / 0x20);
- the event object is 0x20 bytes (Ego_r members added to the local CEventExpressionPerformedTo).
Landing: merge the Q25 overlays first, then this chunk's CThingAICreature.h opt-in (hdr/README.md). Graded with hdr_r2 (no fable_string.h).

## DIFFER (best sources in differ/, residue on line 2)
- 009a3c20 CStreamingFontBank ctor: ported (CStreamingFontBank/CFontBank retail branches, CArray<unsigned char>, CMemoryDataInputStream
  ReadULONG/ReadSLONG/ReadEBOOL, CTexture default ctor = `new CTexture`, Ego_r GlyphBank {FirstGlyph, NoGlyphs, CStreamingGlyph[64]}
  = the 0x104-byte cache). The port fixed three of the draft's four residues; the glyph loop's i/j register swap stayed for 7 spellings.
  The data bank is cast to CBankFile (CStreamingFontDataBank's header is a flattened layout without the RTTI base at offset 0).
  Global 0x013BDA50 is spelled g_StreamingFontBanks here; the landed ~CStreamingFontBank 0x009A1090 spells it
  g_StreamingFontBanks_013bda50 (address-suffixed; no PDB data symbol found), so the coordinator should pick one name.
- 00b1a870 CPSCLight::DatabaseSerialise: ported (fable_string GETCRC_INLINE, CPersistContext, CRGBColour, C3DVector, CArray<C3DVector>,
  CEnginePrimitiveHandle, CPSCBase); one folded xor operand remains (bit 0 of +0x48 write-back).
- 00d39960 CQ_FireHeartScript::StartRound: block placement of the post-loop terminate return. `if (!T) X;` forms place the block as
  retail but swap which cleanup copy the loop returns merge into (work/00d39960.alt_ifnotT.cpp). Not ported to CGameScriptInterface.h.
- 00eb40c0 CMurderTwistAssassin::DoWatchingForTrouble: baseline only (1 grade); the draft's goto / if(0) structure is still there.
- 0090e5d0 CAIStateGroup_BanditGossip::Update: far off; needs a real port. Proven identities for it: 0x00981410
  CAIStateGroupBase::AIAction_InteractWith (Ego_r signature, CIntelligentPointer<CThingAICreature> + CCharString + int + bool),
  0x0090C870 CThingFilter_IsValidGossipee ctor, 0x00770FA0 CTCSolidToThing::AddThingNotSolidTo (the draft had CTCNotSolid),
  member +0x24 WasCarryingWeapon (the draft had WeaponsSheathed), the FindMessage query is
  CFilter_Bind3<IsOfType, ThingSearchFilter<In2DRange>, ThingSearchFilter<IsValidGossipee>> of temporaries (retail stores the filter
  ctor's return straight into the search filter). The draft also defines CInterfaceMap::LowerBound (0x0040F020) out of line in its TU;
  check that it is the real body before reusing it.
- 006ec850 CCombatWheel::PopulateCombatRings: frame slot sharing (insert results share the GetInterface key slot in retail); local
  map/list models still to be ported onto STLport.
- 00a8ad40 C3DMesh2::LoadBinary: register allocation of the two ushort counts v mainSize; not ported.

## Not done (budget)
Rows 0090e5d0, 006ec850, 00a8ad40, 00eb40c0 and 00d39960 still use local models; the shared-header port is the next step for each.
