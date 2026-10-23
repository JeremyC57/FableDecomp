// CMovieSubtitles::ParseSubtitles(...) -- tokenise the subtitle script: per line read start/end times (two __ftol2) and the text key, append CSubtitle records
// Grade: DIFFER(-v1193) -- triage only, no source drafted: 37 distinct callees (string/parser/vector helpers) and inline CCharString temporaries; no landed CMovieSubtitles model
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BD460, 1193 bytes. Decoded from dis_all.txt; see NOTES.md for the next step.
