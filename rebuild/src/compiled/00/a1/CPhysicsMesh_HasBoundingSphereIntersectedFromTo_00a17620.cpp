// CPhysicsMesh::HasBoundingSphereIntersectedFromTo -- broad-phase sphere test.
// Leaf: ?HasBoundingSphereIntersectedFromTo@CPhysicsMesh@@QBE_NMABVC3DVector@@0@Z
// Retail: 0x00a17620,99 bytes,thiscall.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#include "engine/CPhysicsMesh.h"
float FABLE_FASTCALL GFGetShortestDistanceSquaredBetweenPointAndLine(
    const C3DVector&,const C3DVector&,const C3DVector&,float&);
bool CPhysicsMesh::HasBoundingSphereIntersectedFromTo(float sphere_radius,
    const C3DVector& from,const C3DVector& to) const
{
    const float bonus = 15.0f;
    const C3DVector& sphere_centre = GetBoundingSphere().GetCenter();
    const float this_radius = GetBoundingSphere().GetRadius();
    float along;
    float distance = GFGetShortestDistanceSquaredBetweenPointAndLine(from,to,sphere_centre,along);
    float radius = this_radius + sphere_radius + bonus;
    if (distance <= radius * radius) return true;
    return false;
}