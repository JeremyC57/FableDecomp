#include <math.h>
#include <stdio.h>
#include <string.h>
struct C3DVector {
    float X,Y,Z;
    C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X-o.X,Y-o.Y,Z-o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X+o.X,Y+o.Y,Z+o.Z); }
    C3DVector& operator*=(float scale) { X*=scale; Y*=scale; Z*=scale; return *this; }
    void Normalise() { *this *= 1.0f/(float)sqrt(X*X+Y*Y+Z*Z); }
};

struct CMatrix3x4 {
    float M[4][3];
    C3DVector GetPosition() const { return C3DVector(M[3][0],M[3][1],M[3][2]); }
    C3DVector RotateRow(const float* row) const
    {
        return C3DVector(row[0]*M[0][0]+row[1]*M[1][0]+row[2]*M[2][0],
            row[0]*M[0][1]+row[1]*M[1][1]+row[2]*M[2][1],row[0]*M[0][2]+row[1]*M[1][2]+row[2]*M[2][2]);
    }
    void TransformPoint(C3DVector& point) const
    {
        const float row[]={point.X,point.Y,point.Z};
        point=RotateRow(row)+GetPosition();
    }
};
unsigned long Calc_004014a0(unsigned long,const char*,unsigned long);
struct CCharString {
    unsigned long GetCRC() const { return Calc_004014a0(0,Text,(unsigned long)strlen(Text)); }
    const char* Text;
    explicit CCharString(const char* text):Text(text) {}
    const char* GetData() const { return Text; }
    unsigned long Length() const { return (unsigned long)strlen(Text); }
};
unsigned long Calc_004014a0(unsigned long seed,const char* text,unsigned long count)
{
    return seed==0 && count==4 && memcmp(text,"head",4)==0 ? 123 : 0;
}
struct C3DDummyObject { CMatrix3x4 Transform; const CMatrix3x4& PeekTransform() const { return Transform; } };
struct SDummyEntry { C3DDummyObject second; };
struct C3DAnimationBlendState { int State; };
struct C3DAnimationManager { int Calls; };
struct C3DMeshStats {
    SDummyEntry Dummies[2];
    mutable unsigned long LastKey;
    mutable bool Valid;
    const SDummyEntry* FindDummyObjectsBegin(unsigned long key) const { LastKey=key; return Dummies; }
    void GetDummyObjectPositionAnimated(const C3DDummyObject& dummy,C3DDummyObject& out,
         const C3DAnimationBlendState& s1,const C3DAnimationBlendState& s2,float blend,C3DAnimationManager& manager,bool flag) const
    {
        Valid=s1.State==1&&s2.State==2&&blend==0.5f;
        ++manager.Calls; out=dummy; out.Transform.M[3][0]+=blend*(flag?4.0f:2.0f);
    }
};
struct C3DMeshObject {
    const C3DMeshStats* Mesh;
    CMatrix3x4 PositionMatrix;
    void GetDummyObjectPositionAnimated(const CCharString&,long,const C3DAnimationBlendState&,const C3DAnimationBlendState&,
        float,C3DAnimationManager&,C3DVector&,C3DVector&,C3DVector&,bool) const;
};

#define E11 M[0][0]
#define E12 M[0][1]
#define E13 M[0][2]
#define E21 M[1][0]
#define E22 M[1][1]
#define E23 M[1][2]
#define E31 M[2][0]
#define E32 M[2][1]
#define E33 M[2][2]
#define E41 M[3][0]
#define E42 M[3][1]
#define E43 M[3][2]

void C3DMeshObject::GetDummyObjectPositionAnimated(const CCharString& name, long n, const C3DAnimationBlendState& anim_state,
    const C3DAnimationBlendState& anim_state_end, float interpolate, C3DAnimationManager& anim_manager, C3DVector& dummy_pos,
    C3DVector& dummy_direction, C3DVector& dummy_up, bool use_morph_scale) const
{
    const C3DDummyObject* dummy = &Mesh->FindDummyObjectsBegin(name.GetCRC())[n].second;
    C3DDummyObject dest;
    Mesh->GetDummyObjectPositionAnimated(*dummy, dest, anim_state, anim_state_end, interpolate, anim_manager, use_morph_scale);
    const CMatrix3x4& mat = dest.PeekTransform();
    dummy_pos = C3DVector(mat.E41, mat.E42, mat.E43);
    PositionMatrix.TransformPoint(dummy_pos);
    C3DVector direction, up;
    direction.X = mat.E22 * PositionMatrix.E21 + mat.E23 * PositionMatrix.E31 + mat.E21 * PositionMatrix.E11;
    direction.Y = mat.E22 * PositionMatrix.E22 + mat.E23 * PositionMatrix.E32 + mat.E21 * PositionMatrix.E12;
    direction.Z = mat.E22 * PositionMatrix.E23 + mat.E23 * PositionMatrix.E33 + mat.E21 * PositionMatrix.E13;
    up.X = mat.E32 * PositionMatrix.E21 + mat.E33 * PositionMatrix.E31 + mat.E31 * PositionMatrix.E11;
    up.Y = mat.E32 * PositionMatrix.E22 + mat.E33 * PositionMatrix.E32 + mat.E31 * PositionMatrix.E12;
    up.Z = mat.E32 * PositionMatrix.E23 + mat.E33 * PositionMatrix.E33 + mat.E31 * PositionMatrix.E13;
    dummy_direction = direction;
    dummy_up = up;
    dummy_direction.Normalise();
    dummy_up.Normalise();
}

int main()
{
    C3DMeshStats mesh;
    const CMatrix3x4 first={{{1,0,0},{0,0,2},{0,-3,0},{0,0,0}}};
    const CMatrix3x4 second={{{1,0,0},{0,0,2},{0,-3,0},{2,1,3}}};
    mesh.Dummies[0].second.Transform=first;
    mesh.Dummies[1].second.Transform=second;
    C3DMeshObject object={&mesh,{{{0,1,0},{-1,0,0},{0,0,1},{10,0,0}}}};
    C3DAnimationBlendState a={1},b={2};
    C3DAnimationManager animation={0};
    C3DVector pos,up,forward;
    object.GetDummyObjectPositionAnimated(CCharString("head"),1,a,b,0.5f,animation,pos,up,forward,true);
    if(!mesh.Valid || mesh.LastKey!=123 || animation.Calls!=1 ||
        pos.X!=9 || pos.Y!=4 || pos.Z!=3 || up.X!=0 || up.Y!=0 || up.Z!=1 ||
        forward.X!=1 || forward.Y!=0 || forward.Z!=0)
        return 1;
    object.GetDummyObjectPositionAnimated(CCharString("head"),0,a,b,0.5f,animation,pos,up,forward,false);
    if(animation.Calls!=2 || pos.X!=10 || pos.Y!=1 || pos.Z!=0)
        return 2;
    puts("00a36a80_TEST PASS");
    return 0;
}
