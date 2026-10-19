# 200F native audit

The manifest's repeated Orchard Farm/Bandit Camp names are propagated and wrong.
The actual PDB function names (Ego donor addresses differ from retail) are:

| Retail | Donor | Actual PDB name | Master flag | Boast |
|---|---|---|---|---|
|00CE4B70|00CC9780|BoastHobbeCavePerpertratorKilled|HobbeCavePerpertratorKilled +10D|21|
|00CE5620|00CCA4E0|BoastHobbeContestKillMoreHobbes|HobbeContestKillMoreHobbes +11B|28|
|00CE56E0|00CCA5E0|BoastGraveyardKillAllInnocents|GraveyardKillAllInnocents +11C|29|
|00CE5E10|00CCAE30|BoastBountyHuntDecapitation|BountyHuntDecapitation +D9|16|
|00CE6A40|00CCBCA0|BoastFireHeartFreeAllPrisoners|FireHeartFreeAllPrisoners +12F|43|

The first candidate uses the descriptive spelling BoastHobbeCaveKillPerpetrator;
its behavior and signature are the PDB function above. References:
ghidra_out/egor_pdb_names.tsv, ghidra_out/struct_layouts_egor.tsv.
CHeroBoastsScript and CQ_SunnyvaleMasterData have no generated engine headers.
The native CLevelScriptBase projection has Game+40/MasterScriptData+44 whereas
the donor CLevelScriptBase has Game+54/MasterScriptData+58. The donor CScriptBase
has active thread+3C; retail CB7940 reads +2C and its terminating byte+5.
The master-data flags used here have the same offsets in donor PDB and retail.
Opaque ranges cover unused script/master state. Existing actual engine
CGameScriptInterface and fable_string.h declarations are included directly.

RTTI CGameScriptInterface table01260F0C, independently checked entries:
7=006E7410 NewScriptFrame;28=00893730 MsgOnQuestCompletedBeforeScreenShown;
29=008937D0 MsgOnQuestFailedBeforeScreenShown;
305=008915B0 SetBoastAsFailed;306=008915E0 SetBoastAsCompleted.
The latter native methods take long then const CCharString&, and forward a
native copied string to CQuestManager. No arbitrary virtual slot padding exists:
a union of signature-correct member-function pointers indexes the flattened
actual header's table; native union size4 is asserted. On host the PMF union is
full host width and member pointers retain their full representation.

Each candidate owns its CCharString by-value parameter and calls99EAE0 on every
exit (native ret4); the header's existing destructor supplies this codegen.
The production shared header omits the real copy-ctor declaration. Fixtures
therefore explicitly model the caller99EC30 retain before the ordinary C++
by-value argument transfer, without adding a fake copy constructor. Existing
constructor/destructor declarations receive dependency stubs only: owners2 while
candidate runs, owners1 after its parameter destructor, owners0 after caller's
named string dies. This checks parameter lifetime and native caller ownership
accounting, not an implicit-copy implementation supplied by the shared header.
The early temporary-based fixture exposed VC versus host copy elision differences;
the final fixture uses a named caller string on both compilers.

Tests embed exact final source and recursively expanded actual headers. Only
native pointer/layout assertions are conditional for host; VC retains all of
them. Behavior covers initial termination, waiting/yield, flag/message success
and failure, failed-message priority, asynchronous frame state changes, termination
between nested gates, and a completion response changing between successive
queries. CE6A40 intentionally continues after reporting each outcome and yields;
it can report repeatedly until termination. The other four return after outcome.
No row crosses another function boundary; trailing CC bytes are ordinary padding.
