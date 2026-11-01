# family psnd / pdisp (hand part): the two rows fam_gptr.py leaves (a namespace that is closed and reopened on one line; the
# address of a member of the pointed-to object). HE_OV=<overlay>
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all
# 0x00557D20: g_pGameState was declared inside namespace NUISystem (?g_pGameState@NUISystem@@...): the global PSoundManager
edit('00557d20', 'src', '\nnamespace NUISystem {\nstruct C2DVector {', '\nclass CASoundManager;\nextern CASoundManager* PSoundManager;                    // 0x013B8394 (naming wave 13)\nnamespace NUISystem {\nstruct C2DVector {')
edit('00557d20', 'src', 'extern CGameState* g_pGameState;                   // 0x013B8394\n', '')
edit('00557d20', 'src', '    if (g_pGameState) {\n        g_pGameState->PlaySound(0x10, 0, 0);\n        g_pGameState->PlaySound(0x80, 0, 0);\n        g_pGameState->PlaySound(0x40, 0, 0);',
     '    if (((CGameState*)PSoundManager)) {\n        ((CGameState*)PSoundManager)->PlaySound(0x10, 0, 0);\n        ((CGameState*)PSoundManager)->PlaySound(0x80, 0, 0);\n        ((CGameState*)PSoundManager)->PlaySound(0x40, 0, 0);')
# 0x004163B7: g_pDisplayDevice 0x013B8390 is PDisplayManager
edit('004163b7', 'src', 'extern CDisplayDevice* g_pDisplayDevice;           // 0x013B8390', 'class CDisplayManager;\nextern CDisplayManager* PDisplayManager;           // 0x013B8390 (naming wave 13)')
edit('004163b7', 'src', '&g_pDisplayDevice->m_backSurface', '&((CDisplayDevice*)PDisplayManager)->m_backSurface')
edit('004163b7', 'src', 'g_pDisplayDevice->m_backSurface.GetPixelFormat()', '((CDisplayDevice*)PDisplayManager)->m_backSurface.GetPixelFormat()')
# the GFInitialise test links the source object: it defines the manager pointers under their PDB names
edit('004022b0', 'tst', 'fable_u32 g_FablePrimitiveManager_013B8394;\nCDisplayManager* g_FableDisplayManager_013B8390;',
     'class CASoundManager; CASoundManager* PSoundManager;          // 0x013B8394 (naming wave 13)\nCDisplayManager* PDisplayManager;                               // 0x013B8390')
edit('004022b0', 'tst', 'fable_u32 g_FableRenderDevice_013B8388;\nfable_u32 g_FableResourceManager_013B838C;\nfable_u32 g_FableTextureManager_013B837C;',
     'class CInputManager; CInputManager* PInputManager;              // 0x013B8388\nclass CFontManager; CFontManager* PFontManager;                 // 0x013B838C\n'
     'class CGraphicBankManager; CGraphicBankManager* PGraphicBankManager;   // 0x013B837C')
edit('004022b0', 'tst', '        g_FablePrimitiveManager_013B8394 != 0x7C7C7C7C ||\n        g_FableRenderDevice_013B8388 != 0x58585858 ||\n        g_FableResourceManager_013B838C != 0x84848484 ||\n        g_FableTextureManager_013B837C != 0x6C6C6C6C)',
     '        (fable_u32)PSoundManager != 0x7C7C7C7C ||\n        (fable_u32)PInputManager != 0x58585858 ||\n        (fable_u32)PFontManager != 0x84848484 ||\n        (fable_u32)PGraphicBankManager != 0x6C6C6C6C)')
edit('004022b0', 'tst', '        g_FableDisplayManager_013B8390 != &g_displayManager ||', '        PDisplayManager != &g_displayManager ||')
# tests that link the respelled source define PSoundManager 0x013B8394
edit('008368f0', 'tst', 'void* g_pGameState = 0;', 'class CASoundManager; CASoundManager* PSoundManager = 0;   // 0x013B8394 (naming wave 13)')
edit('008368f0', 'tst', 'int gs; g_pGameState = &gs;', 'int gs; PSoundManager = (CASoundManager*)&gs;')
edit('0088f660', 'tst', 'CGSIStopSoundEmitter* CGSIStopSound_Emitter;', 'class CASoundManager; CASoundManager* PSoundManager;   // 0x013B8394 (naming wave 13)')
edit('0088f660', 'tst', '    CGSIStopSound_Emitter =\n        (CGSIStopSoundEmitter*)emitterObject;', '    PSoundManager =\n        (CASoundManager*)emitterObject;')
edit_all('0088f660', 'tst', '        g_ObservedEmitter != CGSIStopSound_Emitter ||', '        g_ObservedEmitter != (CGSIStopSoundEmitter*)PSoundManager ||', 2)
