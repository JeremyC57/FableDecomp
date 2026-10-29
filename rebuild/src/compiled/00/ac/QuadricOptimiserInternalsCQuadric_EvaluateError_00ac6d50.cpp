// 00ac6d50 QuadricOptimiserInternals::CQuadric::EvaluateError
// Flags:
// Leaf: ?EvaluateError@CQuadric@QuadricOptimiserInternals@@QBENABVC3DVector@@@Z
#pragma pack(push,1)
class C3DVector{public:float X,Y,Z;};
#pragma pack(pop)
namespace QuadricOptimiserInternals{
// Layout: original donor88bytes, double AA0/AB8/AC16/AD24/BB32/BC40/BD48/
// CC56/CD64/DD72/Weight80. No engine header exists for this nested class.
class CQuadric{private:double AA,AB,AC,AD,BB,BC,BD,CC,CD,DD,Weight;
public:
 double EvaluateError(const C3DVector&)const;
};
double CQuadric::EvaluateError(const C3DVector&v)const{
 float x=v.X,y=v.Y,z=v.Z;
 return (z*z)*CC+(y*z*2.0f)*BC+(y*y)*BB+(x*z*2.0f)*AC+(x*y*2.0f)*AB
       +(z*2.0f)*CD+(y*2.0f)*BD+(x*2.0f)*AD+(x*x)*AA+DD;
}
}