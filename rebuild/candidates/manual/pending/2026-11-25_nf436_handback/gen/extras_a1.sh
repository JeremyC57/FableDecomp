# source before `python3 gen/gen436a.py a1 ...`: per-row macro / include extras (taken from each header's landed includers)
for a in 00d2a7c0 00e93cf0 00ef9130; do export EXTRA_DEFS_$a="FABLE_RETAIL_CCHARSTRING" EXTRA_INC_$a="fable_string.h engine/CCountedPointer.h engine/CScriptThing.h"; done
export EXTRA_DEFS_00d55d10=FABLE_RETAIL_CQ_GUILDTRAININGPREMELEESCRIPT
