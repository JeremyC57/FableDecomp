// CUnprovenSoundWorld_00c1c460::UpdateSourcePositions() -- when enabled and the manager's listener is set, build the listener's world-to-listener matrix (and its inverse) from the listener position / front / top in Y-up axes, then under the sound lock hand every playing positional source its position in listener space to CODA (relative positions straight through the inverse, absolute ones through the listener frame with depth mirrored to the front), and finally run the library's post-update step
// Grade: DIFFER(445v445) -- same length, frame, slots and calls; 15 of 116 instructions differ. Residue: (1) the loop index and the
//   playback-slot pointer swap registers (retail i = edi, slot = ebx: `xor edi,edi ; cmp ecx,edi ; mov [esp+0FCh],edi` ... `mov ebx,eax`;
//   ours i = ebx, slot = edi). c2prio (--blocks): slot priority 44 (block 5 cost 4 at w 2 K 5 = +40) beats i 34, so slot is coloured
//   first and takes edi; f3 (event pointer local), h1 (slot reference), h2 (while / i++), g2 (nested ifs), f4 (signed index) leave
//   the table unchanged. (2) the copy of the selected transform result into listener_space (0x00C1C595): retail loads all three
//   dwords before the first store (`mov ecx,[eax] ; mov edx,[eax+4] ; mov eax,[eax+8] ; mov [esp+10h],ecx`), ours interleaves
//   load / store. Next: find the spelling that gives i the higher priority (a body reference of i, or one fewer slot reference in
//   the Get block).
// Leaf: ?UpdateSourcePositions@CUnprovenSoundWorld_00c1c460@@QAEXXZ
// Flags: /EHsc /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1C460, 445 bytes (+3 int3), thiscall, plain `ret`, EH frame (handler 0x0121247D, the lock
//   at state 0), esp frame 0xE4.
// Callees: SwapYZ 0x00C1BFB0 (fastcall), the file statics BuildListenerMatrices 0x00C1BF00 (private convention: world_to_listener
//   in esi, top in edi, front in ebx, inverse / position on the stack), TransformHelperPoint_00c1c3f0 0x00C1C3F0 (matrix ecx, point
//   edx, result eax) and TransformPointThroughMirroredFrame_00c1c330 0x00C1C330 (a eax, point ecx, b edx, result esi) -- their
//   private conventions need their bodies in this TU --, PKCProtectionInstance 0x00C0FFC0 / ~ 0x00C0FFE0 over g_soundLock 0x0143BAFC,
//   PKCFrallocStatic<CPlaybackSlot>::Get 0x00C13700, CQuadHolder::GetValue 0x00C34600, C3DVector::Copy 0x00C1BFF0,
//   CODA_SetSourcePosition 0x00C33FD0 (stdcall), GlobalMethodTailThunk_00c33690 0x00C33690.
// Headers: FABLE_HDR_OVERLAY=$A/hdr (new libpk/CUnprovenSoundWorld_00c1c460.h, libpk/PKCFrallocStatic.h; LHAudioEventPosition.h and
//   C3DVector.h opt-ins).
#define FABLE_RETAIL_C3DVECTOR_LIBPK_OUTOFLINE
#include "libpk/CUnprovenSoundWorld_00c1c460.h"

// 0x00C1BF00: the listener frame: axes right = top x front, top, front as the columns, the translation -axis.position, and its
// inverse.
static void BuildListenerMatrices(CMatrix3x4& world_to_listener, CMatrix3x4* listener_to_world, const C3DVector& position,
                                  const C3DVector& front, const C3DVector& top)
{
    C3DVector right = top.Cross(front);
    world_to_listener.E11 = 1.0f; world_to_listener.E12 = 0.0f; world_to_listener.E13 = 0.0f;
    world_to_listener.E21 = 0.0f; world_to_listener.E22 = 1.0f; world_to_listener.E23 = 0.0f;
    world_to_listener.E31 = 0.0f; world_to_listener.E32 = 0.0f; world_to_listener.E33 = 1.0f;
    world_to_listener.E41 = 0.0f; world_to_listener.E42 = 0.0f; world_to_listener.E43 = 0.0f;
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
            for (unsigned long i = 0; i < PManager->EventSet.Playbacks.m_count; ++i) {
                CPlaybackSlot* slot = PManager->EventSet.Playbacks.Get(i);
                if (slot->PEvent && slot->PEvent->PPlayingData && slot->Positional) {
                    CSoundPosition event_position = slot->PEvent->PositionHolder.GetValue();
                    C3DVector listener_space;
                    C3DVector source = SwapYZ(event_position.Position);
                    if (event_position.Relative)
                        listener_space = TransformHelperPoint_00c1c3f0(listener_to_world, source);
                    else
                        listener_space = TransformPointThroughMirroredFrame_00c1c330(world_to_listener, listener_to_world, source);
                    C3DVector coda_position;
                    coda_position = listener_space.Copy();
                    CODA_SetSourcePosition(slot->SourceHandle, &coda_position);
                }
            }
        }
    }
    GlobalMethodTailThunk_00c33690();
}
