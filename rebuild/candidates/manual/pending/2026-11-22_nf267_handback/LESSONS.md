# NF267 lessons
- An absence-of-record argument needs a control: the Ego_d type dump has no LF_FRIENDCLS anywhere, so "no friend in this class" proves nothing there.
- Callers decide access cheaply: when every retail call site of a member is inside another class, the member is public; pair it with the PDB field-list access and the body (which list it pushes on) and the twin names fall out.
- Prove an in-place helper rename by sections: objsec.py with old=new renames gave ALL_EQ on both rows before any recheck, so the grade only confirmed it.
- A "reproduces the retail out-of-line copy byte for byte" comment can be wrong for years: pk the helper copies against the retail address (75 v 28 here) before repeating it.
- Precheck the unchanged HEAD sources and tests of control rows too, before the batch recheck that grades them, not only the files you changed.
- Link the HEAD objects first to get the duplicate baseline; then "count after" means something (4 -> 4, same functions, new names).
