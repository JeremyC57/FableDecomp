# sub-wave mig8 follow-up (hand): CMurderTwistSheriff::Main 0x00EAEF30 calls its private members by the PDB names (Ego_r.pdb /
# FableWin.pdb order = retail order 0x00EAF810 .. 0x00EB1EC0): DoRemindingHeroToKillSheriff -> DoGivingRewardToHero (0x00EB15C0),
# DoSuspicious -> DoSayingWellDoneToHero (0x00EB19C0), DoFightHero -> AttackHero (0x00EB1EC0); all six are private (AAE).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import resub
A = '00eaef30'
resub(A, 'src', r'\bDoRemindingHeroToKillSheriff\b', 'DoGivingRewardToHero')
resub(A, 'src', r'\bDoSuspicious\b', 'DoSayingWellDoneToHero')
resub(A, 'src', r'\bDoFightHero\b', 'AttackHero')
from he import edit
edit(A, 'src', '''public:
    void DoAskingHeroToKillAssassin();                 // 0x00EAF810''', '''private:   // the six members are private in Ego_r.pdb / FableWin.pdb (AAE; naming wave 11)
    void DoAskingHeroToKillAssassin();                 // 0x00EAF810''')
