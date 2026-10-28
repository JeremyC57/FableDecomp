// CBankFileEntryUpdateData::SetFilenames(const CArray<CCharString>& names) -- size the entry's filename list to match and copy every name across
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CC840, 104 bytes (+8 int3), __fastcall, ret 4.
// Callees: CCharString::CCharString() 0x0099E4B0, CSmallVector<CCharString,8>::resize(long, const CCharString&)
// 0x009D2660 (labelled with another instantiation), CCharString::~CCharString 0x0099EAE0, CCharString::operator=
// 0x0099EFB0.
// Layout: Ego_r.pdb CBankFileEntryUpdateData -- Filenames (CSmallVector<CCharString,8>, data pointer first) at
// +0x0C; the source array is a retail CArray (first / last / end of storage).
// Shape: the default-constructed fill string is a temporary homed in the dead `names` argument slot (the
// array reference lives in esi), destroyed right after the resize; the copy loop re-derives names.size()
// (`(last-first) >> 2`, unsigned compare) after every assignment, and both element addresses are formed
// as `begin() + i` from the scaled index (the STLport `*(begin() + n)` operator[] shape).
class CCharString {
public:
    void* m_pData;
    CCharString();                                   // 0x0099E4B0
    ~CCharString();                                  // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

template <class T>
class CArray {                                       // retail CArray: 12 bytes
public:
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    const T* begin() const { return m_pFirst; }
    unsigned long size() const { return m_pLast - m_pFirst; }
    const T& operator[](unsigned long n) const { return *(begin() + n); }
};

template <class T, int N>
class CSmallVector {
public:
    T* m_pData;                                      // +0
    unsigned long m_count;                           // +4
    T* begin() { return m_pData; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    void resize(long count, const T& fill);          // <CCharString,8>: 0x009D2660
};

class CBankFileEntryUpdateData {
public:
    unsigned long StateBlockCRC;                     // +0x00
    unsigned long InfoSize;                          // +0x04
    char* InfoBuffer;                                // +0x08
    CSmallVector<CCharString, 8> Filenames;          // +0x0C
    void SetFilenames(const CArray<CCharString>& names);
};

void CBankFileEntryUpdateData::SetFilenames(const CArray<CCharString>& names)
{
    Filenames.resize(names.size(), CCharString());
    for (unsigned long i = 0; i < names.size(); ++i)
        Filenames[i] = names[i];
}