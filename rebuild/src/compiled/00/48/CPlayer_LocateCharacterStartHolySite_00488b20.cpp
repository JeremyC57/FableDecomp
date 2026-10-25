// CPlayer::LocateCharacterStartHolySite(CThing** site) -- picks the holy site the character starts at: the one script-named by the start holy site name when no respawn point is set, else the site nearest the respawn position
// Leaf: ?LocateCharacterStartHolySite@CPlayer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00488B20, 494 bytes, `ret 4`.
// Callees: the /Gr (__fastcall) template copy 0x0048D5C0 (hidden result in ecx; copies the list's things whose +0xD4 flag
//   is set onto back_inserter(sites)), CWideString::ConvertToCharString 0x0099B2C0 / operator const wchar_t* 0x0099AF10 on the
//   global 0x013B866C (the start holy site name), CCharString copy 0x0099EC30 / default 0x0099E4B0 / dtor 0x0099EAE0 /
//   Format 0x0099F1F0 ("*** WARNING : failed to find a holy site with ScriptName %S" 0x01238074), SGI __find_if 0x0048CC60
//   (/Gr: first ecx, last edx, the binder by value, the iterator tag), the bound predicate 0x0048BC70
//   (`bool __fastcall(CThing*, CCharString)`: strcmp of the thing's ScriptName +0x74), CThing::GetPosition 0x004C73D0,
//   free 0x00BFEA14 (vector storage).
// Shape: vector<CThing*> sites filled from ThingManager(+0x20)->(+4)->list(+0x8c); without a respawn point (RespawnInfo
//   +0xE8 pos, +0xF4 flag) find_if(bind2nd(ptr_fun(fn), CCharString(name))) through an inline find_if wrapper (the binder is
//   built in the frame then copied); not found -> warning text formatted and the respawn position zeroed; then the site
//   nearest the respawn position (MagSquared, start 1e10f). The owner pointer is a named local declared BEFORE the vector.
// Shape (__find_if): the STLport std::__find_if instantiation is defined in this TU (noinline, /Gr, real four-way unrolled body +
//   remainder switch, tag by const reference) as retail compiled it in the caller's TU; with the body visible VC7.1 homes
//   the iterator-tag temporary in the live `site` argument slot and pushes its address, as retail. find_if, bind2nd and
//   ptr_fun are the STLport inline layers (the binder's operator() copies the bound name per call).
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CCharString {
public:
    void* PStringData;
    CCharString();                                 // 0x0099E4B0
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    void Format(const char* format, ...);          // 0x0099F1F0
};
struct CWideString {
    void* PStringData;
    CCharString ConvertToCharString() const;       // 0x0099B2C0
    operator const unsigned short*() const;               // 0x0099AF10
};
extern CWideString g_CWideString_013b866c;         // 0x013B866C (the character start holy site name)

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
    float MagSquared() const { return X * X + Y * Y + Z * Z; }
};
class CThing {
public:
    const C3DVector& GetPosition() const;          // 0x004C73D0
};

template <class T>
struct CRetailVector {                             // STLport vector<T>: start, finish, end of storage
    T* m_pStart;
    T* m_pFinish;
    T* m_pEndOfStorage;
    CRetailVector() : m_pStart(0), m_pFinish(0), m_pEndOfStorage(0) { }
    ~CRetailVector()
    {
        if (m_pStart)
            free(m_pStart);
    }
    T* begin() const { return m_pStart; }
    T* end() const { return m_pFinish; }
};
template <class C> struct back_insert_iterator {
    C* container;
    explicit back_insert_iterator(C& c) : container(&c) { }
};
template <class C> inline back_insert_iterator<C> back_inserter(C& c) { return back_insert_iterator<C>(c); }

struct SThingNode { SThingNode* _M_next; SThingNode* _M_prev; CThing* _M_data; };
struct SThingListIterator {
    SThingNode* _M_node;
    SThingListIterator(SThingNode* node) : _M_node(node) { }
};
struct CThingList {
    SThingNode* _M_node;
    SThingListIterator begin() const { return SThingListIterator(_M_node->_M_next); }
    SThingListIterator end() const { return SThingListIterator(_M_node); }
};
struct CThingListOwner {
    unsigned char _pad00[0x8c];
    CThingList    HolySites;                       // +0x8c
};
struct CThingManager {
    unsigned char    _pad00[4];
    CThingListOwner* Owner;                        // +0x04
};
// 0x0048D5C0: copies the list's things (those whose +0xD4 flag is set) to the output iterator (/Gr template)
template <class It, class Out> Out __fastcall CopyHolySites(It first, It last, Out out);

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Arg1, class _Arg2, class _Result>
struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};

