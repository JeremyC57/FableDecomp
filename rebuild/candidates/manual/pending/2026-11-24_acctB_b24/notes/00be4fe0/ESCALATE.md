# 0x00BE4FE0 -- hold from landing until the shared template text is settled (B24C2)
The sea row matches (RELOCATION_MATCH, strict vland clean) with four different loop-tail spellings of the ONE template
CWaterPatchTesselatorBase<T>::TesselateEdge: the continue-shape in differ/ (m1a/fanvert), and the else-chain skip stores with a
`continue` after the span (s_z4.cpp), in the found arm (s_z6.cpp) or in the not-found arm (s_z7.cpp). Its oracle therefore does not
fix the template text. The water instantiation 0x00BE4660 (same template, same TU in retail) agrees with none of them: its closest text
(4 hunks) has no `continue` in the bridged branch (../00be4660/z2.cpp), which the sea row rejects (s_z2.cpp 22 hunks).
Recommendation: land 0x00BE4FE0 only together with 0x00BE4660 under one text, or accept this payload knowing the text may be respelt
when the water row is solved (then regenerate test / payload / vland / pick-any here). No inferred device or forced order is used.
