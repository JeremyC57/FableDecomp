// CTCDExplosion::SetSpecificThingsToHit(const vector<CThing*>& things) -- append an intelligent pointer to each given thing to the explosion's list of things it must hit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080CC40, 162 bytes (`ret 4`; VC7.1 aligns the frame itself, `and esp,-8`, for the
// 8-byte intelligent-pointer temporary).
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10
// (CIntelligentPointer<CThing> vtable 0x01238C6C), the vector's _M_insert_overflow @ 0x004AC940 (manifest
// "Std_Vector_Insert_Range_SmartPtr").
// Layout: Ego_r.pdb CTCDExplosion SpecifiedThingsToHit +0x78 sits at retail +0x6c (three 12-byte vectors before it).
// Shape: an iterator loop over the argument vector re-reading its end through the reference every pass; each element becomes
// a CIntelligentPointer temporary (the item loaded before the base ctor) handed to the inline STLport push_back (placement
// copy: base ctor, vtable, SetPItem(GetPItem()); or _M_insert_overflow with the __false_type tag in the high byte of the
// frame word below the temporary), destroyed through the base dtor.
class CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct __false_type { __false_type() { } };
inline void* operator new(unsigned int, void* where) { return where; }
template <class T> struct CRetailVector {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long fill_len, bool atend);   // 0x004AC940
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
class CTCDExplosion { public:
    virtual void v00();
    unsigned char _pad04[0x6c - 0x04];
    CRetailVector<CIntelligentPointer> SpecifiedThingsToHit;   // +0x6c (PDB +0x78)
    void SetSpecificThingsToHit(const CRetailVector<CThing*>& things);
};

void CTCDExplosion::SetSpecificThingsToHit(const CRetailVector<CThing*>& things)
{
    for (CThing* const* it = things.begin(); it != things.end(); ++it)
        SpecifiedThingsToHit.push_back(CIntelligentPointer(*it));
}