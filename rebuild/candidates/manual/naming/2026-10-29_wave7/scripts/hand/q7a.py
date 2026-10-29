# wave 7 hand edits, chunk 562/563 NAME_FIXES (cheap signature / identity rows). Exact replacements; HE_OV=<overlay>.
#   0x0051ECC0 ?RemoveFromUpdateList@CThingManager@@QAEXAAVCThing@@@Z (Ego_r + FableWin: CThing&)
#   0x004374B0 ?Open@CPlayerGui@@QAEXXZ, 0x004374F0 ?IsOpen@CPlayerGui@@QBE_NXZ (Ego_r CPlayerGui order Open 0x44A140,
#     ForceOpen +0x20, IsOpen +0x40 = retail 0x4374B0 / 0x4374F0; Open sets mode 2 unless locked, IsOpen = mode != 1)
#   0x006BFB90 / 0x006C0F00 / 0x006C0FD0 CTCBuyableHouse NeedsMainSignPost / CreateMainSignPost / Evict: private (AAE)
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, path

edit('0051f070', 'src', '    void RemoveFromUpdateList(CThing* thing);   // 0x0051ECC0\n',
     '    void RemoveFromUpdateList(CThing& thing);   // 0x0051ECC0 (Ego_r.pdb / FableWin.pdb: CThing&)\n')
edit('0051f070', 'src', '        RemoveFromUpdateList(thing);\n', '        RemoveFromUpdateList(*thing);\n')

edit('0047d78b', 'src', '//          0x013B8A98, CPlayerGui::Close() @ 0x004374B0 (g_pPlayerGui 0x013B8790)',
     '//          0x013B8A98, CPlayerGui::Open() @ 0x004374B0 (g_pPlayerGui 0x013B8790)')
edit('0047d78b', 'src', 'class CPlayerGui { public: void Close(); };                 // 0x004374B0\n',
     'class CPlayerGui { public: void Open(); };                  // 0x004374B0 (Ego_r.pdb CPlayerGui::Open)\n')
edit('0047d78b', 'src', '    g_pPlayerGui->Close();\n', '    g_pPlayerGui->Open();\n')

edit('0056a980', 'src', 'CPlayerGui::Close 0x004374B0', 'CPlayerGui::Open 0x004374B0')
edit('0056a980', 'src', 'struct CPlayerGuiCalls {\n    void Close();                                       // 0x004374B0\n};\nextern CPlayerGuiCalls* g_pPlayerGui;',
     'class CPlayerGui { public:\n    void Open();                                        // 0x004374B0 (Ego_r.pdb CPlayerGui::Open)\n};\nextern CPlayerGui* g_pPlayerGui;')
edit('0056a980', 'src', '                g_pPlayerGui->Close();\n', '                g_pPlayerGui->Open();\n')

edit('0071ae90', 'src', 'Callees: the player GUI\'s 0x004374F0 (manifest\n// "C3DMeshInfo::HasPhysicsMesh" is propagated; it returns `[this+294h] != 1`)',
     'Callees: CPlayerGui::IsOpen 0x004374F0 (Ego_r.pdb; manifest\n// "C3DMeshInfo::HasPhysicsMesh" is propagated; it returns `[this+294h] != 1`)')
edit('0071ae90', 'src', '    bool IsEmoteDrawingAllowed() const;            // 0x004374F0\n', '    bool IsOpen() const;                           // 0x004374F0\n')
edit('0071ae90', 'src', 'g_pPlayerGui->IsEmoteDrawingAllowed()', 'g_pPlayerGui->IsOpen()')

# --- 006c17b0 CTCBuyableHouse::SetOwnedByPlayer: private house helpers; CTCBase accessors by base injection (Ego_r CTCBase:
#     protected GetPlayerManager 0x00686D40 CPlayerManager&, protected PeekWorld 0x00686A80 const CWorld&); CWorld slot 13 is
#     FableWin PeekWorldMap (slot 12 GetWorldMap); CPlayerGui::UpdateMiniMapMarkers(bool) 0x004378C0 (chunk 563)
F = '006c17b0'
edit(F, 'src', '// player-manager accessor @ 0x00686D40,', '// CTCBase::GetPlayerManager @ 0x00686D40,')
edit(F, 'src', 'CTCBase::GetWorld\n// @ 0x00686A80, CWorld virtual slot 13 GetWorldMap,', 'CTCBase::PeekWorld\n// @ 0x00686A80, CWorld virtual slot 13 PeekWorldMap,')
edit(F, 'src', '// CreateMainSignPost @ 0x006C0F00, CPlayerGui::Refresh(long) @ 0x004378C0 on g_pPlayerGui 0x013B8790.',
     '// CreateMainSignPost @ 0x006C0F00 (the three private), CPlayerGui::UpdateMiniMapMarkers(bool) @ 0x004378C0 on g_pPlayerGui 0x013B8790.')
