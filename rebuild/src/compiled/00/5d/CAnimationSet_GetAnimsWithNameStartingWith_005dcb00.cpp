// CAnimationSet::GetAnimsWithNameStartingWith(const CCharString& prefix, vector<const CAnimationEntry*>& out, const CAnimFilterBase& filter) const -- collect every animation whose name starts with `prefix` and that the filter accepts; consecutive entries with the same key are skipped once their name failed to match
// Leaf: ?GetAnimsWithNameStartingWith@CAnimationSet@@QBEXABVCCharString@@AAV?$vector@PBVCAnimationEntry@@V?$allocator@PBVCAnimationEntry@@@std@@@std@@ABVCAnimFilterBase@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DCB00, 255 bytes, `ret 0Ch`. Ego_r.pdb ?GetAnimsWithNameStartingWith@CAnimationSet@@QBEXABVCCharString@@
//   AAV?$vector@PBVCAnimationEntry@@V?$allocator@PBVCAnimationEntry@@@std@@@std@@ABVCAnimFilterBase@@@Z (const; `out` and `filter` by reference --
//   the draft's pointer / non-const signature matched no symbol).
// Callees: CDefStringTable::GetString(long) 0x009D49B0 on the table 0x013CA828 (CCharString by value), strncmp 0x00BFEAF8, ~CCharString
//   0x0099EAE0, the filter's virtual operator() (slot 0), vector<const CAnimationEntry*>::_M_insert_overflow 0x005E1420.
// Layout: Ego_r.pdb CAnimationSet Anims (CVectorMap<unsigned long, CAnimationEntry>, 0x14-byte elements) at +0; CAnimationEntry.AnimName +4.
// Shape: the name is a named CCharString local in its own block (the inline StartsWith as the test); STLport push_back inline. The
//   read-only same-TU _M_insert_overflow body (GOTCHAS 00a26d40 rule) lets VC7.1 keep the pushed-pointer temporary and the tag in retail's
//   slots and home the name in the dead `out` argument slot (frame 0xC).
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int count);   // 0x00BFEAF8

class CCharStringData { public: char* PData; unsigned long StringLength; };
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                                     // 0x0099EAE0
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    operator const char*() const { return PStringData == 0 ? "" : PStringData->PData; }
    bool StartsWith(const CCharString& prefix) const { return strncmp(*this, prefix, prefix.GetLength()) == 0; }
};
class CDefStringTable { public:
    CCharString GetString(long id);                     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828

class CDefString { public: long TablePos; };
class CAnimationEntry { public:                                // 16 bytes
    long       BankIndex;                               // +0x00
    CDefString AnimName;                                // +0x04
    CDefString GroupName;                               // +0x08
    void*      Components;                              // +0x0c
};
class CAnimFilterBase { public:
    virtual bool operator()(const CAnimationEntry* entry) const;   // slot 0
};

namespace std {
struct __true_type {};
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {      // STLport 4.6 vector (12 bytes)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int fill_len, bool atend);   // 0x005E1420
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else {
            __true_type tag;
            _M_insert_overflow(_M_finish, x, tag, 1, true);
        }
    }
};
}
namespace std {
// Same-TU read-only body for _M_insert_overflow 0x005E1420 (GOTCHAS: a const& argument temporary stays per-iteration until the
template <class T, class A> T* GrowVectorStorage_005e1420(vector<T, A>* v, unsigned int n);
#pragma auto_inline(off)
template <class T, class A> __declspec(noinline) void vector<T, A>::_M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int fill_len, bool atend)
{
    T* p = GrowVectorStorage_005e1420(this, fill_len);
    *p = x;
    if (atend)
        _M_finish = p + fill_len;
}
#pragma auto_inline(on)
}
typedef std::vector<const CAnimationEntry*> CAnimEntryPtrVector;

class SAnimMapEntry { public:                                  // CVectorMap<unsigned long, CAnimationEntry> element (0x14)
    unsigned long   Key;
    CAnimationEntry Entry;
};

class CAnimationSet {
public:
    SAnimMapEntry* _M_start;                            // +0x00 Anims
    SAnimMapEntry* _M_finish;                           // +0x04
    void GetAnimsWithNameStartingWith(const CCharString& prefix, CAnimEntryPtrVector& out, const CAnimFilterBase& filter) const;
};

void CAnimationSet::GetAnimsWithNameStartingWith(const CCharString& prefix, CAnimEntryPtrVector& out, const CAnimFilterBase& filter) const
{
    bool skipKey = false;
    unsigned long lastKey = 0;
    for (const SAnimMapEntry* it = _M_start; it != _M_finish; ++it) {
        unsigned long key = it->Key;
        if (key != lastKey || !skipKey) {
            bool match;
            {
                CCharString name = g_DefStringTable.GetString(it->Entry.AnimName.TablePos);
                match = name.StartsWith(prefix);
            }
            if (match) {
                if (filter(&it->Entry))
                    out.push_back(&it->Entry);
                skipKey = false;
            } else
                skipKey = true;
            lastKey = key;
        }
    }
}