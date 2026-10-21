// std::vector<CActionInputControl>::resize(unsigned int new_size) -- resize, filling new slots with a value-initialised CActionInputControl
// Leaf: ?resize@
// Retail: Fable.exe @ 0x00412AA0, 49 bytes, `ret 4`, unreferenced (its copy is inlined into
// CPersistContext_TransferableVector<CActionInputControl>'s resize slot 0x00412C10, vtable 0x0122E014, RTTI
// .?AV?$CPersistContext_TransferableVector@VCActionInputControl@@@@). STLport resize(n) { resize(n, _Tp()); }: a zeroed
// 28-byte temporary passed to resize(n, const T&) (0x004129F0, itself mislabelled vector<CBrainBehaviour> -- that
// element is 12 bytes, this one is 0x1c: `imul edx,edx,1Ch`). Catalog label vector<EWeaponClass>::_Umove is wrong.
// Layout: CActionInputControl (Ego_r.pdb, 0x1c): GameAction +0, ControllerType +4, KeyboardKey +8, XboxButton +0xc,
// MouseButton +0x10, C2DVector ControlDirection +0x14; no engine/ header. The user default constructor zeroing every
// field (as modelled by the landed CKeyRedefiner ctor 0x00558540) sets the schedule: new_size is loaded before the
// zero fill; with an implicit (POD) value-initialisation the load sinks below the stores (52 bytes, DIFFER).
// Shape: /O2.
enum EGameAction {};
enum EControllerType {};
enum EInputKey {};
enum EXboxControllerButton {};
enum EMouseButtonControl {};
class C2DVector {
public:
    float X;                                             // +0x00
    float Y;                                             // +0x04
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class CActionInputControl {
public:
    EGameAction           GameAction;                    // +0x00
    EControllerType       ControllerType;                // +0x04
    EInputKey             KeyboardKey;                   // +0x08
    EXboxControllerButton XboxButton;                    // +0x0c
    EMouseButtonControl   MouseButton;                   // +0x10
    C2DVector             ControlDirection;              // +0x14
    CActionInputControl()
        : GameAction((EGameAction)0), ControllerType((EControllerType)0), KeyboardKey((EInputKey)0),
          XboxButton((EXboxControllerButton)0), MouseButton((EMouseButtonControl)0), ControlDirection(0.0f, 0.0f) {}
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    void resize(unsigned int new_size, const T& x);      // 0x004129F0 for CActionInputControl
    void resize(unsigned int new_size) { resize(new_size, T()); }
};
}
template class std::vector<CActionInputControl>;