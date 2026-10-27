// CConversation::Copy(const CDefClassBase* other) -- copy another conversation def: the CDefClassBase part, Lines and the four string vectors
// Leaf: ?Copy@CConversation@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E54D10, 73 bytes, thiscall, `ret 4` (virtual slot 19 of the def vtable).
// Identity (proven): vtable 0x012E5FF0 (no RTTI) is the one the landed CConversation::CConversation(const CConversation&)
//   0x00E54CA0 and the default constructor 0x00F336F0 store; Ego_r.pdb CConversation has Transfer, Copy and GetSizeofClass
//   beyond CDefClassBase (?Transfer@CConversation@@UAEXAAVCPersistContext@@@Z, ?Copy@CConversation@@UAEXPBVCDefClassBase@@@Z);
//   slot 18 is the keyed persist (Transfer, the keys are the member names), slot 19 the member-wise copy (Copy), slot 20
//   0x00E54D00 GetSizeofClass, as in the landed CFireHeartPatternDef def vtable.
// Layout: CDefClassBase is 0x28 in retail; Ego_r.pdb CConversation: long Lines +0x28, vector<CCharString> Speaker +0x2c,
//   Dialogue +0x38, Animation +0x44, AnimLoop +0x50 (STLport vectors 0xC).
// Header debt: engine/CDefClassBase.h flattens the vptr into a data member and has no virtual interface; local model.
// Callees: CDefClassBase::operator= 0x00431F10, std::vector<CCharString>::operator= 0x00432EE9 (x4).
// Shape: `*this = *(const CConversation*)other` -- the implicit operator= inlined (base operator=, then the members').
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector { // STLport 4.6: start, finish, end of storage (0xC)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector& operator=(const vector& other);             // 0x00432EE9 for vector<CCharString>
};
}
class CCharString {
public:
    void* PData;
};
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
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
class CConversation : public CDefClassBase {            // vtable 0x012E5FF0 (no RTTI)
public:
    long                     Lines;                     // +0x28
    std::vector<CCharString> Speaker;                   // +0x2c
    std::vector<CCharString> Dialogue;                  // +0x38
    std::vector<CCharString> Animation;                 // +0x44
    std::vector<CCharString> AnimLoop;                  // +0x50
    virtual void Transfer(CPersistContext& context);    // slot 18
    virtual void Copy(const CDefClassBase* other);      // slot 19
};

void CConversation::Copy(const CDefClassBase* other)
{
    *this = *(const CConversation*)other;
}