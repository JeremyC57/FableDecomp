// Standalone C++03 behavior fixture containing the selected MakeStereoBuffer body.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(expression) do { if (!(expression)) { fprintf(stderr,"line %d: %s\n",__LINE__,#expression); exit(1); } } while (0)

enum { C_FORMAT_INT16=1, C_FORMAT_INT8=2 };
enum { C_OK=0, C_ERR_OUTOFMEMORY=-5, C_ERR_NORESOURCES=-9, C_ERR_BUFFERTYPE=-14 };
enum { AL_NO_ERROR=0, AL_OUT_OF_MEMORY=0xa005, AL_FORMAT_STEREO8=0x1102, AL_FORMAT_STEREO16=0x1103 };

struct _CBUFFERDESC {
    unsigned int ffFormat, nChannels, nSamplesPerSec, cbSize, cbLoopStart, cbLoopEnd;
    void* pvData;
};

class CSalOpenALBuffer;
class CSalOpenAL {
public:
    CSalOpenAL() { Reset(); }
    void Reset() {
        TraceLength = 0;
        Trace[0] = 0;
        Suspended = false;
        AllocationFails = false;
        Allocation = 0;
        AllocationSize = 0;
        GenerationError = AL_NO_ERROR;
        UploadError = AL_NO_ERROR;
        Uploaded = false;
        UploadedBuffer = 0;
        UploadedFormat = 0;
        UploadedSize = 0;
        UploadedFrequency = 0;
    }
    int GetError() { Record('e'); return AL_NO_ERROR; }
    int CheckError() {
        Record('c');
        int error = Uploaded ? UploadError : GenerationError;
        if (error == AL_NO_ERROR) return C_OK;
        return error == AL_OUT_OF_MEMORY ? C_ERR_OUTOFMEMORY : C_ERR_NORESOURCES;
    }
    void GenBuffers(unsigned int count, unsigned int* buffer) {
        CHECK(count == 1 && Suspended);
        Record('g');
        *buffer = 42;
    }
    void DeleteBuffers(unsigned int count, unsigned int* buffer) {
        CHECK(count == 1 && Uploaded && Suspended && *buffer == UploadedBuffer);
        Record('d');
    }
    void BufferData(unsigned int buffer, int format, void* data, unsigned int bytes, unsigned int frequency) {
        CHECK(!Uploaded && data == Allocation && bytes == AllocationSize && bytes <= sizeof(Data));
        Record('u');
        Uploaded = true;
        UploadedBuffer = buffer;
        UploadedFormat = format;
        UploadedSize = bytes;
        UploadedFrequency = frequency;
        memcpy(Data, data, bytes);
    }
    void Record(char operation) { CHECK(TraceLength + 1 < sizeof(Trace)); Trace[TraceLength++] = operation; Trace[TraceLength] = 0; }
    char Trace[24];
    unsigned int TraceLength;
    bool Suspended;
    bool AllocationFails;
    void* Allocation;
    unsigned int AllocationSize;
    int GenerationError, UploadError;
    bool Uploaded;
    unsigned int UploadedBuffer;
    int UploadedFormat;
    unsigned int UploadedSize, UploadedFrequency;
    unsigned char Data[32];
private:
    friend class CSalOpenALBuffer;
    bool IsSuspended() const { return Suspended; }
    void Suspend() { Record('s'); Suspended = true; }
    void Resume() { CHECK(Suspended); Record('r'); Suspended = false; }
};
CSalOpenAL g_OpenAL;

extern "C" void* CodaMalloc(unsigned int size)
{
    g_OpenAL.Record('a');
    g_OpenAL.AllocationSize = size;
    if (g_OpenAL.AllocationFails)
        return 0;
    g_OpenAL.Allocation = malloc(size ? size : 1);
    CHECK(g_OpenAL.Allocation);
    memset(g_OpenAL.Allocation, 0xa5, size);
    return g_OpenAL.Allocation;
}

extern "C" void CodaFree(void* data)
{
    g_OpenAL.Record('f');
    CHECK(data != 0 && data == g_OpenAL.Allocation);
    free(data);
    g_OpenAL.Allocation = 0;
}

class CSalOpenALBuffer {
public:
    CSalOpenALBuffer(_CBUFFERDESC& desc, bool created) : m_ffStatus(created ? 3U : 1U), m_pDesc(&desc) {
        m_uiBuffer[0] = 17;
        m_uiBuffer[1] = created ? 19 : 0xc0daf00d;
    }
    int MakeStereoBuffer();
    unsigned int m_uiBuffer[2];
    unsigned int m_ffStatus;
    _CBUFFERDESC* m_pDesc;
};

