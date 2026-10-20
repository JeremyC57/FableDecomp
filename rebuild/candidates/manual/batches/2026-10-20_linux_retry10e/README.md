# Retry 10, fifth landing (2026-10-20)

Segments five and six of the Fable 5.1 retry pass: 438 rows worked, 264 matched in total; the 115 new rows landed here with
`verify_and_land.py --land` (verify.log; 0093c830 needed the const LowerBound copy as a separate overload, as 00942a00 did).
The big families this time came from same-TU callee stand-ins: never store through an empty tag parameter in a stand-in
(the temporary gets its own frame slot; retail homes it on a live slot), and growth-callee stand-ins must READ the pushed
`const T&` value rather than write it (a store makes the caller's initialising store dead). The agent's sweep scripts
(refsweep -> tagsweep2 -> rdsweep) are in `$S/w22/retry10/var/`. The pass continues on Opus at extra-high effort from
STATUS.tsv.
