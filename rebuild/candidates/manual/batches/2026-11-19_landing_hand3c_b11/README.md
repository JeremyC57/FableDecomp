# Landing hand3c b11 (2026-11-19): 2 new rows + 1 re-port

vland --land, strict headers: RELOCATION_MATCH + PASS. 522 landed users of CThingCreatureBase.h / CTCCreatureNavigation.h rechecked: 521 good; the one
FAIL (0x00661F70 CThingCreatureBase::IsChild) was pre-existing -- a stale catalog PassPattern ('CThingCreatureBase_00661f70_TEST PASS' vs the test's
'00661f70_TEST PASS'), fixed in build_candidates.ps1 and rechecked good.
- 0x006CC800 CTCCreatureNavigation::FrameUpdate (new): FableWin 0x02479E80 statement for statement on shared headers; retail_asm via GFFloatToLongCeil
  (0x006CC903). Opt-ins: CTCCreatureNavigation.h FRAMEUPDATE (Agents typed vector<const CSteeringAgent*>, inline GetParentCreature, slot-9 override),
  CThingCreatureBase.h GETCURRENTSPEED (0x00661CD0 declaration). `// Inferred:` GetUniqueID() cast to __int64 so the stagger modulo binds the signed
  _allrem as retail / FableWin (same placement as the landed stagger rows).
- 0x009C38C0 CFileInstaller::CActiveFile::OnWriteFinished (new): plain C++; the parked "LTCG esi convention" is VC7.1's for a noinline __fastcall
  file-static (result in esi, `ret 4`). Closed by FableWin 0x0304C9B0's inline SetDirtyDiscError text `Get().ExitThread = Get().ExitThread || on;`.
- Re-port 0x00644FC5 NPlayerGui::CDrawQuestInfo::GetElementByIndex: const QBE spelling, `optimize("s", on)` only, ordinary definition; MATCH.
Queued: unify the landed 0x009C3530 SetDirtyDiscError text to FableWin's (and recheck its inliners); land GetMarkerFileName 0x009C33A0 (not catalogued).
