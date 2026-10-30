# item 2 (hand part): quest-script classes landed in the global namespace -- every CQ_/CV_ script class lives in NScript
# (Ego_r.pdb / FableWin.pdb spellings of the called members; class key V)
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import nscript
nscript('00e0de90', 'src', 'CQ_UndeadRisingScript')          # 0x00E0E040 ?ActivateNextLamp@CQ_UndeadRisingScript@NScript@@QAEXXZ
nscript('00d874f0', 'src', 'CQ_HeroSoulsGuildmasterScript')  # 0x00D872D0 ?PlayCutscene@CQ_HeroSoulsGuildmasterScript@NScript@@QAEXVCCharString@@@Z
nscript('00cffb60', 'src', 'CQ_AwakeningTheOracleScript')    # 0x00CFF9A0 ?PlayCutscene@CQ_AwakeningTheOracleScript@NScript@@QAEXVCCharString@@_N@Z
nscript('00d00050', 'src', 'CQ_AwakeningTheOracleScript')
