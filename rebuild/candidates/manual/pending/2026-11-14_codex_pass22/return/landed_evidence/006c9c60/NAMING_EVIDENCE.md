# Corrected extent and identity

The supplied237-byte extent holds three independent functions. Assigned head006C9C60 ends after its ret8 at006C9CC7..006C9CCA, length106. Padding ends at006C9CD0, where an82-byte vector-resize function begins; more padding ends at006C9D20, followed by a45-byte copy wrapper. BOUNDS_FIX.tsv records all boundaries. Only the assigned first head is reconstructed; the two later functions are not part of the returned implementation.

The exact leaf is `??$Transfer@V?$CIntelligentPointer@VCThing@@@@@CPersistContext@@QAEXPBDAAV?$CIntelligentPointer@VCThing@@@@@Z`. Ego_r007004F0 and FableWin01F2FF20 give that member-template signature. The native caller is CTCHeroOpinionDeedLog::OnSerialise006C7370. The original donor calls GetDefaultVal<CIntelligentPointer<CThing>> through017A2907 ->01F36ED0, then the three-argument Transfer specialization through0180321B ->01F36F50, then destroys the default. Native006C9C60 expands GetDefaultVal and the original intelligent-pointer copy but otherwise follows the same sequence. The old bound name omitted the template encoding and is recorded in NAME_FIXES.tsv.

# Original shared code and callees

The source explicitly instantiates the existing CPersistContext::Transfer(name,value) body, which calls Transfer(name,value,GetDefaultVal<T>()). Existing GetDefaultVal constructs CPersistTraits<T> and returns its DefaultVal member. This is not NRVO: the member is copied through the real intelligent-pointer copy constructor, which constructs its base, reads GetPItem and calls SetPItem. Original existing flags enable exactly those shared bodies; no header changes, local substitutes or new inline controls are needed.

All nine relocations are individually decoded. CBaseIntelligentPointer ctor00A01B10, GetPItem00A01B50, SetPItem00A01B90, destructor00A01C10 and CIntelligentPointer<CThing> vtable01238C6C agree with canonical bindings and the original shared header's PDB evidence. The only newly named call is006C9A10, the three-argument Transfer template (Ego_r00700390/FableWin01F36F50); the native callee uses the context's mode switch and its default argument, matching the original donor member. No declared-only callee body was invented or supplied.

# Behavior validation

Ten fixtures retain the exact two original template bodies and an instrumented original intelligent-pointer copy. A stub for the external three-argument callee checks name/reference forwarding, the null default, and destruction boundaries across five context modes with populated/empty values. VC7.1 /Od /W3 /GR- without /EHsc and g++ C++03 pass. The initial fixture incorrectly relied on two identical source literals sharing an address under /Od; it was corrected to pass the same named pointer, preserving the intended identity assertion. No production-source change resulted. The test does not rely on NRVO or fixed host long size.
