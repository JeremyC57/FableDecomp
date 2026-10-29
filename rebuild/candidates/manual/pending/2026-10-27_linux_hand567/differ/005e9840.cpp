// CPersistContext::TransferVector<unsigned long>(const char* name, std::vector<unsigned long>& value) -- persist one vector<unsigned long> member: CRC-check the key, then by mode text load / text save / binary load / binary save / reset to empty
// Grade: DIFFER(180v533) -- re-split at 0x005E9840: the row runs on past the jump table into the vector-length helper 0x005E9900 and the getters 0x005E9930..0x005E9A52; the true extent 0x005E9840..0x005E98F3 (158 bytes of code, `mov edi,edi`, the 5-entry table; 180 bytes) is byte-exact with relocations masked (pfx2: 0 mismatches)
// Leaf: ??$TransferVector@K@CPersistContext@@QAEXPBDAAV?$vector@KV?$allocator@K@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E9840, 180 bytes (0x005E9840..0x005E98F3: 158 bytes of code, `mov edi,edi` and the
//   five-entry jump table 0x005E98E0), `ret 8`. The catalog row runs 533 bytes on into separate functions (0x005E9900, a
//   3-float vector length; then int3-padded getters 0x005E9930..0x005E9A52).
// Identity (proven): the text arms build the wrapper whose vtable 0x01255884 has the RTTI type
//   `.?AV?$CPersistContext_TransferableVector@K@@` (T = unsigned long; slots Size 0x005E8CB0 / Resize 0x005E9820 /
//   TransferOut 0x005E8CF0 / TransferIn 0x005E8CC0), the function takes two arguments (`ret 8`), and Ego_r.pdb holds
//   `??$TransferVector@K@CPersistContext@@QAEXPBDAAV?$vector@KV?$allocator@K@std@@@std@@@Z`. The catalog label
//   `Transfer<signed char>` (three arguments) is propagated from the scalar Transfer<T> bodies.
// Callees: CPersistContext::CheckCRC 0x00404500 (protected), std::vector<unsigned long>::clear 0x005156D0 (out of line,
//   its only caller is this function), GFSerialiseVectorBinaryOut<unsigned long> 0x0046486F and
//   GFSerialiseVectorBinaryIn<unsigned long> 0x004648A1 (`__fastcall` stream&, vector&; their other caller is the binary-only
//   transfer at 0x00464841), CPersistContext::TransferVectorSaveText 0x009BAC80 and TransferVectorLoadText 0x009BB110
//   (protected, taking the CPersistContext_TransferableVectorGenericType base).
// Shape: the persist mode (+0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset) is a closed enum, so
//   the switch ends in `default: __assume(0)` (bare `jmp [eax*4+table]`); the arms are laid out in source order and
//   retail has them 4,3,2,1,0. The 8-byte frame is the {vtable, TransVector} wrapper of the text arms; the key stays in
//   edi from the CheckCRC push.
class CDataOutputStream;
class CMemoryDataInputStream;
class CCharString;
class CStringParser;
class CPersistContext;

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void clear();                                   // 0x005156D0 for _Tp = unsigned long (out of line)
};
}

template <class T> void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream& stream, std::vector<T>& value);
template <class T> void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, std::vector<T>& value);

class CPersistContext_TransferableVectorGenericType {
public:
    CPersistContext_TransferableVectorGenericType() { }
    virtual long Size() = 0;                                                           // slot 0
    virtual void Resize(long count) = 0;                                               // slot 1
    virtual void TransferOut(CPersistContext& context, CCharString& out, long index) = 0;     // slot 2
    virtual void TransferIn(CPersistContext& context, CStringParser& parser, long index) = 0; // slot 3
};
template <class T> class CPersistContext_TransferableVector : public CPersistContext_TransferableVectorGenericType {
public:
    std::vector<T>* TransVector;                                                       // +0x04
    CPersistContext_TransferableVector(std::vector<T>& v) : TransVector(&v) { }
private:
    virtual long Size();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext& context, CCharString& out, long index);
    virtual void TransferIn(CPersistContext& context, CStringParser& parser, long index);
};

class CPersistContext {
public:
    enum EMode { MODE_LOAD_TEXT, MODE_SAVE_TEXT, MODE_LOAD_BINARY, MODE_SAVE_BINARY, MODE_RESET };
    void*                   PDefinitionManager;   // +0x00
    void*                   PSymbolMap;           // +0x04
    CStringParser*          PParser;              // +0x08
    void*                   ErrorCallback;        // +0x0C
    CCharString*            PStringToClean;       // +0x10
    bool                    UseDefaults;          // +0x14
    bool                    UseCRCCheck;          // +0x15
    bool                    SingleTransfer;       // +0x16
    bool                    Finished;             // +0x17
    EMode                   Mode;                 // +0x18
    bool                    Binary;               // +0x1C
    CCharString*            PStringToSaveTo;      // +0x20
    CMemoryDataInputStream* PLoadStream;          // +0x24
    CDataOutputStream*      PSaveStream;          // +0x28
    template <class T> void TransferVector(const char* name, std::vector<T>& value);
protected:
    void CheckCRC(const char* name);                                                                         // 0x00404500
    void TransferVectorSaveText(const char* name, CPersistContext_TransferableVectorGenericType& item);   // 0x009BAC80
    void TransferVectorLoadText(const char* name, CPersistContext_TransferableVectorGenericType& item);   // 0x009BB110
};

template <class T>
void CPersistContext::TransferVector(const char* name, std::vector<T>& value)
{
    CheckCRC(name);
    switch (Mode) {
    case MODE_RESET:
        value.clear();
        break;
    case MODE_SAVE_BINARY:
        GFSerialiseVectorBinaryOut(*PSaveStream, value);
        break;
    case MODE_LOAD_BINARY:
        GFSerialiseVectorBinaryIn(*PLoadStream, value);
        break;
    case MODE_SAVE_TEXT: {
        CPersistContext_TransferableVector<T> item(value);
        TransferVectorSaveText(name, item);
        break;
    }
    case MODE_LOAD_TEXT: {
        CPersistContext_TransferableVector<T> item(value);
        TransferVectorLoadText(name, item);
        break;
    }
    default:
        __assume(0);
    }
}

template void CPersistContext::TransferVector<unsigned long>(const char* name, std::vector<unsigned long>& value);
