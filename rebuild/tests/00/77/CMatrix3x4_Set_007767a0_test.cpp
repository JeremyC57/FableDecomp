// Behaviour fixture: original math types and selected target body; ABI is checked by the actual-header grade.
#include <cstdio>
#include <cstdlib>
#include <cstring>
class C3DVector { public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
    inline const C3DVector operator^(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
};
class CRightHandedSet {protected:C3DVector Up,Forward;public:
    CRightHandedSet(const C3DVector& up,const C3DVector& forward):Up(up),Forward(forward){}
    const C3DVector& GetForward()const{return Forward;}
    const C3DVector& GetUp()const{return Up;}
    C3DVector GetRight()const{return Forward^Up;}
};
class CMatrix3x4 { public:
    union {struct {float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;};float Data[4][3];};
    void Set(const CRightHandedSet& rhset,const C3DVector& translate);
    inline void Set(const C3DVector& right,const C3DVector& forward,const C3DVector& up,const C3DVector& translate) {
        float* axis=Data[0]+1;
        *--axis=right.X; E12=right.Y; E13=right.Z;
        E21=forward.X; E22=forward.Y; E23=forward.Z;
        E31=up.X; E32=up.Y; E33=up.Z;
        E41=translate.X; E42=translate.Y; E43=translate.Z;
    }
};
void CMatrix3x4::Set(const CRightHandedSet& rhset,const C3DVector& translate)
{
    Set(rhset.GetRight(), rhset.GetForward(), rhset.GetUp(), translate);
}

static void Check(bool value){if(!value)std::abort();}
int main(){
    const float coords[][3]={{0,0,0},{1,0,0},{0,1,0},{0,0,1},{-2,3,4},{4,-2,1},{-3,-1,2}};
    unsigned cases=0;
    for(unsigned u=0;u<7;++u)for(unsigned f=0;f<7;++f)for(unsigned t=0;t<7;++t){
        C3DVector up(coords[u][0],coords[u][1],coords[u][2]);
        C3DVector forward(coords[f][0],coords[f][1],coords[f][2]);
        C3DVector translate(coords[t][0],coords[t][1],coords[t][2]);
        CRightHandedSet axes(up,forward);CMatrix3x4 matrix;std::memset(&matrix,0xCD,sizeof(matrix));
        matrix.Set(axes,translate);
        Check(matrix.E11==forward.Y*up.Z-forward.Z*up.Y);
        Check(matrix.E12==forward.Z*up.X-forward.X*up.Z);
        Check(matrix.E13==forward.X*up.Y-forward.Y*up.X);
        Check(matrix.E21==forward.X&&matrix.E22==forward.Y&&matrix.E23==forward.Z);
        Check(matrix.E31==up.X&&matrix.E32==up.Y&&matrix.E33==up.Z);
        Check(matrix.E41==translate.X&&matrix.E42==translate.Y&&matrix.E43==translate.Z);
        ++cases;
    }
    Check(cases==343);std::puts("007767a0_TEST PASS");return 0;
}