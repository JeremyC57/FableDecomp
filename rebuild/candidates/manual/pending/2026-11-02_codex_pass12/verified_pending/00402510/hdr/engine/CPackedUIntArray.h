#ifdef FABLE_RETAIL_BANK_SYMBOLS
#ifndef FABLE_ENGINE_CPACKEDUINTARRAY_H
#define FABLE_ENGINE_CPACKEDUINTARRAY_H
class CPackedUIntArray{public:unsigned long operator[](unsigned long)const;unsigned long GetSize()const{return Size;}private:unsigned long*PackedInts;unsigned long Size;unsigned char Bits;unsigned long Bias;};
#endif
#else

#endif
