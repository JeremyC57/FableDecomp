// CPersistContext::TransferVector<bool>(const char* name, std::vector<bool>& value) -- persist one bool-vector member: CRC-check the key, then by mode text load / text save / binary load / binary save / reset to empty
// Leaf: ??$TransferVector@_N@CPersistContext@@QAEXPBDAAV?$vector@_NV?$allocator@_N@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDCF80, 188 bytes (the switch table follows the code), thiscall, `ret 8`.
// Identity (proven): 5 call sites -- the landed CFireHeartPatternDef::Transfer 0x00D35070 ("PadActive", Ego_r.pdb member
//   vector<bool> PadActive; reloc_bindings already binds this address there as ??$TransferVector@_N@CPersistContext@@...),
//   NScript::CV_BookCollectingScript::OnPersist 0x00E54880 (twice), NScript::CV_TalentlessBardScript::OnPersist 0x00ED85A0
//   and 0x00CDC9A3; its binary callees are the landed GFSerialiseVectorBinaryOut<bool> 0x004675C5 / GFSerialiseVectorBinaryIn<bool>
//   0x004675F6 and the reset arm's vector<bool>::erase 0x004676BB. Ego_r.pdb (donor 0x0046B4CB) spells it
//   ??$TransferVector@_N@CPersistContext@@QAEXPBDAAV?$vector@_NV?$allocator@_N@std@@@std@@@Z. The manifest label
//   "Transfer<signed char>" is propagated across byte-identical bodies.
// Callees: CPersistContext::CheckCRC 0x00404500, std::vector<bool>::erase(bool*, bool*) 0x004676BB (thiscall, the inline
//   clear()), GFSerialiseVectorBinaryOut<bool> 0x004675C5 / GFSerialiseVectorBinaryIn<bool> 0x004675F6 (`__fastcall` stream,
//   vector), CPersistContext::TransferVectorSaveText 0x009BAC80 / TransferVectorLoadText 0x009BB110 (both take the
//   CPersistContext_TransferableVector<bool> wrapper, vtable 0x012C2EDC).
// Shape: the landed CPersistContext::TransferVector<CCharString> 0x0049B8D0 (byte-identical twin). The persist mode
//   (CPersistContext +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset) is a closed enum, so the switch
//   ends in `default: __assume(0)` and VC7.1 emits the bare `jmp [eax*4+table]`; the case bodies are laid out in source
//   order and retail has them 4,3,2,1,0. The 8-byte frame is the {vtable, vector*} wrapper shared by the two text arms;
//   the key stays in edi from the CheckCRC push.
// Layout: retail STLport vector<bool> is a plain byte vector (12 bytes: _M_start, _M_finish, _M_end_of_storage; the landed
//   erase / GFSerialise rows count elements as byte differences). CPersistContext from Ego_r.pdb (0x2C bytes): UseDefaults
//   +0x14, Mode +0x18, PLoadStream +0x24, PSaveStream +0x28 (no engine header).
// Signatures: FableWin.pdb CPersistContext_TransferableVector<bool> (ctor(vector<bool>&), private virtual Resize / Size /
//   TransferIn / TransferOut) and the GFSerialiseVectorBinary pair taking the streams by reference.
class CDataOutputStream;
class CMemoryDataInputStream;
class CStringParser;
class CCharString;
class CPersistContext;

namespace std {
template <class _Tp> class allocator { public: };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
    _Tp* erase(_Tp* __first, _Tp* __last);                  // 0x004676BB for bool (out of line)
    void clear() { erase(begin(), end()); }
};
}

template <class T> void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, std::vector<T>& value);   // 0x004675F6 for bool
template <class T> void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream& stream, std::vector<T>& value);       // 0x004675C5 for bool

class CPersistContext_TransferableVectorGenericType { public:
private:
    virtual void Resize(long count) = 0;
    virtual long Size() = 0;
    virtual void TransferIn(CPersistContext& context, CStringParser& parser, long index) = 0;
    virtual void TransferOut(CPersistContext& context, CCharString& text, long index) = 0;
};
template <class T> class CPersistContext_TransferableVector : public CPersistContext_TransferableVectorGenericType {   // vtable 0x012C2EDC
public:
    std::vector<T>* TransVector;
    CPersistContext_TransferableVector(std::vector<T>& v) : TransVector(&v) {}
private:
    virtual void Resize(long count);
    virtual long Size();
    virtual void TransferIn(CPersistContext& context, CStringParser& parser, long index);
    virtual void TransferOut(CPersistContext& context, CCharString& text, long index);
};

class CPersistContext { public:
    unsigned char           _pad00[0x14];
    bool                    UseDefaults;          // +0x14
    unsigned char           _pad15[3];
    long                    Mode;                 // +0x18
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;           // +0x24
    CDataOutputStream*      PSaveStream;           // +0x28
protected:
    void CheckCRC(const char* name);                                                                       // 0x00404500
    void TransferVectorSaveText(const char* name, CPersistContext_TransferableVectorGenericType& item);     // 0x009BAC80
    void TransferVectorLoadText(const char* name, CPersistContext_TransferableVectorGenericType& item);     // 0x009BB110
public:
    template <class T> void TransferVector(const char* name, std::vector<T>& value);
};

template <class T>
void CPersistContext::TransferVector(const char* name, std::vector<T>& value)
{
    CheckCRC(name);
    switch (Mode) {
    case 4:
        value.clear();
        break;
    case 3:
        GFSerialiseVectorBinaryOut(*PSaveStream, value);
        break;
    case 2:
        GFSerialiseVectorBinaryIn(*PLoadStream, value);
        break;
    case 1: {
        CPersistContext_TransferableVector<T> item(value);
        TransferVectorSaveText(name, item);
        break;
    }
    case 0: {
        CPersistContext_TransferableVector<T> item(value);
        TransferVectorLoadText(name, item);
        break;
    }
    default:
        __assume(0);
    }
}

template void CPersistContext::TransferVector<bool>(const char* name, std::vector<bool>& value);