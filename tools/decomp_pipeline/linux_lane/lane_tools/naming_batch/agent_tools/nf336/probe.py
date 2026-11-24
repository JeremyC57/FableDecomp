# probe.py <addr> <Class> <decl> [control]: override probe (slot condition). Builds work/probe/<addr>.cpp from rows/<addr>.cpp's preamble
# (every #define except the *_RTTI_DECLS opt-in, every #include / #pragma / forward `class|struct|enum X;` line) and appends
#   struct NF323Probe : <Class> { <decl> override; };
# then clang -fsyntax-only on work/tree. PASS = some base of <Class> visible in the row's TU declares that virtual with that signature.
# With `control`, the decl's name is replaced by a made-up one (must FAIL). -> work/probe.log
import re, sys, subprocess, os
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF336')
S = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad')
a, X, decl = sys.argv[1:4]; ctl = len(sys.argv) > 4
if ctl: decl = re.sub(r'(\w+)\s*\(', r'NF323MadeUpName(', decl, count=1)
L = (A / 'rows' / (a + '.cpp')).read_text(encoding='utf-8').split('\n')
keep = [l for l in L if (re.match(r'\s*#\s*(define|include|pragma)', l) and not re.search(r'_(RTTI_DECLS|TRANSFER_DECL|RUMBLE_DECLS|RENDERPRIMITIVE_DECL)\b', l)) or re.match(r'\s*(class|struct|enum)\s+\w+\s*;\s*$', l)]
d = A / 'work/probe'; d.mkdir(exist_ok=True); f = d / (a + ('_ctl' if ctl else '') + '.cpp')
f.write_text('\n'.join(keep + ['struct NF323Probe : %s { %s override; };' % (X, decl.rstrip(';')), '']), encoding='utf-8')
cmd = ['clang++', '-target', 'i686-pc-windows-msvc', '-fms-extensions', '-fms-compatibility', '-fms-compatibility-version=13.10', '-fsyntax-only', '-w',
       '-Wno-address-of-temporary', '-I' + str(A / 'work/tree'), '-isystem', str(S / 'wine/vc71/include'), str(f)]
r = subprocess.run(cmd, capture_output=True, text=True, timeout=90)
errs = [l for l in (r.stdout + r.stderr).splitlines() if ': error' in l]
res = 'PASS' if not errs else 'FAIL'
line = '%s %s %s %s :: %s%s' % (a, 'CONTROL' if ctl else 'PROBE', res, X, decl, ('  [' + errs[0].split('error:')[1].strip()[:120] + ']') if errs else '')
open(A / 'work/probe.log', 'a').write(line + '\n'); print(line)
