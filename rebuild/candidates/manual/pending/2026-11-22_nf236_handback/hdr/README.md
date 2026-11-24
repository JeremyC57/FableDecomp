# NF236 header overlay (2026-11-22): empty

No shared header changes in this hand-back. Every row and test uses the repo's rebuild/include as it stands (nf235 and Codex pass 27
landed): entry 2 (0x004065D0) uses the existing engine/CUserProfileManager.h FABLE_RETAIL_CUSERPROFILEMANAGER_DEFAULTPROFILENAME opt-in;
entry 4's tests use the existing CTOR_DTOR / SETQUIT / CMESHBANKMANAGER_CTOR_DTOR opt-ins of the rows they test; entries 1, 3 and 5 are
in-place respells inside row-local declarations (extern lines, member declarations). So no pin_asserts / assert_names_check /
equivalence run is owed (no includer's preprocessed text can change); orig/ is empty for the same reason.
