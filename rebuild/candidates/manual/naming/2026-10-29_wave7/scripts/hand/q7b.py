# wave 7 hand edits, part b (chunk 563 NAME_FIXES + conflict settlements). Exact replacements; HE_OV=<overlay>.
#   0x00661E80 ?IsInMelee@CThingCreatureBase@@QBE_NXZ (Ego_r + FableWin; body = MeleeAttacker || MeleeAttacked, +0x15C bits 2/3)
#   0x0071BA60 ?GetMorphStrength@CTCHeroMorph@@QBEMW4EHeroMorphType@@@Z (Ego_r + FableWin: const, enum argument)
#   0x006D4D70 ?IsFollower@CTCFollowed@@QBE_NPBVCThing@@@Z, 0x006D4DB0 ?IsAIFollower@CTCFollowed@@QBE_NPBVCThingAICreature@@@Z
#     (Ego_r order IsFollower 0x00709A80 / IsAIFollower 0x00709AC0 = retail 0x006D4D70 / 0x006D4DB0, same 0x40 stride)
#   0x004C0000 ?UnregisterComponent@CTCGraphicAppearance@NGraphicAppearance@@QAEXAAVCAGraphicAppearanceComponent@2@@Z
#   0x013B8AB8 / 0x013B8AC0 NUISystem::CMovie statics PEngine / MovieTexture (bound spellings of the landed FreeTextures
#     0x00547B90 and ~CMovie 0x005483C0); CIEngine slots 69/70 Allocate/FreeTextureFromEngineResourcePools (Ego_r.pdb)
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
from he7 import edit_n, word

# ---------------- IsInMelee 0x00661E80
F = '00752480'
edit(F, 'src', '// Callees: CThingCreatureBase combat-state query @ 0x00661E80 (bits 2/3',
     '// Callees: CThingCreatureBase::IsInMelee @ 0x00661E80 (Ego_r.pdb; bits 2/3')
word(F, 'src', 'IsInCombatMode', 'IsInMelee', 2)
edit(F, 'tst', '// Callees: CThingCreatureBase combat-state query @ 0x00661E80 (bits 2/3',
     '// Callees: CThingCreatureBase::IsInMelee @ 0x00661E80 (Ego_r.pdb; bits 2/3')
word(F, 'tst', 'IsInCombatMode', 'IsInMelee', 3)

F = '006d77c0'
edit(F, 'src', 'CThingCreatureBase::IsIncapacitated 0x00661E80', 'CThingCreatureBase::IsInMelee 0x00661E80 (Ego_r.pdb)')
edit(F, 'src', '        return false;\n    }\n    bool IsIncapacitated();                     // 0x00661E80\n};\n',
     '        return false;\n    }\n};\nclass CThingCreatureBase : public CThing { public:\n'
     '    bool IsInMelee() const;                     // 0x00661E80\n};\n')
edit(F, 'src', '        && !ParentThing->IsIncapacitated()) {', '        && !((CThingCreatureBase*)ParentThing)->IsInMelee()) {')

for F in ('00978e60', '00978f10'):
    edit(F, 'src', 'CThingCreatureBase::IsIncapacitated @ 0x00661E80', 'CThingCreatureBase::IsInMelee @ 0x00661E80 (Ego_r.pdb)')
    edit(F, 'src', '    bool IsIncapacitated();                         // 0x00661E80 (flags byte',
         '    bool IsInMelee() const;                         // 0x00661E80 (flags byte')
    edit(F, 'src', '->IsIncapacitated())', '->IsInMelee())')
edit('00978e60', 'src', '-- the target is not incapacitated, holds', '-- the target is not in melee, holds')
edit('00978f10', 'src', '-- the target lives, is not incapacitated and holds', '-- the target lives, is not in melee and holds')

F = '006d5b50'
edit(F, 'src', 'class CThingAICreature : public CThing { public:\n',
     'class CThingCreatureBase : public CThing { public:\n'
     '    bool IsInMelee() const;                                 // 0x00661E80 (Ego_r.pdb)\n};\n'
     'class CThingAICreature : public CThingCreatureBase { public:\n')
edit(F, 'src', '    bool LooksAtHeadHelper() const;                         // 0x00661E80\n', '')
edit(F, 'src', '            if (LooksAtHeadHelper()) {', '            if (IsInMelee()) {')
word(F, 'tst', 'LooksAtHeadHelper', 'IsInMelee', 3)

