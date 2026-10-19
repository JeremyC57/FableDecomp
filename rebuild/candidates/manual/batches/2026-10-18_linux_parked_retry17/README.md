# Parked code-section-manager destructor recovery

009f1560 CCodeSectionManager::~CCodeSectionManager,185 bytes, independently RELOCATION_MATCH + PASS. Authoring attempts1–2 recovered the generic destructor; coordinator attempt3 uses verified ownership and implicit map-member destruction. No shared class headers exist.

Evidence: RTTI0129c764 identifies CCodeSectionManager and points to deleting destructor009f1540, which calls009f1560. Final vptr0129c75c identifies CTBaseSingleton. Donor layout names ShortNameToHandle, LongNameToHandle, SectionDesc, with16-byte map headers; retail uses12-byte headers at+4/+10/+1c. SectionDesc erases through009f0d40; both name maps use00579435. Propagated CWound::Kill and earlier CWorld attribution are rejected.

The actual-source fixture runs27 combinations of absent, empty and populated maps. It checks root/map receivers, exact erase/free order, sentinel reset and size before freeing, and final base-vtable restoration from a derived test object. Only the external free dependency is substituted. g++ and canonical VC7.1/Wine PASS. Final evidence: landing_evidence.txt.

Already-landed009f1540 was independently rechecked (owner_recheck_payload.json, owner_oracle.tsv, verify_owner.log) and corrected in place from CLandscapeBackgroundPatch to CCodeSectionManager. Its new actual-source fixture verifies flags0–3, destructor-before-delete order, receiver and return pointer. Catalog/source/test metadata were renamed in place; this correction adds zero functions.
