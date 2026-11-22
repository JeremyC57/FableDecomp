# Native VC7.1 source experiments

| Probe | Source change | Result | Aligned retail instructions |
| --- | --- | --- | ---: |
| `baseline` | Regrade the incoming draft. | DIFFER 2692/2576 | 734 |
| `context_geometry_v2` | Use shared geometry and a single digging component with the earlier local engine definitions. | DIFFER 2776/2576 | 568 |
| `r_context_shared_initial` | Port all local engine classes onto existing shared owners and type-correct calls. | COMPILE_FAIL |  |
| `r_context_shared_types` | Enable the existing creature type and counted string dependencies. | COMPILE_FAIL |  |
| `r_context_shared_owners` | Select shared definition, stamina, door and opinion log owners. | COMPILE_FAIL |  |
| `r_context_shared_complete` | Enable the real inventory primitive handles. | COMPILE_FAIL |  |
| `r_context_shared_containers` | Use the shared STLport list methods. | COMPILE_FAIL |  |
| `r_context_shared_opinion` | Use shared opinion component metadata and the marker-vector scan. | DIFFER 2844/2576 | 360 |
| `r_context_shared_inline_box` | Let the first C2DBoxI radius constructor expand and eliminate its unused box. | DIFFER 2904/2576 | 362 |
| `r_context_shared_inline_box_t120` | Measure a broader native inlining threshold. | DIFFER 2816/2576 | 362 |
| `r_context_shared_release_pointee` | Route the CDefPointer release through its pointee count helper. | DIFFER 2904/2576 | 362 |
| `r_context_shared_release_selfdelete` | Measure the one-sided self-deleting handle model. This drops retail deletion paths with the ordinary pointee body. | DIFFER 2568/2576 | 526 |
| `r_context_shared_force_coord` | Expand the two donor ToC2DCoordI calls using the original shared body. | DIFFER 2832/2576 | 566 |
| `r_context_shared_delete_count_no_ub` | Compare the self-deleting pointee chain and guard missing targeting components. | DIFFER 2828/2576 | 539 |
| `r_context_collected_shared` | Use normal shared release behavior, the original square helper and safe targeting checks. | DIFFER 2828/2576 | 539 |
| `r_context_collected_gift` | Use the donor positive gift-definition lookup block. | DIFFER 2832/2576 | 532 |
| `r_context_collected_trophy` | Use the donor nested trophy checks to share the false-return cleanup. | DIFFER 2772/2576 | 540 |
| `r_context_collected_pinned` | Regrade the selected shared port with all pre-existing assertion identifiers pinned. | DIFFER 2772/2576 | 540 |

Selected `r_context_collected_pinned`. The shared typing, matching external destinations and donor branch behavior govern selection. Stack slots, cleanup merges and x87 schedules still differ.

## Continued main-chat grades

The selected body compiles to 2,608 versus 2,576 bytes. All 13 mapped bodies match, including the gift and trophy lookup owners. Both dispatch tables and all 14 entries agree. The early required-component paths, register assignments and cleanup exits still differ.

| Probe | Compiled bytes | Aligned instructions | Mapped copies equal / unequal |
|---|---:|---:|---:|
| round2_diglookup_scount | 2664 | 607 | 10 / 3 |
| round2_diglookup_sdirect | 2608 | 604 | 12 / 1 |
| round2_diglookup_spointee | 2608 | 604 | 13 / 0 |
| round2_diglookup_sreread | 2608 | 604 | 13 / 0 |
| round2_diglookup_sycount | 2664 | 607 | 10 / 3 |
| round2_diglookup_sydirect | 2608 | 604 | 12 / 1 |
| round2_diglookup_sypointee | 2608 | 604 | 13 / 0 |
| round2_diglookup_syreread | 2608 | 604 | 13 / 0 |
| round2_diglookup_ycount | 2664 | 607 | 10 / 3 |
| round2_diglookup_ydirect | 2608 | 604 | 12 / 1 |
| round2_diglookup_ypointee | 2608 | 604 | 12 / 1 |
| round2_diglookup_yreread | 2608 | 604 | 12 / 1 |
| round2_digowner2_chain | 2776 | 517 | 10 / 0 |
| round2_digowner2_names | 2776 | 517 | 10 / 0 |
| round2_digowner2_nameschain | 2776 | 517 | 10 / 0 |
| round2_digowner2_noforce | 2776 | 517 | 10 / 0 |
| round2_digowner2_release | 2776 | 517 | 10 / 0 |
| round2_digowner2_search | 2776 | 517 | 10 / 0 |
| round2_digowner_chain | COMPILE_FAIL |  |  /  |
| round2_digowner_names | COMPILE_FAIL |  |  /  |
| round2_digowner_nameschain | COMPILE_FAIL |  |  /  |
| round2_digowner_noforce | COMPILE_FAIL |  |  /  |
| round2_digowner_release | COMPILE_FAIL |  |  /  |
| round2_digowner_search | COMPILE_FAIL |  |  /  |
| round2_digreturn_baseb | 2644 | 483 | 12 / 1 |
| round2_digreturn_based | 2724 | 504 | 10 / 1 |
| round2_digreturn_basenone | 2776 | 517 | 10 / 0 |
| round2_digreturn_bases | 2688 | 490 | 12 / 0 |
| round2_digreturn_exitb | 2608 | 602 | 12 / 1 |
| round2_digreturn_exitd | 2672 | 534 | 10 / 1 |
| round2_digreturn_exitnone | 2716 | 547 | 10 / 0 |
| round2_digreturn_exits | 2632 | 589 | 12 / 0 |
| round2_digreturn_giftb | 2612 | 589 | 12 / 1 |
| round2_digreturn_giftd | 2680 | 521 | 10 / 1 |
| round2_digreturn_giftnone | 2728 | 533 | 10 / 0 |
| round2_digreturn_gifts | 2636 | 576 | 12 / 0 |
| round2_digstruct_allop | 2608 | 604 | 12 / 1 |
| round2_digstruct_cast | 2608 | 604 | 12 / 1 |
| round2_digstruct_full | 2608 | 604 | 12 / 1 |
| round2_digstruct_null | 2608 | 601 | 12 / 1 |
| round2_digstruct_one | 2632 | 587 | 12 / 0 |
| round2_digstruct_op | 2608 | 607 | 12 / 1 |
| round2_digstruct_outer | 2608 | 604 | 12 / 1 |
| round2_digstruct_reread | 2608 | 604 | 12 / 1 |