int CSalOpenALBuffer::MakeStereoBuffer()
{
    void* pvStereoData = CodaMalloc(m_pDesc->cbSize * 2);
    if (!pvStereoData)
        return C_ERR_OUTOFMEMORY;

    signed short* pShortSrc;
    signed short* pShortDest;
    unsigned char* pByteSrc;
    unsigned char* pByteDest;
    int format;
    int cr;
    switch (m_pDesc->ffFormat) {
    case C_FORMAT_INT16: {
        pShortSrc = static_cast<signed short*>(m_pDesc->pvData);
        pShortDest = static_cast<signed short*>(pvStereoData);
        for (unsigned int i = 0; i < m_pDesc->cbSize / 2; ++i) {
            pShortDest[i * 2] = pShortSrc[i];
            pShortDest[i * 2 + 1] = pShortDest[i * 2];
        }
        format = AL_FORMAT_STEREO16;
        break;
    }
    case C_FORMAT_INT8: {
        pByteSrc = static_cast<unsigned char*>(m_pDesc->pvData);
        pByteDest = static_cast<unsigned char*>(pvStereoData);
        for (unsigned int i = 0; i < m_pDesc->cbSize; ++i) {
            pByteDest[i * 2] = pByteSrc[i];
            pByteDest[i * 2 + 1] = pByteDest[i * 2];
        }
        format = AL_FORMAT_STEREO8;
        break;
    }
    default:
        CodaFree(pvStereoData);
        return C_ERR_BUFFERTYPE;
    }

    g_OpenAL.GetError();
    if (!(m_ffStatus & 2)) {
        g_OpenAL.GetError();
        {
            const bool was_suspended = g_OpenAL.IsSuspended();
            g_OpenAL.Suspend();
            g_OpenAL.GenBuffers(1, &m_uiBuffer[1]);
            if (!was_suspended)
                g_OpenAL.Resume();
        }
        cr = g_OpenAL.CheckError();
        if (cr < C_OK) {
            CodaFree(pvStereoData);
            return cr;
        }
        m_ffStatus |= 2;
    }
    g_OpenAL.BufferData(m_uiBuffer[1], format, pvStereoData, m_pDesc->cbSize * 2, m_pDesc->nSamplesPerSec);
    cr = g_OpenAL.CheckError();
    CodaFree(pvStereoData);
    if (cr < C_OK) {
        {
            const bool was_suspended = g_OpenAL.IsSuspended();
            g_OpenAL.Suspend();
            g_OpenAL.DeleteBuffers(1, &m_uiBuffer[1]);
            if (!was_suspended)
                g_OpenAL.Resume();
        }
        m_ffStatus &= ~2;
        m_uiBuffer[1] = 0xC0DAF00D;
        return cr;
    }
    return C_OK;
}

static void CheckEarlyExits()
{
    unsigned char samples[] = {3, 1, 255};
    _CBUFFERDESC desc = {C_FORMAT_INT8, 1, 22050, sizeof(samples), 0, 0, samples};
    CSalOpenALBuffer instance(desc, false);
    g_OpenAL.Reset();
    g_OpenAL.AllocationFails = true;
    CHECK(instance.MakeStereoBuffer() == C_ERR_OUTOFMEMORY);
    CHECK(strcmp(g_OpenAL.Trace,"a") == 0 && g_OpenAL.AllocationSize == 6 && !g_OpenAL.Allocation);
    CHECK(instance.m_ffStatus == 1 && instance.m_uiBuffer[1] == 0xc0daf00d);
    g_OpenAL.Reset();
    desc.ffFormat = 99;
    CHECK(instance.MakeStereoBuffer() == C_ERR_BUFFERTYPE);
    CHECK(strcmp(g_OpenAL.Trace,"af") == 0 && !g_OpenAL.Allocation);
    CHECK(instance.m_ffStatus == 1 && instance.m_uiBuffer[1] == 0xc0daf00d);
}

