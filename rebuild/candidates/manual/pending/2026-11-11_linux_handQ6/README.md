# Chunk Q6 (fast lane, Opus 5.5): 40 parked near misses that already compile to retail length

4 rows matched. After a coordinator-requested rework onto shared headers, 2 are clean and landed in landing ci:
- 0x00A66FD0 CMouse::ProcessMaintainedEventsFromEvent
- 0x00912840 CAIStateGroup_FlyingThingBase::AddRandomBezierCurveToPath

The other 2 still match but keep local models, so they are parked in differ/ with the exact gap on line 2:
- 0x00542780 CIconText::AddIcon: no shared inline bodies yet for the graphics-bank frame accessors.
- 0x007DDA30 CTCCreatureFlyInCircles::QueueSwoop: needs CCardinalSpline / CTCCreatureFlyInCircles headers and a vector traits decision.

The 36 other rows are same-length DIFFERs. Several are down to one reload, one store or the stack-slot layout (0x0087A7C0, 0x008DD7E0, 0x00911300, 0x00E95770).

The coordinator also fixed 0x00A66FD0 before landing. Its enum and setter names had Right and Middle swapped. Retail's SetAs*MouseButtonHeld bodies store type 5 (Left, 0x00A03D10), 8 (Middle, 0x00A03DC0) and 0xB (Right, 0x00A03E70), so Middle is 7-9 and Right is 10-12. The bytes were unchanged; only the bound names were wrong.
