// CCountedPointer<CIVRecord_00696560>::DeleteData(void* data) -- the counted pointer's per-type deleter: destroy the record (release its intrusive counted member at +0xc) and free it
// Leaf: DeleteData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00696560, length 42 bytes (+ int3 padding), plain `ret`. No calls: its
// address is stored as CCPPointerInfo::DeleteFunc by the CCountedPointer<T>(T*) constructor 0x006967E0 and by Reset(T*)
// 0x00696830 (`mov dword ptr [eax+4], 0x696560`), and the release path calls it with the object in ecx
// (`mov ecx,[info+8] ; call [info+4]`). The record (0x10 bytes) is built at 0x00693040 by copying three dwords and an
// add-ref'd intrusive pointer at +0xc; its class is not recoverable (descriptive name). The manifest label
// (_Dest_val<allocator<pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase>>>>) is propagated and wrong.
// Callees: the pointee's virtual slot 1 (+4, its self-delete) on a zero IVRefCount; operator delete @ 0x00BFE9BC.
// Shape: `delete (T*)data` = null test, the inlined (non-virtual) record destructor -- ~CIVCountedPointer with the zero
// store inside the non-null arm -- then operator delete; `__fastcall` so the object arrives in ecx.
void __cdecl operator delete(void* p);                       // 0x00BFE9BC

class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    virtual void DeleteThis();                               // slot 1 (+4)
    unsigned long IVRefCount;                                // +0x04
};

template<class T> class CIVCountedPointer {
public:
    T* Object;
    ~CIVCountedPointer()
    {
        T* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->DeleteThis();
            Object = 0;
        }
    }
};

struct CIVRecord_00696560 {                                  // 0x10 bytes
    long m_value00;                                          // +0x00
    long m_value04;                                          // +0x04
    long m_value08;                                          // +0x08
    CIVCountedPointer<CIVCountedPointeeBase> m_pShared;      // +0x0c
};

class CCPPointerInfo;

template<class T> class CCountedPointer { public:
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
    static void __fastcall DeleteData(void* data)
    {
        delete static_cast<T*>(data);
    }
};

template class CCountedPointer<CIVRecord_00696560>;