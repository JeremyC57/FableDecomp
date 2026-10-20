// CThing::AddAndCopyTC(const CTCBase* other, bool flag) -- adds a component of the same name as the given one, then copies the given component into the thing's component of that type (retail reads an unset pointer when the type is absent)
// Grade: DIFFER(ours v retail) -- 168v166: this/type callee-saved colouring swapped (retail this=edi reused as the map pointer, type=esi); the out-param/map-pointer forms keep the colouring but load the component into eax and copy it to ecx where retail loads straight into ecx
// Leaf: AddAndCopyTC
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C9F60, 166 bytes. `ret 8`.
// Callees: CTCBase virtual slot 22 (+0x58, name by hidden pointer) and slot 21 (+0x54, type) on the source component,
//          CThing::AddTC(const CCharString&, bool, CTCParamsBase*) 0x004C9D60, ~CCharString 0x0099EAE0, puts 0x00BFEAF2 /
//          abort 0x00BFEAEC (the STLport bitset range check, "bitset" @ 0x0122E068), CVectorMap::LowerBound(const long*)
//          0x0040F020, CTCBase virtual slot 12 (+0x30, copy from) on the found component.
// Shape: the name temporary and later the lookup key both live in the second argument's home slot (the flag is loaded
//        into eax before); the type is kept in esi through the range-checked presence test; the copy call is duplicated
//        for the found and not-found tails, the latter reading the out pointer the lookup never wrote (retail bug kept).
// Layout: UsingComponents bitset words +0x20, Components pairs +0x44/+0x48.
extern "C" int  __cdecl puts(const char* text);     // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();   // 0x00BFEAEC
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                  // 0x0099EAE0
};
struct CTCParamsBase;
struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void CopyFrom_slot12(const CTCBase* other);   // slot 12 (+0x30)
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual long GetType_slot21() const;             // slot 21 (+0x54)
    virtual CCharString GetName_slot22() const;      // slot 22 (+0x58)
};
struct CPresenceBits {                               // std::bitset<274>
    unsigned long words[9];
    bool test(unsigned long position) const
    {
        if (position >= 274) {
            puts("bitset");
            abort();
        }
        return (words[position >> 5] & (1UL << (position & 31))) != 0;
    }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits UsingComponents;                   // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    __forceinline CTCBase* FindTC(long wanted)        // the inlined map find: lower bound on a key temporary, end when absent
    {
        long type = wanted;
        CInterfaceMap::SEntry* it = m_interfaces.LowerBound(&type);
        if (it == m_interfaces.last || wanted < it->type)
            it = m_interfaces.last;
        return it->component;
    }
    CTCBase* AddTC(const CCharString& name, bool a2, CTCParamsBase* params);   // 0x004C9D60
    void AddAndCopyTC(const CTCBase* other, bool flag);
};
void CThing::AddAndCopyTC(const CTCBase* other, bool flag)
{
    AddTC(other->GetName_slot22(), flag, 0);
    long type = other->GetType_slot21();
    CTCBase* tc;                                     // left unset when the type is absent (retail reads it anyway)
    if (UsingComponents.test(type))
        tc = FindTC(type);
    tc->CopyFrom_slot12(other);
}
