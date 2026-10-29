# fam_sm8.py <file>...: small exact-signature respellings (each the only PDB member with that signature; Ego_r + FableWin):
#   0x00999230 ?PathExists@CAFile@@SI_NABVCWideString@@@Z   free `bool __fastcall PathExists(const CWideString&)` -> CAFile static
#   0x0051ED80 ?GetPThingWithUID@CThingManager@@QAEPAVCThing@@_K@Z   GetThingWithUID(__int64 / unsigned __int64) -> GetPThingWithUID(unsigned __int64)
#   0x00A22F80 ?Rotate@CRightHandedSet@@QAEXABVC3DVector@@M@Z   RotateAboutAxis -> Rotate
#   0x0049D810 ?GetWorldSeed@CWorld@@QBEPAKXZ   GetPRandomSeed -> GetWorldSeed (`[world+8] + 0x1613C` = CMainGameComponent::WorldSeed, Ego_r +90428)
#   0x0073ABA0 ?RemoveMode@CTCCreatureModeManager@@QAEXK@Z   RemoveMode(long) -> RemoveMode(unsigned long)
# Idempotent.
import sys, re
def pathexists(t):
    if not re.search(r'\bbool\s+__fastcall\s+PathExists\(\s*const\s+CWideString\s*&', t) or re.search(r'\b(class|struct)\s+CAFile\b', t): return t
    t = re.sub(r'(?<![\w:])PathExists\(', 'CAFile::PathExists(', t)
    t = re.sub(r'\bbool(\s+)__fastcall(\s+)CAFile::PathExists\((\s*const\s+CWideString\s*&\s*\w*\s*)\)\s*;',
               lambda m: 'class CAFile { public: static bool __fastcall PathExists(%s); };' % m.group(3).strip(), t, count=1)
    return t
def thinguid(t):
    if not re.search(r'CThing\s*\*\s*GetThingWithUID\(', t): return t
    t = re.sub(r'(CThing\s*\*\s*(?:CThingManager::)?GetThingWithUID\(\s*)(?:unsigned\s+)?__int64', r'\1unsigned __int64', t)
    return re.sub(r'\bGetThingWithUID\b', 'GetPThingWithUID', t)
def rotate(t):
    if not re.search(r'void\s+RotateAboutAxis\(\s*const\s+C3DVector\s*&', t): return t
    return re.sub(r'\bRotateAboutAxis\b(?!_)', 'Rotate', t)
def wseed(t):
    if not re.search(r'unsigned\s+long\s*\*\s*GetPRandomSeed\(\s*\)\s*const', t): return t
    return re.sub(r'\bGetPRandomSeed\b', 'GetWorldSeed', t)
def rmode(t):
    if not re.search(r'\bvoid\s+RemoveMode\(\s*long\s+\w+\s*\)\s*;\s*//\s*0x0073ABA0', t): return t
    return re.sub(r'(\bvoid\s+(?:CTCCreatureModeManager::)?RemoveMode\(\s*)long(\s+\w+\s*\))', r'\1unsigned long\2', t)
for p in sys.argv[1:]:
    t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read(); o = t
    for f in (pathexists, thinguid, rotate, wseed, rmode): t = f(t)
    if t != o: open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(t); print('edited', p)
    else: print('skip', p, 'nothing to respell')
