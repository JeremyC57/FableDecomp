// CVertexShaderInput::AddStream(unsigned long streamIndex, const CVertexStreamFormat& format, bool generateDeclaration) -- append a {stream index, format copy} description to the input's stream list (+4), then optionally rebuild the D3D declaration
// Leaf: ?AddStream@CVertexShaderInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB3B40, length 160 bytes, `ret 0Ch`.
// Callees: CArray<CVertexStreamFormat::CDataElement>::CArray(const CArray&) @ 0x00A5E0A0 (twice: the temporary and the
//   placement copy), vector<CStreamInfo>::_M_insert_overflow @ 0x00AB4540 (landed), CRT free @ 0x00BFEA14 (the
//   temporary's element array), CBaseClass::~CBaseClass @ 0x0099A2E0, CVertexShaderInput::GenerateD3DDeclaration
//   @ 0x00AB3870 (private).
// Identity: Ego_r.pdb `?AddStream@CVertexShaderInput@@QAEXKABVCVertexStreamFormat@@_N@Z` with
//   `??0CStreamInfo@CVertexShaderInput@@QAE@KABVCVertexStreamFormat@@@Z`; callers are the landed
//   CVertexShaderInputStatic/Animated::Initialise (0x00A905FC / 0x00A906D3), which already name 0x00AB3B40. The
//   manifest's "CVertexShaderInput_InsertElementAt" is descriptive.
// Layout: CVertexShaderInput = vptr word +0, DataStreams (12-byte CArray<CStreamInfo>) +4; CStreamInfo 20 bytes
//   (StreamIndex, CVertexStreamFormat); CVertexStreamFormat 16 bytes (vptr 0x0129D578, DataElements +4).
// Shape: `DataStreams.push_back(CStreamInfo(streamIndex, format))` -- the 20-byte temporary on the frame (index, derived
//   vtable, array copy), STLport push_back inlined (placement copy or _M_insert_overflow with the tag homed on the
//   flag's argument slot), then the temporary's compiler-generated destructor (inline array free, out-of-line base
//   destructor, no vtable re-store); the flag is tested last. The tag lands on the (live) flag slot only when the
//   overflow body is in the TU as a noinline member that never reads its tag (declaration-only homes it on the dead
//   streamIndex slot: `lea edx,[esp+28h]` v retail `[esp+30h]`).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);             // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CBaseClass {
public:
    virtual ~CBaseClass();                         // 0x0099A2E0
};

template <class T> class CArray {                  // retail 12-byte array: start, finish, end of storage
public:
    T* m_first;
    T* m_last;
    T* m_end;
    CArray(const CArray& other);                   // 0x00A5E0A0 (CDataElement instance)
    ~CArray() { if (m_first) free(m_first); }
};

class CVertexStreamFormat : public CBaseClass {    // vtable 0x0129D578, 16 bytes; compiler-generated destructor
public:
    struct CDataElement { unsigned long Usage; long Type; };
    CArray<CDataElement> DataElements;             // +0x04
    CVertexStreamFormat(const CVertexStreamFormat& other) : DataElements(other.DataElements) { }
};

namespace _STL {

struct __false_type { __false_type() { } };

template <class T> class allocator { public: allocator() { } };

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&);                        // 0x00AB4450
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&);   // 0x00AB4490
template <class _ForwardIter>
void __fastcall _Destroy(_ForwardIter __first, _ForwardIter __last);                      // range destructor

template <class Value, class T, class Alloc> class _STLP_alloc_proxy : public Alloc {
public:
    Value _M_data;
};

template <class T, class Alloc> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;
};

template <class T, class Alloc = allocator<T> > class vector : public _Vector_base<T, Alloc> {
public:
    // 0x00AB4540 (landed): STLport 4.6 _M_insert_overflow. Its body is in this TU in retail; compiling it here as a
    // noinline member that never reads its tag parameter lets VC7.1 home the push_back's tag temporary on the flag's
    // argument slot like retail (GOTCHAS: STLport tag on a live argument slot).
    __declspec(noinline) void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLen, bool atEnd = false)
    {
        const unsigned int oldSize = (unsigned int)(this->_M_finish - this->_M_start);
        const unsigned int len = oldSize + (oldSize < fillLen ? fillLen : oldSize);
        T* newStart = len ? (T*)malloc(len * sizeof(T)) : 0;
        T* newFinish = __uninitialized_copy(this->_M_start, position, newStart, __false_type());
        if (fillLen == 1) {
            new (newFinish) T(x);
            ++newFinish;
        }
        else
            newFinish = __uninitialized_fill_n(newFinish, fillLen, x, __false_type());
        if (!atEnd)
            newFinish = __uninitialized_copy(position, this->_M_finish, newFinish, __false_type());
        _Destroy(this->_M_start, this->_M_finish);
        if (this->_M_start)
            free(this->_M_start);
        this->_M_start = newStart;
        this->_M_finish = newFinish;
        this->_M_end_of_storage._M_data = newStart + len;
    }
    void push_back(const T& x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) T(x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, x, __false_type(), 1UL, true);
    }
};

}   // namespace _STL

class CVertexShaderInput {
public:
    class CStreamInfo {                            // 20 bytes
    public:
        unsigned long       StreamIndex;           // +0x00
        CVertexStreamFormat StreamFormat;          // +0x04
        CStreamInfo(unsigned long streamIndex, const CVertexStreamFormat& format)
            : StreamIndex(streamIndex), StreamFormat(format) { }
        CStreamInfo(const CStreamInfo& other) : StreamIndex(other.StreamIndex), StreamFormat(other.StreamFormat) { }
    };
    void*                         __vftable;       // +0x00
    _STL::vector<CStreamInfo>     DataStreams;     // +0x04
    void AddStream(unsigned long streamIndex, const CVertexStreamFormat& format, bool generateDeclaration);
private:
    void GenerateD3DDeclaration();                 // 0x00AB3870
};

void CVertexShaderInput::AddStream(unsigned long streamIndex, const CVertexStreamFormat& format, bool generateDeclaration)
{
    DataStreams.push_back(CStreamInfo(streamIndex, format));
    if (generateDeclaration)
        GenerateD3DDeclaration();
}