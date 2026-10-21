// CPersistContext_TransferableVector<CActionInputControl>::Resize(long new_size) -- resize the transferred vector, new slots default-constructed
// Leaf: ?Resize@?$CPersistContext_TransferableVector@VCActionInputControl@@@@QAEXJ@Z
// Retail: Fable.exe @ 0x00412C10, 55 bytes, `ret 4`; slot 1 of vtable 0x0122E010 (RTTI
// .?AV?$CPersistContext_TransferableVector@VCActionInputControl@@@@, base CPersistContext_TransferableVectorGenericType);
// Ego_r.pdb ?Resize@?$CPersistContext_TransferableVector@VCActionInputControl@@@@EAEXJ@Z (private virtual). Body:
// TransVector (+4) -> resize(n), STLport resize(n) { resize(n, _Tp()); } inlined: a zeroed 28-byte CActionInputControl
// passed to vector<CActionInputControl>::resize(n, const T&) (0x004129F0). Catalog label vector<EWeaponClass>::_Umove is
// wrong. Modelled without virtuals (vptr placeholder at +0); the slot is the only reference.
// Layout: CPersistContext_TransferableVector<T> (Ego_r.pdb, 8 bytes): vptr +0, TransVector +4. CActionInputControl
// (Ego_r.pdb, 0x1c): GameAction +0, ControllerType +4, KeyboardKey +8, XboxButton +0xc, MouseButton +0x10, C2DVector
// ControlDirection +0x14, with the user default constructor zeroing every field (landed CKeyRedefiner ctor 0x00558540).
// Shape: /O2 (new_size is loaded before the zero fill, as in the standalone resize(n) at 0x00412AA0).
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
template <class T> class CPersistContext_TransferableVector {
public:
    void*                _vfptr_or_pad;                  // +0x00 vptr
    std::vector<T>*      TransVector;                    // +0x04
    void Resize(long new_size) { TransVector->resize(new_size); }
};
template class CPersistContext_TransferableVector<CActionInputControl>;