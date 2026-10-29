# wave 9 scope 3: the five wave-8 RENAMES proposals -- the defining rows respelled to their DECISIONS_W8 finals
# (Ego_r.pdb + FableWin.pdb spellings; code unchanged: a reference / pointer return passes the same address, a static
# no-argument member is called like the free function, an unsigned __int64 by value occupies the same two stack slots)
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
# 0x0049D810 ?GetWorldSeed@CWorld@@QBEPAKXZ (returns &PMainGameComponent->WorldSeed, Ego_r CMainGameComponent +90428)
edit('0049d810', 'src', 'struct C0049d810 { char pad[0x8]; long f; long M(); };\nlong C0049d810::M(){ return this->f + 90428; }',
     '// CWorld::GetWorldSeed() const @ 0x0049D810 -- the address of CMainGameComponent::WorldSeed (+0x1613C) of the world\'s game component (+0x8)\n'
     '// Leaf: GetWorldSeed\nclass CWorld { public: char pad[0x8]; long PMainGameComponent; unsigned long* GetWorldSeed() const; };\n'
     'unsigned long* CWorld::GetWorldSeed() const { return (unsigned long*)(this->PMainGameComponent + 90428); }')
# 0x008FCF80 ?PeekPlayerManager@CAIStateGroupBase@@IBEABVCPlayerManager@@XZ (PBrain +0x4, its player manager +0x20)
edit('008fcf80', 'src', 'struct I008fcf80 { char pad[0x20]; unsigned long f; };\n// Leaf: CountReferences\nstruct C008fcf80 { char pad[0x4]; I008fcf80* p; unsigned long M(); };\nunsigned long C008fcf80::M(){ return this->p->f; }',
     '// CAIStateGroupBase::PeekPlayerManager() const @ 0x008FCF80 -- *PBrain->PPlayerManager (Ego_r accessor order IsRunnableMain .. IsStopping)\n'
     'class CPlayerManager;\nstruct I008fcf80 { char pad[0x20]; CPlayerManager* f; };\n// Leaf: PeekPlayerManager\n'
     'class CAIStateGroupBase { public: char pad[0x4]; I008fcf80* p; protected: const CPlayerManager& PeekPlayerManager() const; };\n'
     'const CPlayerManager& CAIStateGroupBase::PeekPlayerManager() const { return *this->p->f; }')
# 0x00405F00 ?GFIntToWideString@@YI?AVCWideString@@J@Z
edit('00405f00', 'src', 'CWideString __fastcall IntToWideString(long value)\n{', '// Leaf: GFIntToWideString\nCWideString __fastcall GFIntToWideString(long value)\n{')
# 0x0051ED80 ?GetPThingWithUID@CThingManager@@QAEPAVCThing@@_K@Z (the UID map at +0x58; found node's value at +0x18)
edit('0051ed80', 'src', '// CWorld::GetThingWithUID @ 0x0051ed80  (__fastcall, returns DWORD)\ntypedef unsigned long DWORD;\n\nstruct Node {\n    Node* _00;\n    char pad[0x14];\n    DWORD _18;     // value at +0x18\n};',
     '// CThingManager::GetPThingWithUID(unsigned __int64 uid) @ 0x0051ed80 -- the thing mapped to `uid`, or 0\n// Leaf: GetPThingWithUID\nclass CThing;\n\nstruct Node {\n    Node* _00;\n    char pad[0x14];\n    CThing* _18;   // value at +0x18\n};')
edit('0051ed80', 'src', 'class CWorld { public:\n    char pad[0x58];\n    Sub sub;                    // at +0x58\n    DWORD GetThingWithUID(int a1, int a2);\n};\n\nDWORD CWorld::GetThingWithUID(int a1, int a2)\n{\n    Sub* s = &this->sub;\n    Node* r = s->Find(&a1);',
     'class CThingManager { public:\n    char pad[0x58];\n    Sub sub;                    // at +0x58\n    CThing* GetPThingWithUID(unsigned __int64 uid);\n};\n\nCThing* CThingManager::GetPThingWithUID(unsigned __int64 uid)\n{\n    Sub* s = &this->sub;\n    Node* r = s->Find(&uid);')
# 0x00438570 ?PeekGuiDef@CPlayerGui@@SIABVCPlayerGuiDef@@XZ (static; already bound here by 0066e742)
edit('00438570', 'src', 'CPlayerGuiDef *CPlayerGui_PeekGuiDef()\n{', '// Leaf: PeekGuiDef\nclass CPlayerGui { public: static const CPlayerGuiDef& __fastcall PeekGuiDef(); };\n\nconst CPlayerGuiDef& __fastcall CPlayerGui::PeekGuiDef()\n{')
edit('00438570', 'src', '    return g_playerGuiDef;\n}', '    return *g_playerGuiDef;\n}')
