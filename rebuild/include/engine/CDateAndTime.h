#ifndef FABLE_ENGINE_CDATEANDTIME_H
#define FABLE_ENGINE_CDATEANDTIME_H
// Original Ego_r TPI and frontend destructor: trivial28-byte element.
#ifndef FABLE_RETAIL_CDATEANDTIME_CTOR
class CDateAndTime {protected:long Year;unsigned long Month,Day,Hour,Minute,Second,Millisecond;};
#else
// Retail opt-in (row 0x00A60D40): ??0CDateAndTime@@QAE@XZ is a real out-of-line ctor (0x009E9AB0) that retail calls.
class CDateAndTime {protected:long Year;unsigned long Month,Day,Hour,Minute,Second,Millisecond;
public: CDateAndTime();};
#endif
#endif
