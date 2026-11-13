# Codex passes 11-13 and 15-21 released to the normal parked pool (2026-11-13, owner)

The owner has handed back the Codex work, so the ten `RESERVED.tsv` lists are renamed `RELEASED.tsv`. Of their 558 rows, 430 have
landed; the 128 still unlanded are ordinary parked rows from now on. Four of them have no source on file (decode only so far):
0x007C0690, 0x007BFE30, 0x009FDE80, 0x0058F9BC.

For the other 124, every source version on file (308 in all: the Codex returns and the in-house parked drafts) is being regraded
against today's headers; `STATUS.tsv` and `differ/<addr>.cpp` (the closest version of each row) are added when that finishes.
