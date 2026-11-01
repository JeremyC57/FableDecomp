# family sfv (hand): 0x00AF9000 is the free NParticleEngine::GFSmoothFadeValue (FableWin.pdb ?GFSmoothFadeValue@NParticleEngine@@YIMM@Z,
# 0x02F2ACA0 between the CParticleSystem accessors and CPSCRenderSprite, the retail neighbours of 0x00AF9000); 0x00B05E10 /
# 0x00B1AD90 are CPSCSingleSprite::SmoothFadeValue IBE / CPSCLight::SmoothFadeValue QBE (not landed). The body never reads
# `this`, so thiscall and __fastcall (float on the stack, ret 4) give the same bytes. RENAMES.tsv moves src + test. HE_OV=<overlay>
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
A = '00af9000'
edit(A, 'src', '// NParticleEngine::CPSCLight::SmoothFadeValue(float t) -- cosine-table smooth fade: 0.5 - 0.5 * lerp(cos[i], cos[i+1]) over the 1024-entry table\n// Leaf: SmoothFadeValue',
     '// NParticleEngine::GFSmoothFadeValue(float t) -- cosine-table smooth fade: 0.5 - 0.5 * lerp(cos[i], cos[i+1]) over the 1024-entry table\n// Leaf: ?GFSmoothFadeValue@NParticleEngine@@YIMM@Z')
edit(A, 'src', '// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF9000, length 99 bytes (identical bodies at 0x00B05E10, 0x00B1AD90).',
     '// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF9000, length 99 bytes (identical bodies at 0x00B05E10, 0x00B1AD90).\n'
     '// Identity (naming wave 13, owner decision 2026-11-01): the free NParticleEngine::GFSmoothFadeValue (FableWin.pdb, between\n'
     '//   the CParticleSystem accessors and CPSCRenderSprite, as retail 0x00AF9000 sits after ScaleParticleDimensions); the two\n'
     '//   identical bodies are CPSCSingleSprite::SmoothFadeValue (IBE) and CPSCLight::SmoothFadeValue (QBE). Was catalogued as\n'
     '//   CPSCLight::SmoothFadeValue; the body never reads `this`, so the __fastcall free form gives the same bytes.')
edit(A, 'src', 'struct CPSCLight {\n    float SmoothFadeValue(float t);\n};', 'namespace NParticleEngine {\nfloat __fastcall GFSmoothFadeValue(float t);   // FableWin.pdb ?GFSmoothFadeValue@NParticleEngine@@YIMM@Z\n}')
edit(A, 'src', 'float CPSCLight::SmoothFadeValue(float t)\n{', 'float __fastcall NParticleEngine::GFSmoothFadeValue(float t)\n{')
edit(A, 'tst', 'struct CPSCLight { float SmoothFadeValue(float t); };', 'namespace NParticleEngine { float __fastcall GFSmoothFadeValue(float t); }')
edit(A, 'tst', 'float CPSCLight::SmoothFadeValue(float t)\n{', 'float __fastcall NParticleEngine::GFSmoothFadeValue(float t)\n{')
edit(A, 'tst', '    CPSCLight l;\n    float a = l.SmoothFadeValue(0.0f), b = l.SmoothFadeValue(0.5f), c = l.SmoothFadeValue(1.0f), d = l.SmoothFadeValue(0.25f);',
     '    using NParticleEngine::GFSmoothFadeValue;\n    float a = GFSmoothFadeValue(0.0f), b = GFSmoothFadeValue(0.5f), c = GFSmoothFadeValue(1.0f), d = GFSmoothFadeValue(0.25f);')
