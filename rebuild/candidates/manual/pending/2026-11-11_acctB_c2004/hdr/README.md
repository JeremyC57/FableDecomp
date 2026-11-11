# Chunk 2004 header overlay (`FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2004/hdr`)

Every change is an opt-in (`#ifdef FABLE_RETAIL_...`) or a new header that only the chunk's rows include; the default path of each
copied repo header is unchanged (copied from HEAD 2026-11-11, then the block below added). Evidence is two sources each (PDB name
set + retail bytes / donor order / RTTI).

## New headers
| header | opt-in | content | evidence |
|---|---|---|---|
| engine/CAIStateGroup_DeedReactionRoot.h | FABLE_RETAIL_CAISTATEGROUP_DEEDREACTIONROOT | class : CAIStateGroupBase, Ego_r members ThingReactingTo +0x14 .. DeliveredSpeech +0x35 (sizeof 0x38), the six PDB overrides | struct_layouts_egor.tsv (no STL member, PDB offsets = retail) + row 0x009609B0 reads +0x14/+0x1C/+0x20/+0x24/+0x28/+0x30; Ego_r/FableWin name sets |
| engine/CThingFilter_IsOpinionSourceWithFreshDeed.h | FABLE_RETAIL_CTHINGFILTER_ISOPINIONSOURCEWITHFRESHDEED | PCreature / LastVDeedID / LastSourceUID (24 bytes), inline ctor (FableWin ??0..@@QAE@PAVCThingAICreature@@J_K@Z), operator() decl (0x00960FC0) | Ego_r TPI layout + retail ctor expansion 0x009609F5..0x00960A1B; vtable 0x0123E7E0 RTTI .?AV?$CIntelligentPointer@VCThingAICreature@@@@ |
| engine/CTCOpinionSource.h | FABLE_RETAIL_CTCOPINIONSOURCE | : CTCBase; IsAlwaysAwareOf / PeekVirtualDeed / PeekVirtualDeedID / StatGetInterfaceType inline; Ego_r members at retail offsets (vector 12 bytes: -4) | PDB name sets + retail reads +0x75 / +0x80 / +0x84 (0x00960D9D, 0x00960BC2) and +0x18 / +0x77 in the landed filter 0x00960FC0 |
| engine/CTCRegionFollower.h | FABLE_RETAIL_CTCREGIONFOLLOWER | : CTCBase; IsRegionFollower(const CThing*) const (0x006AEB60), StatGetInterfaceType 0x7A | FableWin name set + retail call 0x00960D58 / HasTC bit 0x00960D22 |
| engine/CBezierCurve.h | FABLE_RETAIL_CBEZIERCURVE | Point1..4 (48 bytes), inline GetPoint1/3/4, SetPoint1..4, default ctor | Ego_r TPI + PDB accessor names + retail stores 0x00912BC6.. |
| engine/CAIStateGroup_FlyingThingBase.h | FABLE_RETAIL_CAISTATEGROUP_FLYINGTHINGBASE | : CAIStateGroupBase; MinHeight* / MaxHeightAboveGround / FlightRange (0x14..0x20); GetRandomFlightPos / AddRandomBezierCurveToPath / CalculatePrettyPath (protected) | Ego_r TPI + PDB names + retail reads +0x18 / +0x1C / +0x20 |
| engine/CTCPhysicsControlled.h | FABLE_RETAIL_CTCPHYSICSCONTROLLED | : CTCPhysicsStandard; ImpulseVelocity +0x88 .. LastRenderZOffset +0xA8 (sizeof 0xAC); PerformImpulseBasedMovement decl | RTTI base chain (rtti_bases.tsv) + Ego_r TPI + retail reads +0x88..+0x90 (row 0x00725500) |

