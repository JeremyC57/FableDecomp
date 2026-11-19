# Landing hand4a b2 (2026-11-19): CTCInGameMenu::PCSetQuitToFrontEnd (0x0047D296) and PCSetToLoad (0x0047DD9D) -- Inferred brackets, cross-check

vland --land, strict headers: RELOCATION_MATCH + PASS, no warnings. Ported from local models to shared headers (opt-ins of landed
VideoOptionsChanged 0x0047E651); Ego_d locals pos + one pclickable reused for both buttons (FableWin 0x01AB8D21); PCSetToLoad parameter `index`.
New opt-in FABLE_RETAIL_CTCINGAMEMENU_PCSETQUIT (declaration). Label 0x0047D289 PCQuitToFrontEnd (`LABELS.tsv`; Ego_r 0x00489DCC, FableWin thunk).

The cross-check the advisor asked for on SetToSave's bracket (batch hand4a_b1): both rows match ONLY with SetToSave's centring spelling, both
halves byte-required here (without X: ndiff 4; without Y: ndiff 2). The bracket is therefore reproduced in six same-TU rows (SetToSave, these
two, the X half in landed 0x0047E651 / 0x0047E9AE / 0x0047F30F); it is still unsourced (FableWin divides by a double 2.0). Both rows carry an
`// Inferred:` note; the landed SetToSave note is upgraded to "cross-checked" (comment only, re-verified RELOCATION_MATCH + PASS). Owner review
together with SetToSave.
