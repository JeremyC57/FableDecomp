// std::vector<CScriptGameResourceObjectScriptedThingBase>::resize(unsigned int) -- STLport vector<T>::resize(n): forwards to resize(n, T()) with a default-constructed 16-byte element temporary
// Leaf: ?resize@?$vector@VCScriptGameResourceObjectScriptedThingBase@@V?$allocator@VCScriptGameResourceObjectScriptedThingBase@@@std@@@std@@QAEXI@Z
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD3DA4, 41 bytes. Pragmas: frame pointer kept
// (`optimize("y",off)`) in a size-optimised TU (`push dword ptr [ebp+8]`, `leave`).
// Callees: 0x007E72A0 element constructor, 0x00CD3CCB vector::resize(size_type, const T&),
// 0x007E74D0 element destructor.
//
// The manifest label `CPersistContext::Transfer<CAnimationSet>` is propagated and wrong. The three
// callers (0x00CCC73B, 0x00CCCA4F, 0x00CCD1C6) zero a three-pointer local (first/last/end) and call
// this with a count: it is the one-argument STLport `resize(size_type n) { resize(n, T()); }`,
// emitted out of line. The two-argument resize @ 0x00CD3CCB is the STLport body
// (`n < size() ? erase(begin()+n, end()) : insert(end(), n-size(), x)`, 16-byte elements).
// The element is a 16-byte intelligent-pointer record: its constructor runs the
// CBaseIntelligentPointer constructor (0x0099A380, vtable 0x0129A7D8), stores its own vtable
// (0x0127094C) and clears +0x8/+0xC; its destructor releases the counted block at +0xC. The
// temporary is passed as the constructor's returned `this` (`push eax`); the module is built
// without C++ exceptions (no EH frame around the temporary).
// Identity (respell, chunk 639): the element is CScriptGameResourceObjectScriptedThingBase -- ctor 0x007E72A0 and virtual
// dtor 0x007E74D0 (vtable 0x0127094C) are the handle class of FollowerHandles in CQ_FireHeartScript (0x00D3A050, Ego_r.pdb
// member type vector<CScriptGameResourceObjectScriptedThingBase>), and 0x00CD3CCB is that vector's resize(size_type, const T&).
// The former stand-in spellings were CThingRef / CThingRefVector::Resize.
class CScriptGameResourceObjectScriptedThingBase { public:   // 16 bytes, vtable 0x0127094C
    unsigned char _body[0x10 - 4];
    CScriptGameResourceObjectScriptedThingBase();     // 0x007E72A0
    virtual ~CScriptGameResourceObjectScriptedThingBase();    // 0x007E74D0
};
namespace std {
template <class T> class allocator;
template <class T, class A> class vector;
template <> class vector<CScriptGameResourceObjectScriptedThingBase, allocator<CScriptGameResourceObjectScriptedThingBase> > {
public:
    CScriptGameResourceObjectScriptedThingBase* _M_start;
    CScriptGameResourceObjectScriptedThingBase* _M_finish;
    CScriptGameResourceObjectScriptedThingBase* _M_end_of_storage;
    void resize(unsigned int new_size, const CScriptGameResourceObjectScriptedThingBase& x);   // 0x00CD3CCB
    void resize(unsigned int new_size);
};

void vector<CScriptGameResourceObjectScriptedThingBase, allocator<CScriptGameResourceObjectScriptedThingBase> >::resize(unsigned int new_size)
{
    resize(new_size, CScriptGameResourceObjectScriptedThingBase());
}
}
