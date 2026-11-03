#ifdef FABLE_RETAIL_BANK_SYMBOLS
#ifndef FABLE_ENGINE_CBANKFILEENTRYUPDATEDATA_H
#define FABLE_ENGINE_CBANKFILEENTRYUPDATEDATA_H
class CBankFileEntryUpdateData{friend class CBankFile;private:unsigned long StateBlockCRC,InfoSize;char*InfoBuffer;unsigned char Filenames[8];bool RequiresUpdate,Exists;};
#endif
#else

#endif
