# h26_gst.py: naming wave 26, family gst -- two GetSupportedTypes overrides that call vector<long>::_M_insert_overflow 0x00440990
#  (c689 queue row, scope 2) re-authored on the landed family-J027 model of CEnginePrimitiveManager2DAnimatedMeshes::GetSupportedTypes
#  0x00BD28A0 (same 122-byte shape: two inline CEnginePrimitiveTypeList::Add calls): the own row becomes the PDB
#  ?GetSupportedTypes@<class>@@UBEXAAVCEnginePrimitiveTypeList@@@Z (Ego_r.pdb; RTTI vtable slot 2) and the grow-insert the STLport
#  ?_M_insert_overflow@?$vector@JV?$allocator@J@std@@@std@@IAEXPAJABJABU__true_type@2@I_N@Z.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he, rows
R = rows.rows()
base = open(os.path.join(rows.ROOT, 'rebuild/src/compiled', R['00bd28a0']['Source']), encoding='utf-8').read()
code = base[base.index('extern "C" void* __cdecl malloc'):]
code = code[:code.index('class CEnginePrimitiveManagerOcclusionBase : public CEnginePrimitiveManager {')]
def row(a, cls, vt, chain, vals, desc, was):
    A = a.upper()
    head = ('// %s::GetSupportedTypes(CEnginePrimitiveTypeList& types) const -- report the primitive types this manager creates: append %s\n'
            '//   to the caller\'s type list\n'
            '// Leaf: ?GetSupportedTypes@%s@@UBEXAAVCEnginePrimitiveTypeList@@@Z\n'
            '// Retail: Fable.exe (TLC, VC7.1) function @ 0x%s, 122 bytes, thiscall (`this` unused), `ret 4`.\n'
            '// Identity (naming wave 26, family gst): slot 2 of vtable 0x%s (RTTI .?AV%s@@), the GetSupportedTypes slot; Ego_r.pdb\n'
            '//   ?GetSupportedTypes@%s@@UBEXAAVCEnginePrimitiveTypeList@@@Z. The body appends %s (%s). Class chain from RTTI:\n'
            '//   %s <- CEnginePrimitiveManager <- CEnginePrimitiveRenderUtil <- CBaseClassNonCopyable <- CBaseClass.\n'
            '// Callees: STLport vector<long>::_M_insert_overflow 0x00440990 (?_M_insert_overflow@?$vector@JV?$allocator@J@std@@@std@@IAEXPAJABJABU__true_type@2@I_N@Z,\n'
            '//   protected; same-TU body kept out of line).\n'
            '// Model: the landed family-J027 row 0x00BD28A0 (CEnginePrimitiveManager2DAnimatedMeshes::GetSupportedTypes, the same 122-byte shape):\n'
            '//   CEnginePrimitiveTypeList holds one 12-byte STLport vector<long> at +0 (member name descriptive) and its inline\n'
            '//   Add(EEnginePrimitiveType) is the PDB ?Add@CEnginePrimitiveTypeList@@QAEXW4EEnginePrimitiveType@@@Z.\n'
            '// Shape: each Add binds the enum-to-long temporary (the first homed in the `push ecx` word, the second in the dead `types` slot).\n'
            '// Was %s.\n') % (cls, ' and '.join(vals), cls, A, vt, cls, cls, ' then '.join(vals), desc, cls, was)
    tail = ''
    prev = 'CEnginePrimitiveManager'
    for c in chain:
        tail += 'class %s : public %s {\n};\n' % (c, prev); prev = c
    tail += ('class %s : public %s {   // RTTI .?AV%s@@, vtable 0x%s\npublic:\n'
             '    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const;   // slot 2\n};\n\n'
             'void %s::GetSupportedTypes(CEnginePrimitiveTypeList& types) const\n{\n') % (cls, prev, cls, vt, cls)
    for v in vals: tail += '    types.Add((EEnginePrimitiveType)%s);\n' % v
    tail += '}\n'
    he.put(a, 'src', head + code + tail)
row('00bb1640', 'CEnginePrimitiveManager2DSprites', '012A5664', [], ['0x22', '0x23'], '2D sprite, 2D clock sprite',
    'CEnginePrimitiveManager2DSprites::GetSupportedTypes(CArray<long>&) over a CArray<T> stand-in with InsertGrow(..., SScalarTag, unsigned long) and a\n//   GrowArrayStorage stand-in body')
row('00bc7920', 'CEnginePrimitiveManagerText', '012A68B0', [], ['5', '0x27'], '3D text, 2D text',
    'CEnginePrimitiveManagerText::GetSupportedTypes(CRetailVector_long&) over a CRetailVector_long stand-in with InsertOverflow(..., SScalarTag,\n//   unsigned long) and a GrowArrayStorage stand-in body')
