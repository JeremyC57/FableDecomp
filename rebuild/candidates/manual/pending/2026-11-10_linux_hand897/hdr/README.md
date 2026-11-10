# agent897 header overlay

- `engine/CString.h`: new opt-in `FABLE_RETAIL_CSTRING_SPAN` (rows 0x00C0E2C0 SpanIncluding / 0x00C0E310 SpanExcluding):
  declarations of the two MFC strcore.cpp members only, inside `#ifdef`; the default path is byte-identical.
