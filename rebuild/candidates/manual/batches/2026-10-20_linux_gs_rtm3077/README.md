# /GS rows landed with the RTM compiler (2026-10-20)

Five parked `/GS` rows (00cac4a0, 009f1ac0, 00690c70, 00438310, 0099d000) differ only in the stack-cookie prologue under the
Linux lane's DDK 13.10.4035 build. With cl 13.10.3077 (VS .NET 2003 RTM, the compiler the retail Rich header names for the
game objects) they grade RELOCATION_MATCH; tests written by an Opus agent pass; landed with `verify_and_land.py --land`
under `FABLE_VC71=$S/wine/vc71rtm` (no catalog tag: RTM is the default toolset of the catalog). `verify.log` here.
