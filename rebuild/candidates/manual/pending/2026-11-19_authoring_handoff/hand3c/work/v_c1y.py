Z='    float rz = b.Row[1].z * y; rz += b.Row[0].z * x; rz += b.Row[2].z * z; rz += b.Translation.z;'
R='    result.x = rx; result.y = ry; result.z = rz;'
def f1(s):
    s=s.replace(Z,'    float rz = b.Row[1].z * y; rz += b.Row[0].z * x; rz += b.Row[2].z * z;')
    return s.replace(R,'    result.x = rx; result.y = ry; result.z = rz + b.Translation.z;')
def f2(s):
    return s.replace(Z,'    float rz = b.Row[1].z * y; rz += b.Row[0].z * x; rz = rz + b.Row[2].z * z + b.Translation.z;')
def f3(s):
    s=s.replace('rx += b.Translation.x;','').replace('ry += b.Translation.y;','').replace(' rz += b.Translation.z;','')
    return s.replace(R,'    result.x = rx + b.Translation.x; result.y = ry + b.Translation.y; result.z = rz + b.Translation.z;')
def f4(s):
    return s.replace(Z,'    float rz = b.Row[1].z * y; rz += b.Row[0].z * x; rz = (rz + b.Row[2].z * z) + b.Translation.z;')
VARIANTS=[('zT_in_result',f1),('zexpr',f2),('allT_in_result',f3),('zparen',f4)]
