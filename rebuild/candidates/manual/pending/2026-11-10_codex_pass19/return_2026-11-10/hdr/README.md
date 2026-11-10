# Merged pass19 header overlay

Overlay these files onto the supplied pass19 include/ tree. The returned sources were compiled against this merged overlay, not individual experimental header directories. The untouched base headers remain required and are already in the incoming package. Do not overlay any HISTORY header snapshots.

119 corrected/new headers: 39 new,80 existing. Default token preservation and the two manual reviews are documented in verification/. Independent feature branches were merged for C3DVector, CThingCreatureBase, CDefinitionManager, CIntelligentPointer, CMeshDataBank, CWorld, CTCWeapon and the other shared owners. The original CTCInGameMenu typed SaveGameToLoad field is retained. The BanditKingThrowWeapons full model is guarded separately, retaining the supplied default.

C3DVector's alignment1 feature is the sole new packing exception, explicitly approved by the owner for a proven original type; see PACKING_APPROVAL.md and rows/00986ed0/NAMING_EVIDENCE.md. Original CFileInstaller qualifiers are backed by both PDBs and are covered by the existing EXTRA_RULES exception, not a scheduling trick. Existing unrelated packing/qualifiers remain unchanged.

The experimental CDefClassBase_GetSubDef suppression overlay was removed. Use the supplied original plain helper header. Historical versions are retained solely as attempt evidence.
