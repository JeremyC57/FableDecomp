I='#define FABLE_RETAIL_CMATRIX4X4\n'
OLD="""    E11 *= scale.X; E12 *= scale.X; E13 *= scale.X;
    E21 *= scale.Y; E22 *= scale.Y; E23 *= scale.Y;
    E31 *= scale.Z; E32 *= scale.Z; E33 *= scale.Z;"""
def body(pfx):
    L=[]
    for r,c in ((0,'X'),(1,'Y'),(2,'Z')):
        for k in range(3): L.append(f"    {pfx}({r}, {k}) *= scale.{c};")
    return "\n".join(L)
VARIANTS=[
 ('opcall', lambda s: s.replace(I, I+'#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS\n').replace(OLD, body('(*this)'))),
 ('opcall_ref', lambda s: s.replace(I, I+'#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS\n').replace(OLD, "    CMatrix4x4& matrix = *this;\n"+body('matrix'))),
]
