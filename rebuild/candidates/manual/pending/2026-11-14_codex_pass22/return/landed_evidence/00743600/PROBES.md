# Attempts

|Attempt|Strategy|Result|
|---|---|---|
|1|Original implicit destructor on the shared model, materialised by the original constructor|main_01:121/121; ctor151/151; deleting helper30/30|
|Review|Restore missing original inherited AnimUpdate slot in the new pass22 guard|main_01b: all three bodies unchanged; corrected full vtable;25 fixtures PASS in both toolchains|

One genuine source strategy, two compiler invocations. The second is a symbol/vtable correction, not a new strategy. No new packing or inline devices.
