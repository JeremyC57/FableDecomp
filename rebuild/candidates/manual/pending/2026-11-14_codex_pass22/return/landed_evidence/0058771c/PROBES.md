# Attempts

One original source strategy; two compiler-mode probes and one build correction.

| Grade | Change | Result |
|---|---|---|
| main_01_original_reset | Original for_each(mem_fun_ref(Clear)) then vector clear, new typed shared member | Build failure: include was accidentally added to an inactive header branch |
| main_01b_retail_include | Correct include placement; same body |52/45 bytes; speed-mode scheduling/load shape differs |
| main_02_neighbor_size_mode | Apply supplied same-TU neighbours' size optimization to target definition |45/45 masked bytes, all three relocations correct; carried Clear57/57 |

No forced inline devices, hand-built layouts or extra operations. Size-mode selection is not counted as an additional source-shape attempt. Behavior tests pass on both required compilers.
