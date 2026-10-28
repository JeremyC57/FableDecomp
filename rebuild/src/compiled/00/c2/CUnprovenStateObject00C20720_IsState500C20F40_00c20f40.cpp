// CUnprovenStateObject_00C20720::IsState5_00C20F40() -- check current state and report whether its tag matches the requested state.
// Leaf: ?IsState5_00C20F40@CUnprovenStateObject_00C20720@@QAEHXZ
// Identity: descriptive; own target calls00C20720(-1), then compares originalreceiver State WORD+AC with5 and returns normalizedEAX0/1. Actual helper only reads State, bounds-checks enum0..6 and dispatches all seven entries to same return; no writes or external effects, so real helper preserves ECX. External-only lane declaration may require callee-save receiver absent retail. No original owner/method/source field names or complete base identity proven; all descriptive address-suffixed.
// Helper evidence: own00c20720 reads the State word, bounds checks0..6, its seven byte-table entries arezero and sole jump-table entry is00c2073a ret4. Genuine source switch has no writes or external effects; explicit visibility gives compiler its real clobber contract.
// Layout: explicit unknown prefix through+AC, native ushort tag. No header/PDB binding exists for libpk owner. No invented field/base identity; only exact observed field and genuine helper prototype projected. Retail same-TU clobber knowledge retained as lane-gap evidence if external call spills this.
class CUnprovenStateObject_00C20720{public:unsigned char _unmodeled00[0xAC];unsigned short State;void CheckState_00C20720(long);int IsState5_00C20F40();};
#pragma auto_inline(off)
void CUnprovenStateObject_00C20720::CheckState_00C20720(long){switch(State){case 0:case 1:case 2:case 3:case 4:case 5:case 6:return;default:return;}}
#pragma auto_inline(on)
int CUnprovenStateObject_00C20720::IsState5_00C20F40(){CheckState_00C20720(-1);return State==5;}