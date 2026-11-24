# NF221 lessons (one line each)
- Census the stand-in callers too: the Reset / ShareData / operator= stand-ins each have their own landed callers (0x00419463 <- 0x00499AA0, 0x0041947A <- the two AcquireFont rows), so a family that looks like 40 rows closes only after callers.py runs on every self address whose leaf changes, stand-ins included.
- Diff the full relocation list (target, symbol) of each changed row against HEAD (tools/sc.sh), not only the family targets: a port respells callees whose return types name the stand-in (0x009E2C80 GetFont, 0x009F83D0 OpenGraphicBank) and DeleteData addresses stored by Reset.
- RELEASEDATA_DECL_ONLY can DIFFER where HEAD already emitted a visible ReleaseData copy (0x005C8459 / 0x005C9A95): RELEASEDATA_NOINLINE keeps the body visible to c2 and binds the same symbol.
- `x.ReleaseData(); x.Data = 0;` reproduces retail's "call release; mov [x],0" sites on the shared header (0x004175E5) without a stand-in Clear().
- Porting a Reset stand-in onto the header emits a pick-any DeleteData<T> copy (incomplete T: plain delete) where HEAD declared it only; precedent 0x0042FD04, but it is a differing pick-any body: note it.
- A helper script that loops over rows must not lose its globals in `nohup bash -c '...'` (unexported $A): write the loop as its own script.
