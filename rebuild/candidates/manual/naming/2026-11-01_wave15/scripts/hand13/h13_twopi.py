# family twopi (hand part): TWO_PI 0x0124F2B8 ?TWO_PI@@3NB (ESTABLISHED_SPELLINGS.tsv; replaces g_twoPi / NCameraModes::Scale).
# 0x0089BDF0 is done by fam_glob.py twopi. HE_OV=<overlay>
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all
# 0x00A0C450 defined a TU-local copy (internal symbol _g_twoPi): the shared extern constant instead (same bytes; one name)
edit('00a0c450', 'src', 'const double g_twoPi = 6.2831854820251465;           // (double)(2 * 3.1415927f)',
     'extern const double TWO_PI;                           // 0x0124F2B8 (double)(2 * 3.1415927f); established spelling (the TU defined a local copy before naming wave 13)')
edit_all('00a0c450', 'src', 'g_twoPi', 'TWO_PI', 4)
# 0x00853830 declared it as NCameraModes::Scale: the global constant is declared at global scope
edit('00853830', 'src', '#pragma intrinsic(sin,cos)\nnamespace NCameraModes {',
     '#pragma intrinsic(sin,cos)\nextern const double TWO_PI;   // 0x0124F2B8 (established spelling; was NCameraModes::Scale)\nnamespace NCameraModes {')
edit('00853830', 'src', 'C3DVector position;};extern const double Scale;', 'C3DVector position;};')
edit('00853830', 'src', 'double scale=Scale;', 'double scale=TWO_PI;')
