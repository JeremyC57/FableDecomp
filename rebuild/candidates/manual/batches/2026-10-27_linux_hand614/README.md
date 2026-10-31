# Chunk 614 (eleventh parked-row chunk from the sweep ranking, 10 same-length rows 364-412 bytes): 4 of 10 landed

Landed (vland --land 4/4 RELOCATION_MATCH + PASS, multi-check 0): 0x0085B5B0 CThingSearchTools::GetAllThings<CThingGameObject, list,
CFilter_Bind4<...>> const (copied the landed sibling 0x008AC660; __forceinline Bind4 operator(); same-file LowerBound body;
out-parameter GetInterface), 0x00820840 CMap::ScriptedMapBrushMoveThings (protected virtual; every callee re-proved; STLport 4.6
list ctor; a real CThingPosAndOrientation {C3DVector, C3DAngles} copy gives retail's `add eax,0Ch`; a named physics pointer for the
ResetPosition call only), 0x0090BD40 CAIStateGroup_MinionWorkInInn::CleanBench (a block-scoped `bool reached` shares a frame byte
with the inlined presence bool), 0x006AB5D0 CThingPlayerCreature::GetCurrentMaxMovementSpeed (out-parameter PeekTC; named float
sprint; band cases follow retail's jump table 0x006AB758 -- the parked source had bands 3 and 4 swapped and q.py masks jump tables;
vland caught it). Parked 6 (pending/.../differ/): 00854180 69 -> 30 lines (GetTC key slot not shared with the roll local), 00b9e120
WorldPos copy eax/edx, 00a26ad0 register rotation + sete cl, 009c5890 CreateEventW push order (real STLport fixed push edi),
00783a30 ebx/ebp + UID copy registers, 009c38c0 406v409 (static __stdcall GetMarkerFileName gives ESI result + ret 4; retail's dead
`mov al,[eax+7Ah]` dropped). Six of ten parked own symbols were wrong (Methods-class names, public vs private/protected, pointer
parameters, un-nested CActiveFile). 14 NAME_FIXES + 6 review rows queued (prefix c614; 0x0051ED80 GetPThingWithUID has nine bindings).
