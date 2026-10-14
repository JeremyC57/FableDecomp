// C3DAnimationInfo::Sizeof() -- virtual slot 2: the size of the animation data block (CAnimationData::Sizeof @ 0x00A25B20 plus the 4-byte counted pointer, or just the pointer) plus the 0x14 bytes of cached values
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A265B0, length 25 bytes.
// Callees: C3DAnimationInfo::CAnimationData::Sizeof @ 0x00A25B20 (thiscall on Data_Object).
// Layout from engine/C3DAnimationInfo.h: Data_Object +0xc. The conditional expression yields
// `size + 4` or `4`, and the trailing `+ 0x14` (Duration, NonLoopingDuration and the movement
// vector) is added on both paths: VC folds it into `add eax,18h` on the call path but keeps
// `mov eax,4 ; add eax,14h` on the constant path.
struct CAnimationData {
    unsigned long Sizeof();                // 0x00A25B20
};

struct C3DAnimationInfo {                  // engine/C3DAnimationInfo.h offsets
    virtual void v00();
    virtual void v01();
    virtual unsigned long Sizeof();        // slot 2 @ 0x00A265B0
    void* Bank;                            // +4
    unsigned long Index;                   // +8
    CAnimationData* Data;                  // +0xc
};

unsigned long C3DAnimationInfo::Sizeof()
{
    return (Data ? Data->Sizeof() + sizeof(Data) : sizeof(Data)) + 0x14;
}
