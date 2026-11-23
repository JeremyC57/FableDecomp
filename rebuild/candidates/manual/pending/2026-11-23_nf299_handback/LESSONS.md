# NF299 lessons (2026-11-23)
- **Before deleting a table line, scan the exe for every caller of the target, not just the witness.** NF296 deleted the
  CTCUnholyPower::CancelAbilities line when it respelled the witness; two other landed rows still bound it (found here from xs_all.tsv +
  bk.py). The table keeps one witness per spelling.
- **A "STALE" filter hit is usually a table-only fix, but not always.** Seven witnesses' HEAD objects already bound the PDB name (delete the
  made-up line); for entry 37 the witness was fixed, yet three other callers still declared the stand-in.
- **Access wraps must reopen the section they were in.** `protected: X(); public:` is wrong inside a private section (0x0099B7D0 needed
  `protected: ... private:` so the following data member stays private); tools/acc.py prints the label in effect at a line.
- **Donor address-taken checks:** a thread proc or callback is never a call in the donor, so dcalls.py says NO; tools/drefs.py finds the
  address as an operand.
- **Anonymous-namespace hashes follow the TU path:** objsec of rows/<addr>.cpp v the repo copy shows UNMAPPED `?A0x` symbols; map them
  (proof2.py `?A0x=x`) rather than calling it a difference.
- recheck.py needs its output directory to exist (mkpay.py now creates it), and the payload leaf must follow a changed `// Leaf:` line.
