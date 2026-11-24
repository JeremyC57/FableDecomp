#include <stdio.h>
struct C2DVector {
    float X,Y;
    C2DVector(float x=0, float y=0):X(x),Y(y){}
    C2DVector operator*(float s)const{return C2DVector(X*s,Y*s);}
};
struct C3DVector {
    float X,Y,Z;
    C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
    C3DVector operator+(const C3DVector& p)const{return C3DVector(X+p.X,Y+p.Y,Z+p.Z);}
    C3DVector operator-(const C3DVector& p)const{return C3DVector(X-p.X,Y-p.Y,Z-p.Z);}
    const C3DVector& operator+=(const C2DVector& d){X+=d.X;Y+=d.Y;return *this;}
    float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
};
static const C3DVector ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM(0,0,2);
static float GFSqr(float x){return x*x;}
struct CANavQuadTreeNode {
    bool open;float size;
    bool IsNavigable()const{return open;}
    float GetSize()const{return size;}
};
struct CNavigatorManager {
    const CANavQuadTreeNode* route[4]; mutable unsigned read;
    const CANavQuadTreeNode* GetNodeAt(const C3DVector&,unsigned char layer)const {
        if(layer!=1 || read>=4) return 0;
        return route[read++];
    }
};
class CNavigatorAStar {
public:
    CNavigatorManager NavigationManager;
    mutable unsigned heights;
    long GetNavigationLevelAt(const C3DVector&)const{return 1;}
    float FindPosZAt(const C3DVector& p,bool enabled)const{++heights;return enabled?p.Z+p.X-p.Y:-32;}
    const CANavQuadTreeNode* GetNodeAt(const C3DVector&)const;
    C3DVector GetPosInDirection(const C3DVector&,const C2DVector&,float)const;
};
inline const CANavQuadTreeNode* CNavigatorAStar::GetNodeAt(const C3DVector& pos) const
{
    return NavigationManager.GetNodeAt(pos, (unsigned char)GetNavigationLevelAt(pos));
}

C3DVector CNavigatorAStar::GetPosInDirection(const C3DVector& start_pos,
    const C2DVector& direction, float distance_to_keep_within) const
{
    const float req_dist_sq = GFSqr(distance_to_keep_within);
    C3DVector possible_pos(start_pos);
    C3DVector last_valid_pos(possible_pos);
    const CANavQuadTreeNode* ppossible_node = GetNodeAt(start_pos);
    while (ppossible_node && ppossible_node->IsNavigable()) {
        last_valid_pos = possible_pos;
        possible_pos += direction * (ppossible_node->GetSize() * 0.5f);
        if ((possible_pos - start_pos).GetSquaredMagnitude2D() > req_dist_sq) {
            last_valid_pos.Z = FindPosZAt(last_valid_pos + ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM, true);
            return last_valid_pos;
        }
        ppossible_node = GetNodeAt(possible_pos);
    }
    last_valid_pos.Z = FindPosZAt(last_valid_pos + ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM, true);
    return last_valid_pos;
}

int main() {
    CANavQuadTreeNode open={true,4},blocked={false,4};
    const C3DVector start(2,1,4);
    const C2DVector direction(1,0);
    struct Case { const CANavQuadTreeNode* nodes[4]; float distance, expected_x; unsigned visits; };
    const Case cases[]={
        {{0,0,0,0},8,2,1},{{&blocked,0,0,0},8,2,1},
        {{&open,0,0,0},1,2,1},{{&open,&blocked,0,0},2,2,2},
        {{&open,&open,0,0},2,4,2},{{&open,&open,&open,0},8,6,4}
    };
    for(unsigned i=0;i<sizeof(cases)/sizeof(cases[0]);++i) {
        CNavigatorAStar nav;nav.heights=0;nav.NavigationManager.read=0;
        for(int j=0;j<4;++j)nav.NavigationManager.route[j]=cases[i].nodes[j];
        C3DVector p=nav.GetPosInDirection(start,direction,cases[i].distance);
        if(p.X!=cases[i].expected_x||p.Y!=1||p.Z!=5+p.X||
           nav.heights!=1||nav.NavigationManager.read!=cases[i].visits)return i+1;
    }
    puts("00a1ad40_TEST PASS");
}