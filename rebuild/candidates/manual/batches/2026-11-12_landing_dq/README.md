# 2026-11-12 landing dq -- naming agent NF27: queue dn and FAM10's name fixes, 57 landed rows changed, 37 renamed

Source: `naming/2026-11-12_agentNF27/` (STATUS, APPLIED, REJECTED, RENAMES, NOTES, LESSONS, QUEUE, FAM10_FIXES, hdr README).

**Queue dn (4/4 done)**
- 0x0063F150 / 0x0063F1E0 are the out-of-line `operator()<CThingPhysical>` of CFilter_Bind2 / CFilter_Bind3 (HasNavigationInformation,
  OverlappingBoundingBox). Both have new tests.
- 0x00A12890 is CPhysicsMeshObject::IsPointContainedInBoundingBox, and its callee 0x00A11F20 IsSpherePartContainedInBoundingBox is
  respelled.
- The landing-dn RELOC_CONFLICT is resolved:
  - CCountedPointer<CModeSitBase>: ctor 0x0094B3A0, dtor 0x008AB320, DeleteData 0x00938060, Reset 0x00938070.
  - CCountedPointer<CModeSitFloor>: ctor 0x008AB380, dtor 0x008AB3D0, DeleteData 0x008AA810.
  - Seven self rows are renamed. 0x0093B1D0 uses the converting ctor, and 0x008A8C60 / 0x0090F830 are respelled.

**FAM10's fixes (22 done, 2 partial)**
- 19 CCountedPointer<T>::Reset rows renamed with their binders: CShape, CCRCSymbolMap, C3DAnimationManager, the mesh optimiser /
  stripifier, the manager singletons, CSurface, CParticleSimulation, NParticleEngine::CPSCBase and more. 0x00A002E0 returns void, per
  both PDBs.
- CPlayer::FreezeControls / SpecialAbilityFreezeControls (0x00487140 / 0x00487170).
- CPlayerManager::FreezePlayerControls / UnfreezePlayerControls (0x00449A30 / 0x00449A50).
- CUserProfileManager::Get (0x0040D2A0) and CTCShapeManager::GetInstantiationFromEnum (0x007624E0).
- RemoveMode / AddMode stand-in binders respelled.
- 0x004516E7 is the std::vector<float> fill ctor; its binder 0x00453210 is left.
- Extra: 0x0050EEF0 is CThingFilter_HasNavigationInformation::operator(), and binder 0x008266E0 is respelled.
- Partial: binder 0x00B3B240 of 0x00B3E4A0 is left (other stand-in callees).

**Coordinator fix at landing:** NF27's RENAMES section 1 put `Class::Method` in the name column. The first apply wrote catalog names
like `CPlayer::CPlayer::FreezeControls`. The names were stripped to the method and re-applied before the recheck
(`RENAMES_applied.tsv` is the corrected file).

**Headers** (opt-ins only, macros new): CPhysicsMeshObject.h BOUNDINGBOXCONTAINS (rebased on Q23's NAVIGATION block; taken whole as a
superset of the repo copy), CPlayer.h INITIALISEIFUNINITIALISED, CPlayerManager.h FREEZEPLAYERCONTROLS, CTCShapeManager.h
GETINSTANTIATIONFROMENUM, fable_user_profile.h CUSERPROFILEMANAGER_PCLASS, candidates/mouse_dx_accessors_compiled.h CMOUSEDX_MOUSECURSOR.
Behaviour tests of eight rows carry their own small model classes, as the test template asks; no row source adds a local model of a
header class.

**Verification:** all 57 changed rows recheck 3 MATCH and 54 RELOCATION_MATCH, every test PASS (`logs/recheck.log`).
naming/rebind.py: removed 161, added 152. bindings_multi_check stays at its baseline, and funclet_includes_check finds 1317 checked,
0 broken.
**Left (NOTES):** CPlayer::SetupInitialPlayerMode / InitModes self names, six more CUserProfileManager::Get binders, the 0x013CAA28 /
0x013B7D4C global stand-ins, and the CCountedPointer.h comment at ~line 124. Naming agent FAM11 took these.
