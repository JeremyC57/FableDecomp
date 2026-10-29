# wave 6 family: CPersistContext::TransferObjectSaveText 0x009BAD60 / TransferObjectLoadText 0x009BBA40 are protected and take
# the wrapper base `class CPersistContext_TransferableComponentGenericType` (Ego_r.pdb
# ?TransferObjectSaveText@CPersistContext@@IAEXPBDAAVCPersistContext_TransferableComponentGenericType@@@Z, LoadText alike;
# struct_layouts_egor.tsv has CPersistContext_TransferableComponentGenericType, 4 bytes = the vptr). The descriptive
# CPersistContext_TransferableObjectBase is in neither PDB. Landed 0x004045C0 / 0x00410620 / 0x004106F0 already use the PDB form.
# Transform (idempotent): `struct|class CPersistContext_TransferableObjectBase {` -> `class CPersistContext_TransferableComponentGenericType { public:`,
# forward declarations and every other spelling renamed; the two member declarations inside CPersistContext get
# `protected: ... public:` around them (the CheckCRC style already used in these rows).
import re, sys
OLD, NEW = 'CPersistContext_TransferableObjectBase', 'CPersistContext_TransferableComponentGenericType'
DEF = re.compile(r'\b(?:struct|class)[ \t]+' + OLD + r'[ \t]*\{([ \t]*public:)?')
FWD = re.compile(r'\b(?:struct|class)[ \t]+' + OLD + r'[ \t]*;')
DECL = re.compile(r'^([ \t]*)(?:virtual[ \t]+)?void[ \t]+(TransferObject(?:Save|Load)Text)[ \t]*\(([^;\n]*)\)[ \t]*;([ \t]*//[^\n]*)?$', re.M)
def fix(t):
    if OLD not in t: return None, 'no ' + OLD
    t = DEF.sub('class ' + NEW + ' { public:', t)
    t = FWD.sub('class ' + NEW + ';', t)
    t = t.replace(OLD, NEW)
    def rep(m):
        if 'virtual' in m.group(0): return m.group(0)
        return '%sprotected: void %s(%s); public:%s' % (m.group(1), m.group(2), m.group(3), m.group(4) or '')
    t = DECL.sub(rep, t)
    return t, 'ok'
if __name__ == '__main__':
    for p in sys.argv[1:]:
        t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
        new, why = fix(t)
        if new is None or new == t: print('skip', p, why); continue
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new); print('edited', p, why)
