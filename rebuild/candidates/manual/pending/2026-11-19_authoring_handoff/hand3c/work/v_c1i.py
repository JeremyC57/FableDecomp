import re
A='static __declspec(noinline) C3DVector TransformPointThroughMirroredFrame_00c1c330'
def body(new):
    def f(s):
        i=s.index(A); j=s.index('// Same-TU caller')
        return s[:i]+new+'\n'+s[j:]
    return f
H1='''static __declspec(noinline) C3DVector TransformPointThroughMirroredFrame_00c1c330(const C3DMatrix& a, const C3DVector& point, const C3DMatrix& b)
{
    float x = a.Row[2].x * point.z + a.Row[1].x * point.y + a.Row[0].x * point.x + a.Translation.x;
    float y = a.Row[2].y * point.z + a.Row[0].y * point.x + a.Row[1].y * point.y + a.Translation.y;
    float z = a.Row[2].z * point.z + a.Row[0].z * point.x + a.Row[1].z * point.y + a.Translation.z;
    if (z < 0.0f)
        z *= -1.0f;
    C3DVector result;
    result.%s
    result.%s
    result.%s
    return result;
}'''
VX='x = b.Row[1].x * y + b.Row[2].x * z + b.Row[0].x * x + b.Translation.x;'
VY='y = b.Row[1].y * y + b.Row[0].y * x + b.Row[2].y * z + b.Translation.y;'
VZ='z = b.Row[1].z * y + b.Row[0].z * x + b.Row[2].z * z + b.Translation.z;'
NEG='''static __declspec(noinline) C3DVector TransformPointThroughMirroredFrame_00c1c330(const C3DMatrix& a, const C3DVector& point, const C3DMatrix& b)
{
    float x = a.Row[2].x * point.z + a.Row[1].x * point.y + a.Row[0].x * point.x + a.Translation.x;
    float y = a.Row[2].y * point.z + a.Row[0].y * point.x + a.Row[1].y * point.y + a.Translation.y;
    float z = a.Row[2].z * point.z + a.Row[0].z * point.x + a.Row[1].z * point.y + a.Translation.z;
    float mz = z;
    if (z < 0.0f)
        mz = z * -1.0f;
    C3DVector result;
    result.x = b.Row[2].x * mz + b.Row[1].x * y + b.Row[0].x * x + b.Translation.x;
    result.y = b.Row[2].y * mz + b.Row[0].y * x + b.Row[1].y * y + b.Translation.y;
    result.z = b.Row[2].z * mz + b.Row[0].z * x + b.Row[1].z * y + b.Translation.z;
    return result;
}'''
YN='''static __declspec(noinline) C3DVector TransformPointThroughMirroredFrame_00c1c330(const C3DMatrix& a, const C3DVector& point, const C3DMatrix& b)
{
    float x = a.Row[2].x * point.z + a.Row[1].x * point.y + a.Row[0].x * point.x + a.Translation.x;
    float y = a.Row[2].y * point.z + a.Row[0].y * point.x + a.Row[1].y * point.y + a.Translation.y;
    float z = a.Row[2].z * point.z + a.Row[0].z * point.x + a.Row[1].z * point.y + a.Translation.z;
    if (z < 0.0f)
        z *= -1.0f;
    C3DVector result;
    result.x = b.Row[2].x * z + b.Row[1].x * y + b.Row[0].x * x + b.Translation.x;
    result.y = b.Row[2].y * z + b.Row[0].y * x + b.Row[1].y * y + b.Translation.y;
    result.z = b.Row[2].z * z + b.Row[0].z * x + b.Row[1].z * y + b.Translation.z;
    return result;
}'''
VARIANTS=[('zyx', body(H1%(VZ,VY,VX))),('mz', body(NEG)),('helperorder', body(YN))]