# ---------------- GetMorphStrength 0x0071BA60
for F, use_old, use_new in (('0071c9f0', 'GetMorphStrength(7);', 'GetMorphStrength((EHeroMorphType)7);'),
                            ('0071de80', 'GetMorphStrength(type);', 'GetMorphStrength((EHeroMorphType)type);')):
    edit(F, 'src', 'class CTCHeroMorph : public CTCBase {\npublic:\n',
         'enum EHeroMorphType { };\nclass CTCHeroMorph : public CTCBase {\npublic:\n')
    edit(F, 'src', '    float GetMorphStrength(long type);                  // 0x0071BA60\n',
         '    float GetMorphStrength(EHeroMorphType type) const;  // 0x0071BA60 (Ego_r.pdb)\n')
    edit(F, 'src', use_old, use_new)

# ---------------- CTCFollowed::IsFollower 0x006D4D70 / IsAIFollower 0x006D4DB0
F = '006d56e0'
edit(F, 'src', '    bool IsAIFollower(CThingAICreature* creature);          // 0x006D4DB0\n',
     '    bool IsAIFollower(const CThingAICreature* creature) const;   // 0x006D4DB0\n')
F = '0094c900'
edit(F, 'src', 'CTCFollowed::IsAIFollower @ 0x006D4D70', 'CTCFollowed::IsFollower @ 0x006D4D70')
edit(F, 'src', '    bool IsAIFollower(CThingAICreature* creature);   // 0x006D4D70\n',
     '    bool IsFollower(const CThing* thing) const;      // 0x006D4D70 (Ego_r.pdb)\n')
edit(F, 'src', 'followed->IsAIFollower(GetCreature())', 'followed->IsFollower(GetCreature())')
F = '0095f9a0'
edit(F, 'src', 'CTCFollowed::IsAIFollower\n//   @ 0x006D4D70', 'CTCFollowed::IsFollower\n//   @ 0x006D4D70')
edit(F, 'src', '    bool IsAIFollower(CThingAICreature* creature);  // 0x006D4D70\n',
     '    bool IsFollower(const CThing* thing) const;     // 0x006D4D70 (Ego_r.pdb)\n')
edit(F, 'src', 'followed->IsAIFollower(group->GetCreature())', 'followed->IsFollower((const CThing*)group->GetCreature())')
for F in ('006d50c0', '006d5180'):
    edit(F, 'src', '    bool IsAIFollower(const CThing* thing) const;   // 0x006D4DB0\n',
         '    bool IsAIFollower(const CThingAICreature* creature) const;   // 0x006D4DB0 (Ego_r.pdb)\n')
    edit(F, 'src', 'IsAIFollower(&follower)', 'IsAIFollower((const CThingAICreature*)&follower)')

# ---------------- NGraphicAppearance::CTCGraphicAppearance::UnregisterComponent(CAGraphicAppearanceComponent&) 0x004C0000
for F in ('00640f80', '00706e00', '0077bd20', '0077c2a0', '0077c7e0'):
    edit(F, 'src', '    void UnregisterComponent(CAGraphicAppearanceComponent* component);   // 0x004C0000\n',
         '    void UnregisterComponent(CAGraphicAppearanceComponent& component);   // 0x004C0000\n')
    edit(F, 'src', 'PTCGraphicAppearance->UnregisterComponent(this);', 'PTCGraphicAppearance->UnregisterComponent(*this);')
F = '0077b6b0'
edit(F, 'src', 'Callees: CTCGraphicAppearance::UnregisterComponent\n// @ 0x004C0000 (this chunk)',
     'Callees: NGraphicAppearance::CTCGraphicAppearance::\n// UnregisterComponent(CAGraphicAppearanceComponent&) @ 0x004C0000')
edit(F, 'src', 'class CPrimitive;\n\n}\n\nstruct CTCGraphicAppearance {\n'
     '    void UnregisterComponent(NGraphicAppearance::CAGraphicAppearanceComponent* component);   // 0x004C0000\n};\n',
     'class CPrimitive;\n\nclass CTCGraphicAppearance { public:\n'
     '    void UnregisterComponent(CAGraphicAppearanceComponent& component);   // 0x004C0000\n};\n\n}\n')
