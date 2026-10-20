// CCameraTagVector::_M_insert_overflow(SCameraTagEntry* position, const SCameraTagEntry& x, const __false_type&, unsigned long fill_len, bool atend) -- STLport vector<pair<float, CSubtitledMessage::CCameraTag> > regrowth: allocate max(size, n) more, copy the head, construct the n new elements, copy the tail unless appending, free the old block
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D1360, 247 bytes (+9 int3 padding), `ret 14h`. Label correction: the catalog's
//   "CSubtitledMessage::ReserveAndInsertCameraTags" is propagated; the body is the STLport member of the CameraTags vector
//   map's vector (element 28 bytes: the 0x92492493 / sar 4 divide, `imul 1Ch`), called from push_back 0x004D1934.
// Callees (`/Gr` SGI-style free templates, `__fastcall`): __uninitialized_copy 0x004CFA10 (first ecx, last edx, result +
//   tag on the stack), __uninitialized_fill_n 0x004CFA70 (first ecx, n edx, x + tag on the stack); CRT malloc 0x00BFEA0E
//   / free 0x00BFEA14. The element destructor is trivial, so _M_clear is only the null-tested free.
// Shape: the landed CActiveEntityScriptVector::_M_insert_overflow (0x00CBC850) pattern: `(max)(old_size, fill_len)`
//   through const references (`old_size` in the push-ecx frame word, `lea ecx` of the winner), `n != 0 ? malloc(n*28) :
//   0`, the single-element case the inlined placement copy (the pair's memberwise copy: the float key as a dword, the
//   CCameraTag by its implicit copy through `lea ecx,[edi+4]`), every `__false_type()` tag homed in the `atend` slot.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
struct __false_type { __false_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }

enum EConversationProtagonist { PROTAGONIST_NONE };
enum EConversationCameraPosition { CAMERA_POSITION_NONE };
enum EConversationCameraFocus { CAMERA_FOCUS_NONE };
enum EConversationCameraZoomType { CAMERA_ZOOM_NONE };
enum EConversationCameraView { CAMERA_VIEW_NONE };

struct CCameraTag {                                    // CSubtitledMessage::CCameraTag (24 bytes, Ego_r.pdb)
    EConversationProtagonist    CameraPositionPerson;  // +0x00
    EConversationCameraPosition CameraPosition;        // +0x04
    EConversationProtagonist    CameraFocusPerson;     // +0x08
    EConversationCameraFocus    CameraFocusPosition;   // +0x0c
    EConversationCameraZoomType CameraZoomType;        // +0x10
    EConversationCameraView     CameraView;            // +0x14
};
struct SCameraTagEntry {                               // pair<float, CCameraTag> (28 bytes)
    float      first;                                  // +0x00
    CCameraTag second;                                 // +0x04
    SCameraTagEntry(const SCameraTagEntry& other) : first(other.first), second(other.second) { }
};
SCameraTagEntry* __fastcall __uninitialized_copy(SCameraTagEntry* first, SCameraTagEntry* last,
                                                 SCameraTagEntry* result, const __false_type&);    // 0x004CFA10
void* FindRef_free___uninitialized_copy_2();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail, as in the landed 0x00CBC850): it only writes a word through the
// reference parameters inline, so VC7.1 knows the caller's address-taken tag temporaries do not escape the call.
#pragma auto_inline(off)
__declspec(noinline) SCameraTagEntry* __fastcall __uninitialized_copy(SCameraTagEntry* first, SCameraTagEntry* last, SCameraTagEntry* result, const __false_type& p3)
{
    void* p = FindRef_free___uninitialized_copy_2();
    if (!p)
        return 0;
    *(void**)first = p;
    *(void**)last = p;
    *(void**)result = p;
    return 0;
}
#pragma auto_inline(on)
SCameraTagEntry* __fastcall __uninitialized_fill_n(SCameraTagEntry* first, unsigned long n,
                                                   const SCameraTagEntry& x, const __false_type&); // 0x004CFA70
void* FindRef_free___uninitialized_fill_n_1();   // stand-in callee of the stand-in body below
// Stand-in body (see above).
#pragma auto_inline(off)
__declspec(noinline) SCameraTagEntry* __fastcall __uninitialized_fill_n(SCameraTagEntry* first, unsigned long n, const SCameraTagEntry& x, const __false_type& p3)
{
    void* p = FindRef_free___uninitialized_fill_n_1();
    if (!p)
        return 0;
    *(void**)first = p;
    *(void**)&x = p;
    return 0;
}
#pragma auto_inline(on)
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }

struct CCameraTagVector {                              // STLport vector (12 bytes in retail)
    SCameraTagEntry* _M_start;
    SCameraTagEntry* _M_finish;
    SCameraTagEntry* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    static SCameraTagEntry* allocate(unsigned long n)
    {
        return n != 0 ? (SCameraTagEntry*)malloc(n * sizeof(SCameraTagEntry)) : 0;
    }
    static void deallocate(SCameraTagEntry* p)
    {
        if (p)
            free(p);
    }
    void _M_clear()
    {
        deallocate(_M_start);
    }
    void _M_set(SCameraTagEntry* s, SCameraTagEntry* f, SCameraTagEntry* e)
    {
        _M_start = s;
        _M_finish = f;
        _M_end_of_storage = e;
    }
    void _M_insert_overflow(SCameraTagEntry* position, const SCameraTagEntry& x, const __false_type&,
                            unsigned long fill_len, bool atend);
};

void CCameraTagVector::_M_insert_overflow(SCameraTagEntry* position, const SCameraTagEntry& x,
                                          const __false_type&, unsigned long fill_len, bool atend)
{
    const unsigned long old_size = size();
    const unsigned long len = old_size + (max)(old_size, fill_len);
    SCameraTagEntry* new_start = allocate(len);
    SCameraTagEntry* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    }
    else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}
