// std::fill(SQuestStringRecord* first, SQuestStringRecord* last, const SQuestStringRecord& val) -- assign val to every 16-byte {three strings, two flags} quest record of [first, last)
// Leaf: fill
// Retail: Fable.exe @ 0x005664B0, 89 bytes, `ret 4`: __fastcall (first ecx -> esi, last edx -- spilled to the `push ecx`
// frame word -- and the value reference on the stack -> edi). Only call site 0x0056963B in the vector resize/fill path
// at 0x00569550 (catalogued std::vector_QuestPair_::resize_with_fill).
// Element (from the body, 16 bytes): three CCharStrings at +0 / +4 / +8 (each assigned through CCharString::operator=)
// and two byte flags at +0xC / +0xD. The catalog's pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>
// is 12 bytes in Ego_r.pdb (CPersistInfo = QuestName, PersistScript) and has no flag bytes, so the retail record is kept
// as a descriptive model with the same leading strings.
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0 (x3).
// Shape: the record's implicit copy assignment is memberwise (CCharString declares operator=): the three assignment
// calls with the source member addresses hoisted out of the loop (edi, `lea ebx,[edi+4]`, `lea ebp,[edi+8]`), then
// the two bytes; `last` is re-read from the frame word for the loop test.
struct CCharString {
    void* m_pStringData;                        // +0x00
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct SQuestStringRecord {                     // 16 bytes
    CCharString Name;                           // +0x00
    CCharString QuestName;                      // +0x04
    CCharString PersistScript;                  // +0x08
    bool        m_flag0;                        // +0x0C
    bool        m_flag1;                        // +0x0D
};

namespace std {
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
template void __fastcall fill(SQuestStringRecord*, SQuestStringRecord*, const SQuestStringRecord&);
}