// 00aad650 full selected target body; portable dependency fixtures, no separate parser substitute.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
static void Check(bool v){if(!v){std::puts("00aad650_TEST FAIL");std::exit(1);}}
class CWaveFileException {public: const char* Message; CWaveFileException(const char* p):Message(p){} };
template<class T> class CArray:public std::vector<T>{public:CArray(){} explicit CArray(unsigned long n):std::vector<T>((size_t)n){} };
class CWaveFile {public:unsigned long Channels,SampleLength,SamplingFrequency;CArray<short> AudioData; CWaveFile():Channels(0),SampleLength(0){} void Initialise(unsigned long chan,unsigned long freq,const CArray<short>& data){Channels=chan;SamplingFrequency=freq;AudioData=data;SampleLength=AudioData.size()/Channels;} };
template<class T>class CCountedPointer {struct Owner{T* p;unsigned refs;Owner(T* x):p(x),refs(1){}};Owner* own;void Release(){if(own&&!--own->refs){delete own->p;delete own;}}public:CCountedPointer():own(0){} CCountedPointer(const CCountedPointer& x):own(x.own){if(own)++own->refs;}~CCountedPointer(){Release();} void Reset(T* p){Release();own=new Owner(p);}T* operator->()const{return own?own->p:0;}bool IsNull()const{return !own;} };
class CCharString {std::string value;public:CCharString(const char* p,unsigned long n):value(p,(size_t)n){}bool operator==(const char* p)const{return value==p;}bool operator!=(const char* p)const{return value!=p;} };
class CDataInputStream {const std::vector<unsigned char>& file;unsigned long pos;public:std::vector<unsigned long> seeks;explicit CDataInputStream(const std::vector<unsigned char>& f):file(f),pos(0){} unsigned long GetPosition()const{return pos;}void SetPosition(unsigned long p){Check(p<=file.size());seeks.push_back(p);pos=p;}unsigned char ReadUBYTE(){Check(pos<file.size());return file[pos++];}unsigned short ReadUWORD(){unsigned a=ReadUBYTE(),b=ReadUBYTE();return (unsigned short)(a|(b<<8));}short ReadSWORD(){return (short)ReadUWORD();}unsigned long ReadULONG(){unsigned long a=ReadUWORD(),b=ReadUWORD();return a|(b<<16);}CCharString ReadString(unsigned long n){Check(pos+n<=file.size());CCharString text((const char*)&file[pos],n);pos+=n;return text;} };
class CWaveLoader {public:class CWaveFormatHeader {public:unsigned short FormatTag,Channels;unsigned long SamplesPerSec,AvgBytesPerSec;unsigned short BlockAlign,BitsPerSample;CWaveFormatHeader():FormatTag(0){} };CCountedPointer<CWaveFile> LoadWaveFile(CDataInputStream& is);};
CCountedPointer<CWaveFile> CWaveLoader::LoadWaveFile(CDataInputStream& is)
{
    CCountedPointer<CWaveFile> wavefile;

    CCharString file_header = is.ReadString(4);
    if (file_header != "RIFF")
        throw CWaveFileException("Not a WAV file!");
    unsigned long total_length = is.ReadULONG();
    unsigned long end_pos = is.GetPosition() + total_length;

    CCharString datatype = is.ReadString(4);
    if (datatype != "WAVE")
        throw CWaveFileException("Not a WAV file!");

    CWaveFormatHeader wave_format;
    while (is.GetPosition() < end_pos) {
        CCharString chunk_header = is.ReadString(4);
        unsigned long chunk_length = is.ReadULONG();
        unsigned long chunk_start = is.GetPosition();
        if (chunk_header == "fmt ") {
            wave_format.FormatTag = is.ReadUWORD();
            wave_format.Channels = is.ReadUWORD();
            wave_format.SamplesPerSec = is.ReadULONG();
            wave_format.AvgBytesPerSec = is.ReadULONG();
            wave_format.BlockAlign = is.ReadUWORD();
            wave_format.BitsPerSample = is.ReadUWORD();
        } else if (chunk_header == "data") {
            if (wave_format.FormatTag == 1) {
            unsigned long samples = chunk_length / wave_format.BlockAlign;
            unsigned long size = samples * wave_format.Channels;
            CArray<short> audio_data(size);
            unsigned long bits = wave_format.BlockAlign * 8 / wave_format.Channels;
            if (bits == 16) {
                // Donor033FF2E7 and033FF32B retain the original constant-false loop-scope guard.
                if (false) {} else for (unsigned long c1 = 0; c1 < size; c1++)
                    audio_data[c1] = is.ReadSWORD();
            } else if (bits == 8) {
                // Donor033FF2E7 and033FF32B retain the original constant-false loop-scope guard.
                if (false) {} else for (unsigned long c1 = 0; c1 < size; c1++)
                    audio_data[c1] = (short)(((unsigned short)is.ReadUBYTE() - 0x80u) << 8);
            } else {
                throw CWaveFileException("WAV data is not 16 or 8 bits!");
            }
            wavefile.Reset(new CWaveFile);
            wavefile->Initialise(wave_format.Channels,wave_format.SamplesPerSec,audio_data);
            } else {
                throw CWaveFileException("Unsupported WAV compression!");
            }
        }
        is.SetPosition(chunk_start + chunk_length);
    }
    return wavefile;
}

