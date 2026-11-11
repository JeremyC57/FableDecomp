# Chunk 2015 header overlay (`FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2015/hdr`)

Every change is an opt-in (`#ifdef FABLE_RETAIL_...`) or a new header that only this chunk's rows include; the default path of each
copied repo header is unchanged (copied from HEAD 2026-11-11; CTCCreatureOpinionOfHero.h copied from the acctB_c2004 hand-back overlay,
which already carries that chunk's HEADPENDINGDEED opt-in). Evidence is two sources each (PDB name set / layout + retail bytes).

## New headers
| header | content | evidence |
|---|---|---|
| engine/CThingFilter_HasScriptedControl.h | 1-byte filter `bool CheckForActive` (+0), inline ctor(bool), operator() decl (0x006A8560), unary_function<CThing,bool> base (unary_negate needs argument_type) | Ego_r layout; Ego_r/FableWin ??0..@@QAE@_N@Z, ??R..@@QBE_NABVCThing@@@Z, ??R?$unary_negate@VCThingFilter_HasScriptedControl@@@std@@; retail store of 1 into the filter byte 0x0097FF63 and `cmp byte ptr [edi],0` in 0x006A8560 |
| engine/CMessageEventDef.h | CMessageEventDef : CParentDefClassBase, MaxSeenRadius +0x3C, MaxHeardRadius +0x40, Lifespan +0x44, ReactionSpeechType +0x48, Type +0x4C | Ego_r layout (names, Ego_r +0x40..+0x50); retail CParentDefClassBase 0x39 bytes (landed ctor 0x004556B8); AddMessage 0x00687730 reads +0x3C/+0x40 (fild) and copies +0x44; FindAGuard scales +0x44 |
| engine/NEntityEvents_CEventExpressionPerformedTo.h | NEntityEvents::CEventBase (slots 0..4, WorldFrame +4; guarded so it can sit beside the hit-event header only if that one is not included) and CEventExpressionPerformedTo (PerformedBy +8, PExpressionDef +0x10, ...), inline GetPExpressionDef / StatGetType (3) | Ego_r layout (32 bytes); FableWin ?GetPExpressionDef@..QBE?AV?$CDefPointer@$$CBVCExpressionDef@@@@XZ, ?StatGetType@..SI?AW4EEventType@2@XZ; retail `cmp eax,3` after the slot-1 GetType and the add-ref copy of +0x10 (0x009801BA / 0x009801C6) |
| engine/CThingFilter_IsTargetable.h | 2-byte filter ZTargeting / UsingLightning, inline ctor(bool,bool), operator() decl (0x006A84F0) | Ego_r layout; FableWin ??0CThingFilter_IsTargetable@@QAE@_N0@Z, ??RCThingFilter_IsTargetable@@QBE_NABVCThing@@@Z; retail stores 0 / 1 at 0x007A0F5A / 0x007A0F5F; the landed search body 0x007A3CA0 calls 0x006A84F0 |
| engine/CThingCompare_NearestToPos.h | 12-byte comparator (C3DVector Pos), inline ctor(const C3DVector&), template operator() decl | Ego_r layout; FableWin ??0CThingCompare_NearestToPos@@QAE@ABVC3DVector@@@Z, ??$?RVCThing@@@CThingCompare_NearestToPos@@QBE_NPAVCThing@@0@Z (landed 0x007A2F90); retail builds it in std::sort's argument slot 0x007A0FBF |
| engine/CFilter_Bind4.h | CFilter_Bind4<F1..F4> (four const references) + __fastcall GFFilter_Bind4 factory | Ego_r/FableWin ??0?$CFilter_Bind4@..@@QAE@ABV..@Z, ??$GFFilter_Bind4@..@@YI?AV?$CFilter_Bind4@..; the CFilter_Bind2 / Bind3 family; retail stores the four filter addresses 0x007A0F75..0x007A0F92 |

## Opt-ins added to copies of repo headers
| header | opt-in | change |
|---|---|---|
| engine/CAIStateGroupBase.h | FABLE_RETAIL_CAISTATEGROUPBASE_FINDAGUARD (inside MEMBERS) | protected `AIAction_FindAGuardAndYellForHelp(C3DVector, unsigned long, bool)` decl (Ego_r / FableWin ?AIAction_FindAGuardAndYellForHelp@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@VC3DVector@@K_N@Z) + `class C3DVector;` forward. |
| engine/CTCVillage.h | FABLE_RETAIL_CTCVILLAGE_YELLFORHELP | RemoveApologiseableCrimes(long) decl (Ego_r; landed 0x0065CE56), inline IsHeroEnemyBecauseOfCrime() const / SetFrameBribeEnds(long) (FableWin; retail reads +0x7C / stores +0xA8 in place). |
| engine/CTCEntityEvents.h | FABLE_RETAIL_CTCENTITYEVENTS_EVENTS (retail branch) | inline GetEvents() const (FableWin/Ego_r ?GetEvents@..QBEABV?$list@..@XZ; retail walks +0x0C in place), RemoveEvent(NEntityEvents::CEventBase&) decl (0x008112D0; both PDBs). |
| engine/CThingCreatureBase.h | FABLE_RETAIL_CTHINGCREATUREBASE_ATTACKEDBY | ClearAttackedByCreature() decl (Ego_r 0x005454B0 / FableWin ?ClearAttackedByCreature@CThingCreatureBase@@QAEXXZ; retail 0x006691C0 zeroes PLastAttackedByCreature +0xF4) and the inline GetPLastAttackedByCreature() const (FableWin) unless FABLE_RETAIL_CTCTARGETINGAI_ATTACK already supplies it. |
| engine/CTCCreatureOpinionOfHero.h | FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_REPORTCRIMES | ReportAllCrimes(CTCCreatureOpinionOfHero*) decl (Ego_r ?ReportAllCrimes@CTCCreatureOpinionOfHero@@QAEXPAV1@@Z, retail 0x00684210) and the inline StatGetInterfaceType (0x4F) when no other opt-in declares it. |
| engine/CMessageEventManager.h | FABLE_RETAIL_CMESSAGEEVENTMANAGER_ADDDEFMESSAGE | the def overload AddMessage(const CMessageEventDef&, const C3DVector&, CThing*, CThing*, long) decl (Ego_r / FableWin ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@ABVCMessageEventDef@@ABVC3DVector@@PAVCThing@@2J@Z; landed 0x00687730). |
| engine/NCreatureMode.h | FABLE_RETAIL_NCREATUREMODE_LIGHTNINGABLE | IsLightningable(const CThing&) __fastcall decl (Ego_r ?IsLightningable@NCreatureMode@@YI_NABVCThing@@@Z, landed 0x0085FAA0). |
| engine/CTCWillLightning.h | FABLE_RETAIL_CTCWILLLIGHTNING_CLASS | the polymorphic retail class `CTCWillLightning : CTCBase` with typed members at the generated retail offsets (CDefPointer m_lspDef +0x28, list m_lbBeams +0x68, CIntelligentPointer m_cbpPlayer +0x70 / PTargetOverride +0x88 / PItemRemoved +0x11, ...), the nested CLightningBeam (Ego_r member names at retail offsets, 0x7C bytes; ctor 0x007A0630) and the private GetTargets decl. The flattened default branch is unchanged. |

Landing note: re-copy HEAD first and re-apply the blocks (other chunks may have changed the same headers since 2026-11-11).
