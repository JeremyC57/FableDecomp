// C3DAnimationSequenceData::WriteCompressedToFile(CDataOutputStream&) const -- write the bone name, packed header fields and the four compressed tracks
// Retail @ 0x00A4BEB0, 480 bytes (ret 4).
// Callees: CDataOutputStream::WriteData (vtable slot 4), WriteBool 0x00993E30, WriteUChar 0x00993E50,
//   WriteUShort 0x00993E80, WriteShort 0x00993E90, WriteULong 0x00993EA0, WriteFloat 0x00993EE0.
// Shape: the name goes out raw with its terminator (inline strlen + 1 through the virtual WriteData); the bitfield
// members are written one by one (BoneType, AnimatedFlag, the three 2-bit track modes; EnabledFlag/IdentityFlag are
// not saved); then size + elements for the rotation track (CQuaternion, through a reference to the element), the
// paletted rotation bytes, the C3DVectorWord position track and the paletted position bytes.
// Layout: Ego_r.pdb C3DAnimationSequenceData (0x2C bytes), offsets confirmed by the bytes; no engine header exists.
extern "C" unsigned int __cdecl strlen(const char* text);
#pragma intrinsic(strlen)

struct CDataOutputStream {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void WriteData(const void* data, unsigned long size); // slot 4 (+0x10)
    void WriteBool(bool value);                                // 0x00993E30
    void WriteUChar(unsigned char value);                      // 0x00993E50
    void WriteUShort(unsigned short value);                    // 0x00993E80
    void WriteShort(short value);                              // 0x00993E90
    void WriteULong(unsigned long value);                      // 0x00993EA0
    void WriteFloat(float value);                              // 0x00993EE0
};

struct CQuaternion { float X, Y, Z, W; };
struct C3DVectorWord { short X, Y, Z; };

struct C3DAnimationSequenceData {
    char* BoneName;                            // +0x00
    float SamplesPerSecond;                    // +0x04
    unsigned short FrameCount;                 // +0x08
    unsigned char BoneType : 5;                // +0x0A
    unsigned char EnabledFlag : 1;
    unsigned char AnimatedFlag : 1;
    unsigned char IdentityFlag : 1;
    unsigned char RotationTrackMode : 2;       // +0x0B
    unsigned char PositionTrackMode : 2;
    unsigned char ScalingTrackMode : 2;
    float PositionFactor;                      // +0x0C
    float ScalingFactor;                       // +0x10
    CQuaternion* RotationTrack;                // +0x14
    unsigned short RotationTrackSize;          // +0x18
    unsigned short PositionTrackSize;          // +0x1A
    unsigned short PalettedRotationTrackSize;  // +0x1C
    unsigned short PalettedPositionTrackSize;  // +0x1E
    C3DVectorWord* PositionTrack;              // +0x20
    unsigned char* PalettedRotationTrack;      // +0x24
    unsigned char* PalettedPositionTrack;      // +0x28
    void WriteCompressedToFile(CDataOutputStream& out) const;
};

void C3DAnimationSequenceData::WriteCompressedToFile(CDataOutputStream& out) const
{
    out.WriteData(BoneName, strlen(BoneName) + 1);
    out.WriteUChar(BoneType);
    out.WriteFloat(SamplesPerSecond);
    out.WriteULong(FrameCount);
    out.WriteBool(AnimatedFlag);
    out.WriteUChar(RotationTrackMode);
    out.WriteUChar(PositionTrackMode);
    out.WriteUChar(ScalingTrackMode);
    out.WriteFloat(PositionFactor);
    out.WriteFloat(ScalingFactor);
    out.WriteUShort(RotationTrackSize);
    for (unsigned long i = 0; i < RotationTrackSize; ++i) {
        const CQuaternion& q = RotationTrack[i];
        out.WriteFloat(q.X);
        out.WriteFloat(q.Y);
        out.WriteFloat(q.Z);
        out.WriteFloat(q.W);
    }
    out.WriteUShort(PalettedRotationTrackSize);
    for (unsigned long j = 0; j < PalettedRotationTrackSize; ++j)
        out.WriteUChar(PalettedRotationTrack[j]);
    out.WriteUShort(PositionTrackSize);
    for (unsigned long k = 0; k < PositionTrackSize; ++k) {
        const C3DVectorWord& v = PositionTrack[k];
        out.WriteShort(v.X);
        out.WriteShort(v.Y);
        out.WriteShort(v.Z);
    }
    out.WriteUShort(PalettedPositionTrackSize);
    for (unsigned long m = 0; m < PalettedPositionTrackSize; ++m)
        out.WriteUChar(PalettedPositionTrack[m]);
}
