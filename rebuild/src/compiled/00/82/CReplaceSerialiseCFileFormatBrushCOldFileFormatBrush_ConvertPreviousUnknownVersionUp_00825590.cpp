// CReplaceSerialise<CFileFormatBrush,COldFileFormatBrush>::ConvertPreviousUnknownVersionUp(void* data) -- build a fresh current-version brush record from an older one: convert the record up to the old format first when it is older still, then hand back the new record in a counted pointer
// Leaf: ?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCFileFormatBrush@@VCOldFileFormatBrush@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00825590, 240 bytes, static __fastcall: the hidden CCountedPointer return slot in ecx,
//   data in edx, plain `ret`; 0x10-byte frame (the new record word, the result slot copy and the 8-byte temporary).
// Callees: operator new @ 0x00BFEA1A (the 0x49-byte record, the 0xC-byte count block), CBaseSerialise<COldFileFormatBrush,1>::
//   ConvertPreviousUnknownVersionUp @ 0x00824C50 (same convention; manifest "CEngineWorldMap::CreateNewBankMap" is
//   propagated), CFileFormatBrush::ConvertFromOld(const COldFileFormatBrush*) @ 0x0081E4F0 (landed under the descriptive
//   name CRigidBodyFileRecord_0081e4f0::CopyFrom), the count blocks' deleter (slot +4, object in ecx), operator delete
//   @ 0x00BFE9BC; the new block's deleter is 0x00822C00 CCountedPointer<CFileFormatBrush>::DeleteData.
// Caller: CReplaceSerialise<CFileFormatBrush,COldFileFormatBrush>::ConvertToThisVersion 0x00825D40.
// Identity: FableWin.pdb `?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCFileFormatBrush@@VCOldFileFormatBrush@@...`;
//   Ego_r.pdb CFileFormatBrush = 73 (0x49) bytes {GameArea C2DBoxI +5, ScriptName char[32] +0x15, CreationTimeSeconds
//   +0x35, CreationPattern +0x39, NumberOfThingsToMove/Delete/Add +0x3D/+0x41/+0x45} -- exactly the fields the landed
//   0x0081E4F0 copies and clears; COldFileFormatBrush (61 bytes) is CBaseSerialise<COldFileFormatBrush, 1> (version 1).
// Label: the catalog's "CAFile_ConstructRigidBody" is descriptive and wrong.
// Shape: the landed 0x00518040 template: `CCountedPointer<T> result(new T); CCountedPointer<TOld> held; TOld* old = data;
//   if (old version != TOld::GetClassVersionNumber()) { held = TOld::ConvertPreviousUnknownVersionUp(data); old =
//   held.object; } result.object->ConvertFromOld(old); return result;` -- the copy-out, then held and result released.
//   The record's (empty, inline) user default constructor -- C2DBoxI's -- is what keeps held in ebx/ebp (ebx shared with
//   the allocation's null-test zero) and result.object in the frame word (`mov [esp+14h],esi` before the block's `new`);
//   with a constructor-less record VC keeps result.object in ebx and homes held.object instead (234 v 240). `held` is
//   built from a null pointer as in the landed 0x0051C2F0.
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* block);             // 0x00BFE9BC

struct CCPPointerInfo {
    unsigned long refs;                                // +0x0
    void (__fastcall* destroy)(void*);                 // +0x4
    void* object;                                      // +0x8
    CCPPointerInfo(void* p, void (__fastcall* d)(void*)) : refs(1), destroy(d), object(p) {}
};
template <class T> class CCountedPointer {
public:
    T*              object;                            // +0x0
    CCPPointerInfo* control;                           // +0x4
    static void __fastcall DeleteData(void* data);     // <CFileFormatBrush>: 0x00822C00
    CCountedPointer() : object(0), control(0) {}
    CCountedPointer(T* p) : object(p), control(p ? new CCPPointerInfo(p, DeleteData) : 0) {}
    CCountedPointer(const CCountedPointer& x) : object(x.object), control(x.control)
    {
        if (control)
            ++control->refs;
    }
    CCountedPointer& operator=(const CCountedPointer& x)
    {
        CCPPointerInfo* c = x.control;
        T* o = x.object;
        if (control != c) {
            if (control && --control->refs == 0) {
                control->destroy(control->object);
                operator delete(control);
            }
            object = o;
            control = c;
            if (control)
                ++control->refs;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        if (control) {
            --control->refs;
            if (control->refs == 0) {
                control->destroy(control->object);
                operator delete(control);
            }
        }
    }
};

struct C2DBoxI {                                       // integer 2D box; user (empty) default constructor
    long m_x0, m_y0, m_x1, m_y1;
    C2DBoxI() {}
};

#pragma pack(push, 1)
template <class T, long V> class CBaseSerialise {
public:
    unsigned long ByteLength;                          // +0x00
    unsigned char SerialiseVersionNumber;              // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return (unsigned short)V; }
    static CCountedPointer<T> __fastcall ConvertPreviousUnknownVersionUp(void* data);
};
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned long ByteLength;                          // +0x00
    unsigned char SerialiseVersionNumber;              // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return T::VERSION; }
    static CCountedPointer<T> __fastcall ConvertPreviousUnknownVersionUp(void* data);
};
class COldFileFormatBrush : public CBaseSerialise<COldFileFormatBrush, 1> {
public:
    C2DBoxI GameArea;                                  // +0x05
    char    ScriptName[32];                            // +0x15
    float   CreationTimeSeconds;                       // +0x35
    long    CreationPattern;                           // +0x39 (61 bytes)
};
class CFileFormatBrush : public CReplaceSerialise<CFileFormatBrush, COldFileFormatBrush> {
public:
    enum { VERSION = 2 };
    C2DBoxI GameArea;                                  // +0x05
    char    ScriptName[32];                            // +0x15
    float   CreationTimeSeconds;                       // +0x35
    long    CreationPattern;                           // +0x39
    long    NumberOfThingsToMove;                      // +0x3D
    long    NumberOfThingsToDelete;                    // +0x41
    long    NumberOfThingsToAdd;                       // +0x45 (73 bytes)
    void ConvertFromOld(const COldFileFormatBrush* old);   // 0x0081E4F0
};
#pragma pack(pop)

template <>
CCountedPointer<COldFileFormatBrush> __fastcall
CBaseSerialise<COldFileFormatBrush, 1>::ConvertPreviousUnknownVersionUp(void* data);   // 0x00824C50
template <class T, class TOld>
CCountedPointer<T> __fastcall CReplaceSerialise<T, TOld>::ConvertPreviousUnknownVersionUp(void* data)
{
    CCountedPointer<T> result(new T);
    CCountedPointer<TOld> held((TOld*)0);
    TOld* old = (TOld*)data;
    if (((TOld*)data)->SerialiseVersionNumber != TOld::GetClassVersionNumber()) {
        held = TOld::ConvertPreviousUnknownVersionUp(data);
        old = held.object;
    }
    result.object->ConvertFromOld(old);
    return result;
}

template CCountedPointer<CFileFormatBrush> __fastcall
CReplaceSerialise<CFileFormatBrush, COldFileFormatBrush>::ConvertPreviousUnknownVersionUp(void* data);