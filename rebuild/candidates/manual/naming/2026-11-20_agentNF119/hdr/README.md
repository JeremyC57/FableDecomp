# NF119 overlay headers

None. Every NF119 row compiles against the repo's rebuild/include as it stands (orig/ and hdr/ are empty on purpose). The opt-ins used
are all already in the repo: FABLE_RETAIL_CTHINGCREATUREBASE_BEDSLEEP, FABLE_RETAIL_CAISTATEGROUP_GOSSIP (declares
CCreatureAction_SheatheWeapons::PerformAction), FABLE_RETAIL_CCARDINALSPLINE_APPENDCONTROLPOINT, FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE,
FABLE_RETAIL_STLP_FIND_IF_INPUT, FABLE_RETAIL_STLP_REMOVE_COPY_IF, FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_OUTOFLINE. No landed includer changes.
No pin_asserts run (no header edited).
