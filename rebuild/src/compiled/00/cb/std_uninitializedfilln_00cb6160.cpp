// std::__uninitialized_fill_n(pair<CCharString, CScriptInfo>* first, unsigned int n, const pair<CCharString, CScriptInfo>& x, const __false_type&) -- copy-construct n copies of a named script registration into raw storage at first and return first + n
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe @ 0x00CB6160, 84 bytes, `ret 8`: __fastcall (first ecx -> ebx, n edx -> ebp; x and the empty tag by
// reference on the stack), returning the end pointer in eax. Only call site 0x00CB7091.
// Types (Ego_r.pdb): CScriptInfo (20 bytes) = AllocFunc +0, MasterScriptDataAlloc +4 (cdecl factory pointers), Name
//   CCharString +8, ID +0xC, MasterScript bool +0x10; the pair is 24 bytes (first CCharString +0, second +4).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (twice).
// Shape: STLport's pair copy constructor ("VC++ needs this") is memberwise and CScriptInfo's implicit copy constructor
// is memberwise too (CCharString declares one): first's copy call, the two factory pointers, Name's copy call, ID and
// the bool byte. That nesting (a user copy constructor over a member with its own) is what makes VC7.1 keep the second
// induction register at second.Name (`lea esi,[ebx+0Ch]`, GOTCHAS 00769000/00720310).
#include <new>
class CCharString { public:
    void* m_pStringData;                        // +0x00
    CCharString(const CCharString& other);      // 0x0099EC30
};
class CScriptBase;
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CScriptInfo { public:                            // 20 bytes
    CScriptBase*     (__cdecl* AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();                                  // +0x04
    CCharString      Name;                      // +0x08
    long             ID;                        // +0x0C
    bool             MasterScript;              // +0x10
};
namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
}
typedef std::pair<CCharString, CScriptInfo> CScriptInfoPair;

struct __false_type { __false_type() {} };
namespace std {
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}
template CScriptInfoPair* __fastcall __uninitialized_fill_n(CScriptInfoPair*, unsigned int, const CScriptInfoPair&, const __false_type&);
}