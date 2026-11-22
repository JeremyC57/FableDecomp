#ifndef FABLE_ENGINE_CBANKFILEDATATYPECOUNT_H
#define FABLE_ENGINE_CBANKFILEDATATYPECOUNT_H
#ifdef FABLE_RETAIL_CBANKFILE_READENTRIES_PASS26
// Both debug layouts put DataType at zero and Count at four. The native input reads two successive words per entry.
class CBankFileDataTypeCount {
public:
    unsigned long DataType;
    unsigned long Count;
};
#endif
#endif
