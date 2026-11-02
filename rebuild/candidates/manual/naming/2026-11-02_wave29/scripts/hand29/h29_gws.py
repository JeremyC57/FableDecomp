# h29_gws.py: naming wave 29, family gws (scope 3; DECISIONS_W28 D1: "the remaining GetPRandomSeed binders move to it in a later wave, row by
#  row"): 0x0049D810 is Ego_r.pdb / FableWin.pdb ?GetWorldSeed@CWorld@@QBEPAKXZ (wave 9; 33 landed rows bind it). engine/CWorld.h declares it
#  (the generator's CWorld "members" entry needs the same line: gen_class_headers.py, beside GetFrame), so the two rows that called it through
#  invented derived structs (CWorld_Methods::GetPRandomSeed QAEPAIXZ, WorldMethods::GetPRandomSeed QAEPAKXZ) call the header member.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he, hdr
from common import note
hdr.edit('engine/CWorld.h', '    static long __fastcall GetFrame();                       // 0x0049D870 ?GetFrame@CWorld@@SIJXZ\n',
         '    static long __fastcall GetFrame();                       // 0x0049D870 ?GetFrame@CWorld@@SIJXZ\n'
         '    unsigned long* GetWorldSeed() const;                     // 0x0049D810 ?GetWorldSeed@CWorld@@QBEPAKXZ\n')
he.edit('00914c60', 'src', 'struct CWorld_Methods : CWorld { unsigned int* GetPRandomSeed(); };          // 0x0049D810\n', '')
he.edit('00914c60', 'src', 'unsigned int* seed=((CWorld_Methods*)&PeekWorld())->GetPRandomSeed();', 'unsigned long* seed=PeekWorld().GetWorldSeed();')
he.edit('00914c60', 'src', 'CWorld::GetPRandomSeed 0x0049D810', 'CWorld::GetWorldSeed 0x0049D810')
note('00914c60', 'gws', 'the seed comes from the const ?GetWorldSeed@CWorld@@QBEPAKXZ 0x0049D810 (Ego_r.pdb / FableWin.pdb;\n'
     '//   engine/CWorld.h) through PeekWorld(). Was the invented CWorld_Methods::GetPRandomSeed (QAEPAIXZ).')
he.edit('008a7e00', 'src', 'struct WorldMethods:CWorld{unsigned long* GetPRandomSeed();}; //0049D810\n', '')
he.edit('008a7e00', 'src', 'RandomIndex(reinterpret_cast<WorldMethods*>(World)->GetPRandomSeed(),count)', 'RandomIndex(World->GetWorldSeed(),count)')
note('008a7e00', 'gws', 'the seed comes from the const ?GetWorldSeed@CWorld@@QBEPAKXZ 0x0049D810 (Ego_r.pdb / FableWin.pdb;\n'
     '//   engine/CWorld.h). Was the invented WorldMethods::GetPRandomSeed (QAEPAKXZ).')
