#ifndef FIXTURE_STRING_H
#define FIXTURE_STRING_H
extern int FixtureStringLive;
class CWideString {public:const unsigned short*Text;CWideString(){static const unsigned short empty[1]={0};Text=empty;++FixtureStringLive;}CWideString(const CWideString&p):Text(p.Text){++FixtureStringLive;}~CWideString(){--FixtureStringLive;}CWideString&operator=(const CWideString&p){Text=p.Text;return *this;}};
class CCharString;
#endif
