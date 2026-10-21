// CInputTypeMouseWheelMovementUpEvent::operator==(const CInputTypeBase& other) const -- same input type when both report the same type id
// Leaf: ??8CInputTypeMouseWheelMovementUpEvent@@UBE_NABVCInputTypeBase@@@Z
// Retail: Fable.exe @ 0x0066F510, 34 bytes, `ret 4`; CInputTypeMouseWheelMovementUpEvent vtable 0x0123185C slot 2; Ego_r.pdb ??8CInputTypeMouseWheelMovementUpEvent@@UBE_NABVCInputTypeBase@@@Z
// (catalog label CInputTypeMouseMovementEvent::operator== is ICF-propagated). Body: GetType() == other.GetType() (the right operand is called first) through
// slot 1 (+4; this class's entry 0x00445770 returns a constant id; the name GetType is inferred), as `sub ; neg ; sbb ; inc`.
// Layout: CInputTypeBase has only its vptr here; modelled locally with the virtuals for the slot offsets.
class CInputTypeBase {
public:
    virtual ~CInputTypeBase();                           // slot 0
    virtual long GetType() const = 0;                    // slot 1 (+4)
    virtual bool operator==(const CInputTypeBase& other) const = 0;   // slot 2 (+8)
};
class CInputTypeMouseWheelMovementUpEvent : public CInputTypeBase {
public:
    virtual long GetType() const;
    virtual bool operator==(const CInputTypeBase& other) const;
};
bool CInputTypeMouseWheelMovementUpEvent::operator==(const CInputTypeBase& other) const
{
    return GetType() == other.GetType();
}