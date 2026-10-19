// CNavigatorBase::IsAnyPartOfNodeWithinDistOfPos -- test node square against a radius and produce a candidate point.
// Retail00a471d0 length283. Actual navigator/node headers and native PDB const
// signature used. CalleesA14510 C2DVector::AccurateNormalise, A75B30 node GetSize const.
// Pool129ba3c is0.0001f. C2DVector PDB ctor/copy/operator signatures audited.
#include <math.h>
#define CNavigatorBase CNavigatorBaseLayout
#include "engine/CNavigatorBase.h"
#undef CNavigatorBase
#define CANavQuadTreeNode CANavQuadTreeNodeLayout
#include "engine/CANavQuadTreeNode.h"
#undef CANavQuadTreeNode
struct C2DVector;
struct C3DVector {float X,Y,Z; C3DVector(float x,float y,float z):X(x),Y(y),Z(z){} C3DVector operator+(const C2DVector&) const;};
struct C2DVector {
    float X,Y;
    C2DVector(float x,float y):X(x),Y(y) {}
    C2DVector(const C2DVector& p):X(p.X),Y(p.Y) {}
    C2DVector(const C3DVector& p):X(p.X),Y(p.Y) {}
    C2DVector operator-(const C2DVector& p) const {return C2DVector(X-p.X,Y-p.Y);}
    C2DVector operator+(const C2DVector& p) const {return C2DVector(X+p.X,Y+p.Y);}
    const C2DVector operator*(float f) const {return C2DVector(X*f,Y*f);}
    void operator=(const C3DVector& p) {X=p.X;Y=p.Y;}
    C2DVector& AccurateNormalise(); //0x00a14510
    float LengthSquared()const {return X*X+Y*Y;}
    float SquareDistance(const C2DVector& v) const {float dx=X-v.X,dy=Y-v.Y;return dx*dx+dy*dy;}
};
inline C3DVector C3DVector::operator+(const C2DVector& p) const {return C3DVector(X+p.X,Y+p.Y,Z);}
struct CANavQuadTreeNode : CANavQuadTreeNodeLayout {
    const C2DVector& GetPosition() const {return *(const C2DVector*)&Position_X;}
    float GetSize() const; //0x00a75b30
};
struct CNavigatorBase : CNavigatorBaseLayout {
    bool IsAnyPartOfNodeWithinDistOfPos(const CANavQuadTreeNode*,float,const C3DVector&,C2DVector&) const;
};
bool CNavigatorBase::IsAnyPartOfNodeWithinDistOfPos(const CANavQuadTreeNode* node,float distance,const C3DVector& position,C2DVector& result) const {
    if(C2DVector(position).SquareDistance(node->GetPosition())<=distance*distance) {
        result=node->GetPosition();return true;
    }
    C2DVector center=node->GetPosition();
    C2DVector normal=(center-C2DVector(position)).AccurateNormalise();
    result=position+normal*(distance-0.0001f);
    float deltaX=center.X-result.X,deltaY=center.Y-result.Y;
    if(2.0f*(float)fabs(deltaX)<=node->GetSize()&&2.0f*(float)fabs(deltaY)<=node->GetSize()) return true;
    return false;
}
