// Grade: DIFFER(89v420) -- retail's slerp-weight block survives only because its float->int conversions are inline-asm fistp (fstp/fld through a float slot, then fistp); every result is dead, so compiler-emitted C++ deletes the whole block. asm_bake family: not byte-pure reconstructible.
// C3DAnimation2::CalcLocalTransformationsRelative(C3DKeyframe* keyframes, unsigned long unused, float time, C3DMesh2* mesh) -- per sequence: work out the slerp table weights between this frame's and the next frame's rotation keys (dead results) and copy the mesh's base keyframe
// Leaf: CalcLocalTransformationsRelative
// Layout: retail C3DAnimation2 is 4 smaller than Ego_r.pdb from +0x40 (landed 0x00A4E150: MovementTrack +0x40): SequenceCount +0x58,
//   Sequences +0x5c (8-byte counted pointers). C3DMesh2 base keyframes at +0xa0 (0x30-byte C3DKeyframe).
struct C3DKeyframe { float Rotation[4]; float Position[4]; float Scaling[4]; };
struct C3DAnimationSequence {
    unsigned char  _00[8];
    unsigned short FrameCount;          // +0x08
    unsigned char  _0a;
    unsigned char  Flags;               // +0x0b bit0 indexed, bit1 rotation, bit2.. other
    unsigned char  _0c[8];
    float        (*Rotations)[4];       // +0x14
    unsigned char  _18[0xc];
    unsigned char* KeyIndex;            // +0x24
};
struct CSeqPtr { C3DAnimationSequence* Data; void* Info; };
struct C3DMesh2 { unsigned char _00[0xa0]; C3DKeyframe* BaseKeyframes; };
extern float g_SlerpTable[];            // 0x013CB530
class C3DAnimation2 {
public:
    unsigned char _00[0x58];
    unsigned long SequenceCount;        // +0x58
    CSeqPtr*      Sequences;            // +0x5c
    void ConvertTimeToFrames(float time, long* frame, float* fraction);
    void CalcLocalTransformationsRelative(C3DKeyframe* keyframes, unsigned long unused, float time, C3DMesh2* mesh);
};
static inline long FloatToLong(float f) { return (long)f; }

void C3DAnimation2::CalcLocalTransformationsRelative(C3DKeyframe* keyframes, unsigned long, float time, C3DMesh2* mesh)
{
    long frame; float fraction;
    ConvertTimeToFrames(time, &frame, &fraction);
    for (unsigned long i = 0; i < SequenceCount; ++i) {
        C3DAnimationSequence* sequence = Sequences[i].Data;
        unsigned long next = (unsigned long)(frame + 1) < sequence->FrameCount ? frame + 1 : 0;
        if (sequence->Flags & 2) {
            unsigned long a = frame, b = next;
            if ((sequence->Flags & 3) != 3 || (a = sequence->KeyIndex[frame]) != (b = sequence->KeyIndex[next])) {
                const float* p = sequence->Rotations[a]; const float* q = sequence->Rotations[b];
                float dot = p[3] * q[3] + p[2] * q[2] + p[1] * q[1] + p[0] * q[0];
                if (dot < 0.0f) dot = -dot;
                if (1.0f - dot > 0.05f) {
                    float scaled = (dot + 1.0f) * 512.5f;
                    long index = FloatToLong(scaled - 0.5f);
                    float t = scaled - (float)index;
                    float angle = ((1.0f - t) * g_SlerpTable[index] + t * g_SlerpTable[index + 1]) * 0.15915494f;
                    FloatToLong((1.0f - fraction) * angle * 1024.0f - 0.5f);
                    FloatToLong(angle * fraction * 1024.0f - 0.5f);
                }
            }
        }
        keyframes[i] = mesh->BaseKeyframes[i];
    }
}
