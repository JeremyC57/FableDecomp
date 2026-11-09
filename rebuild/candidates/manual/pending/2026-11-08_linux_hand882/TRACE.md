# TRACE -- 0x0061E6D0 CTCInventoryQuests::ConstructQuestListPC, playbook session (2026-11-09)

Tools: tools/ (copies of chunk 883's session tools, adapted: cmpx.py shares / call segments, bcalls.py <obj> call-list diff v
retail c.txt, c2/nl.sh tuple count, inl/inl.sh inline-decision trace with g/pen/f/sc per site, lb.sh one-at-a-time grader).

## Step 1: regime
- tools/c2/nl.sh: base (= differ/0061e6d0.cpp, six inline_depth(0) regions) **5027 IL tuples** -> regime 0 (block weight
  1 << loop depth, regime-0 inline score).
- Baseline grade DIFFER(12357v12356): exact 68.93%, regs 72.11%, regs+frame 97.03%, frame 0x424 v 0x41C, 509 v 507 call
  segments (the ConvertVector region outlines `CDefPointer::operator->` and the C2DVector copy ctor), 133 differing segments.

## Step 2: the inline decisions (tools/inl, regime-0 score sc = c1'*(1/f + 2)/3 for f > 1, c1' = c1 + pen,
## pen ~ sz*(a+g)/18256 once a+g > 9000, expanded iff sc < 74)
- nod = base without the six devices (12676v12356, 507 calls but six late sites expanded). The inline trace maps the six device
  sites (nod numbering): delete_action CCountedPointer<CAction> #487 (f 46.9, g 7931), take_boast_entry
  CCountedPointer<CComponent> #507 (f 23.4, g 8173), teleport `new CAction` CAction() #512 (f 11.7) + its CCountedPointer
  (f 23.4), zoom entries #548 / #553 (f 93.8 / 46.9), ConvertVector #2 #562 (f 375, g 8775). All have pen 0 (a+g < 9000):
  retail's caller size g is higher at those sites.
- **Lever 1 (natural, retail STLport 4.6 shapes, proven in chunk 883):** FABLE_RETAIL_STLP_LIST_CREATE_CONSTRUCT +
  FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND (hdr/stlport/fable_containers.h). s1 = nod + both: g +360 at the delete site;
  ConvertVector #2 g 9175, pen 40, sc 74 -> **called, as retail, with no device**; zoom entries sc 73 (one short).
  s2 = base + opt-ins - ConvertVector device: ConvertVector expanded again (the five device sites no longer add their bodies to g).
- **Budget diagnostic (x2 / x3 = s1 + 2 / 3 dummy `CCountedPointer<CAction> p(new CAction);` before the per-quest loop):**
  take_boast_entry, teleport CCountedPointer, both zoom entries and ConvertVector called; delete's CCountedPointer #588 at
  g 9042 pen 39 sc 73 (needs g > ~9110, about one more dummy); **teleport's CAction() #615 at f 11.7, g 9322, pen 33,
  c1' 94, sc 65 stays expanded**.
- **Infeasibility (measured numbers):** teleport's CAction() is called in retail (call list 313-315: new, ctor:CAction 0x0042BE50,
  ctor:CCountedPointer). At f 11.7 that needs c1' >= 107 -> pen >= 46 -> a+g >= ~12900, while pop_action's
  CCountedPointer (expanded in retail, f 46.9, same block, ~1000 g earlier) needs pen < 40 -> a+g < ~9300. No budget
  consumer satisfies both: retail's teleport `new CAction` site is >= 3.9x colder (f <= ~2.97 at a+g ~9600) or costs >= 13
  more c1 (excluded: chunk 883's f 0.5 CAction() site is expanded in retail, so c1 < 64.75). The source shape around the
  teleport action (its block frequency) is what differs from retail, not only the budget.

## Step 3: the frequency rule resolves it (playbook: code after a `for` stays at the loop's 5x; a `while` drops back)
- The take block sits after the boast loop `for (boast = begin; boast != end; ++boast)`, so c2 runs it at 375/8 = 46.9 instead
  of 75/8 = 9.4. b1 = s1 with `boast = begin(); while (boast != end()) { ...; ++boast; }`: **code-identical** (tools/odiff.py
  s1 v b1, frame masked: only the late sites differ), take block f 9.38, teleport CAction() f 2.34; zoom entries (sc 74 / 76)
  and ConvertVector (sc 75) called with no device.
- New inequality (b1 numbers): with pen 0 below a+g 9000, a CCountedPointer site is called only once a+g > 9000. Retail expands
  pop_action's CCountedPointer (b1 #458, g 7596) and calls delete_action's (#519, g 8258): **Delta g in (723, 1385]** before
  pop_action. Every other late site is then consistent (teleport CAction() needs pen >= 30.5 at f 2.34: satisfied above 9000).
- **Diagnostic b1x3** (b1 + 3 dummy `CCountedPointer<CAction> p(new CAction);` before the per-quest loop, ~+780 g): the call
  list equals retail's except the dummies' own calls -- all six late sites out of line, no inline_depth device. Lever proven.
- b2 (b1 + the three take-block devices only): 12519v12356 but regs+frame 98.05% (base 97.03%): the ConvertVector region's two
  extra calls (CDefPointer::operator->, C2DVector copy ctor) cost about 1% of register share. b4 (= base + while boast + both
  opt-ins, six devices) is byte-identical to base and is the new differ source (one step nearer the device-free form).
