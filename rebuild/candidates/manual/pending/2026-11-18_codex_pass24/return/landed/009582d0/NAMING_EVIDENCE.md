# CAIStateGroup_MinionReactToTrespassing::Update

The VC7.1 object exports `?Update@CAIStateGroup_MinionReactToTrespassing@@UAEXXZ`. The source-order record in Ego_d identifies `CAIStateGroup_MinionReactToTrespassing::Update`. The submitted extent is 1098 bytes.

- egor donor procedure at 0097cef0
- fablewin donor procedure at 02c06eb0

- abs .rdata at 0x0128C54C (a data reference: in .rdata usually a vtable slot, see names/rtti-vtables.tsv; in .text an EH or jump table)

The native RTTI descriptor for the vtable at 01238C6C identifies CIntelligentPointer<CThing>. The look call takes 0.0027777f. The signed module static FRAMES_BETWEEN_CHECKING_IS_STILL_VALID is present in Ego_d and initialized to 30 in retail. Three calls from IsStillRunnable use the typed FindMostRecentDeed specialization.

Fourteen state scenarios exercise movement, looking, exits, forced and throttled validity checks, missing components and the three deed predicates.
