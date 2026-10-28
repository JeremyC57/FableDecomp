// std::vector<CActiveSoundInfo>::push_back(const CActiveSoundInfo& x) -- copy-construct the sound record at finish and advance when there is room, else grow through _M_insert_overflow
// Leaf: ?push_back@?$vector@VCActiveSoundInfo@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AACEF0, length 72 bytes (+8 int3), `ret 4`.
// Callees: std::vector<CActiveSoundInfo>::_M_insert_overflow(pointer, const CActiveSoundInfo&, const __false_type&,
//   size_type, bool) @ 0x00AACDA0 (thiscall, five arguments: finish, value, the tag, 1, true).
// Identity: element vtable 0x0129E674 = CActiveSoundInfo (16 bytes); caller 0x00AAC645 is in the CSoundEmitterLH
//   module. The manifest's "Vector_PushBack_SingleElement" is descriptive.
// Shape: STLport 4.6 push_back -- placement copy (null test, implicit copy: vtable store + three words) then
//   `add [ecx+4],10h`; the overflow arm passes `__false_type()` homed on the value argument slot.
inline void* operator new(unsigned int, void* p) { return p; }

class CBaseClass {
public:
    CBaseClass();                                  // 0x0099A2D0
    virtual ~CBaseClass();                         // 0x0099A2E0
};

class CActiveSoundInfo : public CBaseClass {       // vtable 0x0129E674, 16 bytes
public:
    void* m_pSound;                                // +0x04
    void* m_pOwner;                                // +0x08
    void* m_pInfo;                                 // +0x0c
    virtual ~CActiveSoundInfo();
};

namespace std {

struct __false_type { __false_type() { } };

template <class T> class allocator { public: allocator() { } };

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
    protected:
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLen, bool atEnd = false);   // 0x00AACDA0
    public:
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

}   // namespace std

template void std::vector<CActiveSoundInfo>::push_back(const CActiveSoundInfo&);