#ifndef FABLE_ENGINE_PKSERIALISECPARSER_H
#define FABLE_ENGINE_PKSERIALISECPARSER_H
#ifdef FABLE_RETAIL_PKSERIALISECPARSER_READER_PASS23
// Layout: native00c31970 initialises reader+0/writer+4;00c31990 reverses it.
// The .\PKSerialiseCParser.cpp assertion literal proves the class stem.
// Reader/writer fields, IDataStream and ValidateReader are descriptive: no
// libpk type records occur in the three supplied original PDB name sets.
// IDataStream and the struct key preserve the existing00c31970 caller model.
struct IDataStream;
struct PKSerialiseCParser {
 IDataStream* pReader;
 IDataStream* pWriter;
 IDataStream* ValidateReader();
};
#endif
#endif
