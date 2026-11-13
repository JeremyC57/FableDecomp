## 0x00D17A80

- engine/CQ_BountyHuntScript.h: FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_BOASTS declares WatchForBoasts 0x00D17A80 and WatchForQuestInfoDisplayConditions 0x00D17DA0 (full copy of the current repo header + that one opt-in; default path unchanged). The only overlay header the matched row needs.
- (attempt 4) the earlier experimental copies (stlport/fable_containers.h FORCEINLINE/DEPTH0/REAL_DEALLOC probes, engine/CSpawnedFunc.h PLAIN_INLINE_CTOR) are NOT needed and were moved out of the overlay to a4/hdr_experimental/.

