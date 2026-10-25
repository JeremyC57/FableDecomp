// CCharString::Append(char c, unsigned long count) -- make the text block this string's own (or a fresh empty one), then append count copies of c to it
// Leaf: ?Append@CCharString@@QAEXDK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099F3B0, 29 bytes, thiscall, `ret 8`: `push esi ; mov esi,ecx ;
//   call 0x0099EB40 ; push count ; push c ; mov ecx,[esi] ; call 0x009A0460 ; pop esi`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 452 callee "0x99F3B0 CCharString::Append"; hand452 README "ResizeGameMap" ->
//   CCharString::Append): Ego_r.pdb lists ?SetChar@CCharString@@QAEXJD@Z, ?Append@CCharString@@QAEXDK@Z,
//   ?Insert@CCharString@@QAEXJD@Z in that order, and retail has the landed CCharString::SetChar at 0x0099F390 right before
//   this body; its callees are the landed CCharString::MakeOwnCopyOfDataOrEmptyString 0x0099EB40 (Ego_r: protected,
//   ?MakeOwnCopyOfDataOrEmptyString@CCharString@@IAEXXZ) and CBasicString<char>::Append(char, unsigned long) 0x009A0460
//   (landed, ?Append@?$CBasicString@D@@QAEXDK@Z) on PStringData->Data; callers are the script string helpers
//   0x00CBF125 / 0x00CD17A4 / 0x00CD5226. The landed CThingMapManager::ResizeGameMap (Prepare + map->ResizeMap) is a
//   propagated label over the same bytes.
// Layout: Ego_r.pdb CCharString {CCharStringData* PStringData}, CCharStringData {CBasicString<char> Data +0, NoRefs +0xd}
//   (the landed 0x0099EB40 block model; only the members used here are declared).
template <class T> class CBasicString {
public:
    T*            PData;                              // +0x00
    unsigned long StringLength;                       // +0x04
    void Append(T c, unsigned long count);            // 0x009A0460
};
#pragma pack(push, 1)
struct CCharStringData {
    CBasicString<char> Data;                          // +0x00 (13 bytes in the full model)
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;                     // +0x00
    void Append(char c, unsigned long count);
protected:
    void MakeOwnCopyOfDataOrEmptyString();            // 0x0099EB40
};

void CCharString::Append(char c, unsigned long count)
{
    MakeOwnCopyOfDataOrEmptyString();
    PStringData->Data.Append(c, count);
}
