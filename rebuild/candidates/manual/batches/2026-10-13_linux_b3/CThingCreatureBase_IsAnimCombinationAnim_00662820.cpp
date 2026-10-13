// CThingCreatureBase::IsAnimCombinationAnim(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662820, oracle length 42 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push esi ; mov esi,[esp+8] ; test esi,esi ; jle F ; call <display engine> ; mov eax,[eax+3Ch] ; push esi ; mov ecx,eax ; call <animation info> ; mov ecx,eax ; call C3DAnimationInfo::IsCombinationAnimation ; pop esi ; ret 4 ; F: xor al,al ; pop esi ; ret 4
// `this` is unused. A non-positive animation id is never a combination; otherwise the display
// engine (0x004C7AB0, __cdecl global accessor) hands over its animation manager (+0x3C), the
// manager resolves the id to a C3DAnimationInfo (0x00A242C0, __thiscall) and that info answers
// (0x00A23F00, batch linux_b2). No data members of the creature are touched.

struct C3DAnimationInfoCalls {
    bool IsCombinationAnimation();                      // 0x00A23F00
};

struct C3DAnimationManagerCalls {
    C3DAnimationInfoCalls* GetAnimationInfo(long animationId);   // 0x00A242C0
};

struct CDisplayEngineCalls {
    unsigned char             _pad_0x00[0x3C];
    C3DAnimationManagerCalls* AnimationManager;           // +0x3C
};

extern CDisplayEngineCalls* GFGetDisplayEngine();         // 0x004C7AB0

struct CThingCreatureBase_Methods {
    bool IsAnimCombinationAnim(long animationId);
};

bool CThingCreatureBase_Methods::IsAnimCombinationAnim(long animationId)
{
    if (animationId <= 0)
        return false;
    C3DAnimationInfoCalls* info = GFGetDisplayEngine()->AnimationManager->GetAnimationInfo(animationId);
    return info->IsCombinationAnimation();
}
