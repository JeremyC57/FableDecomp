// 00a1ad40 CNavigatorAStar::GetPosInDirection
// Leaf: ?GetPosInDirection@CNavigatorAStar@@ABE?AVC3DVector@@ABV2@ABVC2DVector@@M@Z
// Flags:
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
#define FABLE_RETAIL_CNAVIGATORBASE_ORIGINAL_ACCESS
#define FABLE_RETAIL_C3DVECTOR_PLUSASSIGN2D_PASS27
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE2D
#define FABLE_RETAIL_CNAVIGATORBASE_LEVEL_PASS27
#define FABLE_RETAIL_CNAVIGATORBASE_HEIGHTTHEMES
#define FABLE_RETAIL_CNAVIGATORBASE_FINDPOSZ_PASS26
#define FABLE_RETAIL_CANAVQUADTREENODE_GETSIZE_OUTOFLINE
#define FABLE_RETAIL_CANAVQUADTREENODE_ISNAVIGABLE
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
#include "engine/CNavigatorAStar.h"
#include "engine/CANavQuadTreeNode.h"
#include "engine/CNavigatorManager.h"
#include "engine/CNavigatorExternalInterface.h"
#include "engine/GFSqr.h"

namespace { extern C3DVector ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM; }

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