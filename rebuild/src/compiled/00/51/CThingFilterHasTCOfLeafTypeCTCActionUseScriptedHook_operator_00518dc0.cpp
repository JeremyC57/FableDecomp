// CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>::operator()(CThing& thing) const -- thing filter: true when the thing has interface 0xC2 and that component's class type name is "CTCActionUseScriptedHook"
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- a same-TU read-only noinline CInterfaceMap::LowerBound body (0x0040F020, the escaping callee): the key dies at the call and shares the dead thing slot with the literal temporary
// Leaf: ??R?$CThingFilter_HasTCOfLeafType@VCTCActionUseScriptedHook@@@@QBE_NAAVCThing@@@Z
//
// Retail 0x00518DC0, 181 bytes (then int3 padding), `ret 4` (thiscall; the filter `this` is unused).
// Callees: 0x0040F020 CVectorMap::LowerBound (inline GetTC), 0x0099EBF0 CCharString(const char*, int = -1) on the literal
//   0x01239FE4 "CTCActionUseScriptedHook", the component's virtual slot 22 (+0x58) GetClassTypeName() (CCharString by
//   hidden pointer into the frame word), 0x00411570 the string compare, 0x0099EAE0 ~CCharString (both temporaries).
// Callers: 0x005198B0 and 0x00618E60 (CThingSearchTools::GetAllThings instances; the landed 0x00618E60 declares it).
// Identity: Ego_r.pdb `??R?$CThingFilter_HasTCOfLeafType@VCTCActionUseScriptedHook@@@@QBE_NAAVCThing@@@Z` and
//   `??$GetTCOfLeafType@VCTCActionUseScriptedHook@@@CThing@@QAE_NPAPAV...@Z` (name set only).
// Shape: HasTC (bit 2 of the +0x38 word) -> the inline lookup -> `GetClassTypeName() == StatGetClassTypeName()` with
//   the nested-inline CCharString operator== (the landed SameText model: same data -> true, a null side -> false,
//   length mismatch -> `xor al,al ; mov bl,al`, else `!Compare`), the operands built right to left (literal first),
//   destroyed in reverse, then `if (same) return true; return false;`.
// Tried (4 grades): GetTCOfLeafType plain inline (stays out of line: 17 v 181); __forceinline with the landed GetTC
//   (named key copy: own slot); the key as a temporary bound to `const ETCInterfaceType&` (arg slot, but the compare
//   then reloads it and the literal loses the slot); a by-value Find converting to `const long&` (constant compare,
//   key still in its own slot).
// Next: find the source form whose key temporary dies before the literal is built (VC then overlays both on the
//   `thing` slot), e.g. the key from a separate GetTC<T>(ETCInterfaceType, T**) helper as Ego_r lists.
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570 (strcmp-style, 0 = equal)

class CCharStringData { public:
    char*         m_pData;                      // +0x0
    unsigned long m_length;                     // +0x4
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, int length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
    bool SameText(const CCharStringData& other) const
    {
        if (PStringData->m_length != other.m_length)
            return false;
        return Compare_00411570(PStringData->m_pData, other.m_pData) ? false : true;
    }
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        return SameText(*other.PStringData);
    }
};

enum ETCInterfaceType { TC_INTERFACE_ACTION_USE_SCRIPTED_HOOK = 0xC2 };   // enumerator name descriptive

class CThing;
class CTCBase {
public:
    virtual ~CTCBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21();
    virtual CCharString GetClassTypeName() const;   // slot 22 (+0x58)
    CThing* ParentThing;                        // +0x04
};
class CTCActionUseScriptedHook : public CTCBase {
public:
    static ETCInterfaceType StatGetInterfaceType() { return TC_INTERFACE_ACTION_USE_SCRIPTED_HOOK; }
    static CCharString StatGetClassTypeName() { return CCharString("CTCActionUseScriptedHook"); }   // literal 0x01239FE4
};

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type);       // 0x0040F020
    __forceinline CTCBase* Find(ETCInterfaceType type)
    {
        SEntry* it = LowerBound(type);
        SEntry* sel = (it == last || type < it->type) ? last : it;
        return sel->component;
    }
};

// LowerBound @ 0x0040F020: a same-TU read-only stand-in (GOTCHAS 665, the escaping callee): with its body visible VC7.1
// knows the key's address does not outlive the call, so the key dies before the literal temporary is built and both
// share the dead `thing` slot as retail.
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long& type)
{
    SEntry* lo = first;
    int len = last - first;
    while (len > 0) {
        int half = len >> 1;
        SEntry* middle = lo + half;
        if (middle->type < type) {
            lo = middle + 1;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return lo;
}
#pragma auto_inline(on)

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 bitset<274> of present interface types
    CInterfaceMap m_interfaces;                 // +0x44
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template <class T> __forceinline bool GetTCOfLeafType(T** out)
    {
        if (HasTC(T::StatGetInterfaceType())) {
            CTCBase* tc = m_interfaces.Find(T::StatGetInterfaceType());
            if (tc->GetClassTypeName() == T::StatGetClassTypeName()) {
                *out = (T*)tc;
                return true;
            }
        }
        return false;
    }
};

template <class T> class CThingFilter_HasTCOfLeafType {
public:
    CThingFilter_HasTCOfLeafType() {}
    bool operator()(CThing& thing) const;
};
template <class T> bool CThingFilter_HasTCOfLeafType<T>::operator()(CThing& thing) const
{
    T* tc;
    return thing.GetTCOfLeafType(&tc);
}
template class CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>;