## Opt-ins added to copies of repo headers
| header | opt-in | change |
|---|---|---|
| engine/CTCPerceiveThing.h | FABLE_RETAIL_CTCPERCEIVETHING_AWARE_CALLED (inside the AWARENESS branch) | IsAwareOfThing() const declared, not defined inline (retail calls the COMDAT 0x00643B3B from 15 sites incl. 0x00960DF1); inline IsThingIgnoreFOVLineOfSight (+0x2F, PDB name); StatGetInterfaceType; GetExtendedSightRadius decl (0x00744950: multiplies +0x44 = ExtendedSightRadius; 0x007448F0 is GetSightRadius, +0x40, Ego_r order 0x007573B0 / 0x00757430). Without the opt-in the inline body stays. |
| engine/CTCCreatureOpinionOfHero.h | FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_HEADPENDINGDEED | `const COpinionDeed* GetHeadPendingDeed();` (Ego_r + FableWin ?GetHeadPendingDeed@CTCCreatureOpinionOfHero@@QAEPBVCOpinionDeed@@XZ, 0x00683770) and StatGetInterfaceType when neither INFORM nor GREET is on. |
| engine/COpinionReactionManager.h | FABLE_RETAIL_COPINIONREACTIONMANAGER_PEEKSOURCE | inline PeekThingOpinionSource() const (Ego_r ?PeekThingOpinionSource@COpinionReactionManager@@QBEPBVCThing@@XZ; retail reads +0x714 through GetPItem, 0x009609C7). |
| engine/CThingCreatureBase.h | FABLE_RETAIL_CTHINGCREATUREBASE_HEIGHTBELOW | GetHeightZBelowAt(const C3DVector&, bool, bool) const decl (0x00666D40; both PDBs). |
| stlport/fable_deque.h | FABLE_RETAIL_STLP_DEQUE_PUSHBACK, FABLE_RETAIL_STLP_DEQUE_PUSHBACK_AUX_BODY | STLport 4.6 `_M_decrement`, iterator `operator--`, deque `back()`, `push_back`, `clear()` decl (0x00913A20), protected `_M_push_back_aux_v` decl; the AUX_BODY opt-in adds `_M_reserve_map_at_back` and the visible noinline `_M_push_back_aux_v` body (STLport stl/_deque.c; landed 0x00913D60 is the same TU). |
| engine/CGraphicDataBank.h | FABLE_RETAIL_CGRAPHICDATABANK_LOADGRAPHICFRAME (static-blocks branch) | private LoadGraphicFrame decl; PixelFormats typed `CPixelFormat[11]` (same 44 bytes at +0x288). |
| engine/CGraphicInfo.h | FABLE_RETAIL_CGRAPHICINFO_LOADFRAME | `friend class CGraphicDataBank;` (the bank reads the info's header in place; the original grant is not in the name sets) and the private InitialiseFramesAsSurfaceSections(CManagedTexture&, signed char, CCountedPointer<CTexture>&, bool, bool) decl (Ego_r ?..@@AAEXAAVCManagedTexture@@CAAV?$CCountedPointer@VCTexture@@@@_N2@Z, 0x009FE780). |
| engine/CManagedTexture.h, engine/CManagedTextureData.h | FABLE_RETAIL_CMANAGEDTEXTURE_GETTEXTURE | inline GetTexture() on both (Ego_r ?GetTexture@CManagedTexture@@QAE?AV?$CCountedPointer@VCTexture@@@@XZ / ..CManagedTextureData..; retail copies the counted pointer in place, 0x009FEDB9). |
| engine/CTexture.h | FABLE_RETAIL_CTEXTURE_LEVELS | GetNoLevels() const (0x009F9DF0), GenerateMipmaps(unsigned long, unsigned long) (0x009F9E60) decls (Ego_r names). |
| engine/CPixelFormat.h | FABLE_RETAIL_CPIXELFORMAT_ISEQUIVALENT | IsEquivalent(const CPixelFormat&) const decl (0x009E37F0). |
| engine/CSurface.h | FABLE_RETAIL_CSURFACE_LOADGRAPHICFRAME | GetByteLength / Lock / Unlock / InitialiseAsImage / CopyFromSurface decls (Ego_r names; 0x009F2FC0 / 0x009F33E0 / 0x009F2DC0 / 0x009F3230 / 0x009F37E0). |
| engine/CBankFileAsyncData.h | FABLE_RETAIL_CBANKFILEASYNCDATA_PEEKDATA | PeekData() const decl (0x009D5580) + inline GetLength() (Length +0x10), only when FABLE_RETAIL_C3DANIMATIONINFO_BUILD is off. |
| engine/CTCPhysicsBase.h | FABLE_RETAIL_CTCPHYSICSBASE_IMPULSE | IsPhysicsMeshTypeCylinder, UpdateCollisionGroundIneractions, protected ImpulseRespondToCollision (0x006AF7D0), ImpulseRespondToCollisonWithCylinder, ModifyResponseForNearVerticalSlopes decls (row 0x00725500). |
| engine/CTCPhysicsStandard.h | FABLE_RETAIL_CTCPHYSICSSTANDARD_IMPULSE | protected UpdateVelocityForImpassability (0x007243C0), IsCollisionWithThingToCauseResponse (0x008E5750), UpdateCollisionGameInteractions (0x00723AC0) decls (row 0x00725500). |
| engine/CPhysicsCollideResponse.h | FABLE_RETAIL_CPHYSICSCOLLIDERESPONSE_CTOR | inline ctor setting Friction = 1.0f (retail stores 1.0f once at 0x0072556D; the landed 0x00725290 model's ctor) (row 0x00725500). |

Landing note: the opt-in blocks are additive; when merging into HEAD headers, re-copy HEAD first and re-apply the blocks
(other chunks may have changed the same headers since 2026-11-11).
