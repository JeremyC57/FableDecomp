// CPersistContext::Transfer<CActionInputControl>(const char* name, std::vector<CActionInputControl>& value) -- persist one vector member ("AssignedInputsPassive"): CRC-check the key, then by mode text load / text save / binary load / binary save / reset to empty
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00412DB0 (the switch table follows the code), `ret 8`. Called from CUserProfileManager::SaveGameState (0x00409FD0) with the key "AssignedInputsPassive".
// Callees: CPersistContext::CheckCRC 0x00404500, CPersistContext::TransferVectorLoadText 0x009BB110 and TransferVectorSaveText
//   0x009BAC80 (both take the CPersistContext_TransferableVector<T> wrapper, vtable 0x0122E010), GFSerialiseVectorBinaryIn<T>
//   0x00412C70 and GFSerialiseVectorBinaryOut<T> 0x0040FCA0 (both `__fastcall` stream, vector), and for the reset mode
//   the inline STLport `clear()` = `erase(begin(), end())`, i.e. `__copy(last, finish, first, random_access_iterator_tag(), (ptrdiff_t*)0)` (0x0040E2D0) with the tag temporary in the dead value slot.
// Shape: the persist mode (CPersistContext +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset) is a
//   closed enum, so the switch ends in `default: __assume(0)` and VC7.1 emits the bare `jmp [eax*4+table]` with no range
//   check; VC7.1 lays the case bodies out in source order, and retail has them 4,3,2,1,0, so the source lists them in that
//   order. The 8-byte frame is the {vtable, vector*} wrapper shared by the two text arms; the key stays in edi from the
//   CheckCRC push. CPersistContext layout from Ego_r.pdb (0x2C bytes): UseDefaults +0x14, Mode +0x18, PLoadStream +0x24,
//   PSaveStream +0x28. The element type comes from the PDB member that owns the key where one exists.
struct CDataOutputStream;
struct CMemoryDataInputStream;
struct CActionInputControl { long GameAction; long ControllerType; long KeyboardKey; long XboxButton; long MouseButton; float ControlDirection[2]; };   // 0x1C bytes (PDB)

struct random_access_iterator_tag { random_access_iterator_tag() {} };   // user ctor: no zero store for the tag temporary

CActionInputControl* __fastcall __copy(CActionInputControl* first, CActionInputControl* last, CActionInputControl* result, const random_access_iterator_tag& tag, int* dist);   // 0x0040E2D0

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* erase(T* first, T* last)
    {
        T* i = __copy(last, _M_finish, first, random_access_iterator_tag(), (int*)0);
        _M_finish = i;
        return first;
    }
    void clear() { erase(_M_start, _M_finish); }
};

void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream* stream, vector<CActionInputControl>& value);   // 0x00412C70
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream* stream, vector<CActionInputControl>& value);       // 0x0040FCA0

struct CPersistContext_TransferableVectorGenericType {
    virtual long GetCount();
};
template <class T> struct CPersistContext_TransferableVector : CPersistContext_TransferableVectorGenericType {   // vtable 0x0122E010
    vector<T>* TransVector;
    CPersistContext_TransferableVector(vector<T>& v) : TransVector(&v) {}
    virtual long GetCount();
};

struct CPersistContext {
    unsigned char           _pad00[0x14];
    bool                    UseDefaults;          // +0x14
    unsigned char           _pad15[3];
    long                    Mode;                 // +0x18
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;           // +0x24
    CDataOutputStream*      PSaveStream;           // +0x28
    void CheckCRC(const char* name);              // 0x00404500
    void TransferVectorSaveText(const char* name, CPersistContext_TransferableVectorGenericType& item);   // 0x009BAC80
    void TransferVectorLoadText(const char* name, CPersistContext_TransferableVectorGenericType& item);   // 0x009BB110
    template <class T> void Transfer(const char* name, vector<T>& value);
};

template <class T>
void CPersistContext::Transfer(const char* name, vector<T>& value)
{
    CheckCRC(name);
    switch (Mode) {
    case 4:
        value.clear();
        break;
    case 3:
        GFSerialiseVectorBinaryOut(PSaveStream, value);
        break;
    case 2:
        GFSerialiseVectorBinaryIn(PLoadStream, value);
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

template void CPersistContext::Transfer<CActionInputControl>(const char* name, vector<CActionInputControl>& value);