// CNavigatorAStar::GetIntersectionPointsBetweenNodes -- cross the shared edge of two navigation cells.
// Leaf: ?GetIntersectionPointsBetweenNodes@CNavigatorAStar@@ABE?AU?$pair@VC2DVector@@V1@@std@@ABVCANavQuadTreeNode@@0M@Z
// Retail: 0x00A1A7E0,862bytes,thiscall,hidden result and ret16.
// Flags: none
#define FABLE_RETAIL_CNAVIGATORASTAR_INTERSECTIONS_PASS23
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CENTRE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CANAVQUADTREENODE_GETSIZE_OUTOFLINE
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#define FABLE_RETAIL_CNAVIGATORBASE
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "stlport/fable_containers.h"
#include "engine/CNavigatorAStar.h"
#ifdef _MSC_VER
#pragma intrinsic(fabs)
#endif
std::pair<C2DVector, C2DVector> CNavigatorAStar::GetIntersectionPointsBetweenNodes(const CANavQuadTreeNode& from, const CANavQuadTreeNode& to, float distance) const
{
    C2DBoxF fromBox(from.Position, from.GetSize() * 0.5f);
    C2DBoxF toBox(to.Position, to.GetSize() * 0.5f);
    if ((float)fabs(fromBox.BRX - toBox.TLX) < DELTA_NOTIONAL_ZERO) {
        if ((float)fabs(from.Position.Y - to.Position.Y) < DELTA_NOTIONAL_ZERO) {
            C2DVector first(fromBox.BRX - distance, from.Position.Y);
            C2DVector second(fromBox.BRX + distance, from.Position.Y);
            return std::pair<C2DVector, C2DVector>(first, second);
        } else {
            float gradient = (to.Position.Y - from.Position.Y) / (to.Position.X - from.Position.X);
            float offset1 = from.GetSize() * 0.5f - distance;
            C2DVector first(fromBox.BRX - distance, offset1 * gradient + from.Position.Y);
            float offset2 = from.GetSize() * 0.5f + distance;
            C2DVector second(fromBox.BRX + distance, offset2 * gradient + from.Position.Y);
            return std::pair<C2DVector, C2DVector>(first, second);
        }
    } else if ((float)fabs(fromBox.TLX - toBox.BRX) < DELTA_NOTIONAL_ZERO) {
        if ((float)fabs(from.Position.Y - to.Position.Y) < DELTA_NOTIONAL_ZERO) {
            C2DVector first(fromBox.TLX + distance, from.Position.Y);
            C2DVector second(fromBox.TLX - distance, from.Position.Y);
            return std::pair<C2DVector, C2DVector>(first, second);
        } else {
            float gradient = (to.Position.Y - from.Position.Y) / (to.Position.X - from.Position.X);
            float offset1 = distance - from.GetSize() * 0.5f;
            C2DVector first(fromBox.TLX + distance, offset1 * gradient + from.Position.Y);
            float offset2 = -(from.GetSize() * 0.5f) - distance;
            C2DVector second(fromBox.TLX - distance, offset2 * gradient + from.Position.Y);
            return std::pair<C2DVector, C2DVector>(first, second);
        }
    } else if ((float)fabs(fromBox.BRY - toBox.TLY) < DELTA_NOTIONAL_ZERO) {
        if ((float)fabs(from.Position.X - to.Position.X) < DELTA_NOTIONAL_ZERO) {
            C2DVector first(from.Position.X, fromBox.BRY - distance);
            C2DVector second(from.Position.X, fromBox.BRY + distance);
            return std::pair<C2DVector, C2DVector>(first, second);
        } else {
            float gradient = (to.Position.X - from.Position.X) / (to.Position.Y - from.Position.Y);
            float offset1 = from.GetSize() * 0.5f - distance;
            C2DVector first(offset1 * gradient + from.Position.X, fromBox.BRY - distance);
            float offset2 = from.GetSize() * 0.5f + distance;
            C2DVector second(offset2 * gradient + from.Position.X, fromBox.BRY + distance);
            return std::pair<C2DVector, C2DVector>(first, second);
        }
    } else {
        if ((float)fabs(from.Position.X - to.Position.X) < DELTA_NOTIONAL_ZERO) {
            C2DVector first(from.Position.X, fromBox.TLY + distance);
            C2DVector second(from.Position.X, fromBox.TLY - distance);
            return std::pair<C2DVector, C2DVector>(first, second);
        } else {
            float gradient = (to.Position.X - from.Position.X) / (to.Position.Y - from.Position.Y);
            float offset1 = distance - from.GetSize() * 0.5f;
            C2DVector first(offset1 * gradient + from.Position.X, fromBox.TLY + distance);
            float offset2 = -(from.GetSize() * 0.5f) - distance;
            C2DVector second(offset2 * gradient + from.Position.X, fromBox.TLY - distance);
            return std::pair<C2DVector, C2DVector>(first, second);
        }
    }
}