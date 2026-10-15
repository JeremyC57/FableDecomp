// CTCOnHeroPush::IsAngry() const -- the hero has pushed the creature further than the anger distance
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BBA50, length 25 bytes.
// Globals: the anger distance float @ 0x01383744.
// PDB CTCOnHeroPush: DistPushed +0x0c, PushDir +0x10, FrameSincePushed +0x1c.
// Shape: `fcomp ; test ah,41h ; jne` = `!(DistPushed <= limit)`; `mov eax,1` / `xor eax,eax` returns are a long result.
extern float g_angryPushDistance;                   // 0x01383744

struct CTCBase_007bba50 {
    virtual void v00();
    unsigned char _pad04[8];
};

struct CTCOnHeroPush : CTCBase_007bba50 {
    float DistPushed;                               // +0x0c
    float PushDir[3];                               // +0x10
    long  FrameSincePushed;                         // +0x1c
    long IsAngry() const;
};

long CTCOnHeroPush::IsAngry() const
{
    if (DistPushed > g_angryPushDistance)
        return 1;
    return 0;
}