static void Put16(std::vector<unsigned char>& f,unsigned v){f.push_back((unsigned char)v);f.push_back((unsigned char)(v>>8));}
static void Put32(std::vector<unsigned char>& f,unsigned long v){Put16(f,(unsigned)v);Put16(f,(unsigned)(v>>16));}
static void Tag(std::vector<unsigned char>& f,const char* p){for(int i=0;i<4;++i)f.push_back((unsigned char)p[i]);}
static void Chunk(std::vector<unsigned char>& f,const char* id,const std::vector<unsigned char>& d){Tag(f,id);Put32(f,(unsigned long)d.size());f.insert(f.end(),d.begin(),d.end());}
static void Seal(std::vector<unsigned char>& f){unsigned long n=(unsigned long)f.size()-8;for(int i=0;i<4;++i)f[4+i]=(unsigned char)(n>>(8*i));}
static std::vector<unsigned char> Make(unsigned channels,unsigned bits,unsigned frames,unsigned flags,unsigned format=1){
 std::vector<unsigned char> f,d,fmt,junk;Tag(f,"RIFF");Put32(f,0);Tag(f,"WAVE");
 if(flags&1){junk.push_back(9);junk.push_back(7);junk.push_back(3);Chunk(f,"LIST",junk);}
 Put16(fmt,format);Put16(fmt,channels);Put32(fmt,22050);Put32(fmt,22050*channels*(bits/8));Put16(fmt,channels*(bits/8));Put16(fmt,flags&2?99:bits);
 if(flags&4){fmt.push_back(5);fmt.push_back(6);fmt.push_back(7);}Chunk(f,"fmt ",fmt);
 for(unsigned i=0;i<frames*channels;++i){unsigned sample=(i*71+flags*13)&255;if(bits==8)d.push_back((unsigned char)sample);else Put16(d,(sample<<8)|sample);}
 if(flags&8)Chunk(f,"data",junk);
 Chunk(f,"data",d);
 if(flags&16)Chunk(f,"TAIL",junk);
 Seal(f);return f;
}
int main(){CWaveLoader loader;unsigned cases=0;
 for(unsigned channels=1;channels<=4;++channels)for(unsigned bitcase=0;bitcase<2;++bitcase)for(unsigned frames=0;frames<9;++frames)for(unsigned flags=0;flags<32;++flags){
  unsigned bits=bitcase?16:8;std::vector<unsigned char> file=Make(channels,bits,frames,flags);CDataInputStream input(file);CCountedPointer<CWaveFile> wave=loader.LoadWaveFile(input);
  Check(!wave.IsNull());Check(wave->Channels==channels&&wave->SamplingFrequency==22050&&wave->SampleLength==frames&&wave->AudioData.size()==frames*channels);Check(input.GetPosition()==file.size());
  for(unsigned i=0;i<frames*channels;++i){unsigned sample=(i*71+flags*13)&255;int want=bits==8?((int)sample-128)*256:(int)((sample<<8)|sample);if(bits==16&&want>=32768)want-=65536;Check(wave->AudioData[i]==want);}
  Check(input.seeks.size()==2+((flags&1)?1:0)+((flags&8)?1:0)+((flags&16)?1:0));++cases;
 }
 std::vector<unsigned char> empty;Tag(empty,"RIFF");Put32(empty,4);Tag(empty,"WAVE");CDataInputStream input(empty);CCountedPointer<CWaveFile> none=loader.LoadWaveFile(input);Check(none.IsNull());
#if !defined(_MSC_VER) || defined(_CPPUNWIND)
 for(int bad=0;bad<4;++bad){std::vector<unsigned char> file=Make(1,bad==3?24:16,1,0,bad==2?3:1);if(bad==0)file[0]='X';if(bad==1)file[8]='X';CDataInputStream in(file);bool caught=false;try{loader.LoadWaveFile(in);}catch(const CWaveFileException& e){caught=true;Check(std::strcmp(e.Message,bad<2?"Not a WAV file!":bad==2?"Unsupported WAV compression!":"WAV data is not 16 or 8 bits!")==0);}Check(caught);}
#endif
 Check(cases==2304);std::puts("00aad650_TEST PASS");return 0;
}