# Correct dynamic-list refcount declaration

Existing00541390 uses genuine signed long GetRefCount()const (PDB01909400), while stored IVRefCount remains unsigned. Rename local void cleanup ReleaseReference so it is not misrepresented as actual signed-return Release. No target control-flow/body change. Grade9/10 independently matches271 bytes and root exact-source C++98/O2/no-elision/VC7.1 tests PASS. Two relocation targets, real UI LoadAndReturnDef slot108, const Delete and hidden-result return independently audited; all nine immutable grades preserved, one remains.

Previous canonical source/test retained; replace same files and catalog row without re-landing or incrementing counts. Total34,536/49,568=69.6740%, genuine33,952functions/1,636,603bytes. Dependency fixtures are not full-game integration.
