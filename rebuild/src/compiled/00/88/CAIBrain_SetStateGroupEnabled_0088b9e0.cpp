// CAIBrain::SetStateGroupEnabled(const CCharString& name, bool enabled) -- walks the brain's state groups for one named name and sets its Enabled flag
// Leaf: ?SetStateGroupEnabled@CAIBrain@@QAEXABVCCharString@@_N@Z
// Retail: Fable.exe @ 0x0088B9E0, 93 bytes, thiscall `ret 8`.
// Identity (proven): both PDBs have ?SetStateGroupEnabled@CAIBrain@@QAEXABVCCharString@@_N@Z, and its only caller
//   0x008302D0 forwards CThingAICreature::SetStateGroupEnabled (same PDB signature) through PBrain (+0x1A8).
// Layout: CAIBrain::StateGroupList +0x28 (Ego_r.pdb +0x28: CVectorMap<long, CAIStateGroupBase*, CKeyPairCompareGreater>, retail
//   0x10 with a 12-byte STLport vector); CAIStateGroupBase Name +0x0c, Enabled +0x12 (Ego_r.pdb, no containers ahead).
// Callee: CBasicString<char>::Compare 0x00411570 (?Compare@?$CBasicString@D@@KIJPBD0@Z, the inlined CCharString::operator==;
//   its body sits in the TU under auto_inline(off), as the landed 0x004B8FF0 does).
// Caller: the CThingAICreature forwarder 0x008302C0 / 0x008302D0 (`mov ecx,[ecx+1A8h] ; jmp`, PBrain), which the catalog
//   lands as STLport _Cons_val instantiations (wrong labels).
template <class T> class CBasicString {
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570 (?Compare@?$CBasicString@D@@KIJPBD0@Z)
};
// The landed 0x00411570 body under auto_inline(off) (the landed 0x004B8FF0 idiom): VC7.1 then knows the call writes no
// memory and keeps the searched name's string block in esi across the loop, as retail does.
#pragma auto_inline(off)
template <> long __fastcall CBasicString<char>::Compare(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
class CCharStringData { public:
    const char*   Chars;                                 // +0x00
    unsigned long Length;                                // +0x04
};
class CCharString : public CBasicString<char> {
public:
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->Length == other.PStringData->Length)
            return Compare(PStringData->Chars, other.PStringData->Chars) ? false : true;
        return false;
    }
};
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {                                           // STLport 4.6 vector (12 bytes)
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
};
}
template <class K, class V> class CKeyPairCompareGreater { public: };
template <class K, class V, class Cmp> class CVectorMap {   // retail 0x10: the sorted pair vector + the comparator byte
public:
    typedef std::pair<K, V> value_type;
    typedef typename std::vector<value_type>::iterator iterator;
    typedef typename std::vector<value_type>::const_iterator const_iterator;
    std::vector<value_type> Data;                        // +0x00
    Cmp Compare;                                         // +0x0c
    iterator begin() { return Data.begin(); }
    iterator end() { return Data.end(); }
    const_iterator begin() const { return Data.begin(); }
    const_iterator end() const { return Data.end(); }
};
class CAIStateGroupBase {
public:
    virtual void v00();
    void*         PBrain;                                // +0x04
    long          ScriptAIPriority;                      // +0x08
    CCharString   Name;                                  // +0x0c
    bool          Terminated;                            // +0x10
    unsigned char Priority;                              // +0x11
    bool          Enabled;                               // +0x12
};
class CAIBrain {
public:
    unsigned char _pad00[0x28];
    CVectorMap<long, CAIStateGroupBase*, CKeyPairCompareGreater<long, CAIStateGroupBase*> > StateGroupList;   // +0x28
    bool IsStateGroupPresentAndEnabled(const CCharString& name) const;
    void SetStateGroupEnabled(const CCharString& name, bool enabled);
};
void CAIBrain::SetStateGroupEnabled(const CCharString& name, bool enabled)
{
    for (CVectorMap<long, CAIStateGroupBase*, CKeyPairCompareGreater<long, CAIStateGroupBase*> >::iterator it = StateGroupList.begin();
         it != StateGroupList.end(); ++it)
    {
        if (it->second->Name == name)
        {
            it->second->Enabled = enabled;
            return;
        }
    }
}