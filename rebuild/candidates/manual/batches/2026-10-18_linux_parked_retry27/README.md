# Mesh loading, animation cache and food-hall AI

00a9b640 LoadMeshFile (244 bytes): shared chunk/stream storage and verified virtual views; root/file counted-pointer sharing and disposal, stream position get/set, constructor/load receivers and null/non-null root are tested. Named GetPosition result retains the correct virtual-call sequencing.

00abcf40 ReadFromFile (318): retail12-byte chunk arrays place materials+2c rather than donor+34. Constructor/writer confirm the model. Tests cover one index, -1, largest positive index, empty/high-bit-tagged arrays, fast/overflow/mixed stream reads and exact residual-size forwarding.

0070e520 UpdateCachedAnims (290): shared CWorld, audited retail state/stamp offsets and intrusive reference lifetime. Tests cover every validity combination, short-circuit checks, skip/refresh stamps, fallback, null/same pointers and deletion callback mutation before rereading source.

00947a30 StayNearFoodHall Update (244): root review required shared CAIBrain and replaced an uninitialized/declaration-drift fixture. Corrected verbatim fixture covers13 scenarios, all movement arguments/receivers, hall death/removal, work-hours gate, component bit, sentinel/greater-key map fallback, missing village/cook time, repeated movement and callback-mutated brain creature/position.

All four independently RELOCATION_MATCH + PASS; host and canonical VC7.1/Wine fixtures execute actual source. Target fixtures include actual shared headers; portable stand-ins apply only to the64-bit host. Author grades3/2/4/7, root fixture-only integration.
