#ifndef FABLE_CDEFINDEX_H
#define FABLE_CDEFINDEX_H
class CDefIndex {
public:
 CDefIndex();
 CDefIndex(long val):Val(val) { }
 operator long() const {return Val;}
private:
 long Val;
};
FABLE_STATIC_ASSERT(sizeof(CDefIndex)==4);
#endif
