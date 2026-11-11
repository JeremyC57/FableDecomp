# Chunk 2023 header overlay (`FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2023/hdr`)

Every change to a copied repo header (HEAD 2026-11-11) is an opt-in `#ifdef FABLE_RETAIL_...` block, so landed includers keep their bytes;
the two new headers are only included by this chunk's row. src/0094c050.cpp was regraded after the last header change (gall.sh:
RELOCATION_MATCH), and the same TU's StayInShop COMDAT grades RELOCATION_MATCH against 0x0094BEF0.

## Copied
| header | from | note |
|---|---|---|
| engine/CAIStateGroupBase.h | chunk 2026 hand-back (= chunk 2017 copy) | HEAD + PERFORMACTION_NOWAIT, plus the new MOVETOPOSPROXIMITY block below |

## New headers
| header | content | evidence |
|---|---|---|
| engine/CAIStateGroup_SellToBuyer.h | `class CAIStateGroup_SellToBuyer : public CAIStateGroup_WorkBase<CAIStateGroupBase>`; public virtual Cleanup / IsRunnable / Update, private RefuseCustom / GetBuyersOrder, PBuyer (CIntelligentPointer<CThingCreatureBase>) +0x14 | RTTI .?AVCAIStateGroup_SellToBuyer@@ : CAIStateGroup_WorkBase<CAIStateGroupBase> (rtti_bases.tsv); vtable 0x0128BF84 slots 9/11/12; Ego_r.pdb + FableWin.pdb spellings (UAEXXZ / UAE_NXZ / AAEXXZ / AAEPBVCMessageEvent@@XZ); Ego_r TPI PBuyer +0x14 (sizeof 0x1C); retail reads +0x14 (0x0094C0E0, landed RefuseCustom 0x0094BF60) |
| engine/CCreatureInteraction.h | Participants (std::list<CThingCreatureBase*>) +0, Type +4; inline GetParticipants() const / GetType() const | Ego_r TPI (Participants, Type) with the 4-byte STLport list; Ego_r/FableWin ?GetParticipants@CCreatureInteraction@@QBEABV?$list@...@std@@XZ and ?GetType@..QBE?AW4ECreatureInteractionType@@XZ, no Fable.exe copy (inline); landed IsRunnable 0x0094C010 reads +0/+4 |

## Opt-ins added to copies of repo headers
| header | opt-in | change | evidence |
|---|---|---|---|
| engine/CAIStateGroupBase.h | FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY | decl `AIAction_MoveToPosProximity(EAIActionRepeat, C3DVector, float, bool&, EMovementType, bool, bool, bool, const CThing*, bool, float)` 0x0097E890 (protected) | Ego_r + FableWin ?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z; retail call 0x0094C363 (0x10-byte vector+float argument block, 8 pushes) |
| engine/CAIStateGroup_WorkBase.h | FABLE_RETAIL_CAISTATEGROUP_WORKBASE_STAYINSHOP | in-class body of `bool StayInShop() const` (GetTC<CTCShopKeeper>, GetShopTC, IsShopKeeperAvailable) | Ego_r ?StayInShop@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ donor 0x009702A0 directly before SellToBuyer::Cleanup 0x00970310 (retail 0x0094BEF0 before 0x0094BF50); the body compiled from this TU grades RELOCATION_MATCH against 0x0094BEF0; IsInWorkHours is a different body (0x0091A6F0, interface 0x23) |
| engine/CTCShopKeeper.h | FABLE_RETAIL_CTCSHOPKEEPER_CUSTOMER | decls GetMinDistFromStall() 0x00798D10, GetMaxDistFromCustomer() 0x00798D20, AddNewCustomer(CIntelligentPointer<CThingCreatureBase>) 0x00798D90 | Ego_r/FableWin QAEJXZ / QAEJXZ / QAEXV?$CIntelligentPointer@VCThingCreatureBase@@@@@Z; donor order Min 0x0079E7A0 < Max 0x0079E7B0 < GetHint 0x0079E7C0 = retail 0x00798D10 (`xor eax,eax`) < 0x00798D20 (`mov eax,0Ah`) < GetHint 0x00798D30; GetCustomer 0x0079E850 < AddNewCustomer 0x0079E860 = retail 0x00798D80 < 0x00798D90 |
| engine/CThingCreatureBase.h | FABLE_RETAIL_CTHINGCREATUREBASE_INTERACTION | inline GetPCreatureInteraction() (+ const twin) returning the +0x12C counted pointer; decl RemoveFromCreatureInteraction() 0x006629B0 | FableWin ?GetPCreatureInteraction@CThingCreatureBase@@QAEAAV?$CCountedPointer@VCCreatureInteraction@@@@XZ / QBE...; Ego_r/FableWin ?RemoveFromCreatureInteraction@CThingCreatureBase@@QAEXXZ (bound at 0x006629B0); retail 0x0094C067 reads +0x12C |
| engine/NCreatureAI.h | FABLE_RETAIL_NCREATUREAI_SPEAKING | decls `bool __fastcall IsSpeaking(const CThingCreatureBase&)` 0x008644E0, `void __fastcall StopSpeaking(CThingCreatureBase&)` 0x00864580 | Ego_r/FableWin ?IsSpeaking@NCreatureAI@@YI_NABVCThingCreatureBase@@@Z / ?StopSpeaking@NCreatureAI@@YIXAAVCThingCreatureBase@@@Z (landed Cleanup 0x0094BF50 tail-calls StopSpeaking) |
| engine/CTCPhysicsBase.h | FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_ACCESS | `Radius` (+0x34) in a `public:` section (access only, same layout) | byte evidence only (see below) -- **header debt / review at landing** |

### CTCPhysicsBase Radius access (review at landing)
Retail 0x0094C1F2 loads the creature's PhysicsTC pointer (`mov ebx,[eax+60h]`) BEFORE the two GetMinDistFromStall calls and reads the
radius AFTER them (`fadd dword ptr [ebx+34h]`, 0x0094C216): an lvalue member read. VC7.1 evaluates the inline by-value accessor
GetRadius() (Ego_r ?GetRadius@CTCPhysicsBase@@QBEMXZ) ahead of out-of-line calls in the same expression (probe: `EA()*EB() + p->GetR()`
loads `p->R` first, `q->GetP()->R` loads the pointer first and the float last), and splitting the product into a named local makes
VC7.1 keep the radius on the x87 stack (`fld`/`fcompp`/`fstp st(0)`) once the speech call's inline CIntelligentPointer constructors
follow. Only the member read gives retail's bytes. The PDB access attribute of Radius is not available on this machine (the retail
branch's `protected:` was hand-written in Codex pass 11), so this is one source (bytes); confirm against the Ego_r TPI LF_MEMBER access
when the PDB is at hand. Landing note: re-copy HEAD headers and re-apply the blocks (other chunks may change the same headers).
