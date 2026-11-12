# agentQ16 header overlay (grade with FABLE_HDR_OVERLAY=$A/hdr)

Q16 inherited Q15's overlay; the Q15 rows (first table) are now byte-identical to the repo HEAD copies (they landed), so only the
Q16 rows (second table) are new. Every Q16 change is behind a new `#ifdef FABLE_RETAIL_*` opt-in or is a new header; the default
path of each changed header is byte-identical to HEAD.

## Q15 rows (identical to HEAD now)

Every change is behind a new `#ifdef FABLE_RETAIL_*` opt-in; the default path of each header is byte-identical to HEAD.

| header | opt-in | what | row |
|---|---|---|---|
| engine/CTCSleep.h | FABLE_RETAIL_CTCSLEEP_FASTTRACK | `GetFastTrack()` decl (0x0092C3B0) + inline `GetIsReadyToSleep()` (Ego_r.pdb names) | 0x0092E130 |
| engine/CAIStateGroup_MinionSleep.h | FABLE_RETAIL_CAISTATEGROUP_MINIONSLEEP | retail class deriving CAIStateGroup_GoToBed (0x64, WasCarryingWeapon +0x60) | 0x0092E130 |
| engine/NEntityEvents_CEventHitBy.h | FABLE_RETAIL_NENTITYEVENTS_CEVENTHITBY_TYPE | inline `StatGetType()` (EVENT_HIT_BY = 1) + `GetType()` slot decl (Ego_r / FableWin names); needs _CEVENTBASE_VSLOTS3 | 0x00738A80 |
| engine/NEntityEvents_EventFilters.h | FABLE_RETAIL_NENTITYEVENTS_CISHITEVENTHITBY | new filter class CIsHitEventHitBy (Ego_r layout HitBy +0; ctor `(const CThing&)` and inline operator() from the name set) | 0x00738A80 |
| engine/COptimisedPrimitive.h | FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_METHODS | declarations of GetFaceNormal, FindTextureTangent, FindFaceTextureTangents, PerformContraction, DetectFoldOver, PopFrontContraction, UpdateCost, Optimise (Ego_r name set) | 0x00AC7CB0, 0x00AC7300, 0x00ACD960 |
| engine/QuadricOptimiserTypes.h | FABLE_RETAIL_QUADRIC_COPTIMISEDFACE | COptimisedFace (Ego_r, 0x48) | 0x00AC7CB0, 0x00AC7300 |
| engine/CVectorMap.h | FABLE_RETAIL_CVECTORMAP_GETKEYLIST | `GetKeyList(std::vector<K>&) const` decl (Ego_r name; 0x0086D240) | 0x0086B9D0 |
| engine/CRandomAppearanceMorph.h | FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_SKELETAL | GetRandomSkeletalMorph / AddSkeletalMorphToMorphSet decls (Ego_r, protected const) | 0x0086B9D0 |
| engine/CThingFilter_IsValidGossipee.h | (new header, no default path) | CThingFilter_IsValidGossipee from Ego_r layout + name set | 0x0090C900 |
| engine/CTCVillage.h | FABLE_RETAIL_CTCVILLAGE_CURFEW | IsCurfewTime() const decl (Ego_r; 0x00658D5D) in the FABLE_RETAIL_CTCVILLAGE class | 0x0090C900 |
| engine/NTCScriptedControl_CActionFollowCreature.h | (new header) | CActionFollowCreature : CTCScriptedControl::CActionBase (Ego_r layout, name set) | 0x00713D50 |
| engine/CTCStealth.h | FABLE_RETAIL_CTCSTEALTH_SETSNEAKING | inline StatGetInterfaceType (TCI_STEALTH) and SetSneaking (FableWin names; expanded in retail) | 0x00713D50 |

## Q16 rows

| header | opt-in | what | row |
|---|---|---|---|
| engine/CAIStateGroup_MinionBehaviourCrowdBase.h | (new header, no default path) | CAIStateGroup_MinionBehaviourCrowdBase : CAIStateGroupBase (Ego_r layout: PCrowdThing CIntelligentPointer<const CThing> +0x14, PTaskCrowd +0x1C, MyMessageEvent +0x24, DesignatedCrowdPosition C3DVector +0x2C, IsPartOfCrowd +0x38, sizeof 0x3C); virtuals Initialise / Cleanup / IsRunnable / Update and the pure leaves IsRunnableLeaf / UpdateLeaf / CleanupLeaf / InitLeaf (slots 14-17 from vtable 0x0128C494 and the derived PodiumEmoteNeutral vtable 0x0128D16C -> 0x0096F4A0 / 0x0096F570 / 0x0096F510 / 0x0096F490) | 0x00957DB0 |
| engine/CTCPerceiveThing.h | FABLE_RETAIL_CTCPERCEIVETHING_AWAREOFTHING | inline StatGetInterfaceType (shares the JACK_STAGE1 branch), inline `IsAwareOfThing()` (AwarenessTimer != 0.0f, expanded at 0x00957E30) and the out-of-line `IsAwarenessOfThingDirect() const` decl (0x00744430, Ego_r name) | 0x00957DB0 |
| engine/C3DPrimitive2.h | FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER | IndexBuffer typed `boost::scoped_ptr<CIndexBuffer>` (Ego_r) + private `CreateDegenerateTriangleIndexBuffer()` decl; needs FABLE_RETAIL_SCOPED_PTR (+ _RESET) and FABLE_RETAIL_CINDEXBUFFER | 0x00A90260 |
| engine/CArray.h | FABLE_RETAIL_CARRAY_SIZE_CTOR_OUTOFLINE | `explicit CArray(unsigned int n);` declaration only (called out of line, 0x00A96100 = FableWin ??0?$CArray@K@@QAE@I@Z); checked before FABLE_RETAIL_CARRAY | 0x00A90260, 0x00F3ABF0 |
| engine/CRangeCompressor.h | (new header, no default path) | CRangeCompressor with nested CScriptEntry (24 bytes: Flags, BitCount, RangeBias, RangeShift, ShuffleMask, OrMask; ctor zeroes Flags) and the Ego_r / FableWin member declarations incl. private CalcCompressionScript / CalcBestCompressionForBlock | 0x00F3ABF0 |
| engine/CThingTrackNode.h | FABLE_RETAIL_CTHINGTRACKNODE_LINK | `void SetAsLinkedTo(CThingTrackNode* other);` decl (Ego_r / FableWin, public) | 0x008378D0 |

Rows 0x00774730 (CTCShop::SellItem) needed no header change: it uses the HEAD opt-ins FABLE_RETAIL_CDEFINITIONMANAGER,
FABLE_RETAIL_CRESOURCELIST, FABLE_RETAIL_CTCTHINGOWNER, FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE and
FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE.
