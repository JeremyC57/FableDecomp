O='''    C3DVector result;
    result.x = b.Row[1].x * y + b.Row[2].x * z + b.Row[0].x * x + b.Translation.x;
    result.y = b.Row[1].y * y + b.Row[0].y * x + b.Row[2].y * z + b.Translation.y;
    result.z = b.Row[1].z * y + b.Row[0].z * x + b.Row[2].z * z + b.Translation.z;'''
S1='''    C3DVector result;
    float rx = b.Row[1].x * y; rx += b.Row[2].x * z; rx += b.Row[0].x * x; rx += b.Translation.x;
    float ry = b.Row[1].y * y; ry += b.Row[0].y * x; ry += b.Row[2].y * z; ry += b.Translation.y;
    float rz = b.Row[1].z * y; rz += b.Row[0].z * x; rz += b.Row[2].z * z; rz += b.Translation.z;
    result.x = rx; result.y = ry; result.z = rz;'''
S2='''    C3DVector result;
    result.x = b.Row[1].x * y; result.x += b.Row[2].x * z; result.x += b.Row[0].x * x; result.x += b.Translation.x;
    result.y = b.Row[1].y * y; result.y += b.Row[0].y * x; result.y += b.Row[2].y * z; result.y += b.Translation.y;
    result.z = b.Row[1].z * y; result.z += b.Row[0].z * x; result.z += b.Row[2].z * z; result.z += b.Translation.z;'''
VARIANTS=[('seqlocal', lambda s: s.replace(O,S1)),('seqresult', lambda s: s.replace(O,S2))]
