// CCreatureAction_PlayConversationAnimation::CCreatureAction_PlayConversationAnimation(CThingCreatureBase*, long, const CSoundPair&, long, long) -- base CCreatureActionBase(owner, -1.0, 1.0, a random loaded animation of the attitude's group name, sound, c, d), own vtable (0x012737CC), the flag at +0xa8 cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00842400, length 81 bytes.
// Callees: GetAnimGroupName @ 0x004CD9B0 (register argument: the attitude in ecx; returns the group name by reference), CThingCreatureBase::GetRandomLoadedAnimFromName(const CCharString&, bool) @ 0x00662FA0, CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float, float, CAnimationEntry*, const CSoundPair&, long, long) @ 0x00693B30.
// Shape: `ret 14h`; arguments right to left -- d, c, the sound reference, then the animation (its own `true` pushed before the group-name lookup), 1.0f, -1.0f, the owner.
class CCharString; class CAnimationEntry;
class CSoundPair { public: long FirstSound; long LastSound; };
class CThingCreatureBase { public:
    CAnimationEntry* GetRandomLoadedAnimFromName(const CCharString& name, bool loaded);   // 0x00662FA0
};
const CCharString& __fastcall GetAnimGroupName_004cd9b0(long attitude);   // 0x004CD9B0 (EConversationAttitude)
class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};

class CCreatureAction_PlayConversationAnimation : public CCreatureActionBase { public:   // vtable 0x012737CC
    unsigned char m_finished;               // +0xa8
    virtual void v00();
    CCreatureAction_PlayConversationAnimation(CThingCreatureBase* owner, long attitude, const CSoundPair& sound, long c, long d);
};

CCreatureAction_PlayConversationAnimation::CCreatureAction_PlayConversationAnimation(CThingCreatureBase* owner, long attitude, const CSoundPair& sound, long c, long d)
    : CCreatureActionBase(owner, -1.0f, 1.0f, owner->GetRandomLoadedAnimFromName(GetAnimGroupName_004cd9b0(attitude), true), sound, c, d)
    , m_finished(0)
{
}