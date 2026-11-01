# h19_sfix.py: naming wave 19, family stools (addendum) -- the filter of the 0x007FD110 GetPFirstThingInRoughArea instantiation.
#   The landed 0x007FD110 row is the <CThingFilter_HasScriptName, CThing> instance (its filter operator 0x006A7E30 compares the thing's
#   ScriptName at retail CThing+0x74: labels_leaf_sync / sweep3), the <CThingFilter_HasDefGlobalIndex, CThing> instance is 0x007FCEF0
#   (operator 0x006A70B0). Four callers bind 0x007FD110 under a string-carrying filter class with the wrong name: 0x0095B020 and
#   0x00972830 call it CThingFilter_HasDefGlobalIndex (a CCharString ctor), 0x007FBC90 and 0x0095AB50 call it CThingFilter_HasTrackName
#   (so do 0x008C2240 and 0x0095B3E0, modelled on 0x007FBC90; CThingFilter_HasTrackName is the 0x0093CDB0 GetPBestThingInRoughArea filter); the same spelling was bound to both 0x007FCEF0
#   (by 0x007FBC90) and 0x007FD110 (by 0x00972830) -- the CONFLICT mkbind19 reported. Class rename only (inline ctor, no bytes).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_sfix.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())
NOTE = '   // 0x007FD110 is the <CThingFilter_HasScriptName, CThing> instance (filter operator 0x006A7E30 compares the ScriptName; naming wave 19)\n'
for a, old in (('0095b020', 'CThingFilter_HasDefGlobalIndex'), ('00972830', 'CThingFilter_HasDefGlobalIndex'),
               ('007fbc90', 'CThingFilter_HasTrackName'), ('0095ab50', 'CThingFilter_HasTrackName'),
               ('008c2240', 'CThingFilter_HasTrackName'), ('0095b3e0', 'CThingFilter_HasTrackName')):   # 008c2240 copied the 007fbc90 model (same wrong filter name at 0x007FD110)
    if a in _X: continue
    p, rel, t = he._rw(a, 'src')
    if 'CThingFilter_HasScriptName' in t: print('already', a)
    else:
      t = re.sub(r'\b' + old + r'\b', 'CThingFilter_HasScriptName', t)
      t = t.replace('<HasTrackName> @ 0x007FD110', '<HasScriptName> @ 0x007FD110')
    # one note line after the first line that declares / documents the 0x007FD110 member
      m = re.search(r'^[^\n]*0x007FD110[^\n]*\n', t, re.M)
      if m: t = t[:m.end()] + NOTE.replace('   //', '//' if m.group(0).startswith('//') else '   //', 1) + t[m.end():]
      he._w(p, t); print('ok', a)
    if a == '0095ab50':
        he.resub(a, 'tst', r'\bCThingFilter_HasTrackName\b', 'CThingFilter_HasScriptName')
        he.resub(a, 'src', r'manifest <CThingFilter_HasDefGlobalIndex, CThing>(?!, in fact)', 'manifest <CThingFilter_HasDefGlobalIndex, CThing>, in fact <CThingFilter_HasScriptName, CThing>')
