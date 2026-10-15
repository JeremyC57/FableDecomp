// CTCCameraPointDefinitionBase::GetPointType<CTCCameraPointFixedPoint>(CTCCameraPointFixedPoint** out) -- typed downcast query: if the virtual point-type id is 6, store this in *out and return true
// Leaf: GetPointType
//
// Retail 0x005ECFD0, 31 bytes. One instantiation of the CTCCameraPointDefinitionBase::GetPointType<T>
// template: `mov eax,[esi] ; call [eax+70h]` is the virtual point-type getter (slot 28, 0x70 / 4),
// compared with the instantiation's type id (6 here), then `*out = this ; return true` or
// `return false`. The out pointer is the only stack argument (`ret 4`); `this` stays in esi
// across the virtual call, so it is reused for the store. The manifest label names the
// CTCCameraPointFixedPoint instantiation on every byte-identical twin; the type id 6 is what this
// body actually tests (the label is propagated, the T is not proven).
// No callees except the virtual slot.

class CTCCameraPointDefinitionBase
{
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual long GetPointTypeID() const;            // slot 28 (+0x70)

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
    enum { POINT_TYPE_ID = 6 };
};

template bool CTCCameraPointDefinitionBase::GetPointType<CTCCameraPointFixedPoint>(CTCCameraPointFixedPoint**);