edit(F, 'src', 'CPlayerManager* GetPlayerManager();                 // 0x00686D40\n', '')
edit(F, 'src', '    virtual CWorldMap* GetWorldMap();               // slot 13 (+0x34)\n',
     '    virtual const CWorldMap& PeekWorldMap() const;  // slot 13 (+0x34)\n')
edit(F, 'src', 'class CTCHeroStats { public:\n',
     'class CTCBase { protected:\n    CPlayerManager& GetPlayerManager();             // 0x00686D40\n'
     '    const CWorld& PeekWorld() const;                // 0x00686A80\n};\nclass CTCHeroStats { public:\n')
edit(F, 'src', '    void Refresh(long flag);                        // 0x004378C0\n', '    void UpdateMiniMapMarkers(bool force);          // 0x004378C0\n')
edit(F, 'src', 'class CTCBuyableHouse { public:\n', 'class CTCBuyableHouse : public CTCBase { public:\n')
edit(F, 'src', '    CWorld* GetWorld() const;                       // 0x00686A80\n'
     '    void Evict();                                   // 0x006C0FD0\n'
     '    bool NeedsMainSignPost();                       // 0x006BFB90\n'
     '    void CreateMainSignPost();                      // 0x006C0F00\n'
     '    void SetOwnedByPlayer(bool owned);\n',
     '    void SetOwnedByPlayer(bool owned);\nprivate:\n'
     '    void Evict();                                   // 0x006C0FD0\n'
     '    bool NeedsMainSignPost();                       // 0x006BFB90\n'
     '    void CreateMainSignPost();                      // 0x006C0F00\n')
edit(F, 'src', 'GetPlayerManager()->GetMainPlayer()', 'GetPlayerManager().GetMainPlayer()')
import he
def edit_n(a, k, old, new, n):
    p, rel = path(a, k)
    t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
    c = t.count(old)
    if c != n:
        if c == 0 and new in t: print('already', a, k); return
        raise SystemExit('COUNT %d!=%d in %s for %r' % (c, n, rel, old[:80]))
    open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(t.replace(old, new)); print('ok', a, k)
edit_n(F, 'src', '            long region = GetWorld()->GetWorldMap()->GetLoadedRegion();\n',
       '            long region = PeekWorld().PeekWorldMap().GetLoadedRegion();\n', 2)
edit_n(F, 'src', '            CWorld* world = GetWorld();\n', '            const CWorld* world = &PeekWorld();\n', 2)
edit_n(F, 'src', 'world->GetWorldMap()->GetRegion(region)', 'world->PeekWorldMap().GetRegion(region)', 2)
edit(F, 'src', '    g_pPlayerGui->Refresh(0);\n}', '    g_pPlayerGui->UpdateMiniMapMarkers(false);\n}')

# --- 0076c830 CTCEnemy::OnInitialActivate: the 0x013B8790 call is CPlayerGui::UpdateMiniMapMarkers(false) (fam_umm7 cannot
#     express the free-function stand-in row)
F = '0076c830'
edit(F, 'src', '// g_global singleton at 0x13b8790; its method is a real __fastcall member\nstruct GlobalThing {\n'
     '    void Register(int arg);   // __fastcall, stack arg -> call 0x4378c0\n};\nextern GlobalThing* g_electroGlobal;\n',
     '// the player GUI at 0x013B8790; CPlayerGui::UpdateMiniMapMarkers(bool) 0x004378C0 (Ego_r.pdb / FableWin.pdb, chunk 563)\n'
     'class CPlayerGui { public:\n    void UpdateMiniMapMarkers(bool force);   // 0x004378C0\n};\nextern CPlayerGui* g_pPlayerGui;          // 0x013B8790\n')
edit(F, 'src', '    g_electroGlobal->Register(0);\n', '    g_pPlayerGui->UpdateMiniMapMarkers(false);\n')
edit(F, 'tst', 'struct GlobalThing {\n    int calls;\n    int lastArg;\n    void Register(int arg);   // out-of-line => real emitted symbol\n};\n\n'
     'void GlobalThing::Register(int arg) { calls++; lastArg = arg; }\n\nstatic GlobalThing g_obj = {0,-1};\nGlobalThing* g_electroGlobal = &g_obj;\n',
     'class CPlayerGui { public:\n    int calls;\n    int lastArg;\n    void UpdateMiniMapMarkers(bool force);   // out-of-line => real emitted symbol\n};\n\n'
     'void CPlayerGui::UpdateMiniMapMarkers(bool force) { calls++; lastArg = force; }\n\nstatic CPlayerGui g_obj = {0,-1};\nCPlayerGui* g_pPlayerGui = &g_obj;\n')
