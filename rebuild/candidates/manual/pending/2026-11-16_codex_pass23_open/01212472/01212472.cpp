// 01212472 CUnprovenSoundWorld_00c1c460::UpdateSourcePositions parent and generated guard cleanup
// Leaf: ?UpdateSourcePositions@CUnprovenSoundWorld_00c1c460@@QAEXXZ
// Flags: /EHsc /Gr /d2inlT1
// Inline-budget: native00c1c5b2 calls Copy; ordinary full-body expansion is435B.
// The local /d2inlT1 budget retains that call and its genuine escape information.
#define FABLE_RETAIL_C3DVECTOR_LIBPK_CODA_COPY_BODY_PASS23
#define FABLE_RETAIL_C3DVECTOR_LIBPK_IMPLICIT_COPY_PASS23
#define FABLE_RETAIL_C3DVECTOR_LIBPK_CODA_COPY_PASS23
#define FABLE_RETAIL_C3DVECTOR_LIBPK_OUTOFLINE
#include "libpk/CUnprovenSoundWorld_00c1c460.h"

// 0x00C1BF00: the listener frame: axes right = top x front, top, front as the columns, the translation -axis.position, and its
// inverse.
static void BuildListenerMatrices(CMatrix3x4& world_to_listener, CMatrix3x4* listener_to_world, const C3DVector& position,
                                  const C3DVector& front, const C3DVector& top)
{
    C3DVector right = top.Cross(front);
    world_to_listener.E41=world_to_listener.E42=world_to_listener.E43=world_to_listener.E12=world_to_listener.E13=world_to_listener.E21=world_to_listener.E23=world_to_listener.E31=world_to_listener.E32=0.0f;
    world_to_listener.E11=world_to_listener.E22=world_to_listener.E33=1.0f;
    world_to_listener.E11 = right.X; world_to_listener.E12 = top.X; world_to_listener.E13 = front.X;
    world_to_listener.E21 = right.Y; world_to_listener.E22 = top.Y; world_to_listener.E23 = front.Y;
    world_to_listener.E31 = right.Z; world_to_listener.E32 = top.Z; world_to_listener.E33 = front.Z;
    world_to_listener.E41 = -right.DotProduct(position);
    world_to_listener.E42 = -top.DotProduct(position);
    world_to_listener.E43 = -front.DotProduct(position);
    InvertMatrix3x4(listener_to_world, world_to_listener);
}

// 0x00C1C3F0 (landed): point through a 4x3 matrix.
static C3DVector TransformHelperPoint_00c1c3f0(const CMatrix3x4& matrix, const C3DVector& point)
{
    C3DVector result;
    result.X = matrix.E31 * point.Z + matrix.E21 * point.Y + matrix.E11 * point.X + matrix.E41;
    result.Y = matrix.E32 * point.Z + matrix.E12 * point.X + matrix.E22 * point.Y + matrix.E42;
    result.Z = matrix.E33 * point.Z + matrix.E13 * point.X + matrix.E23 * point.Y + matrix.E43;
    return result;
}

// 0x00C1C330: point through a, depth mirrored to the front (z < 0 -> -z), then through b.
static C3DVector TransformPointThroughMirroredFrame_00c1c330(const CMatrix3x4& a, const CMatrix3x4& b, const C3DVector& point)
{
    float x = a.E31 * point.Z + a.E21 * point.Y + a.E11 * point.X + a.E41;
    float y = a.E32 * point.Z + a.E12 * point.X + a.E22 * point.Y + a.E42;
    float z = a.E33 * point.Z + a.E13 * point.X + a.E23 * point.Y + a.E43;
    if (z < 0.0f)
        z *= -1.0f;
    C3DVector result;
    result.X = b.E21 * y + b.E31 * z + b.E11 * x + b.E41;
    result.Y = b.E22 * y + b.E12 * x + b.E32 * z + b.E42;
    result.Z = b.E23 * y + b.E13 * x + b.E33 * z + b.E43;
    return result;
}

void CUnprovenSoundWorld_00c1c460::UpdateSourcePositions()
{
    if (SourcePositionsEnabled) {
        CSoundEventManager* manager = PManager;
        if (manager->ListenerValid) {
            C3DVector position = SwapYZ(manager->ListenerPosition);
            C3DVector front = SwapYZ(manager->ListenerFront);
            C3DVector top = SwapYZ(manager->ListenerTop);
            CMatrix3x4 listener_to_world;
            CMatrix3x4 world_to_listener;
            BuildListenerMatrices(world_to_listener, &listener_to_world, position, front, top);
            PKCProtectionInstance lock(&g_soundLock);
            for(unsigned long i=0;;++i){
                unsigned long count=PManager->m_events.Playbacks.m_count;
                if(i>=count)break;
                CPlaybackSlot* slot = PManager->m_events.Playbacks.Get(i);
                if (slot->PEvent && slot->PEvent->PPlayingData && slot->Positional) {
                    CSoundPosition event_position = slot->PEvent->PositionHolder.GetValue();
                    C3DVector listener_space;
                    C3DVector source = SwapYZ(event_position.Position);
                    if (event_position.Relative)
                        listener_space = TransformHelperPoint_00c1c3f0(listener_to_world, source);
                    else
                        listener_space = TransformPointThroughMirroredFrame_00c1c330(world_to_listener, listener_to_world, source);
                    CVECTOR coda_position;
                    coda_position = listener_space.Copy();
                    CODA_3DSourceSetPosition(slot->SourceHandle, &coda_position);
                }
            }
        }
    }
    CODA_Commit();
}
