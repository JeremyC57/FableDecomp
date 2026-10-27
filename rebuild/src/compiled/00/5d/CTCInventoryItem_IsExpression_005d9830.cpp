// CTCInventoryItem::IsExpression(long defIndex) -- static: cache the defs for defIndex, then report whether the cached thing def has a CExpressionSubDef sub-definition
// Leaf: ?IsExpression@CTCInventoryItem@@SI_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D9830, 64 bytes, static `__fastcall` (defIndex in ecx straight on to CacheDef), `ret`.
// Identity (probable): FableWin.pdb ?IsExpression@CTCInventoryItem@@SI_NJ@Z; its GetSubDef callee 0x005DA240 builds the sub-def class name
//   "CExpressionSubDef" (the GetSubDef<CExpressionSubDef> instantiation).
// Callees: CTCInventoryItem::CacheDef(long) 0x005D94B0, CDefClassBase::GetSubDef<CExpressionSubDef>(CDefPointer<const CExpressionSubDef>*) const
//   0x005DA240 on the cached thing def (global 0x013B8C14), the sub-def's virtual slot 1 on the last release.
// Shape (family K002, representative 0x005D97F0): the landed CTCInventoryItem::GetContextSensitiveType 0x005D9B90 with a bool
//   result: the out slot is the `push ecx` word zeroed after its address is pushed, the result parked in bl across the
//   CDefPointer release (null-tested `dec [ecx+4] ; jne`);
//   GetSubDef<T> has a same-TU noinline stand-in body (as the landed row): it only fills the out slot.
class CDefPointeeBase {
public:
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
template <class T> class CDefPointer {                  // 4 bytes; the pointee seen through its counted base
public:
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->Release(); }
};
class CDefClassBase : public CDefPointeeBase {
public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x005DA240 for CExpressionSubDef
};
class CThingDef : public CDefClassBase { };
class CExpressionSubDef;
CDefPointeeBase* FindDef_005d9830(const void* owner);       // stand-in callee of the stand-in body below
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    out->Object = FindDef_005d9830(this);
    return out->Object != 0;
}
#pragma auto_inline(on)
template <class T> struct CThingDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
class CTCInventoryItem {                                // static API only
public:
    static bool __fastcall CacheDef(long defIndex);                     // 0x005D94B0
    static bool __fastcall IsExpression(long defIndex);
    static CThingDefPointer<const CThingDef> s_CachedThingDef;        // 0x013B8C14
};
bool __fastcall CTCInventoryItem::IsExpression(long defIndex)
{
    if (CacheDef(defIndex)) {
        CDefPointer<const CExpressionSubDef> sub;
        return s_CachedThingDef->GetSubDef(&sub);
    }
    return false;
}