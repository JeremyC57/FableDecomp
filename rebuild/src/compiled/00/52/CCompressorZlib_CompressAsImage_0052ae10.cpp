// CCompressorZlib::CompressAsImage(const void*, unsigned long, void*, unsigned long, long, long, long, CCompressionInfo&) -- zlib ignores the image dimensions: forward to Compress
// Retail: Fable.exe @ 0x0052AE10, 33 bytes; vtable 0x01245974 slot 6 (abs ref 0x0124598C). Calls slot 5
// ?Compress@CCompressorZlib@@UAEXPBXKPAXKAAVCCompressionInfo@@@Z (0x00A3AC40) virtually with (src, src_size, dst,
// dst_size, info); width/height/bpp are dropped.
// Layout: vtable modelled from Fable.exe 0x01245974: slot 0 dtor, 1-4 CACompressor (CompressAsImage, Compress,
// Decompress, SetQuality on non-const sources), 5-7 CCompressorZlib's const-source Compress/CompressAsImage/Decompress.
// engine/CCompressorZlib.h carries data members only (no virtuals), so the polymorphic shell is local.
class CCompressionInfo;
class CBaseClass {
public:
    virtual ~CBaseClass() {}
};
class CACompressor : public CBaseClass {
public:
    virtual void CompressAsImage(void* src, unsigned long src_size, void* dst, unsigned long dst_size, long width, long height, long bpp, CCompressionInfo& info);
    virtual void Compress(void* src, unsigned long src_size, void* dst, unsigned long dst_size, CCompressionInfo& info);
    virtual void Decompress(void* src, unsigned long src_size, void* dst, unsigned long dst_size, CCompressionInfo& info);
    virtual void SetQuality(float quality);
};
class CCompressorZlib : public CACompressor {
public:
    virtual void Compress(const void* src, unsigned long src_size, void* dst, unsigned long dst_size, CCompressionInfo& info);
    virtual void CompressAsImage(const void* src, unsigned long src_size, void* dst, unsigned long dst_size, long width, long height, long bpp, CCompressionInfo& info);
    virtual void Decompress(const void* src, unsigned long src_size, void* dst, unsigned long dst_size, CCompressionInfo& info);
};
void CCompressorZlib::CompressAsImage(const void* src, unsigned long src_size, void* dst, unsigned long dst_size, long, long, long, CCompressionInfo& info)
{
    Compress(src, src_size, dst, dst_size, info);
}