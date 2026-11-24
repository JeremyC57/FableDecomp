# Landing nf380 b2: two selves named after the wrong function

- 0x00A629D0 `??1CPackedUIntArray@@QAE@XZ` to `?Clear@CPackedUIntArray@@QAEXXZ`: FableWin 009cd740 calls Clear and lands here; Ego_r order Clear < ~CPackedUIntArray as in retail; its identical twin 0x00A62AC0 is the destructor.
- 0x00E0F1D0 `?EndMission@...` to `?WatchForTermination@CQ_WaspBossScript@NScript@@QAEXXZ`: FableWin CQ_WaspBossScript::Main calls it; EndMission is its own body at 0x00E13310.
- Tests respelled the same way. Link of every row naming old or new spelling: duplicates 4 to 2, both new names resolve. Catalog labels and file names queued (NAME_FIXES_QUEUE nf380).
