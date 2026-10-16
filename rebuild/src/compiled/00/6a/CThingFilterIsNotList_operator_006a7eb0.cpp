// CThingFilter_IsNotList::operator()(const CThing* thing) -- true when the thing is not in the filter's exclusion list (a raw CThing* vector, else a CIntelligentPointer<CThing> vector)
// Leaf: ??RCThingFilter_IsNotList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7EB0, length 128 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Layout: PDB CThingFilter_IsNotList { vector<const CThing*>* PNotList (+0); vector<CIntelligentPointer<CThing> >* PNotList2 (+4); };
// STLport vector { _M_start, _M_finish, _M_end_of_storage }; CIntelligentPointer is 8 bytes (vptr + item), hence `sar 3`.
// Shape: each arm takes the element count into an int local once (`sub ; sar`, signed `jle`), then an index loop
// using operator[] as `*(begin() + i)`: the first arm strength-reduces to a cursor, the second re-reads begin()
// after every GetPItem call (`lea ecx,[eax+edi*8]`).
class CThing;
template<class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
struct CBaseIntelligentPointer {
    void* m_vfptr;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {};
struct CThingFilter_IsNotList {
    vector<const CThing*>*                  PNotList;    // +0x00
    vector<CIntelligentPointer<CThing> >*   PNotList2;   // +0x04
    bool operator()(const CThing* thing) const;
};

bool CThingFilter_IsNotList::operator()(const CThing* thing) const
{
    if (PNotList) {
        int count = PNotList->size();
        for (int i = 0; i < count; ++i) {
            if ((*PNotList)[i] == thing)
                return false;
        }
        return true;
    }
    int count = PNotList2->size();
    for (int i = 0; i < count; ++i) {
        if ((*PNotList2)[i].GetPItem() == thing)
            return false;
    }
    return true;
}