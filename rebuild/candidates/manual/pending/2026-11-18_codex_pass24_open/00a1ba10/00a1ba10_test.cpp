#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#ifndef _MSC_VER
#define __fastcall
#endif
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ADD2D
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_ROTATE
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include <cassert>
#include <cmath>
#include <cstdio>

struct CANavQuadTreeNode
{
    bool Blocked;
    long scene_id;
    const CANavQuadTreeNode* neighbor;
    bool operator==(const CANavQuadTreeNode& other) const { return scene_id == other.scene_id; }
    bool IsNeighbourOf(const CANavQuadTreeNode* other) const { return neighbor == other; }
};

struct NavigationFixture
{
    const CANavQuadTreeNode* start_node;
    const CANavQuadTreeNode* ahead_node;
    C3DVector choices[3];
    unsigned query_count, nearby_count, fallback_count, height_count;

    const CANavQuadTreeNode* GetNodeAt(const C2DVector&,unsigned layer)
    {
        assert(layer == (query_count == 0 ? 3 : 0));
        return query_count++ == 0 ? start_node : ahead_node;
    }
    C3DVector GetBlockedRandomPos(const C3DVector&, float distance, bool local)
    {
        assert(distance == 8 && local);
        ++fallback_count;
        return C3DVector(-1, -2, -3);
    }
    float FindPosZAt(const C3DVector& p, bool check)
    {
        assert(check && p.X == 9 && p.Y == 2 && p.Z == 18);
        ++height_count;
        return 29.0f;
    }
    C3DVector GetPosInDirection(const C3DVector&,const C2DVector& dir,float distance)
    {
        assert(distance == 8.0f);
        assert(nearby_count < 3);
        assert(std::fabs(dir.X - (nearby_count == 0 ? 1.0 : 0.0)) < 0.00001);
        const float expected_y[] = {0.0f, 1.0f, -1.0f};
        assert(std::fabs(dir.Y - expected_y[nearby_count]) < 0.00001);
        return choices[nearby_count++];
    }
};

const C3DVector ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM(0,0,15);

void C2DVector::Rotate(float turns)
{
    const double a=turns*double(6.2831855f);
    const float nx=float(X*std::cos(a)-Y*std::sin(a));
    Y=float(X*std::sin(a)+Y*std::cos(a));
    X=nx;
}
C2DVector& C2DVector::Normalise()
{
    const float length=std::sqrt(GetSquaredMagnitude());
    X/=length;
    Y/=length;
    return *this;
}

struct NavigatorInterfaceFixture
{
    NavigationFixture* nav;
    long GetNavigationLevelAt(const C3DVector&) const { return 3; }
    float FindPosZAt(const C3DVector& p,bool check) const { return nav->FindPosZAt(p,check); }
};

class CNavigatorAStar
{
    NavigatorInterfaceFixture fixture;
    NavigationFixture* NavigationManager;
    const NavigatorInterfaceFixture* PInterface;
public:
    explicit CNavigatorAStar(NavigationFixture& nav)
        : NavigationManager(&nav), PInterface(&fixture) { fixture.nav=&nav; }
    C3DVector GetRandomNavigablePos(const C3DVector& p,float distance,bool local) const
    { return NavigationManager->GetBlockedRandomPos(p,distance,local); }
    C3DVector GetPosInDirection(const C3DVector& p,const C2DVector& dir,float distance) const
    { return NavigationManager->GetPosInDirection(p,dir,distance); }
    C3DVector GetRandomNavigablePos(const C3DVector&,float,const C2DVector&,float) const;
};

// The selected implementation is reproduced verbatim for this standalone fixture.
C3DVector CNavigatorAStar::GetRandomNavigablePos(const C3DVector& pos, float distance, const C2DVector& direction, float spread) const
{
    const CANavQuadTreeNode* node = NavigationManager->GetNodeAt(C2DVector(pos.X, pos.Y), (unsigned char)PInterface->GetNavigationLevelAt(pos));
    if (!node)
        return pos;
    if (node->Blocked)
        return GetRandomNavigablePos(pos, distance, true);
    C2DVector dir = direction;
    dir.Normalise();
    C3DVector ahead = pos + dir * distance;
    const CANavQuadTreeNode* ahead_node = NavigationManager->GetNodeAt(C2DVector(ahead.X, ahead.Y), 0);
    if (ahead_node && (ahead_node == node || *ahead_node == *node || ahead_node->IsNeighbourOf(node)))
        return C3DVector(ahead.X, ahead.Y, PInterface->FindPosZAt(C3DVector(ahead.X + ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM.X, ahead.Y + ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM.Y, ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM.Z + ahead.Z), true));
    else {
    C3DVector straight = GetPosInDirection(pos, dir, distance);
    dir.Rotate(spread * 0.5);
    C3DVector left = GetPosInDirection(pos, dir, distance);
    dir.Rotate(-spread);
    C3DVector right = GetPosInDirection(pos, dir, distance);
    float straight_dist = C2DVector(straight.X - pos.X, straight.Y - pos.Y).GetSquaredMagnitude();
    float left_dist = C2DVector(left.X - pos.X, left.Y - pos.Y).GetSquaredMagnitude();
    float right_dist = C2DVector(right.X - pos.X, right.Y - pos.Y).GetSquaredMagnitude();
    if (straight_dist >= left_dist && straight_dist >= right_dist)
        return straight;
    if (left_dist > straight_dist && left_dist > right_dist)
        return left;
    return right;
    }
}

int main()
{
    const C3DVector pos(1, 2, 3);
    CANavQuadTreeNode start = {false, 1, 0}, equal = {false, 1, 0}, connected = {false, 2, &start};
    CANavQuadTreeNode blocked = {true, 3, 0}, far = {false, 4, 0};
    const CANavQuadTreeNode* start_values[] = {0, &blocked, &start, &start, &start, &start, &start};
    const CANavQuadTreeNode* ahead_values[] = {&start, &start, &start, &equal, &connected, 0, &far};
    for (unsigned route = 0; route < 7; ++route) {
        for (int s = 0; s < 4; ++s)
            for (int l = 0; l < 4; ++l)
                for (int r = 0; r < 4; ++r) {
                    NavigationFixture nav;
                    nav.start_node = start_values[route];
                    nav.ahead_node = ahead_values[route];
                    nav.query_count = nav.nearby_count = nav.fallback_count = nav.height_count = 0;
                    nav.choices[0] = C3DVector(pos.X + s, pos.Y, 40);
                    nav.choices[1] = C3DVector(pos.X + l, pos.Y, 41);
                    nav.choices[2] = C3DVector(pos.X + r, pos.Y, 42);
                    C3DVector result = CNavigatorAStar(nav).GetRandomNavigablePos(pos,8,C2DVector(4,0),0.5f);
                    const int expected_choice = s >= l && s >= r ? 0 : l > s && l > r ? 1 : 2;
                    const float expected_z = route == 0 ? 3 : route == 1 ? -3 : route < 5 ? 29 : 40 + expected_choice;
                    assert(result.Z == expected_z && nav.fallback_count == (route == 1));
                    assert(nav.query_count == (route < 2 ? 1U : 2U));
                    assert(nav.nearby_count == (route >= 5 ? 3U : 0U));
                    assert(nav.height_count == (route >= 2 && route < 5 ? 1U : 0U));
                }
    }
    std::puts("00a1ba10_TEST PASS");
    return 0;
}
