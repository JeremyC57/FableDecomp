#ifndef FABLE_ENGINE_EANIMCOMPONENT_H
#define FABLE_ENGINE_EANIMCOMPONENT_H
// Hand-written (chunk 873, row 0x005DD670 CAnimationEntry::StatRegisterClass): the animation-component kind, the key of
// CAnimationEntry::RegisteredComponentsByType / RegisteredComponentDefaults and of CAnimComponentMap::Map (Ego_r.pdb type name).
// Values: each component's StatGetType (StatRegisterClass registers them 0..12 in this order, 0x005DD7D1 .. 0x005DEAAB, and each
// GetType override returns the same value, e.g. CAnimComponentFlags 0x005DFE80 `xor eax,eax`). The enumerator spellings are not in
// either PDB name list: they are the spellings the landed CAnimationEntry::GetComponent rows already use (0x008C7B30, 0x0087E1D0,
// 0x0087E150 and siblings); RECOIL and SET_CREATURE_MODE follow the same pattern (descriptive).
enum EAnimComponent {
    ANIM_COMPONENT_FLAGS = 0,
    ANIM_COMPONENT_TRANSITION_IN_TIME = 1,
    ANIM_COMPONENT_DELAY_TIME = 2,
    ANIM_COMPONENT_COMBAT_HANDEDNESS = 3,
    ANIM_COMPONENT_COMBAT_COMBO_CHAIN = 4,
    ANIM_COMPONENT_COMBAT_RECOIL = 5,
    ANIM_COMPONENT_COMBAT_MISC = 6,
    ANIM_COMPONENT_COMBAT_FLOURISH_TARGET_OFFSET = 7,
    ANIM_COMPONENT_FOLLOWING_ON_ANIM = 8,
    ANIM_COMPONENT_STRIKE_SPEED_MULTIPLIER = 9,
    ANIM_COMPONENT_SET_CREATURE_MODE = 10,
    ANIM_COMPONENT_FOLLOWING_ON_ANIM_FILTER = 11,
    ANIM_COMPONENT_STRIKE_RESPONSE_ANIM = 12,
};
#endif // FABLE_ENGINE_EANIMCOMPONENT_H
