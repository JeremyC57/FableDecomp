# hand4b header overlay (copies of the CURRENT repo headers + new opt-ins only)

Every file is a copy of the repo header as of 2026-11-19 with one NEW opt-in added; no default path changes (an includer that does not
define the new macro preprocesses to the same text as the repo header).

| file | new opt-in | used by | what it adds |
|---|---|---|---|
| engine/C3DAnimationStats.h | `FABLE_RETAIL_C3DANIMATIONSTATS_TIMINGEVENTS` (nested in `..._MOVEMENTVECTOR`) | 0x00A50C20 | `TimingEvents` typed as `std::multimap<CCharString,float>` (Ego_r / Ego_d TTimingEventMap, +0x20, the 2 pad bytes are its alignment) and the stats members `protected` (Inferred: access only; Ego_d has them private with a generated operator=, but retail / FableWin's InitialiseAnimationInstance copies them one by one from the derived class: LoopFlag, Duration, MovementFlag, MovementVector, TimingEvents via tree operator= 0x00A54A90, AnimType; no CBaseObject::operator= call). Includes fable_string.h + stlport/fable_map.h under the macro. Layout unchanged (size assert, now line 63). |
| engine/C3DAnimation2.h | `FABLE_RETAIL_C3DANIMATION2_INSTANCELIST` | 0x00A50C20 | `InstanceList` typed as `std::list<CIntelligentPointer<C3DAnimation2> >` (Ego_r; retail STLport list, 4 bytes at +0x78; 0x00A50CB1 links a node built from `this`) replacing `_pad_75[3]` + `unsigned char InstanceList[4]` (kept under `#else`). Needs engine/CIntelligentPointer.h first. Size assert unchanged (moved 5 lines). |
| engine/CSimpleMemoryPool.h | `FABLE_RETAIL_CSIMPLEMEMORYPOOL_RESET` | 0x00A50C20 | declarations `void Reset();` (0x00A5C790) and `unsigned long CalculateMemoryAllocatedByClient() const;` (0x00A5C850), both called out of line by retail 0x00A50C20 (Ego_r / FableWin ?Reset@CSimpleMemoryPool@@QAEXXZ, ?CalculateMemoryAllocatedByClient@CSimpleMemoryPool@@QBEKXZ). The `FABLE_STATIC_ASSERT_AT` lines shift by 6 (ids unchanged). |

Regrade: the four rows landed from this lane (0x006CE800, 0x0087A7C0, 0x009871C0, 0x006E8EE0) still grade RELOCATION_MATCH with
this overlay on the include path (none of them defines the new macros).
