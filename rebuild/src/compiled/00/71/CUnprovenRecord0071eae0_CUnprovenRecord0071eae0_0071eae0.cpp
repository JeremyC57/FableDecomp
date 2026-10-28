// CUnprovenRecord_0071eae0::CUnprovenRecord_0071eae0() -- initialize the leading value and default-construct the class member.
// Leaf: ??0CUnprovenRecord_0071eae0@@QAE@XZ
// Identity: descriptive; unreferenced body: zero word+0 then CDateAndTime default constructor0x9E9AB0 at+4; no caller proves owning record or constructor-versus-template-helper role; retain address-specific descriptive record.
class CDateAndTime { public: long Year; unsigned long Month,Day,Hour,Minute,Second,Millisecond; CDateAndTime(); };
class CUnprovenRecord_0071eae0 { public: unsigned long UnprovenValue; CDateAndTime UnprovenDate; CUnprovenRecord_0071eae0(); };
CUnprovenRecord_0071eae0::CUnprovenRecord_0071eae0() : UnprovenValue(0) {}