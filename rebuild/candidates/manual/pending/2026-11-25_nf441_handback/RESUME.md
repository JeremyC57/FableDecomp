# NF441 resume note (libpk / CODA placeholder naming)
State (2026-11-25): research done, no batch landed yet.
- work/rvt.json: retail vftables (0x012B1000..0x012C0000) stored by code, slots, store sites (tools/vtscan.py).
- work/chain.json / chain.txt: dtor chain per VA (tools/chain.py). work/part.tsv: GameCODA (CodaFree) partition (tools/part.py).
- work/sdk_rel.json (CODAApi.lib ??_7 slots + function relocs), work/cv_coda.json (CODAApiDbg CodeView classes: size, bases, members, virtuals; tools/cvtypes.py), work/sdk_chains.txt.
- work/coda_struct.txt: every GameCODA VA with slots, shared-slot links. work/cntmatch.txt: exact-instruction-count slot matches vs SDK classes (strongest tie; same-version bodies have equal counts).
- CLASS_TABLE.tsv: identified classes with evidence (render family: 12 VAs).
- KEY FINDING: rebuild/include/engine already has ~65 GameCODA headers (CSalRender.h, CSalSw3DVoice.h, CCodaSource.h 0x012B5A88, CCodaEAX.h 0x012B87F4, CSalOutputKS.h 0x012B8FC8/9148/9160, ...). Renaming a local CVtable_ model to a headered class = SHARED_HEADER_AVAILABLE (lander rejects). Route = port rows onto headers (_hdr batch first), per advisor.
- Locks: none held (the 60 render rows in work/r_rows.txt were locked and then released).
- Open-address list: work/open_status.tsv (tracker rows of the 1,000 libpk_hold); work/coda_open.tsv (208 tied to GameCODA VAs).
- libpk (non-CODA, PKC*) classes: names only from retail .\PKC*.cpp / libpk\*.h strings; untouched.
