# C2DLineF::GetPointOnLine

The VC7.1 object exports `?GetPointOnLine@C2DLineF@@QBEMABVC2DVector@@AAV2@MPAV2@PA_N@Z`. The source-order record in Ego_d identifies `C2DLineF::GetPointOnLine`. The submitted extent is 654 bytes.

- egor donor procedure at 00baf040
- fablewin donor procedure at 0324cea0

- call at 0x00A5747F in landed 0x00A57430 C2DLineF::GetDist
- call at 0x00A57497 in landed 0x00A57430 C2DLineF::GetDist
- call at 0x00A574B2 in landed 0x00A57430 C2DLineF::GetDist
- call at 0x00A574D2 in landed 0x00A57430 C2DLineF::GetDist
- call at 0x00A57744 in landed 0x00A57730 C2DLineF::GetDist
- call at 0x00A57784 in landed 0x00A57770 C2DLineF::GetSquaredDist

From 00A57210 the optional output tests and temporary x87 values differ. The public const leaf and optional output pointee types agree with both donors.

Checks both optional output arguments, endpoint and interior projections, degenerate lines and the clipping radius.
