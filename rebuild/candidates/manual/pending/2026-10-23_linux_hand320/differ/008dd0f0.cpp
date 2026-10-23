// CModeScorpionKingVulnerable::OnActivate() -- the Scorpion King vulnerable mode start: reads the global frame counter (fild 0x01375550) into an 8-aligned frame, sets up animation/sound/physics state through 26 distinct callees
// Grade: DIFFER(-v1513) -- triage only, no source drafted: aligned `and esp,-8` frame from a double/__int64 local (fild of the frame counter) plus 26 unmodelled callees; no CModeScorpionKingVulnerable model in the tree
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DD0F0, 1513 bytes. Decoded from dis_all.txt; see NOTES.md for the next step.
