# h18_eps_test.py: naming wave 18, family eps -- the behaviour test of 0x008754A0 defines the shared 1e-4f constant the source
# links against: it is DELTA_NOTIONAL_ZERO now (ESTABLISHED_SPELLINGS 0x0129BA3C, chunk 680).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
edit('008754a0', 'tst', 'extern const float EPSILON = 0.0001f;   // 0x0129BA3C (naming wave 13)', 'extern const float DELTA_NOTIONAL_ZERO = 0.0001f;   // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB (naming wave 18; was EPSILON)')