static void CheckGenerateFailure(bool suspended, int error)
{
    signed short samples[] = {-3, 104, 32767};
    _CBUFFERDESC desc = {C_FORMAT_INT16, 1, 44100, sizeof(samples), 0, 0, samples};
    g_OpenAL.Reset();
    g_OpenAL.Suspended = suspended;
    g_OpenAL.GenerationError = error;
    CSalOpenALBuffer instance(desc, false);
    CHECK(instance.MakeStereoBuffer() == (error == AL_OUT_OF_MEMORY ? C_ERR_OUTOFMEMORY : C_ERR_NORESOURCES));
    CHECK(strcmp(g_OpenAL.Trace, suspended ? "aeesgcf" : "aeesgrcf") == 0);
    CHECK(instance.m_ffStatus == 1 && instance.m_uiBuffer[0] == 17 && instance.m_uiBuffer[1] == 42);
    CHECK(g_OpenAL.Suspended == suspended && !g_OpenAL.Uploaded && !g_OpenAL.Allocation);
}

static void CheckUpload(unsigned int format, unsigned int bytes, bool created, bool suspended, int error)
{
    unsigned char byte_samples[] = {3, 1, 255};
    signed short short_samples[] = {-3, 104, 32767};
    _CBUFFERDESC desc = {format, 1, 22050, bytes, 0, 0, format == C_FORMAT_INT8 ? static_cast<void*>(byte_samples) : static_cast<void*>(short_samples)};
    g_OpenAL.Reset();
    g_OpenAL.Suspended = suspended;
    g_OpenAL.UploadError = error;
    CSalOpenALBuffer instance(desc, created);
    CHECK(instance.MakeStereoBuffer() == (error == AL_NO_ERROR ? C_OK : error == AL_OUT_OF_MEMORY ? C_ERR_OUTOFMEMORY : C_ERR_NORESOURCES));
    char expected[24];
    strcpy(expected, created ? "aeucf" : suspended ? "aeesgcucf" : "aeesgrcucf");
    if (error != AL_NO_ERROR)
        strcat(expected, suspended ? "sd" : "sdr");
    CHECK(strcmp(g_OpenAL.Trace, expected) == 0);
    CHECK(!g_OpenAL.Allocation && g_OpenAL.Suspended == suspended && instance.m_uiBuffer[0] == 17);
    CHECK(instance.m_ffStatus == (error == AL_NO_ERROR ? 3U : 1U));
    CHECK(instance.m_uiBuffer[1] == (error == AL_NO_ERROR ? (created ? 19U : 42U) : 0xc0daf00d));
    CHECK(g_OpenAL.Uploaded && g_OpenAL.UploadedBuffer == (created ? 19U : 42U));
    CHECK(g_OpenAL.UploadedSize == bytes * 2 && g_OpenAL.UploadedFrequency == 22050);
    CHECK(g_OpenAL.UploadedFormat == (format == C_FORMAT_INT8 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16));
    if (format == C_FORMAT_INT8) {
        for (unsigned int i = 0; i < bytes; ++i)
            CHECK(g_OpenAL.Data[i * 2] == byte_samples[i] && g_OpenAL.Data[i * 2 + 1] == byte_samples[i]);
    } else {
        for (unsigned int i = 0; i < bytes / 2; ++i) {
            signed short pair[2];
            memcpy(pair, g_OpenAL.Data + i * 4, sizeof(pair));
            CHECK(pair[0] == short_samples[i] && pair[1] == short_samples[i]);
        }
        if (bytes % 2)
            CHECK(g_OpenAL.Data[bytes * 2 - 1] == 0xa5 && g_OpenAL.Data[bytes * 2 - 2] == 0xa5);
    }
}

int main()
{
    CheckEarlyExits();
    for (unsigned int suspended = 0; suspended < 2; ++suspended) {
        CheckGenerateFailure(suspended != 0, AL_OUT_OF_MEMORY);
        CheckGenerateFailure(suspended != 0, 0xa003);
        for (unsigned int created = 0; created < 2; ++created) {
            static const int errors[] = {AL_NO_ERROR, AL_OUT_OF_MEMORY, 0xa003};
            for (unsigned int e = 0; e < sizeof(errors) / sizeof(errors[0]); ++e) {
                CheckUpload(C_FORMAT_INT8, 3, created != 0, suspended != 0, errors[e]);
                CheckUpload(C_FORMAT_INT8, 0, created != 0, suspended != 0, errors[e]);
                CheckUpload(C_FORMAT_INT16, 6, created != 0, suspended != 0, errors[e]);
                CheckUpload(C_FORMAT_INT16, 3, created != 0, suspended != 0, errors[e]);
                CheckUpload(C_FORMAT_INT16, 1, created != 0, suspended != 0, errors[e]);
                CheckUpload(C_FORMAT_INT16, 0, created != 0, suspended != 0, errors[e]);
            }
        }
    }
    puts("00c4de00_TEST PASS");
    return 0;
}