// Vendor-proof: libvorbis-1.0.1(lib/bitrate.c) @LACING_ADJUST@4 -- 24 bytes verified against Fable.exe 0x00C9CB60
int __fastcall Math_ComputeByteChunkCount(int value)
{
    return value / 255 + 1 + value;
}