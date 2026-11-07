#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_CBANKFILEASYNC_H
#define FABLE_ENGINE_CBANKFILEASYNC_H
#include "engine/CBankFile.h"
// Layout: original public CBankFile base, native ctor9d5f80 and RTTI secondary base164.
class CBankFileAsync:public CBankFile {
public:
#ifdef FABLE_RETAIL_GFMAIN
 static void __fastcall SetEnableFailureHandling(bool);
#endif
 CBankFileAsync();virtual ~CBankFileAsync();
private:unsigned char _pad_0x110[0x54];
};
FABLE_STATIC_ASSERT(sizeof(CBankFileAsync)==0x164);
#endif

#else
#ifndef FABLE_ENGINE_CBANKFILEASYNC_H
#define FABLE_ENGINE_CBANKFILEASYNC_H
#include "engine/CBankFile.h"
// Layout: original public CBankFile base, native ctor9d5f80 and RTTI secondary base164.
class CBankFileAsync:public CBankFile {
public:CBankFileAsync();virtual ~CBankFileAsync();
private:unsigned char _pad_0x110[0x54];
};
FABLE_STATIC_ASSERT(sizeof(CBankFileAsync)==0x164);
#endif

#endif
