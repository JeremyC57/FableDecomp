#ifndef FABLE_ENGINE_CDATEANDTIME_RENDER2D_H
#define FABLE_ENGINE_CDATEANDTIME_RENDER2D_H
#include "rebuild_abi.h"
class CCharString;struct _SYSTEMTIME;struct _FILETIME;

// Original Ego_r and FableWin declarations: evidence/009da9f0_*_types.json and *_dependencies.json.
class CDateAndTime {
protected: long Year;
protected: unsigned long Month;
protected: unsigned long Day;
protected: unsigned long Hour;
protected: unsigned long Minute;
protected: unsigned long Second;
protected: unsigned long Millisecond;
public: CDateAndTime();
public: CDateAndTime(long,unsigned long,unsigned long,unsigned long,unsigned long,unsigned long,unsigned long);
public: CDateAndTime(const _SYSTEMTIME &);
public: CDateAndTime(const _FILETIME &);
public: void Initialise(const CDateAndTime &);
public: void Initialise(const _SYSTEMTIME &);
public: void Initialise(const _FILETIME &);
public: long GetYear() const;
public: unsigned long GetMonth() const;
public: unsigned long GetDay() const;
public: unsigned long GetHour() const;
public: unsigned long GetMinute() const;
public: unsigned long GetSecond() const;
public: unsigned long GetMillisecond() const;
public: _FILETIME ToFileTime() const;
public: CCharString ToString() const;
public: bool operator==(const CDateAndTime &) const;
public: bool operator!=(const CDateAndTime &) const;
public: bool operator<(const CDateAndTime &) const;
public: bool operator<=(const CDateAndTime &) const;
public: bool operator>(const CDateAndTime &) const;
public: bool operator>=(const CDateAndTime &) const;
public: bool IsValid() const;
};
#endif
