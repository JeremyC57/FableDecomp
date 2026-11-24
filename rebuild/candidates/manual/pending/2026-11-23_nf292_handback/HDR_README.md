# NF292 overlay headers (hdr/; bases in orig/hdr/ = repo HEAD copies, 2026-11-23)

All four are add-only declaration opt-ins in the flattened (`#else`, pack(1)) branch, under ruling NF291 Q4 (a GENERATED branch may take
an add-only declaration opt-in; gen_class_headers.py keeps any header with a `FABLE_RETAIL_` mark). No existing line is edited except the
one assert pin below; no layout change (non-virtual member function declarations only).

| header | new macro | declares (PDB, Ego_r + FableWin) | user |
|---|---|---|---|
| engine/CWorldMap.h | FABLE_RETAIL_CWORLDMAP_FLAT_ISLOADINGANYMAP | `bool IsLoadingAnyMap() const;` 0x004FB4A0 | 0x006E11F0 |
| engine/CMemoryDataOutputStream.h | FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM_FLAT_GETLENGTH_PEEKDATA | `unsigned long GetLength() const;` 0x00994B40, `const void* PeekData() const;` 0x00995E70 | 0x00A99840, 0x00A9BA70 |
| engine/CTCBuyableHouse.h | FABLE_RETAIL_CTCBUYABLEHOUSE_FLAT_SETRENTED_SETOWNED | `void SetRented(bool);` 0x006C1150, `void SetOwnedByPlayer(bool);` 0x006C17B0 | 0x00895ED0 |
| engine/CTCDoor.h | FABLE_RETAIL_CTCDOOR_FLAT_UPDATEOPENSTATUSTRIGGERTYPESCRIPT | `protected: void UpdateOpenStatusTriggerTypeScript(); public:` 0x007310C0 (IAE) | 0x007311F0 |

- Placement: CWorldMap.h after the two existing flat-branch opt-ins (before `__vftable`); the other three after the last data member,
  before the closing brace. Each mirrors the retail branch's existing declaration (MOVEPLAYERPOS / RECEIVEDCONFIRMATION / OPENCLOSE).
- Assert pin (CMemoryDataOutputStream.h): the flat branch's only assert was unpinned (`FABLE_STATIC_ASSERT(sizeof == 0x18)` on line 58).
  It is now `FABLE_STATIC_ASSERT_AT(58, ...)`, proven alone first (pin-only tree, its 1,418 includer/relative rows: 0 differ), then the
  opt-in added above it. The retail branch's assert (line 33) is above every change and does not move. Every assert in the other three
  headers was already `_AT`. No `__LINE__` user in any of the four.
- equiv_check (evidence/equiv_o1.txt): full overlay tree v repo, the four headers' transitive includers plus every relative-source row,
  1,693 rows: 0 differ. Planted control (a member typedef just above each new `#ifdef`, outside it): 80 rows differ
  (evidence/equiv_o1_control.tsv; it includes the five binders and three of their tests).
- Other agents' overlays: NF290's hdr/ was empty when checked (2026-11-23).
