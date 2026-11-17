# agentNF60 header overlay

Empty: no repo header is copied or changed. Every row in rows/ grades against the repo headers, using only opt-ins that
already exist there:

- engine/fable_script_def_global.h: FABLE_RETAIL_CDEFSTRINGTABLE, FABLE_RETAIL_DEF_CLASS_BASE.
- engine/CDefPointer.h: FABLE_RETAIL_CDEFPOINTER_RESET and FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF (0x00CBEEB0 only).
- engine/CDefString.h: FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION.
- fable_string.h: FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_CWIDESTRING, FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE,
  FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO and FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ.
- stlport/fable_containers.h: FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE, FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE,
  FABLE_RETAIL_STLP_VECTOR_ASSIGN and FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE.

A trial opt-in was tried and withdrawn. It is kept at work/overlay_CDefPointer_GET_FORCEINLINE.h and is not part of the
hand-back:

- what it did: FABLE_RETAIL_CDEFPOINTER_GET_FORCEINLINE made Get() / operator-> __forceinline;
- why it was withdrawn: it did not move 0x00E12580, which is still blocked.
