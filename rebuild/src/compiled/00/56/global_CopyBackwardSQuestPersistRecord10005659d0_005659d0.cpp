// _global::CopyBackward_SQuestPersistRecord10_005659d0(SQuestPersistRecord10*, SQuestPersistRecord10*, SQuestPersistRecord10*, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 16-byte {three CCharStrings, two bytes} records, returns the moved-back result
// Leaf: CopyBackward_SQuestPersistRecord10_005659d0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005659D0, length 91 bytes.
// Callees: CCharString::operator= @ 0x0099EFB0 (three per element).
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the two tag arguments on the stack, `ret 0Ch`): `for (n = last - first; n > 0; --n)
// *--result = *--last;` with the signed count computed once; the moved-back result is returned.
// Element: 16 bytes (`sar 4`): CCharString +0, +4, +8 (out-of-line operator= each), then the bytes at +0xC and +0xD
// (moved after the third call, the count's `dec ebx` scheduled ahead of them). The manifest label pair<CCharString,
// CRuntimeQuestPersistence::CPersistInfo> is 12 bytes (Ego_r.pdb CPersistInfo = two strings), so this row is a
// different, 16-byte record: the label is propagated; modelled as the label's three strings plus two flags.
class CCharString { public:
    void* m_pData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct SQuestPersistRecord10 {                   // 16 bytes
    CCharString   m_key;                         // +0x00
    CCharString   m_questName;                   // +0x04
    CCharString   m_persistScript;               // +0x08
    bool          m_flagC;                       // +0x0C
    bool          m_flagD;                       // +0x0D
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SQuestPersistRecord10* __fastcall CopyBackward_SQuestPersistRecord10_005659d0(SQuestPersistRecord10* first, SQuestPersistRecord10* last, SQuestPersistRecord10* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}