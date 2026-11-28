#ifndef FABLE_ENGINE_FABLE_RUNTIME_PERSISTENCE_BINARY_H
#define FABLE_ENGINE_FABLE_RUNTIME_PERSISTENCE_BINARY_H

#ifdef FABLE_RETAIL_ENTITY_LOAD_BINARY_PASS30
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
template<> inline void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStream& is, long& val)
{
    long tval = is.ReadSLONG();
    val = tval;
}

template<> inline void CPersistTraits<unsigned long>::TransferBinaryIn(CMemoryDataInputStream& is, unsigned long& val)
{
    long tval = is.ReadSLONG();
    val = tval;
}
inline void CPersistTraits<unsigned __int64>::TransferBinaryIn(CMemoryDataInputStream& stream, unsigned __int64& value)
{
    unsigned __int64 read;
    stream.Read(&read, sizeof(read));
    value = read;
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif

#ifdef _MSC_VER
#pragma optimize("s", on)
#pragma optimize("y", off)
#endif
template<> inline void CPersistContext::Transfer<long>(long& value)
{
    CheckCRC("");
    CPersistTraits<long> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream, value); break;
    case MODE_SAVE_BINARY: traits.TransferBinaryOut(*PSaveStream, value); break;
    }
}

template<> inline void CPersistContext::Transfer<unsigned long>(unsigned long& value)
{
    CheckCRC("");
    CPersistTraits<unsigned long> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream, value); break;
    case MODE_SAVE_BINARY: traits.TransferBinaryOut(*PSaveStream, value); break;
    }
}

template<> inline void CPersistContext::Transfer<unsigned __int64>(unsigned __int64& value)
{
    CheckCRC("");
    CPersistTraits<unsigned __int64> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream, value); break;
    case MODE_SAVE_BINARY: traits.TransferBinaryOut(*PSaveStream, value); break;
    }
}

#ifdef _MSC_VER
#pragma optimize("", on)
#endif

#endif
#endif
