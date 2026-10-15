// CTCCameraPointDefinitionBase::GetPointType<CTCCameraPointFixedPoint>(CTCCameraPointFixedPoint** out) -- typed downcast query through vtable slot 3: if the virtual type id is 1, store this in *out and return true
// Leaf: GetPointType
//
// Retail 0x00737160, 31 bytes. Same template shape as the CTCCameraPointDefinitionBase::GetPointType<T>
// twins (0x005ECFD0 and friends), but the type getter sits in vtable slot 3 (`call [eax+0Ch]`),
// so this instantiation belongs to a different polymorphic root than the camera-point classes (whose
// getter is slot 28): the manifest label is propagated over the byte-identical shape and the real
// class is unproven. The instantiation tests type id 1; because eax already holds 1 on the taken
// branch VC7.1 writes the `true` result as `mov al,al` instead of `mov al,1` (same source as the
// `mov al,1` twins, only the constant differs). `ret 4`: the out pointer is the only argument.
// No callees except the virtual slot.

class CTCCameraPointDefinitionBase
{
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual long GetPointTypeID() const;            // slot 3 (+0x0c)

    template <class T> bool GetPointType(T** out)
    {
        if (GetPointTypeID() == T::POINT_TYPE_ID)
        {
            *out = static_cast<T*>(this);
            return true;
        }
        return false;
    }
};

class CTCCameraPointFixedPoint : public CTCCameraPointDefinitionBase
{
public:
    enum { POINT_TYPE_ID = 1 };
};

template bool CTCCameraPointDefinitionBase::GetPointType<CTCCameraPointFixedPoint>(CTCCameraPointFixedPoint**);