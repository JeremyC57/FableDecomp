# 0x00C6BE60 SAL_CalcMacroFxGains

Descriptive name, not a PDB name. No symbol in Ego_r or FableWin. The landed
caller CSalSw3DVoice::CalcParams 0x00C54C10 already binds
`?SAL_CalcMacroFxGains@@YAXMMMMPAM@Z`.

Retail 226 bytes, cdecl `ret`. RELOCATION_MATCH. The column subtract sits
before GetFrequencyTableEntry so it schedules between the argument pushes and
the call. GetFrequencyTableEntry's landed body stays in this TU so the column
remains in edx across the call. The truncation is libpk FtoL from libpk/FtoL.h.

functions.tsv still says Audio_ComputeFrequencyResponse. Queued in NAME_FIXES.tsv.
