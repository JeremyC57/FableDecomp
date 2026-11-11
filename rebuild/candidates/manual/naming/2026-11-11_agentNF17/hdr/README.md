# agentNF17 header overlay

None. Every changed row compiles against the current repo headers (no `FABLE_HDR_OVERLAY` additions were needed):

- The CPersistContext one-argument transfer rows use `engine/CPersistContext.h` (`FABLE_RETAIL_CPERSISTCONTEXT`: `Transfer(T&)`,
  `TransferMap(map&)`, `TransferVector(vector&)`, protected `CheckCRC`, `Mode` / `PLoadStream` / `PSaveStream`),
  `engine/CPersistTraits.h` (`FABLE_RETAIL_CPERSISTTRAITS`, plus `FABLE_RETAIL_CPERSISTTRAITS_BOOL_BINARYOUT` for `bool`) and
  `engine/CDataOutputStream.h` (`FABLE_RETAIL_CDATAOUTPUTSTREAM`).
- The GFSerialise*Out adapter rows use the default branch of `engine/CDataOutputStream.h` (it already declares WriteSLONG /
  WriteULONG / WriteUHUGE / WriteEBOOL / WriteNullTerminatedString).

The `hdr/` directory is empty on purpose; grading with or without `FABLE_HDR_OVERLAY=$S/w22/agentNF17/hdr` gives the same objects.
