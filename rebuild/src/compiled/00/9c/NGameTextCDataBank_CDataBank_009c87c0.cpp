// NGameText::CDataBank::~CDataBank() -- close the text and group data, destroy the narrator, group, text and entry arrays, then the secondary base and the CBankFileAsync base
// Leaf: ??1CDataBank@NGameText@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C87C0, 202 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: catalogued as "CGameDataBank_DestructorHelper" (descriptive). It stores NGameText::CDataBank's vtables
// (rtti-vtables.tsv: primary 0x0129B4D4 at +0, the secondary base's 0x0129B528 at +0x164); FableWin/Ego donor
// ??1CDataBank@NGameText@@UAE@XZ (0x00B17970). Only caller: the scalar deleting destructor at 0x009CB843.
// Callees: CDataBank::CloseTextAndGroups 0x009C8660 (donor ?CloseTextAndGroups@CDataBank@NGameText@@IAEXXZ),
// CCharString::~CCharString 0x0099EAE0, the LTCG `__fastcall` element-range destroys std::__destroy_aux 0x009CAC00
// (CDataBankGroupEntry, 4-byte elements) and 0x009CABC0 (CDataBankTextEntry, 8-byte elements) -- ecx = first,
// edx = last, the __false_type tag temporary by reference (`lea eax,[esp+13h]`, the push-ecx slot) -- MSVCR71 free
// 0x00BFEA14, the secondary base's dtor 0x009FC690 and ~CBankFileAsync 0x009D6490 (landed).
// Layout: Ego_r.pdb CDataBank Entries / TextEntryBuffer / GroupEntryBuffer / Narrators (+0x254/+0x264/+0x274/+0x284 with
// 16-byte arrays) sit at retail +0x1E0/+0x1EC/+0x1F8/+0x204 (retail CBankFileAsync 0x164 bytes, secondary base 0x7C bytes,
// STLport 12-byte vectors); the Ego_r NarratorMap is not in the retail object (nothing destroyed after +0x204).
// Shape: as the landed NParticleEngine::CParticleDataBank dtor 0x00AF1600: the body's CloseTextAndGroups, then the
// members in reverse order (the CCharString array's __destroy_aux inlines to a call loop; the two refcounted entry
// types call their out-of-line instantiations), then the bases in reverse order.
extern "C" void __cdecl free(void* block);   // 0x00BFEA14

class CCharString
{
public:
    void* PData;                                       // +0x00 shared string data
    ~CCharString();                                    // 0x0099EAE0
};

namespace NGameText
{
    class CDataBankEntry;
    class CDataBankTextEntry                           // 8 bytes: an intrusive counted pointer + a word
    {
    public:
        void*         m_pText;
        unsigned long m_value;
    };
    class CDataBankGroupEntry                          // 4 bytes: an intrusive counted pointer
    {
    public:
        void* m_pGroup;
    };
}

namespace std
{
    struct __true_type { __true_type() { } };
    struct __false_type { __false_type() { } };

    template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
    template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_destructor; };

    template <class _Tp>
    inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

    template <class _ForwardIterator>
    inline void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
    {
        for (; __first != __last; ++__first)
            _Destroy(&*__first);
    }

    template <class _ForwardIterator>
    inline void __fastcall __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }

    template <> void __fastcall __destroy_aux<NGameText::CDataBankTextEntry*>(NGameText::CDataBankTextEntry* __first,
        NGameText::CDataBankTextEntry* __last, const __false_type&);        // 0x009CABC0
    template <> void __fastcall __destroy_aux<NGameText::CDataBankGroupEntry*>(NGameText::CDataBankGroupEntry* __first,
        NGameText::CDataBankGroupEntry* __last, const __false_type&);       // 0x009CAC00

    template <class _ForwardIterator, class _Tp>
    inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
    {
        typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
        __destroy_aux(__first, __last, _Trivial_destructor());
    }

    template <class _Tp>
    inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, __first); }

    template <class _Tp>
    class _Vector_base
    {
    public:
        ~_Vector_base() { if (_M_start != 0) free(_M_start); }
        _Tp* _M_start;
        _Tp* _M_finish;
        _Tp* _M_end_of_storage;
    };

    template <class _Tp>
    class vector : public _Vector_base<_Tp>
    {
    public:
        ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    };
}

template <class T>
class CArray : public std::vector<T> { };             // retail 12 bytes

class CBankFileAsync                                   // landed ~CBankFileAsync 0x009D6490
{
public:
    virtual ~CBankFileAsync();                         // 0x009D6490
    unsigned char _pad04[0x164 - 0x04];
};

class CBankSecondaryBase                               // the second base (vtable 0x0129B528 in CDataBank)
{
public:
    virtual ~CBankSecondaryBase();                     // 0x009FC690
    unsigned char _pad04[0x7c - 0x04];
};

namespace NGameText
{
    class CDataBank : public CBankFileAsync, public CBankSecondaryBase
    {
    public:
        CArray<CDataBankEntry*>     Entries;           // +0x1E0
        CArray<CDataBankTextEntry>  TextEntryBuffer;   // +0x1EC
        CArray<CDataBankGroupEntry> GroupEntryBuffer;  // +0x1F8
        CArray<CCharString>         Narrators;         // +0x204
        virtual ~CDataBank();
    protected:
        void CloseTextAndGroups();                     // 0x009C8660
    };

    CDataBank::~CDataBank()
    {
        CloseTextAndGroups();
    }
}