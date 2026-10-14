# Batch `2026-10-14_linux_hand30` — sub-agent hand-authoring chunk 30 (CDrawMiniMap .. CDynamicObjectHeapBase): 33 of 40 landed

Chunk 30 of the class-labelled 17–96-byte pool, authored by the single sub-agent with the Wine lane and re-graded by
the lander: the NPlayerGui drawers (CDrawMiniMap's IsFinishedOpening / IsFinishedClosing on a fade at +0x100,
CDrawMouseCursor's Draw and constructor, CDrawNPCName::IsShowingScreenshotMessage, CDrawPerceivers::GetPerceivedThings,
the CDrawQuestInfo list walkers GetMiniGameType / ChangeBarColour / UpdateCounter / UpdateCounterList / UpdateTick /
UpdateMiniGame / GetElementByIndex / Clear / RemoveElementsForThing / RemoveElementsFromScript / Update and the oracle
symbol push_back / push_front pair, the CDrawScreenFade / CDrawStamina / CDrawTargetInfo constructors), a map-brush
forwarder, a hover-anim queue stop, a byte-vector + tree clear, CCombatManager::ResetDialogue, CDriveManager's
DeleteDirectory / DeleteFile (RemoveDirectoryW / DeleteFileW), CDrunkennessDef::Transfer, NUISystem::CDynamicList::
Initialise and the CDynamicObjectHeapBase constructor. Pragmas in the sources: `optimize("s")` for the list walkers,
the predicates and the pass-through constructors; `optimize("y",off)` + `("s")` for the frame-pointer bodies.

Label corrections: 009d81e0 / 00cb2ee0 "CDrawMouseCursor::Draw" are free functions (ClearPendingFlag; a `__fastcall`
SetCurrentPair; named by address); 0064dd4d "CDrawQuickAccessIconsPC" is NPlayerGui::CDrawSpiritTargetingCursor's
constructor (vtable 0x0125A5E8; parked); 006e8300 "CDrawQuickAccessIcons::OnHeroHit" is CCombatManager::ResetDialogue
(DialogueManager +0x38); 004fe510 "OracleSymbolPushBack" is a map-brush forwarder on an unknown owner (named by
address); 0064a3c7 inserts before begin (push_front) and 0064a3a1 is the real push_back (the manifest's
"LinkedList_PushFront" 0x0042B687 / "ListPushFrontWrapper" 0x0065617D are push_back bodies); 00aac2e0 "GetMiniGameType"
is a hover-anim queue stop of the 0x00AA module (named by address); 00ad90c0 "OracleSymbolPopBack" is a byte-vector
erase plus STLport tree clear (named by address); 009e4bc0 "DeleteDirectory" uses DeleteFileW = CDriveManager::
DeleteFile; 0x00436560 "CTCHeroStats::SoberUp" is CFadeInFadeOutBase's constructor; 0x00443F90 / 0x004433F0 /
0x00657037 are vector Resize(n) bodies; 0x00656D20 is vector<CIntelligentPointer<CThing>>::Resize(n, value);
0x00434840 "CEngine::StartNewScene" is CFadeInFadeOutBase::Start(float, float, CRGBColour).

Layout notes: the NPlayerGui drawers sit 4 below the PDB after CDrawBase (12-byte vector), CDrawQuestInfo::Elements
0x10 lower (+0x58), CDrawMiniMap's flags / fade 0x20 below engine/CDrawMiniMap.h (Active +0xe9, FadeInOut +0x100),
CDynamicList's tail 0x30 lower (BInitialised +0x15c), CCombatManager's 8 lower (12-byte trees).

Lessons (also in GOTCHAS, wave 24e): a `while (!empty())` loop whose body re-reads begin() is the condition on the
member with the body through the pointer local; `cmp byte ptr [..],0` flag tests are `unsigned char != 0` under
`optimize("s")` (`bool` gives `mov al ; test` in a speed TU); an object addressed once (`lea ecx,[esi+100h]`) for a
byte test and a tail call is a named pointer local; an end iterator kept in edi across a find call is a named local;
VC7.1's `__fastcall` free find with a 4-byte iterator result takes the hidden pointer in ecx, the value reference in
edx and the two by-value iterators on the stack; a missing closing brace (C1075) makes cl.exe under Wine spin forever
(`timeout` every grade; `pkill -x`, never `pkill -f cl.exe`).

Parked (evidence in the agent's NOTES): 0064e0a1 / 0064dd4d / 005415f0 (a call-prep `push` or `mov ecx,esi` scheduled
inside an inlined member constructor's zero-store group), 00644587 (both float arguments pushed through the FPU),
0064a2db (the const-conversion temporary's dead-slot store placement), 0064a37b (`lea esi,[eax+934h]` v `mov ; add`),
009e4c70 (the /GS argument-load gap).
