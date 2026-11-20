# NF162 overlay (round 10)

No overlay headers: hdr/ is empty and orig/ holds no bases. Every FABLE_RETAIL_* macro the rows in rows/ define resolves in
rebuild/include at the working tree (checked after the restart). No opt-in was added to any shared header, so no landed includer
changes; pin_asserts.py was not needed.

Opt-ins used (all already in rebuild/include): FABLE_RETAIL_CQ_FIREHEARTSCRIPT_PRISONER (Account B p25A, engine/CQ_FireHeartScript.h),
the CV_LostTraderScript BROTHER opt-in, FABLE_RETAIL_CPERSISTTRAITS_TRANSFEROUT, FABLE_RETAIL_CPERSISTTRAITS_DEFAULTVAL_COPYINIT
(class-nested enum T), FABLE_RETAIL_CPERSISTCONTEXT, FABLE_RETAIL_CPERSISTTRAITS, FABLE_RETAIL_CDATAOUTPUTSTREAM, FABLE_RETAIL_NSCRIPT.
