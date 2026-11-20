# NF173 header overlay

## engine/CNavigatorManager.h
- Base: repo working tree copy (orig/engine/CNavigatorManager.h, byte-equal to rebuild/include at the time of copying).
- Added: nested opt-in `FABLE_RETAIL_CNAVIGATORMANAGER_ADDNAVIGATIONMAP_PROCESSREF` inside the existing `FABLE_RETAIL_CNAVIGATORMANAGER_LEVELMAPS`
  block. With it, AddNavigationMap (0x00A157C0) takes `CASuspendableProcess&`, the Ego_r.pdb 0x00B82E70 / FableWin.pdb 0x031C5C10 spelling
  `?AddNavigationMap@CNavigatorManager@@QAEXJABVC2DBoxI@@ABVCCharString@@AAVCAFile@@AAVCASuspendableProcess@@@Z`. Without it the `#else`
  branch is token-identical to the base (the old pointer declaration).
- Used by: rows/00500230.cpp (q4785). The only landed user of LEVELMAPS is 0x00500230 itself. Once that lands, the pointer form under LEVELMAPS
  is used by nothing. The coordinator may fold the opt-in into LEVELMAPS (a header claim).
- pin_asserts.py: pinned 0 (the block holds no FABLE_STATIC_ASSERT).
- Header users: the landed includers of engine/CNavigatorManager.h (grep -rlE 'engine/CNavigatorManager.h' rebuild/src/compiled) do not define
  the new macro, so the opt-in changes none of them.
- Sibling overlays: NF171 / NF172 overlay no CNavigatorManager.h.
