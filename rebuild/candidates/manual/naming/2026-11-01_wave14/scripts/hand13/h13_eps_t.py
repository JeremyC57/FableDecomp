# family eps (hand part): tests that link the respelled source define EPSILON 0x0129BA3C (?EPSILON@@3MB). HE_OV=<overlay>
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
edit('008754a0', 'tst', 'float g_fIntersectionEpsilon = 0.0001f;', 'extern const float EPSILON = 0.0001f;   // 0x0129BA3C (naming wave 13)')
