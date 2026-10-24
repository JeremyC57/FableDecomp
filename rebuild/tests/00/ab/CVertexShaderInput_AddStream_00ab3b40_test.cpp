// Behaviour test for CVertexShaderInput::AddStream(unsigned long, const CVertexStreamFormat&, bool) @ 0x00AB3B40
// Appends {streamIndex, copy of format} to DataStreams (growing storage when full) and rebuilds the declaration
// only when generateDeclaration is true.
#include <cstdio>
#include <cstdlib>
#include <new>

struct CFormatModel { unsigned int ElementCount; unsigned int Tag; };

struct CStreamInfoModel {
    unsigned int StreamIndex;
    CFormatModel StreamFormat;
    CStreamInfoModel(unsigned int index, const CFormatModel& format) : StreamIndex(index), StreamFormat(format) { }
};

struct CStreamList {
    CStreamInfoModel* start;
    CStreamInfoModel* finish;
    CStreamInfoModel* end;
    unsigned int overflows;
    CStreamList() : start(0), finish(0), end(0), overflows(0) { }
    void insert_overflow(const CStreamInfoModel& x)
    {
        ++overflows;
        unsigned int size = (unsigned int)(finish - start);
        unsigned int len = size + (size > 1 ? size : 1);
        CStreamInfoModel* fresh = (CStreamInfoModel*)std::malloc(len * sizeof(CStreamInfoModel));
        for (unsigned int i = 0; i < size; ++i) new (fresh + i) CStreamInfoModel(start[i]);
        new (fresh + size) CStreamInfoModel(x);
        std::free(start);
        start = fresh; finish = fresh + size + 1; end = fresh + len;
    }
    void push_back(const CStreamInfoModel& x)
    {
        if (finish != end) { new (finish) CStreamInfoModel(x); ++finish; }
        else insert_overflow(x);
    }
};

struct CVertexShaderInputModel {
    CStreamList DataStreams;
    unsigned int Declarations;
    CVertexShaderInputModel() : Declarations(0) { }
    void GenerateD3DDeclaration() { ++Declarations; }
    void AddStream(unsigned int streamIndex, const CFormatModel& format, bool generateDeclaration)
    {
        DataStreams.push_back(CStreamInfoModel(streamIndex, format));
        if (generateDeclaration)
            GenerateD3DDeclaration();
    }
};

int main()
{
    CVertexShaderInputModel input;
    CFormatModel f0 = { 3, 10 };
    CFormatModel f1 = { 5, 11 };
    CFormatModel f2 = { 1, 12 };
    input.AddStream(0, f0, false);
    input.AddStream(1, f1, true);
    input.AddStream(4, f2, true);
    int ok = (input.DataStreams.finish - input.DataStreams.start) == 3 && input.Declarations == 2
          && input.DataStreams.overflows >= 2;
    const CStreamInfoModel* s = input.DataStreams.start;
    if (ok && (s[0].StreamIndex != 0 || s[0].StreamFormat.Tag != 10 || s[1].StreamIndex != 1
               || s[1].StreamFormat.ElementCount != 5 || s[2].StreamIndex != 4 || s[2].StreamFormat.Tag != 12)) ok = 0;
    std::free(input.DataStreams.start);
    if (!ok) { std::printf("00ab3b40_TEST FAIL\n"); return 1; }
    std::printf("00ab3b40_TEST PASS\n");
    return 0;
}