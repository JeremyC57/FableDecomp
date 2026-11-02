# h31_pek.py: naming wave 31, family pek (c702 queue: NParticleEngine::CParticleEmitter::Kill 0x00AEE8C0). Ego_r.pdb / FableWin.pdb name the
#  emitter class in namespace NParticleEngine (?Kill@CParticleEmitter@NParticleEngine@@QAEXXZ, ?Update@..., ?Initialise@...@@QAE_NPAVCIEngine@@@Z,
#  ?Kill@CParticleSystem@NParticleEngine@@QAEXXZ); the own row 0x00AEE8C0 and its binders 0x00AEE9F0 (Update, also an own-row respell) and
#  0x005440B0 (~CParticleEffect's ??_G) declared a global CParticleEmitter (?Kill@CParticleEmitter@@QAEXXZ). The class moves into the namespace;
#  bare Leafs ("Kill", "Update") become the emitted symbols.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
a = '00aee8c0'
he.edit(a, 'src', '// Leaf: Kill\n', '// Leaf: ?Kill@CParticleEmitter@NParticleEngine@@QAEXXZ\n')
he.edit(a, 'src', 'struct CParticleSystem;\n', 'namespace NParticleEngine { class CParticleSystem; }\nusing NParticleEngine::CParticleSystem;\n')
he.edit(a, 'src', '#pragma pack(push, 1)\nstruct CParticleEmitter {\n', '#pragma pack(push, 1)\nnamespace NParticleEngine {\nclass CParticleEmitter { public:\n')
he.edit(a, 'src', '    void Kill();\n};\n#pragma pack(pop)\nstruct CParticleSystem {\n    void Kill();                            // 0x00AF5870\n};\nvoid CParticleEmitter::Kill()\n',
        '    void Kill();\n};\n}\n#pragma pack(pop)\nnamespace NParticleEngine {\nclass CParticleSystem { public:\n'
        '    void Kill();                            // 0x00AF5870 ?Kill@CParticleSystem@NParticleEngine@@QAEXXZ\n};\n}\n'
        'void NParticleEngine::CParticleEmitter::Kill()\n')
note(a, 'pek', 'Ego_r.pdb / FableWin.pdb ?Kill@CParticleEmitter@NParticleEngine@@QAEXXZ (was the global CParticleEmitter, bare Leaf "Kill");\n'
     '//   its callee is ?Kill@CParticleSystem@NParticleEngine@@QAEXXZ 0x00AF5870 (both PDBs; was the global CParticleSystem)')
a = '00aee9f0'
he.edit(a, 'src', '// Leaf: Update\n', '// Leaf: ?Update@CParticleEmitter@NParticleEngine@@QAEXXZ\n')
he.edit(a, 'src', '#pragma pack(push, 1)\nclass CParticleEmitter {\n', '#pragma pack(push, 1)\nnamespace NParticleEngine {\nclass CParticleEmitter {\n')
he.edit(a, 'src', '    void Update();\n};\n#pragma pack(pop)\nvoid CParticleEmitter::Update()\n',
        '    void Update();\n};\n}\n#pragma pack(pop)\nvoid NParticleEngine::CParticleEmitter::Update()\n')
note(a, 'pek', 'the class is NParticleEngine::CParticleEmitter (Ego_r.pdb / FableWin.pdb): own row ?Update@CParticleEmitter@NParticleEngine@@\n'
     '//   QAEXXZ (bare Leaf "Update" before), callees ?Initialise@CParticleEmitter@NParticleEngine@@QAE_NPAVCIEngine@@@Z 0x00AEE240 and\n'
     '//   ?Kill@CParticleEmitter@NParticleEngine@@QAEXXZ 0x00AEE8C0 (were the global-class spellings)')
a = '005440b0'
he.edit(a, 'src', 'struct CParticleEmitter {\n    void Kill();                                                          // 0x00AEE8C0\n};\n',
        'namespace NParticleEngine {\nclass CParticleEmitter { public:\n    void Kill();                                                          // 0x00AEE8C0 (Ego_r.pdb / FableWin.pdb)\n};\n}\nusing NParticleEngine::CParticleEmitter;\n')
note(a, 'pek', 'the held emitter is NParticleEngine::CParticleEmitter: ?Kill@CParticleEmitter@NParticleEngine@@QAEXXZ 0x00AEE8C0\n'
     '//   (Ego_r.pdb / FableWin.pdb; was the global ?Kill@CParticleEmitter@@QAEXXZ)')
