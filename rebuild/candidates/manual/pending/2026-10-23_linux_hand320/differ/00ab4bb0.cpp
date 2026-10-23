// CMouseDX::GetNonExclusiveModeDeviceMouseEvent(...) -- poll the non-exclusive DirectInput mouse: cursor position from the window, button/axis deltas scaled into the event (93 x87 instructions, 7 callees)
// Grade: DIFFER(-v1152) -- triage only, no source drafted: x87-heavy (93 ops) DirectInput/Win32 body; needs the IDirectInputDevice8 + CMouseDX layouts; scheduling-dominated
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB4BB0, 1152 bytes. Decoded from dis_all.txt; see NOTES.md for the next step.
