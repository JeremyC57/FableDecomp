// CAIStateGroup_HoverKeepInArea::IsRunnable() -- the group may run once the hoverer has strayed from its start point.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00959F30, length 66 bytes.
// Callees: CThing::GetPosition @ 0x004C73D0 on the brain's creature (manifest "CVertexBufferWin32::DoSizeof" is
// propagated), CAIStateGroup_EnemyCreatureBase::IsRunnable @ 0x0091DD30 (tail-jumped).
//
// Shape: the horizontal distance from StartPos (+0x1c/+0x20, names from the engine header) to the creature is
// computed on the x87 stack (`dx`, `dy` both loaded first, then `dy*dy + dx*dx`, `fsqrt` inline from the
// intrinsic sqrt of the retail /Oi build), compared with the 20.0f radius literal (float pool constant at 0x01294EB0, value 20.0; `fcomp ;
// test ah,41h ; jne` = "not greater"), and only when the creature is further away does the base IsRunnable decide.
// `xor al,al`: bool. The engine header flattens PBrain into a pad, so the base layout is modelled locally.
#include <math.h>

class C3DVector { public: float X, Y, Z; };

class CThing { public:
    const C3DVector& GetPos() const;    // 0x004C73D0
};
class CThingAICreature : public CThing { public:
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};

class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase {
    unsigned char _enemy[0x8];                  // +0x14
    bool IsRunnable();                          // 0x0091DD30
};

struct CAIStateGroup_HoverKeepInArea : CAIStateGroup_EnemyCreatureBase {
    C3DVector StartPos;                         // +0x1c
    bool IsRunnable();
};

bool CAIStateGroup_HoverKeepInArea::IsRunnable()
{
    C3DVector* pPos = (C3DVector*)&PBrain->PCreature->GetPos();
    float dx = pPos->X - StartPos.X;
    float dy = pPos->Y - StartPos.Y;
    if ((float)sqrt(dy * dy + dx * dx) > 20.0f)
        return CAIStateGroup_EnemyCreatureBase::IsRunnable();
    return false;
}