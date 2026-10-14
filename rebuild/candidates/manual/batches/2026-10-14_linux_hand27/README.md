# Batch `2026-10-14_linux_hand27` — coordinator hand-authoring chunk 27 (CCreatureAction_WhisperSommersaultForward .. CDateAndTime): 34 landed here, 1 in `_hand27_b` (35 of 40)

Chunk 27 of the class-labelled 17–96-byte pool, authored by the coordinator with the Wine lane: the CDateAndTime family
(constructors from SYSTEMTIME / FILETIME / seven longs, `Initialise`, the long-valued operator== and its inlined operator!=,
operator< / operator> with the merged unsigned-compare tails), the CDataOutputStream writers (colour, 2D/3D vectors through an
inlined by-value WriteFloat homed in the dead argument slot, strings through the CCharString accessors, a byte-vector block),
CDataInputStream / CDataOutputStream / CEngineDynamic3DResource-style constructors, CDataBank text/group lookups and loaders,
counted-pointer Unload bodies, IsLoaded, CCriticalSectionLock (both Win32 imports through `__declspec(dllimport)`), two
CDBMinion::OnPredicateFail bodies (thing-manager slots 89/90), three size-optimised constructors in the /Os region, and the
WhisperSommersaultForward PerformAction.

Label corrections: 004d4033 / 0056e545 / 006a92b0 "CDateAndTime::CDateAndTime" are three unrelated records (floats, a float +
two flag bytes, four longs + a byte) named by address; 00852710 "CDataInputStream" is NCameraModes::CScriptModuleSpline (vtable
0x01275A6C), 00bf3250 is CEngineDynamic3DResource (parked), 00c3e520 is a TeleportData-derived state record; 00c62320 "Close" is a
six-word cursor constructor; 004dcab9 "CCrimeStatBar" is NThingShot::CTCShotDefinitionForcePushBall, 00656c0a is
NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounterList; 009c8220 "BeginLoadingText" loads a GROUP entry (type 1, callee
0x009C80F0); 009e9b20 "operator==" is operator!=, 009e9be0 "operator<" is operator>; 00c6e660 "WriteCBYTE" is a seven-argument
virtual call with a zeroed two-float record (landed in `_hand27_b`).

Lessons (also in GOTCHAS, wave 24b): a frame-pointer size body WITH `mov eax,ecx` and no spills is `optimize("y",off)` +
`("s",on)` (the `gsy` form spills `this`); a `!(a == b)` over a bool-returning inline `&&` chain gives `mov eax,1 ; test al,al ;
sete`; unsigned `<` on the second..nth field of a lexicographic compare merges into one `sbb eax,eax ; neg eax` tail entered
by each `jne`; a CCharString read for BOTH text and length is one null test with the empty-string pointer assigned first and
`else length = 0` (two accessors test twice); accessors are written null-case-first; a non-polymorphic base's member stored
before the derived vtable means VC placed that base after the vptr; a record whose zero stores are immediates next to `push 0`
immediates holds floats (integer zeroes claim a register and turn the pushes into `push eax`); the counted-pointer release
lives on the object (`if (--m_refCount == 0) Destroy();`) so the pointer stays in ecx for the virtual call.

Not landed (best sources in `$S/w22/agent27/differ/`, notes in NOTES.md): 00bf3250 (the lane sinks the 1.0f register stores
past the zero stores and the vtable), 00c59720 (the lane narrows `(x >> 8) & 0xFFFFFF01` to `0x00FFFF01`), 00995a20 /
00995a80 (retail places the merged null arm out of line after the following blocks), 00e5dde0 (the thing argument loaded
early into ecx instead of into eax before the push).
