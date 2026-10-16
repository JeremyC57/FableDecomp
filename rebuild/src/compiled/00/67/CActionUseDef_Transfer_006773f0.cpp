// CActionUseDef::Transfer(CPersistContext&) -- persist AnimationGroup (a def-string table position),
// SnapToActionPoint, IgnoreActionPoints and TutorialCategory.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006773F0, length 232 bytes (virtual, `ret 4`).
// Callees: CPersistContext::CheckCRC @ 0x00404500 (x4, the empty key "" @ 0x0122D70E), CDefString::TransferBinaryIn
// @ 0x0044FC00 (manifest "CPersistTraits<float>::TransferBinaryIn" propagated), CDataOutputStream::WriteLong
// @ 0x00993EB0 (x2), CPersistTraits<bool>::TransferBinaryIn @ 0x00403EB0 (x2), CDataOutputStream::WriteBool
// @ 0x00993E30 (x2), CPersistTraits<ETutorialCategory>::TransferBinaryIn @ 0x0040FEF0.
// Shape: four inline CPersistContext::Transfer<T> calls of the landed CActivateQuestDef::Transfer (0x007B5740) /
// CReadableDef::Transfer (0x007E81B0) template -- each member address formed before its CRC call (ebx, the last
// `add edi,30h`), the mode switch with the save arm inline; the bool and enum loads use the empty traits temporary
// in the dead argument slot. `this` is the PDB header layout (AnimationGroup_TablePos +0x28, SnapToActionPoint +0x2c,
// IgnoreActionPoints +0x2d, TutorialCategory +0x30).
// Leaf: Transfer
#include "engine/CActionUseDef.h"

struct CMemoryDataInputStream;

struct CDataOutputStream {
    void WriteLong(long value);                         // 0x00993EB0
    void WriteBool(bool value);                         // 0x00993E30
};

struct CDefString {                                     // a name stored as its def-string table position
    long TablePos;
    void TransferBinaryIn(CMemoryDataInputStream* stream);   // 0x0044FC00
};

enum ETutorialCategory { TUTORIAL_CATEGORY_NONE = 0 };

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<bool> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, bool* value);   // 0x00403EB0
    static void TransferBinaryOut(CDataOutputStream* stream, const bool& value) { stream->WriteBool(value); }
};
template<> struct CPersistTraits<ETutorialCategory> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, ETutorialCategory* value);   // 0x0040FEF0
    static void TransferBinaryOut(CDataOutputStream* stream, const ETutorialCategory& value) { stream->WriteLong(value); }
};
template<> struct CPersistTraits<CDefString> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, CDefString* value) { value->TransferBinaryIn(stream); }
    static void TransferBinaryOut(CDataOutputStream* stream, const CDefString& value) { stream->WriteLong(value.TablePos); }
};

struct CPersistContext {
    unsigned char           _pad00[0x18];
    long                    m_mode;                     // +0x18 (2 = load, 3 = save)
    unsigned char           _pad1c[0x08];
    CMemoryDataInputStream* m_pLoadStream;              // +0x24
    CDataOutputStream*      m_pSaveStream;              // +0x28
    void CheckCRC(const char* name);                    // 0x00404500
    template<class T> void Transfer(const char* name, T& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            CPersistTraits<T>().TransferBinaryIn(m_pLoadStream, &value);
            break;
        case 3:
            CPersistTraits<T>::TransferBinaryOut(m_pSaveStream, value);
            break;
        }
    }
};

struct CActionUseDef_Transfer : CActionUseDef {
    CDefString& AnimationGroup() { return *(CDefString*)&AnimationGroup_TablePos; }
    ETutorialCategory& Tutorial() { return *(ETutorialCategory*)TutorialCategory; }
    void Transfer(CPersistContext& context);
};

void CActionUseDef_Transfer::Transfer(CPersistContext& context)
{
    context.Transfer("", AnimationGroup());
    context.Transfer("", SnapToActionPoint);
    context.Transfer("", IgnoreActionPoints);
    context.Transfer("", Tutorial());
}