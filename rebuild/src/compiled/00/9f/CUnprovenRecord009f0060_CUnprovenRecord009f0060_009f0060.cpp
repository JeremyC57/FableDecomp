// CUnprovenRecord_009f0060::CUnprovenRecord_009f0060() -- default constructor of an 8-byte record of two CCharStrings: default-construct both, return this
// Leaf: ??0CUnprovenRecord_009f0060@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F0060, 20 bytes, thiscall, plain `ret`, returns this.
// Identity (descriptive): no callers and no references (an out-of-line inline COMDAT); the console TU (CConsole deleting destructors 0x009EFFF0 / 0x009F0040 before it; its two-string destructor 0x009F0080 after it). Two CCharString
//   default constructors 0x0099E4B0 at +0 / +4 and nothing else: Ego_r.pdb has three 8-byte {CCharString, CCharString}
//   records (pair<CCharString, CCharString>, CBankCreatorException, NScript::CV_TalentlessBardScript::QuestData) and nothing
//   here picks one, so the class is a placeholder.
// Callees: CCharString::CCharString() 0x0099E4B0 (x2).
// Label correction: the manifest's "QuestData::QuestData" is propagated across the five byte-identical bodies (FableWin.pdb
//   QuestData is a talentless-bard script's local type).
// Shape (family K023, representative 0x00765D00): two member default constructors in declaration order, then `mov eax,esi`.
class CCharString {
public:
    CCharString();                                       // 0x0099E4B0
    ~CCharString();                                      // 0x0099EAE0
    void* PData;                                         // +0x00
};
class CUnprovenRecord_009f0060 {
public:
    CUnprovenRecord_009f0060();
    CCharString String0;                                 // +0x00
    CCharString String4;                                 // +0x04
};
CUnprovenRecord_009f0060::CUnprovenRecord_009f0060()
{
}