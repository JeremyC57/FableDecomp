# fam_gmp2 hand part: two rows whose 0x00449970 call goes through a cast chain fam_gmp2 does not parse.
# 008916a0: the lookup-source class IS the player manager (its only member is the getter): class CPlayerManager, CPlayer& GetMainPlayer().
# 008902e0: the `World` view of [this+0x14] is the player manager: the call becomes CPlayerManager::GetMainPlayer on the same pointer.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all
edit('008916a0', 'src', 'class CGSISheatheHeroWeapons_LookupSource\n{\npublic:\n    CPlayer* BeginLookup();\n};',
     'class CPlayerManager\n{\npublic:\n    CPlayer& GetMainPlayer();   // 0x00449970 (Ego_r 0x004450F0)\n};')
edit_all('008916a0', 'src', 'CGSISheatheHeroWeapons_LookupSource*', 'CPlayerManager*')
edit('008916a0', 'src', '            ->BeginLookup()->GetPControlledCreature();', '            ->GetMainPlayer().GetPControlledCreature();')
edit('008902e0', 'src', 'struct World        { CPlayer* GetWorldAccessor(); };                /* 0x00449970 */',
     'class CPlayerManager { public: CPlayer& GetMainPlayer(); };       /* 0x00449970 (Ego_r 0x004450F0) */')
edit('008902e0', 'src', 'CPlayer* accessor = ((World*)(*(void**)((char*)self + 0x14)))->GetWorldAccessor();',
     'CPlayer* accessor = &((CPlayerManager*)(*(void**)((char*)self + 0x14)))->GetMainPlayer();')
edit('008902e0', 'src', '(GetWorldAccessor -> GetHero)', '(CPlayerManager::GetMainPlayer -> GetHero)')
