// CThingCreatureBase::IsAnimCombinationAnim(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662820, oracle length 42 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,[esp+8] ; test esi,esi ; jle F ; call <display engine> ; mov eax,[eax+3Ch] ; push esi ; mov ecx,eax ; call <animation info> ; mov ecx,eax ; call C3DAnimationInfo::IsCombinationAnimation ; pop esi ; ret 4 ; F: xor al,al ; pop esi ; ret 4
// `this` is unused. A non-positive animation id is never a combination; otherwise the display
// engine (0x004C7AB0, __cdecl global accessor) hands over its animation manager (+0x3C), the
// manager resolves the id to a C3DAnimationInfo (0x00A242C0, __thiscall) and that info answers
// (0x00A23F00, batch linux_b2). No data members of the creature are touched.
//
// Lane feedback: needs the manager in a local (engine call before the argument push) AND the positive path
// as the fall-through with `return false` last. RELOCATION_MATCH.
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
    // Positive ids are resolved through the display engine's animation manager; the manager
    // local sequences the engine call ahead of the argument push (retail keeps the id in esi
    // and pushes it after that call). The false exit is the cold tail.
    if (animationId > 0) {
        C3DAnimationManagerCalls* manager = GFGetDisplayEngine()->AnimationManager;
        return manager->GetAnimationInfo(animationId)->IsCombinationAnimation();
    }
    return false;
}
