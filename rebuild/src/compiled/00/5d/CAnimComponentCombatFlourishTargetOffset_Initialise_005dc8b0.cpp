// Retail CStringParser is0x28, proven by landed00404720: Comments one pointer+14, ErrorCallback+18, CursorPos[3]+1c.
// Generated parser header is0x30 because donor Comments occupies12 bytes; its explicit vptr also cannot supply virtual cleanup.
// CAnimComponentCombatFlourishTargetOffset::Initialise(CPersistContext&, const CCharString&) -- enable target offset and parse x/y
// Leaf: ?Initialise@CAnimComponentCombatFlourishTargetOffset@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z
// Retail: Fable.exe @ 0x005DC8B0, 248 bytes (row includes 8 padding bytes). Reset is slot 8; parser, trait, string,
// list and base cleanup callees are 0x00404720, 0x0040F7E0, 0x0099EBF0, 0x0099EAE0,
// 0x004046B0, 0x0099A2E0. The generated data-only header gives size 0x11 and members
// TargetOffset +8, Vector_X +9, Vector_Y +0xd. Its virtual slots are absent, so this
// packed local polymorphic model uses the header's field layout and supplies slots 3/8.
class CPersistContext;
class CCharString { public: void* data; CCharString(const char*, long); ~CCharString(); };
class CBaseClass {
public:  virtual ~CBaseClass(); };
struct CStringParserCommentList { void* head; ~CStringParserCommentList(); }; // retail sentinel-only list, 4 bytes
class CStringParser : public CBaseClass { public:
 char* String; unsigned long BaseLineOffset; CCharString Filename; CCharString TempCharString;
 CStringParserCommentList Comments; void (*ErrorCallback)(CCharString*); long CursorPos[3];
 CStringParser(const CCharString&, void (*)(CCharString*), const CCharString&, long);
 bool SkipPastNextItemIfIs(const CCharString&); // 009B9E00
};
template<class T> struct CPersistTraits { CPersistTraits() {} void TransferIn(CPersistContext&, CStringParser&, T*); };
#pragma pack(push, 1)
class CAnimComponentCombatFlourishTargetOffset { public:
 virtual ~CAnimComponentCombatFlourishTargetOffset(); virtual void v1(); virtual void v2(); virtual bool Initialise(CPersistContext&, const CCharString&);
 virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7(); virtual void Reset();
 long base; bool TargetOffset; float TargetOffsetVector_X; float TargetOffsetVector_Y;
};
#pragma pack(pop)
bool CAnimComponentCombatFlourishTargetOffset::Initialise(CPersistContext& context, const CCharString& text) {
 Reset(); TargetOffset=true;
 CStringParser parser(text, 0, CCharString("", -1), 0);
 CPersistTraits<float>().TransferIn(context, parser, &TargetOffsetVector_X);
 bool notFound;
 { CCharString marker(",", -1); notFound=!parser.SkipPastNextItemIfIs(marker); }
 if (notFound) return false;
 CPersistTraits<float>().TransferIn(context, parser, &TargetOffsetVector_Y);
 return true;
}