edit(F, 'src', '    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64 (retail)\n',
     '    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64 (retail)\n')
edit(F, 'src', 'PTCGraphicAppearance->UnregisterComponent(this);', 'PTCGraphicAppearance->UnregisterComponent(*this);')

# ---------------- NUISystem::CMovie statics PEngine 0x013B8AB8 / MovieTexture 0x013B8AC0
F = '00547c90'
edit(F, 'src', '//   the render device\'s slots 70 (+0x118, release texture) and 69 (+0x114, create texture).\n'
     '// Globals: the playing movie 0x013B8AB4, the render device 0x013B8AB8, the video system 0x013B8ABC, the movie texture 0x013B8AC0.\n',
     '//   CIEngine slots 70 (+0x118) FreeTextureFromEngineResourcePools and 69 (+0x114) AllocateTextureFromEngineResourcePools (Ego_r.pdb).\n'
     '// Globals: the class statics PlayingMovie 0x013B8AB4, PEngine 0x013B8AB8 and MovieTexture 0x013B8AC0 (the bound spellings of the\n'
     '//   landed FreeTextures 0x00547B90 / ~CMovie 0x005483C0), the video system g_pVideoSys 0x013B8ABC.\n')
edit(F, 'src', 'struct CRenderDevice {\n', 'class CTexture;\nclass CIEngine { public:\n')
edit(F, 'src', '    virtual void* CreateTexture(unsigned long flags, const C2DExtentsI* size, unsigned long levels, const CPixelFormat* format);   // slot 69 (+0x114)\n'
     '    virtual void ReleaseTexture(void* texture);   // slot 70 (+0x118)\n',
     '    virtual CTexture* AllocateTextureFromEngineResourcePools(unsigned long flags, const C2DExtentsI& size, long levels,\n'
     '                                                             const CPixelFormat& format);   // slot 69 (+0x114)\n'
     '    virtual void FreeTextureFromEngineResourcePools(CTexture* texture);   // slot 70 (+0x118)\n')
edit(F, 'src', 'extern CRenderDevice* g_pRenderDevice;             // 0x013B8AB8\nextern void* g_pMovieTexture;                      // 0x013B8AC0\n', '')
edit(F, 'src', '    static CMovie*         PlayingMovie;           // 0x013B8AB4\n    void ChangeState(',
     '    static CMovie*         PlayingMovie;           // 0x013B8AB4\n'
     '    static CIEngine*       PEngine;                // 0x013B8AB8\n'
     '    static CTexture*       MovieTexture;           // 0x013B8AC0\n    void ChangeState(')
edit(F, 'src', 'g_pRenderDevice->ReleaseTexture(g_pMovieTexture);', 'PEngine->FreeTextureFromEngineResourcePools(MovieTexture);')
edit(F, 'src', 'g_pMovieTexture = g_pRenderDevice->CreateTexture(0, &size, 1, &format);',
     'MovieTexture = PEngine->AllocateTextureFromEngineResourcePools(0, size, 1, format);')
word(F, 'src', 'g_pMovieTexture', 'MovieTexture', 2)

F = '00547f60'
edit(F, 'src', 'extern CTexture*  g_pMovieTexture;                 // 0x013B8AC0\n', '')
edit(F, 'src', '    static CMovie*         PlayingMovie;           // 0x013B8AB4\n    virtual void Draw(',
     '    static CMovie*         PlayingMovie;           // 0x013B8AB4\n'
     '    static CTexture*       MovieTexture;           // 0x013B8AC0\n    virtual void Draw(')
word(F, 'src', 'g_pMovieTexture', 'MovieTexture', 3)

for F in ('005482d0', '00548350'):
    edit(F, 'src', '//   CEnginePrimitiveHandle 0x0122F598; the engine reference is kept in the global 0x013B8AB8.',
         '//   CEnginePrimitiveHandle 0x0122F598; the engine reference is kept in the class static PEngine 0x013B8AB8 (as FreeTextures 0x00547B90).')
    edit(F, 'src', '    static CIEngine*       Engine;                 // 0x013B8AB8\n',
         '    static CIEngine*       PEngine;                // 0x013B8AB8\n')
edit('005482d0', 'src', '    Engine = &engine;', '    PEngine = &engine;')
for F in ('005482d0', '00548350'):
    word(F, 'tst', 'Engine', 'PEngine', 6)
