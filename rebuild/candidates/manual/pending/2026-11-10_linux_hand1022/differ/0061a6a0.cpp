// CTCInventoryQuests::OnCreate() -- register the quests screen, set the inventory def, run the base OnCreate, size the handle and quest-card lists, fill the six category headers
// Grade: DIFFER(not graded) -- parked with 0 of 10 grades (session budget spent on the four matched rows and 0x00417747); decoded skeleton below
// Leaf: ?OnCreate@CTCInventoryQuests@@UAEXXZ
// Retail: Fable.exe 0x0061A6A0, 508 bytes of code + table (ret at 0x0061A883, 6-entry jump table 0x0061A884..0x0061A89B), the catalog's 512 includes padding.
// Frame: `push ebp ; mov ebp,esp ; and esp,-8` (an 8-aligned local) and no EH state although CWideString temporaries are destroyed.
// Decoded body (callee identities from the landed catalog; spellings to check against Ego_r before landing):
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetPHeroInfoScreensManager() (PClass 0x013B8790, 0x00437CD0)
//       ->RegisterScreen(this ? (NHeroInformationScreens::CBase*)((char*)this + 0x130) : 0)   (0x0066EB11, the null-safe base cast);
//   CDefPointer<const CInventoryQuestsDef?> def; GetParentThing().PBaseDef (+0x70, a counted CDefClassBase pointer, inc/dec of
//       +4 around the call) ->GetSubDef(&def) (0x00485815);  SetPDef(def->[+0xA4]) (CTCInventoryBase::SetPDef 0x005BDB4D);
//   NInventory::CTCInventoryBase::OnCreate() (0x005C0121);  CHandleVector(+0x134).Resize(4) (0x00443F90);
//   std::vector<CQuestCardItem>(+0x140).resize(6) (0x006253D0);  two statics: [0x013B8C68] = 6, [0x013B8C64] = 0 (unnamed);
//   CWideString header (0x0099AED0); for (long category = 0; category < 6; ++category) { switch (category) { 0 "Description",
//       1 "Picture", 2 "Trophy", 3 "Location", 4 "Rewards", 5 "Prerequisites" (wide literals 0x012587E0 .. 0x01258780):
//       header = CWideString(L"..."); }  QuestCardItemList[category] = { category, header } (an 8-byte {long, CWideString}
//       element: +0 the category, +4 CWideString::operator= 0x0099B7D0); }
//   +0x150 = 0; ~CWideString(header) (0x0099B510); the def pointer's release (dec +4, virtual slot 1 on zero).
// Header debt: engine/CTCInventoryQuests.h has no member list (needs +0x130 CBase, +0x134 CHandleVector, +0x140
// vector<CQuestCardItem>, +0x150); CQuestCardItem and the two statics need names (Ego_r CTCInventoryQuests statics).
