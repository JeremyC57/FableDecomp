# agent897 notes (chunk 897)

Result: 22 rows matched (src/ + test/), 1 EXTENT_WRONG (0x00F29FF0, differ/ draft), 3 large rows not attempted (parked for budget).
Grade every src/ file with `FABLE_HDR_OVERLAY=$A/hdr` exported (only SpanIncluding / SpanExcluding need it).

## Identities proven
- 0x00C0E2C0 / 0x00C0E310 CString::SpanIncluding / SpanExcluding: MFC strcore bodies (libpk CString), _MBCS imports
  _mbsspn (IAT 0x0143FFD8) / _mbscspn (IAT 0x0143FFF4), assert string "IsValidString(lpszCharSet)\n" 0x012B1F18.
- 0x00C28540 / 0x00C28560 CHandleBasedMemoryPool_012b4098::InternalSetResource / InternalGetResource (protected): Ego_r name set
  places the pair between Defragment 0x00C28400 and MoveArea 0x00C28580; arity matches ret 8 / ret 4. The catalog labels
  SetHandleAddress / GetHandleAddress are wrong. Class model copied from landed GetSizeFromHandle 0x00C283A0 (local model, see debt).
- 0x00DDE6F0 CPrisonWarden::OnPredicateFail: entity vtable 0x012DCC50 slot 5; body Game->RemoveAllQuestInfoElements()
  (CGameScriptInterfaceBase slot 339 = +0x54C; CGameScriptInterface vtable 0x01260F0C -> 0x00891C30).
- 0x00E8BDE0 CGhostGrannyNecklace::Init (0x012EBB14 slot 2, BrainState = 1), 0x00F00C60 COracleSpectator::Init (0x012F7274 slot 2,
  NextLine = 10), 0x00EE94C0 CExpression_FishScript::Init (0x012F54E4 slot 3, LastFishingLevel = 1 at retail +0x44 = Ego_r +0x58 - 0x14).
- 0x007921B0 CUnprovenRbTree_007921A0::end(): sibling of the landed begin 0x007921A0 (same placeholder class).
- 0x00976E20 CThing::GetTC<CTCJackPillar> (FableWin.pdb), TCI_JACK_PILLAR = 37; explicit instantiation of the shared header template.
- 0x00976E70: 5-byte `jmp 0x0062E900` (~CCombatActionBase) = implicit dtor of a CCombatActionBase-derived action; one of eight
  jump dtors in the Jack-of-Blades TU (0x00976E70/F20/FB0, 0x00977050/0F0/280/310/3B0). The other seven follow the Jack* classes;
  this one's class is not pinned by any reference, so it keeps the placeholder CUnprovenCombatAction_00976E70.
- 0x00BE42D0 CLandscapeTesselatorBase::AddPoly(long,long,long) (Ego_r, protected): three BridgeIndexBuffer push_backs.
- 0x00F29FF0 CCutsceneDef::Copy: vtable 0x012FB6E0 slot 19; 5-byte jmp to the 204-byte implicit operator= at 0x00F2A000.
  EXTENT_WRONG: split into 0x00F29FF0 (5) + new row 0x00F2A000 CCutsceneDef::operator= (204). differ/00f29ff0.cpp builds the 5 bytes.
- 0x0083AD90 NHeroStatModifierEvent::CBase::~CBase (RTTI on vtable 0x0127232C, FableWin public non-virtual dtor).
- 0x0083ADA0..0x0083AE20: STLport 4.6 map COMDATs of one std::map instance (operator*, end, iterator ctors x3, _S_value,
  _Select1st::operator()) plus 0x0083ADF0 operator== on a reverse_iterator of a one-pointer iterator. Key / data / iterator types are
  unproven (CUnprovenKey_0083ADA0 / CUnprovenData_0083ADA0 / CUnprovenIterator_0083ADF0): no caller references them; they sit in the
  TU of CHitThing's ctor 0x0083AE30. A later pass can rename them once the map member (hero-stat modifier tables?) is found.
- 0x005D7081 CTCInventoryStats::ConstructExperienceListPC: the console twin 0x005D6748 with PC component names, scroll-list and
  viewport observe/ignore pairs (UI_EVENT_MOUSE_MOVEMENT), viewport send-owned right-stick events, 303x53 sliders.

## Callee names used
- 0x0062E900 CCombatActionBase::~CCombatActionBase (Ego_r ??1CCombatActionBase@@UAE@XZ, landed binding).
- 0x00891C30 CGameScriptInterface::RemoveAllQuestInfoElements (via vtable slot 339).
- 0x00F2A000 CCutsceneDef::operator= (Ego_r ??4CCutsceneDef@@QAEAAV0@ABV0@@Z), 0x00431F60 / 0x006313A0 / 0x00432EE9 inside it.
- 005d7081 callees all via landed shared headers (NUISystem CComponent / CManager / CAction / CActionParamULONG / SLONG / CViewport).

## Header debt
- engine/CThing.h: the comment saying Fable.exe has no out-of-line GetTC copy is wrong (0x00976E20 is one).
- engine/CString.h: new opt-in FABLE_RETAIL_CSTRING_SPAN in hdr/ (README there); fold into the repo header at landing.
- CHandleBasedMemoryPool_012b4098: still a per-row local model (as the landed 0x00C283A0); a shared header would clean four+ rows.
- Landed 0x0041DC9D models vtable 0x0122F844 as "CActionParamEvent"; RTTI says CActionParamULONG (note only, not a NAME_FIX row).

## Parked rows: what they need next
- 0x005C9A95 CTCInventoryWeapons::ConstructScreenPC (slot 87, 3065 B) and 0x005C8459 ConstructScreen (slot 77, 3235 B): no landed
  twin; gen/ann_005c9a95.txt is the annotated listing. ~30 distinct callees: CreateComponent x14, CCountedPointer<CComponent> x14,
  CreateMenuEntryComponent, GetTextBySymbol x4, CItemPairVector_005bf03c::PushBack, GetSubDef<CWeaponDefPointer>, an unnamed
  0x005C8176 (x2), virtuals +0x1F0 x11 / +0xEC x8 / +0x244 x7. Name 0x005C8176 first; author slot 77 and slot 87 together
  (they are likely a console/PC pair like 0x005D6748 / 0x005D7081). Frame lea ebp,[esp-74h] => `#pragma optimize("s", on)` only.
- 0x005B8743 CTCInventoryClothing::ConstructScreenPC (3838 B, non-virtual QAE, called from 0x0041FBDD): gen/ann_005b8743.txt.
  Same TU flags as weapons; check whether a console clothing ConstructScreen is landed to use as the twin.
