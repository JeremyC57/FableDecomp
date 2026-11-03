# Codex pass 11: 0x006286F0 GFPlayFullScreenXBoxMovie, on new movie and video headers (2026-11-03)

`verify.log`: RELOCATION_MATCH 1864/1864, test PASS. The working attempts are in `probe/006286f0.log`: 13 attempts, 6 matching, v13 selected because its locals are exactly Ego_r's list.

`HEADER_NEEDS.md` lists every callee, global and member offset in the body, the header that owns each one, and what was missing.

## Headers

- **New (10):** CASoundManager, CMovieSubtitles, CTextureRenderer, CInputManager, CInputEvent, CInputEventScanInfo, CRenderManager2D, CRenderManager, CUVBox, Win32Types. Each compiles standalone.
- **Opt-in retail branches added (13):**
  - C2DBoxF, CAFile, CBaseVideoRenderer, CDisplayManager, CPixelFormat, CRenderManagerCore, CSystemManager.
  - CTCBulletTime, CTexture, CVideoSys (full retail class, 0x8C), fable_game.h, fable_string.h (CWIDESTRING: non-explicit wide constructor; Replace).
  - fable_user_profile.h (CUserProfileManager, 0x140).
- Every changed header reads default=same (`probe/hdrcheck2.txt` stubs the system headers so fable_string.h is really compared).

## Includer rechecks

123/123 on the landed headers (coordinator, `includer_recheck_grades.tsv`: 21 MATCH + 102 RELOCATION_MATCH). The set is:
- every direct includer of the changed headers;
- every definer of a touched FABLE_RETAIL_ macro, including all CWIDESTRING users.

## Names

- CASoundManager is proven by RTTI and by Ego_r's typing of CMovieSubtitles::PSoundManager. The sound module has no method names in either PDB, so its slots and setters carry descriptive `<Name>_<addr>` names, per the documented rule.
- Enums without proven enumerators stay numeric.
- `NAME_FIXES.tsv` (31 rows) is queued PRIORITY (cx11m). Included:
  - the six PDB spellings that differ from current bindings (CSystemManager::Update, CDisplayManager::BeginRender, CRenderManager2D::Render2DDrawList, CTexture::CopyFromTexture, Stop, Init);
  - the `__imp__Enter/LeaveCriticalSection@4` import slots replacing `g_clearFn0/1`;
  - the sound setters at 0x009915E0 / 0x00991600 / 0x00991620, which are landed under the wrong classes.
- 0x009915C0 (CASoundManager slot 6) is landed as CCreatureActionBase::GetAnimStartFrame; its class is wrong, but no name is proven yet.
