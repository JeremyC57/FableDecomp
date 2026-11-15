# Scoreboard findings

The old1178-byte source was a false starting point for professional closure: GetDecryptedDocument merely cleared/returned a vector, string range initialization borrowed the input storage, and operator== was really an inequality stub. Real shared classes and genuine bodies must precede byte comparisons.

Both original PDBs enumerate ENS_SUCCESS=1000, ENS_FAILURE=1001, ENS_PENDING=1002. The old NETWORK_BUSY=1001 label was wrong. GetGameIdent/GetCDKey are original methods; OpenRequest/GetHeader were invented wrappers. The original activity enum and CActivity are nested in CTCHeroOnlineScoreboard.

A local enum key declared before reversal prevented its loop counter remaining in a register. Moving act_enum to its original final use, immediately before map[], removed16 bytes and restored1178. Source scopes matter even with matching member layouts and genuine helper bodies.

Inspect the actual constructor callback relocation. The array constructor iterator passes MSVC's default-constructor closure `??_Fvector...`, which matches11 bytes at00567EA0. The ordinary vector constructor is15 bytes and is not the callback. Do not assign the ordinary constructor to that address by type resemblance.

GetDecryptedDocument's actual code extent is321, ending ret8 at00560CEE; older notes called it320. Explode has653 code bytes plus3 alignment bytes. The parked receipt distinguishes target call-order identity checks from strict relocation-offset checks on independently exact helpers.
