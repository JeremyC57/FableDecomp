// CIntelligentPointer<CThingBuilding>::operator==(const CIntelligentPointer<CThingBuilding>& other) const -- true when both smart pointers hold the same item
// Leaf: ??8?$CIntelligentPointer@
// Retail: Fable.exe @ 0x0080DD90, 32 bytes, `ret 4`: __fastcall (this in ecx -> esi, `other` the one stack argument).
// No callers (an unreferenced COMDAT instantiation kept by the linker); the catalog label operator==<CThingBuilding>
// comes from the Ego_r.pdb template (there a /Gr free function); the retail body is a member with `this` in ecx,
// and nothing in the bytes pins T (the body is the same for every CIntelligentPointer<T>).
// Shape: `other.GetPItem()` is called first and kept in edi, then this->GetPItem() in eax, `cmp edi,eax ; sete al`
// (a bool result with no `xor eax,eax` before setcc comes from the `? true : false` form).
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50.
class CBaseObject;
class CThingBuilding;
class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* GetPItem() const;                       // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual void v00();
    bool operator==(const CIntelligentPointer<T>& other) const;
};
template <class T> bool CIntelligentPointer<T>::operator==(const CIntelligentPointer<T>& other) const
{
    CBaseObject* item = other.GetPItem();
    return item == GetPItem() ? true : false;
}
template class CIntelligentPointer<CThingBuilding>;