# Chunk 704 (NA047) -- 10 of 10 byte-exact; four catalog labels wrong, two byte-twin pairs split into four symbols (2026-11-02)

All ten rows landed (`verify.log`: WINS 10, RELOCATION_MATCH + PASS; bindings multi-check 0; 11 probes in all; no asm, volatile or flags): ~CAtmosProcess
0x006B1550 (was "CCounter::operator="; traced from its ??_G 0x004A8380 and CCountedPointer<CAtmosProcess>::DeleteData 0x004A9B50 back to CWorld::PostInit's
new(0x20); PRainPlayer a pack(1) CIntelligentPointer at +0x0A), CTCStealth::OnCreate 0x00747530 (slot 4; ResetStealthVariables inlined from the same
TU), DirectShow strmbase ~CBaseVideoRenderer 0x00CA7310 (was "CTCStealth::OnCreate"; the DX9 SDK renbase.cpp empty dtor on the real AM_NOVTABLE
hierarchy -- five per-subobject vtable stores), the CTCKraken twins StartLowAttackBeam 0x007FC7A0 / StartHighAttackBeam 0x007FC7D0 (CKrakenDef +0x4C /
+0x50, modes 4 / 5; the private PDB member SetMode inlined from the same TU -- Ego_r 0x007F7B00 sits right before them, so this is retail's own form),
CMap::Update 0x00820B50 (protected virtual, slot 13), CCreatureAction_ActivateZTarget::PerformAction 0x008424A0, CCreatureAction_PlayAnimation::FrameUpdate
0x008426E0 (slot 56), and the CPhysicsLeaf<...>::FrameUpdate twins 0x00850640 / 0x0085CA10 (two template instantiations: vtables 0x01275974 / 0x01276AEC,
each source instantiates only its own). Decisions (coordinator): the same-TU SetMode form accepted (a real private PDB member); descriptive names for
the TLC-only CASoundManager / CAtmosMap virtual slots accepted (in neither PDB; a virtual name emits no symbol here).

23 labels applied (`ghidra_out/labels_chunk704_2026-10-27.tsv`; the eight vtable rows are binding fixes, not labelled); NAME_FIXES.tsv 31 rows + two
suggestions queued c704 for naming wave 32. PRIORITY: the five CBaseVideoRenderer vtables 0x012BF5D8 / 598 / 584 / 560 / 54C now carry both the real
??_7CBaseVideoRenderer spellings and the landed ctor 0x00CA7360's ??_7CVolumeDerived_00ca7360 stand-ins -- wave 32 re-authors 0x00CA7360 (and 0x00CA61D0's
CVolume* models) on the real strmbase hierarchy so each address has one name. Also: the CAtmosProcess ??_G / DeleteData, CCreatureActionBase::GetPlayer
(protected, returns CPlayer&) / FrameUpdate (protected virtual), CPlayer::AddMode (six wrong binders), ~CBaseIntelligentPointer 0x00A01C10 (~25 invented
binders), STLport _List_base<pair<C3DVector,float>>::clear 0x0056CAD0 (landed as ?Clear@List@@ with a dead Leaf), the private CTCStealth / protected CMap
callees, CTCStealth::OnSetInLimbo virtual, the CTCKraken private members (KillBeamEffect, KillTentacles, InitTentaclesOnly / AndHead, CreateTentacles,
EndAttackBeam) landed as free-function stand-ins. Suggestions (one source each, queued not labelled): 0x00747500 ResetStealthVariables, 0x00CA61D0
??0CBaseRenderer. Header debt (NOTES.md): CTCBase.h (flat branch), CCreatureActionBase.h, CCreatureAction_PlayAnimation.h, CIDrawMap.h, CBaseClass.h,
CPlayer.h flatten the vptr; no headers for CAtmosProcess / CMap / CTCStealth / CTCKraken / CKrakenDef / NCameraModes; Ego_r's CAtmosProcess (0x1C)
lacks TLC's CMyAtmosMap at +0x04 (retail 0x20).
