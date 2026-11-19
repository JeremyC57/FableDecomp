X='result.x = b.Row[1].x * y + b.Row[2].x * z + b.Row[0].x * x + b.Translation.x;'
Y='result.y = b.Row[1].y * y + b.Row[0].y * x + b.Row[2].y * z + b.Translation.y;'
Z='result.z = b.Row[1].z * y + b.Row[0].z * x + b.Row[2].z * z + b.Translation.z;'
def sw(s):
    s=s.replace(X,'result.x = b.Row[2].x * z + b.Row[1].x * y + b.Row[0].x * x + b.Translation.x;')
    s=s.replace(Y,'result.y = b.Row[0].y * x + b.Row[1].y * y + b.Row[2].y * z + b.Translation.y;')
    s=s.replace(Z,'result.z = b.Row[0].z * x + b.Row[1].z * y + b.Row[2].z * z + b.Translation.z;')
    return s
VARIANTS=[
 ('swap12', sw),
 ('zneg', lambda s: s.replace('z *= -1.0f;','z = -z;')),
 ('ymul_first', lambda s: s.replace(X,'result.x = y * b.Row[1].x + z * b.Row[2].x + x * b.Row[0].x + b.Translation.x;').replace(Y,'result.y = y * b.Row[1].y + x * b.Row[0].y + z * b.Row[2].y + b.Translation.y;').replace(Z,'result.z = y * b.Row[1].z + x * b.Row[0].z + z * b.Row[2].z + b.Translation.z;')),
]
