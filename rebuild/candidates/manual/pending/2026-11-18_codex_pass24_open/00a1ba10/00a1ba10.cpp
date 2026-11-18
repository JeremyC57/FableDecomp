// CNavigatorAStar::GetRandomNavigablePos(const C3DVector& pos, float distance, const C2DVector& direction, float spread) const -- a navigable point about `distance` along `direction` from pos
// Grade: DIFFER(642v642) -- The height constructor restores Z operand order but X and Y still enter the x87 stack differently.
// Leaf: ?GetRandomNavigablePos@CNavigatorAStar@@UBE?AVC3DVector@@ABV2@MABVC2DVector@@M@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) 0x00A1BA10, 642 bytes, `ret 14h`.
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ADD2D
#define FABLE_RETAIL_C3DVECTOR_TO2D
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#define FABLE_RETAIL_CNAVIGATORBASE
#define FABLE_RETAIL_CNAVIGATORASTAR_RANDOMPOS
#define FABLE_RETAIL_C2DVECTOR_ROTATE
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_CNAVIGATORMANAGER
#define FABLE_RETAIL_CNAVIGATORMANAGER_GETNODEAT
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "stlport/fable_containers.h"
#include "engine/CNavigatorAStar.h"
#include "engine/CANavQuadTreeNode.h"
#include "engine/CNavigatorManager.h"
#include "engine/CNavigatorExternalInterface.h"
extern const C3DVector ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM;               // 0x013D27D0 (Ego_d file static in an anonymous namespace)

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
