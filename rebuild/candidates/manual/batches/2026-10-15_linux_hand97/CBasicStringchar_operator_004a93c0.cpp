// CBasicString<char>::operator==(const CBasicString<char>&) const -- equal lengths and Compare(PData, other.PData) == 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004A93C0, length 34 bytes (catalogued as the propagated label
// "CTCParticleAttacher::OnInitialActivate"; the PDB names it ??8?$CBasicString@D@@QBE_NABV0@@Z, and its 71 callers
// inline CCharString's null/same-block tests and call it on two CCharStringData blocks, whose first member is the
// CBasicString<char>: PData +0, StringLength +4).
// Leaf: ??8
// Callees: CBasicString<char>::Compare(const char*, const char*) @ 0x00411570 (protected static __fastcall long).
// Shape: the length test is `this->StringLength` against the argument's (`cmp edx,[eax+4]`), the early false is `xor al,al`;
// the Compare result is turned into the bool with `neg ; sbb eax,eax ; inc eax`.
template<class T> class CBasicString {
public:
    T*            PData;                            // +0x00
    unsigned long StringLength;                     // +0x04
    unsigned long DataLength;                       // +0x08 (31 bits) + UseFastExtend
    bool operator==(const CBasicString& other) const;
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570
};

template<class T> bool CBasicString<T>::operator==(const CBasicString<T>& other) const
{
    if (StringLength != other.StringLength)
        return false;
    return Compare(PData, other.PData) == 0;
}

template class CBasicString<char>;
