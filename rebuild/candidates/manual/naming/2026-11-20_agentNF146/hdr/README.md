# NF146 header overlay

Empty: no shared header was overlaid or changed. Every row uses opt-ins already in the repo headers (engine/CSurface.h
FABLE_RETAIL_CSURFACE_BITMAP, engine/CAFile.h FABLE_RETAIL_CAFILE, fable_string.h FABLE_RETAIL_CWIDESTRING, engine/GFIntToWideString.h,
engine/CAIStateGroup_StealStealableItem.h FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM_FINDPSTEALABLEITEM (NF141, landed nf141),
stlport/fable_containers.h). orig/ is empty for the same reason; pin_asserts.py was not needed. No landed includer is affected.
