// CTCSkeletalMorph::AddSkeletalMorph(long type, const CCharString& name, float value) -- append a skeletal-morph entry built from the arguments to the component's morph array and mark the morph as changed
// Leaf: AddSkeletalMorph
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00786670, 131 bytes (+ int3 padding to 144), __fastcall `ret 0Ch`.
// Identity: Ego_r.pdb `?AddSkeletalMorph@CTCSkeletalMorph@@QAEXJABVCCharString@@M@Z`; callers 0x0071E0D3, 0x00728331,
//   0x00786907 (CTCSkeletalMorph::OnCreate).
// Callees: CCharString copy constructor @ 0x0099EC30 (x2: the temporary entry, then the placement copy),
//   _M_insert_overflow(pos, x, __false_type, 1, true) @ 0x00787380, ~CCharString @ 0x0099EAE0.
// Layout: retail CTCSkeletalMorph: Changed +0x0c, Morphs +0x10 (CArray<CSkeletalMorph> = 12-byte STLport vector; the
//   PDB's CArray is 16 bytes), PSkeletalMorph +0x1c; CSkeletalMorph(long, const CCharString&, float) is 12 bytes.
// Shape: `Morphs.push_back(CSkeletalMorph(type, name, value)); Changed = true;` -- the temporary entry is built in the
//   frame (type stored around the name's copy constructor, value after it), CArray::push_back (out of line as
//   0x00787500) is inlined: placement copy under VC's null test, `add dword ptr [esi+4],0Ch`, or the overflow call with
//   the empty __false_type homed in the dead `type` argument slot; the temporary's name dies before `Changed = true`.
// Label correction: catalogued as `Vector_PushBack_TextureMorph`; the body is CTCSkeletalMorph::AddSkeletalMorph.
class CThing;

class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
};

struct __false_type { __false_type() { } };
inline void* operator new(unsigned int, void* where) { return where; }

template <class T> class CArray { public:                       // retail: the 12-byte STLport vector
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long fill_len, bool atend);
    void push_back(const T& x)                           // out of line as 0x00787500
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                 // +0x04
    bool    ToBeRemoved;                                 // +0x08
    bool    IsInGlobalUpdateTCs;                         // +0x09
};

class C3DSkeletalMorph;
template <class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
};

class CTCSkeletalMorph : public CTCBase {
public:
    class CSkeletalMorph { public:                              // 12 bytes
        long        Type;                                // +0x00
        CCharString Name;                                // +0x04
        float       Value;                               // +0x08
        CSkeletalMorph(long type, const CCharString& name, float value) : Type(type), Name(name), Value(value) { }
        CSkeletalMorph(const CSkeletalMorph& o) : Type(o.Type), Name(o.Name), Value(o.Value) { }
    };

    bool                              Changed;           // +0x0c
    CArray<CSkeletalMorph>            Morphs;            // +0x10
    CCountedPointer<C3DSkeletalMorph> PSkeletalMorph;    // +0x1c

    void AddSkeletalMorph(long type, const CCharString& name, float value);
};

template <> void CArray<CTCSkeletalMorph::CSkeletalMorph>::_M_insert_overflow(
    CTCSkeletalMorph::CSkeletalMorph* position, const CTCSkeletalMorph::CSkeletalMorph& x, const __false_type&,
    unsigned long fill_len, bool atend);                 // 0x00787380

void CTCSkeletalMorph::AddSkeletalMorph(long type, const CCharString& name, float value)
{
    Morphs.push_back(CSkeletalMorph(type, name, value));
    Changed = true;
}