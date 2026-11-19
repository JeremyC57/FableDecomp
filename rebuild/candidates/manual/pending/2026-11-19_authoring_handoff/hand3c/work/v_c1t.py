A='''    float x = a.Row[2].x * point.z + a.Row[1].x * point.y + a.Row[0].x * point.x + a.Translation.x;
    float y = a.Row[2].y * point.z + a.Row[0].y * point.x + a.Row[1].y * point.y + a.Translation.y;
    float z = a.Row[2].z * point.z + a.Row[0].z * point.x + a.Row[1].z * point.y + a.Translation.z;'''
A2='''    float x = a.Row[2].x * point.z; x += a.Row[1].x * point.y; x += a.Row[0].x * point.x; x += a.Translation.x;
    float y = a.Row[2].y * point.z; y += a.Row[0].y * point.x; y += a.Row[1].y * point.y; y += a.Translation.y;
    float z = a.Row[2].z * point.z; z += a.Row[0].z * point.x; z += a.Row[1].z * point.y; z += a.Translation.z;'''
R='    result.x = rx; result.y = ry; result.z = rz;'
VARIANTS=[('seqboth', lambda s: s.replace(A,A2)),
 ('res_zyx', lambda s: s.replace(R,'    result.z = rz; result.y = ry; result.x = rx;')),
 ('res_decl_late', lambda s: s.replace('    C3DVector result;\n','').replace(R,'    C3DVector result;\n'+R)),
 ('rx_direct', lambda s: s.replace('    float rx = b.Row[1].x * y; rx += b.Row[2].x * z; rx += b.Row[0].x * x; rx += b.Translation.x;','    result.x = b.Row[1].x * y; result.x += b.Row[2].x * z; result.x += b.Row[0].x * x; result.x += b.Translation.x;').replace(R,'    result.y = ry; result.z = rz;'))]
