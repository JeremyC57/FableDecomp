// COpinionReactionManager::GetOpinions(...) -- clamp the five opinion axes (+-1 / +-2 bounds via pointer-select ternaries), then add CTCGuard/CTC* modifiers fetched by inline GetTC (ids 0x23/0x22)
// Grade: DIFFER(-v1322) -- triage only, no source drafted: pointer-select clamp ternaries (`lea edi,[esp+x]` per arm) in an 85-op x87 loop plus two inline GetTC lookups; drafting deferred
// Retail: Fable.exe (TLC, VC7.1) @ 0x00700AE0, 1322 bytes. Decoded from dis_all.txt; see NOTES.md for the next step.
