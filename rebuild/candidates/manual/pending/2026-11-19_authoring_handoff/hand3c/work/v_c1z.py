A='''    float rx = b.Row[1].x * y; rx += b.Row[2].x * z; rx += b.Row[0].x * x; rx += b.Translation.x;
    float ry = b.Row[1].y * y; ry += b.Row[0].y * x; ry += b.Row[2].y * z; ry += b.Translation.y;
    float rz = b.Row[1].z * y; rz += b.Row[0].z * x; rz += b.Row[2].z * z; rz += b.Translation.z;'''
U='''    float rx = b.Row[1].x * y; rx += b.Row[2].x * z; rx = rx + b.Row[0].x * x + b.Translation.x;
    float ry = b.Row[1].y * y; ry += b.Row[0].y * x; ry = ry + b.Row[2].y * z + b.Translation.y;
    float rz = b.Row[1].z * y; rz += b.Row[0].z * x; rz = rz + b.Row[2].z * z + b.Translation.z;'''
U2='''    float rx = b.Row[1].x * y + b.Row[2].x * z; rx = rx + b.Row[0].x * x + b.Translation.x;
    float ry = b.Row[1].y * y + b.Row[0].y * x; ry = ry + b.Row[2].y * z + b.Translation.y;
    float rz = b.Row[1].z * y + b.Row[0].z * x; rz = rz + b.Row[2].z * z + b.Translation.z;'''
VARIANTS=[('uniform',lambda s: s.replace(A,U)),('uniform2',lambda s: s.replace(A,U2))]
