# manual3.py (NF326) [--out <dir>]: the three in-class respells written by hand before respell_inclass.py existed (0x00D73970 GetPos,
# 0x00DE3F90 GetPos, 0x00DA8970 GetDataString: local `class CScriptThing` models). Exact text replacements on the landed source -> rows/
# (or --out). Kept so the hand-back's rows/ can be reproduced from the repo: python3 manual3.py --out <dir>; diff <dir> rows/.
import sys
from pathlib import Path
A = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(A / 'tools')); import nf
note = '        // NF326 one-definition respell: the body in the shape of the self row 0x%s (retail\'s out-of-line copy), so this TU\'s pick-any COMDAT equals it.\n'
E = {
 '00d73970': ('        if (!PImp_Data)\n            return NullCoord;\n        return PImp_Data->GetPos();\n',
              note % '004AA980' + '        const CScriptThing* imp = PImp_Data;\n        if (imp == 0)\n            return NullCoord;\n        return imp->GetPos();\n'),
 '00de3f90': ('        if (!PImp.Data)\n            return g_ZeroVector;\n        return PImp.Data->GetPos();\n',
              note % '004AA980' + '        const CScriptThingImpl* imp = PImp.Data;\n        if (imp == 0)\n            return g_ZeroVector;\n        return imp->GetPos();\n'),
 '00da8970': ('        if (!PImp.Data)\n            return CScriptThing::NullString;\n        return PImp.Data->GetDataString();\n',
              note % '004AA900' + '        const CScriptThingImp* imp = PImp.Data;\n        if (imp == 0)\n            return CScriptThing::NullString;\n        return imp->GetDataString();\n'),
}
od = Path(sys.argv[sys.argv.index('--out') + 1]) if '--out' in sys.argv else A / 'rows'
for a, (o, n) in E.items():
    s = Path(nf.src_of(a)).read_text(encoding='utf-8'); assert s.count(o) == 1, a
    (od / (a + '.cpp')).write_text(s.replace(o, n), encoding='utf-8'); print('WROTE', od / (a + '.cpp'))
