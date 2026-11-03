#ifndef FABLE_ENGINE_CCRC_H
#define FABLE_ENGINE_CCRC_H
// Original Ego_r TPI0xbd09, stateless class and static fastcall API.
// CRC32TAB exact raw public/data symbol; native0129a168 is the256-word table.
class CCharString;
extern const unsigned long CRC32TAB[];
class CCRC {
public:
#ifndef FABLE_RETAIL_FACTIONS
    static unsigned long __fastcall Calc(unsigned long,const void*,unsigned long);
#else
    static unsigned long __fastcall Calc(unsigned long seed,const void* data,unsigned long length) {
        unsigned long result=seed;
        if(length!=0)for(unsigned long index=0;index<length;++index)
            result=(result>>8)^CRC32TAB[((const unsigned char*)data)[index]^(result&255)];
        return result;
    }
 #endif
    static unsigned long __fastcall Calc(const void* data,unsigned long length){return Calc(0,data,length);}
    static unsigned long __fastcall CalcFromString(const CCharString&);
    static unsigned long __fastcall CalcFromPathname(const CCharString&);
};
#endif