// The bound function is `bool __fastcall(CThing*, CCharString)`: this TU is not /Gr, so the pointer type names the
// convention the /Gr STLport TU (0x0048CC60) gets by default.
template <class _Arg1, class _Arg2, class _Result>
class pointer_to_binary_function : public binary_function<_Arg1, _Arg2, _Result> {
protected:
    _Result (__fastcall* _M_ptr)(_Arg1, _Arg2);
public:
    pointer_to_binary_function() { }
    explicit pointer_to_binary_function(_Result (__fastcall* __x)(_Arg1, _Arg2)) : _M_ptr(__x) { }
    _Result operator()(_Arg1 __x, _Arg2 __y) const { return _M_ptr(__x, __y); }
};
template <class _Arg1, class _Arg2, class _Result>
inline pointer_to_binary_function<_Arg1, _Arg2, _Result> ptr_fun(_Result (__fastcall* __x)(_Arg1, _Arg2))
{
    return pointer_to_binary_function<_Arg1, _Arg2, _Result>(__x);
}

template <class _Operation>
class binder2nd {
protected:
    _Operation op;
    typename _Operation::second_argument_type value;
public:
    binder2nd(const _Operation& __x, const typename _Operation::second_argument_type& __y) : op(__x), value(__y) { }
    typename _Operation::result_type operator()(const typename _Operation::first_argument_type& __x) const
    {
        return op(__x, value);
    }
};
template <class _Operation, class _Tp>
inline binder2nd<_Operation> bind2nd(const _Operation& __fn, const _Tp& __x)
{
    typedef typename _Operation::second_argument_type _Arg2_type;
    return binder2nd<_Operation>(__fn, _Arg2_type(__x));
}

#pragma auto_inline(off)
// 0x0048CC60: the STLport random-access __find_if instantiation (compiled in this TU, /Gr there): four-way unrolled
// loop plus the remainder switch; the iterator tag by const reference.  Same symbol as the landed 0x0048CC60 row.
template <class _RandomAccessIter, class _Predicate>
__declspec(noinline) _RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last,
                                                            _Predicate __pred, const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for ( ; __trip_count > 0 ; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
    case 0:
    default:
        return __last;
    }
}
#pragma auto_inline(on)
template <class _Tp> inline random_access_iterator_tag iterator_category(_Tp* const&) { return random_access_iterator_tag(); }
template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    return __find_if(__first, __last, __pred, iterator_category(__first));
}
}

bool __fastcall IsThingScriptNamed(CThing* thing, CCharString name);   // 0x0048BC70

struct CPlayer {
    unsigned char  _pad00[0x20];
    CThingManager* ThingManager;                   // +0x20
    unsigned char  _pad24[0xe8 - 0x24];
    C3DVector      RespawnPos;                     // +0xe8 (CThingRespawnInfo)
    bool           RespawnValid;                   // +0xf4
    bool LocateCharacterStartHolySite(CThing** site);
};

bool CPlayer::LocateCharacterStartHolySite(CThing** site)
{
    CThingListOwner* owner = ThingManager->Owner;
    CRetailVector<CThing*> sites;
    CopyHolySites(owner->HolySites.begin(), owner->HolySites.end(), back_inserter(sites));
    if (!RespawnValid) {
        CThing** found = std::find_if(sites.begin(), sites.end(), std::bind2nd(std::ptr_fun(&IsThingScriptNamed), g_CWideString_013b866c.ConvertToCharString()));
        if (found != sites.end()) {
            *site = *found;
            return true;
        }
        CCharString message;
        message.Format("*** WARNING : failed to find a holy site with ScriptName %S", (const unsigned short*)g_CWideString_013b866c);
        RespawnPos = C3DVector(0.0f, 0.0f, 0.0f);
    }
    CThing* best = 0;
    float bestDistance = 1e10f;
    for (CThing** it = sites.begin(); it != sites.end(); ++it) {
        CThing* thing = *it;
        float distance = (thing->GetPosition() - RespawnPos).MagSquared();
        if (distance < bestDistance) {
            bestDistance = distance;
            best = thing;
        }
    }
    if (best) {
        *site = best;
        return true;
    }
    return false;
}