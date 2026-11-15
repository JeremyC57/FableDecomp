#ifndef FIXTURE_STRING_H
#define FIXTURE_STRING_H
class CCharString {public:const char*Text;CCharString(const char*t="",long=-1):Text(t){}CCharString(const CCharString&p):Text(p.Text){}const char*GetAsCharArray()const{return Text;}long GetLength()const{return (long)strlen(Text);}};
#endif
