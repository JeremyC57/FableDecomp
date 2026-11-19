# Landing hand3c b7 (2026-11-19): 4 rows

vland --land, strict headers: MATCH / RELOCATION_MATCH + PASS for all four. Header changes are additive (`*.diff`).
- 0x0052EE00 NUISystem::CComponent::GetTargetRenderPosition: MATCH after 2 grades -- FableWin 0x0229BB60 order on shared headers (old park: local C2DVector model).
- 0x007F8C20 CTCAnimatingObject::AddQueuedAnimation: FableWin 0x02674140 shape with landed sibling 0x007F8F30's opt-ins + CANIMATIONSET_ANIMEXISTS.
  NEW engine/CAnimatingObjectDef.h (CSubDefClassBase, StatGetTypeName FableWin 0x02676280, Animation +0x28 per Ego_r and retail `add eax,28h`);
  CTCAnimatingObject.h ADDQUEUEDANIMATION declaration.
- 0x006B2960 CGameNavigatorManagerExternalInterface::GetNavigationInformation: retail_asm through the shared GFFloatToLongFloor / Near helpers
  (FableWin calls them by name); no row asm. `// Inferred:` CCharString("NULL") as a named local in an inner block (a temporary gives 249v250,
  a function-scope local 250v250 with frame 0x34). Header opt-in CNAVIGATORMANAGEREXTERNALINTERFACE_GETNAVIGATIONINFORMATION: Ego_r field list
  0x32FC4 base slot 1 is the pure GetNavigationInformation(const C2DBoxF&, CNavigationAreaUpdate&) const (retail vtable 0x01238C58 slot 1 =
  0x006B2960); the default path's slot-1 `Initialise()` is a placeholder name (NAME_FIXES). OWNER REVIEW: the inner-block local is Inferred.
- 0x00B905A0 CEnginePrimitiveManagerOcclusionBase::PrepareOcclusionTest: FableWin 0x02ED79B0 shape (Ego_d names) on landed 0x00BB3E80's header set.
After merging both batches' headers, every landed row including NCameraModes_CBase.h / CGameCameraManager.h / CTCAnimatingObject.h /
CGameNavigatorManagerExternalInterface.h was rechecked (`logs/recheck_header_users.txt`).
