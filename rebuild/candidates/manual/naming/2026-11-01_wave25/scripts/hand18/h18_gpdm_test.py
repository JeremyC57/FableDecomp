# h18_gpdm_test.py: naming wave 18, family gpdm -- the behaviour test of 0x004022B0 defines the display-manager pointer the source
# links against: ?GPDisplayManager@@3PAVCDisplayManager@@A (0x013B8390, ChatGPT pass 10 audit; was PDisplayManager).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit_all
edit_all('004022b0', 'tst', 'PDisplayManager', 'GPDisplayManager')
