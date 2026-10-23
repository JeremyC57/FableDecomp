// 00c6a080 catalog TransformGeometry/CQ_GuildTrainingSkillScript is generated and unverified.
// Inferred two-stage block processing into aligned512-byte temporary, postprocess and ringwrite.
// Names are descriptive only, observed offsets supply the layout; no recovered owner/header.
struct Ring {float* Data;unsigned long Length,Position;void Advance(unsigned long n){Position+=n;if(Position>=Length)Position-=Length;}};
// Direct callee audit: first-stage configuration is2 floats; second-stage is
// {unsigned delay; float gain, outputGain}, copied as12 bytes at00c76d84..91.
struct ConfigurationView {unsigned char Prefix[0x220];unsigned char First[8],Second[12];};
struct StageOne {unsigned char Data[0x14];int Mode;unsigned char Tail[0x68-0x18];
 void Process(void*,const float*,float*,unsigned long); //00c76e90
 void Blend(void*,void*,const float*,float*,unsigned long,unsigned long); //00c770b0
};
struct StageTwo {unsigned char Data[0x10];int Mode;
 void Process(void*,float*,float*,unsigned long); //00c76b60
 void Blend(void*,void*,float*,float*,unsigned long,unsigned long); //00c76c20
};
struct PostStage {void Process(float*,unsigned long);}; //00c75140
struct Backend {void Write(Ring*,unsigned long,float*,unsigned long);}; //00c76460
extern Backend* Backend_0143ccdc;
struct BufferState {
 void* Unknown;ConfigurationView* First;ConfigurationView* Second;
 unsigned char ToMode[0x18-0xc];int Mode;unsigned long BlendPosition;
 unsigned char ToRing[0x60-0x20];Ring Output;
 unsigned char ToStage[0x1e0-0x6c];StageOne One;StageTwo Two;PostStage Post;
 void ProcessBlock(unsigned long count,const float* input);
};
void BufferState::ProcessBlock(unsigned long count,const float* input){
 __declspec(align(16)) float temporary[128];
 ConfigurationView* first=First;ConfigurationView* second=Second;
 if(Mode==One.Mode)One.Blend(first->First,second->First,input,temporary,count,BlendPosition);
 else One.Process(first->First,input,temporary,count);
 if(Mode==Two.Mode)Two.Blend(first->Second,second->Second,temporary,temporary,count,BlendPosition);
 else Two.Process(first->Second,temporary,temporary,count);
 Post.Process(temporary,count);Backend_0143ccdc->Write(&Output,Output.Position,temporary,count);Output.Advance(count);
}