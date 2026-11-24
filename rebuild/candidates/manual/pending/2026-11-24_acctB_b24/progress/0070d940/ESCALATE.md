# 0x0070D940 -- CANDIDATE inline-decision / compile-order residue (B24C6, NOT proven: no c2 inline trace run)
- After the port (differ/0070d940.cpp = c6/v2.cpp, shared headers) the call SEQUENCE equals retail's except at four sites, all after the
  transition loop: ours expands the main transition's SetStartState / SetEndState (retail calls 0x00710560 / 0x007105A0 at 0x0070DDBA /
  0x0070DDD7), the transitions ~vector (retail calls 0x00711760 at 0x0070DD14), and the FIRST combination pass's push_back overflow (retail
  calls 0x00711080 at 0x0070DF89, but expands the second pass's). Before the loop both rows call SetStartState / SetEndState /
  GetProgressFromAlpha / push_back alike; inside the loop both expand them.
- The non-monotone pattern (first combination pass called, second expanded) is the shape of 0x00E93EB0's two-path fact (b24a/00e93eb0/
  ESCALATE.md): callees compiled AFTER the caller in retail's object go down c2's budget path. Retail addresses: 0x00710560, 0x007105A0,
  0x00711080, 0x00711760 all lie after 0x0070D940 in the same object; our one-row TU compiles them first.
- To prove it (next agent): the 00e93eb0 inl trace (budget totals per decision) on c6/v2.cpp, and the same with those callees forced
  down the budget path. Until then this is a hypothesis; the FableWin /Od text differences in NOTES.md (CIVCountedPointer ctors,
  C3DAnimationStateBase ctor/dtor) are the competing explanation (bigger inline bodies eat the budget earlier).
