// CFireHeartPatternDef::Copy(const CDefClassBase* other) -- copy another pattern def: the CDefClassBase part, then PadActive
// Leaf: ?Copy@CFireHeartPatternDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D34FB0, 31 bytes, thiscall, `ret 4` (virtual slot 19 of the def vtable).
// Identity (proven): vtable 0x012CCC50 (no RTTI) is the one CPersistContext_TransferableVectorOfSubComponents<CFireHeartPatternDef>::Resize
//   0x00F342B0 and the def's constructor 0x00F33DC0 store; Ego_r.pdb CFireHeartPatternDef has exactly Transfer, Copy and
//   GetSizeofClass beyond CDefClassBase, and its one member is vector<bool> PadActive (Ego_r +0x28). Slot 18 is the keyed
//   PadActive persist (Transfer), slot 19 the member-wise copy from a CDefClassBase* (Copy), slot 20 0x00D34EF0 GetSizeofClass.
// Layout: CDefClassBase is 0x28 in retail (Ego_r 0x25 rounded up); PadActive +0x28.
// Header debt: engine/CDefClassBase.h flattens the vptr into a data member and has no virtual interface; local model.
// Callees: CDefClassBase::operator= 0x00431F10, std::vector<bool>::operator= 0x00462E78.
// Shape: `*this = *(const CFireHeartPatternDef*)other` -- the implicit operator= inlined (base operator=, then the member's).
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector { // STLport 4.6 (no partial specialisation: vector<bool> is a byte vector, 0xC)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector& operator=(const vector& other);             // 0x00462E78 for vector<bool>
};
}
class CPersistContext {
public:
    template <class T> void TransferVector(const char* name, std::vector<T>& value);
};
class CDefClassBase {                                   // retail 0x28 (Ego_r 0x25 + padding): vptr, base, PDefManager +0x1c, GlobalIndex +0x20, flags +0x24
public:
    virtual ~CDefClassBase();
    virtual void Transfer(CPersistContext& context);
    virtual void Copy(const CDefClassBase* other);
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
    unsigned char _body[0x24];
};
class CFireHeartPatternDef : public CDefClassBase {     // vtable 0x012CCC50 (no RTTI)
public:
    std::vector<bool> PadActive;                        // +0x28
    virtual void Transfer(CPersistContext& context);    // slot 18
    virtual void Copy(const CDefClassBase* other);      // slot 19
};

void CFireHeartPatternDef::Copy(const CDefClassBase* other)
{
    *this = *(const CFireHeartPatternDef*)other;
}