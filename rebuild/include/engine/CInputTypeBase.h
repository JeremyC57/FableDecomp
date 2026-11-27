#ifdef FABLE_RETAIL_SHARED_CONTROLS_PASS29
#ifndef FABLE_ENGINE_CINPUTTYPEBASE_H
#define FABLE_ENGINE_CINPUTTYPEBASE_H
namespace NControlSystem {
enum EInputType {
    INPUT_TYPE_KEYBOARD_KEY, INPUT_TYPE_MOUSE_BUTTON, INPUT_TYPE_XBOX_PAD_BUTTON,
    INPUT_TYPE_XBOX_PAD_LEFT_STICK, INPUT_TYPE_XBOX_PAD_RIGHT_STICK, INPUT_TYPE_MOUSE_MOVEMENT,
    INPUT_TYPE_MOUSE_WHEEL_MOVEMENT, INPUT_TYPE_MOUSE_WHEEL_MOVEMENT_UP, INPUT_TYPE_MOUSE_WHEEL_MOVEMENT_DOWN
};
}
// Layout: Ego_d class 52752, one vfptr. Original slots 0, 1 and 2 are the destructor, GetType, and equality.
class CInputTypeBase {
public:
    virtual ~CInputTypeBase();
    virtual NControlSystem::EInputType GetType() const = 0;
    virtual bool operator==(const CInputTypeBase&) const = 0;
};
#endif
#endif
