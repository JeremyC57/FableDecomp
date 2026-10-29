# wave 7 hand edits: CIntelligentPointer<T>::operator= identities (coordinator RELOC_CONFLICTs 2026-10-29)
#   0x004A9760 ??4?$CIntelligentPointer@VCThing@@@@QAEAAV0@ABV0@@Z (FableWin.pdb; labels_landed_names_2026-10-24: caller member
#              CCategory+0x0 is CIntelligentPointer<CThing>; already bound by 007c9180); ctor 0x004A96F0
#              ??0?$CIntelligentPointer@VCThing@@@@QAE@PAVCThing@@@Z (FableWin.pdb, bound)
#   0x0048BD20 ??4?$CIntelligentPointer@VCThingPlayerCreature@@@@QAEAAV0@ABV0@@Z (FableWin.pdb; Ego_r
#              CAIStateGroup_BanditKingThrowWeapons +0x14 Player is CIntelligentPointer<CThingPlayerCreature>); ctor 0x0048BD00
#   0x0050F440 ??4?$CIntelligentPointer@VCThingAICreature@@@@QAEAAV0@ABV0@@Z (FableWin.pdb; Ego_r CTCDummyVillager +0x30
#              Creature is CIntelligentPointer<CThingAICreature>); ctor 0x0050F3F0
#   The four 26-byte bodies sit at four addresses (no ICF), so each instance keeps its own T.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he7 import edit, edit_n, word, ip_specialize
from he import path
def specialized(a, T):
    p, _ = path(a, 'src')
    return ('template <> class CIntelligentPointer<%s>' % T) in open(p, encoding='utf-8', errors='surrogateescape').read()

for a, s in (('0095e790', 'CIntelligentPointerThing'), ('0096f770', 'CIntelligentPointerThing'), ('0096fa90', 'CIntelligentPointerThing'),
             ('00646709', 'CIntelligentPointer_CThing'), ('00787e12', 'CIntelligentPointer_CThing'),
             ('00788229', 'CIntelligentPointer'), ('007e00d0', 'CIntelligentPointer'), ('008597a0', 'CIntelligentPointer'),
             ('00954470', 'CIntelligentPointer')):
    ip_specialize(a, s, 'CThing')

# 00961550 CAIStateGroup_BanditKingThrowWeapons::IsRunnable: the Player pointer is CIntelligentPointer<CThingPlayerCreature>
F = '00961550'
if not specialized(F, 'CThingPlayerCreature'):
  edit(F, 'src', '    CIntelligentPointerCreature(CThing* item);                  // 0x0048BD00\n',
     '    CIntelligentPointerCreature(CThingPlayerCreature* item);    // 0x0048BD00\n')
  edit(F, 'src', 'struct CIntelligentPointerCreature : CBaseIntelligentPointer   // CIntelligentPointer<CThingCreatureBase>\n',
     'struct CIntelligentPointerCreature : CBaseIntelligentPointer   // CIntelligentPointer<CThingPlayerCreature> (Ego_r member type)\n')
edit(F, 'src', '    CThing* hero = GetPPlayerManager()->GetMainPlayer().GetPControlledCreature();\n',
     '    CThingPlayerCreature* hero = GetPPlayerManager()->GetMainPlayer().GetPControlledCreature();\n')
ip_specialize(F, 'CIntelligentPointerCreature', 'CThingPlayerCreature')
edit(F, 'src', '// CIntelligentPointer<CThingCreatureBase>::CIntelligentPointer(CThingCreatureBase*) @ 0x0048BD00 and ::operator= @ 0x0048BD20\n',
     '// CIntelligentPointer<CThingPlayerCreature>::CIntelligentPointer(CThingPlayerCreature*) @ 0x0048BD00 and ::operator= @ 0x0048BD20\n')

# 0065f01d CTCDummyVillager::Evict: Creature is CIntelligentPointer<CThingAICreature>
F = '0065f01d'
edit(F, 'src', 'namespace N_0050f3f0 { namespace N_0050f440 { struct CIntelligentPointerCreature : CBaseIntelligentPointer {\n'
     '    CIntelligentPointerCreature(CThing* thing);   // 0x0050F3F0\n'
     '    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other);   // 0x0050F440\n'
     '}; } using N_0050f440::CIntelligentPointerCreature; } using N_0050f3f0::CIntelligentPointerCreature;\n',
     'class CThingAICreature;\ntemplate <class T> class CIntelligentPointer;\n'
     'template <> class CIntelligentPointer<CThingAICreature> : public CBaseIntelligentPointer { public:\n'
     '    CIntelligentPointer(CThingAICreature* item);   // 0x0050F3F0\n'
     '    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x0050F440\n};\n')
edit(F, 'src', '    CIntelligentPointerCreature Creature;   // +0x30\n', '    CIntelligentPointer<CThingAICreature> Creature;   // +0x30\n')
edit(F, 'src', '    Creature = (CThing*)0;\n', '    Creature = (CThingAICreature*)0;\n')
