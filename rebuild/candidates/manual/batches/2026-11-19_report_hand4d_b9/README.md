# Re-port hand4d b9 (2026-11-19): CTCShop::GetFreeSaleItemPlaces (0x00774B40)

This replaces the landed local model with FableWin.pdb's text (0x0261ABB0), written over the shared headers with Ego_d local names. Old source and test are
in this folder. The recheck gives RELOCATION_MATCH + `00774b40_TEST PASS`; the test killed 6 of 6 mutants.

The body is:
- clear(), then AddSaleItemPlacesFromThing;
- the dead `C2DBoxI area` (one `// Inferred:` line for how its radius is spelled);
- GFFilter_Bind2(IsNot, In3DRange) into GetPFirstThing.

This is the coordinator's lever 5 for 0x00774D20 BuyItem, which is still DIFFER(1149v1149) after 31 grades.

Corrections to earlier hand4d batches:
- b7: the mutant count is 8 killed, 1 hang, 1 equivalent.
- b8: NAME_FIXES gains 0x00A8EFA0 = ?ConvertToVBFormatStaticNonBump@C3DPrimitive2@@AAEXPAVCTVertexNormTex1@@@Z.
