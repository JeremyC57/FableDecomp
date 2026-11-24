// 00560d00 CTCHeroOnlineScoreboard::LogOnToRegistrationServer
// Leaf: ?LogOnToRegistrationServer@CTCHeroOnlineScoreboard@@AAE?AW4ENetworkSuccess@1@AAVCCharString@@@Z
// Flags: /Gr
// FableWin and Ego_r name the private member. FableWin's original local scopes put the transaction result after both response branches.
#define FABLE_RETAIL_STLP_STRING_RANGE_ERASE_PASS28
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_NETWORK
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_SHOWSCOREBOARD
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_LOGON_PASS28
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_SCOREBOARD
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_STRING_FIND
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_SEARCH
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_EXPLODE
#define FABLE_RETAIL_LHHTTP2_DOCUMENT
#define FABLE_RETAIL_STLP_FILL_CHAR
#define FABLE_RETAIL_SCOREBOARD_TRANSACTION_PASS28
#define FABLE_RETAIL_LHHTTPHEADERSTATUS_LOCATION
#define FABLE_RETAIL_LHHTTP2_SENDREQUEST
#define FABLE_RETAIL_LHHTTP2_GETHEADER
#define FABLE_RETAIL_LHHTTP2_RECEIVEDOCUMENT
#define FABLE_RETAIL_PASS22_CCHARSTRING_GETASCHARARRAY_ORIGINAL_IFELSE
#include "fable_compiler.h"
#include "engine/CTCHeroOnlineScoreboard.h"
#include "stlport/fable_string_bodies.h"
#include "engine/LHHttpHeaderStatus.h"

CCharString FABLE_FASTCALL operator+(const CCharString&, const char*);
CCharString FABLE_FASTCALL operator+(const CCharString&, const CCharString&);
long FABLE_FASTCALL GFCharStringToInt(const CCharString&);

inline CCharString::operator const char*() const { return GetAsCharArray(); }

namespace std {
template<class C,class Traits,class Alloc>
basic_string<C,Traits,Alloc> __cdecl operator+(const basic_string<C,Traits,Alloc>& first, const basic_string<C,Traits,Alloc>& second) {
    typedef basic_string<C,Traits,Alloc> String;
    String result(typename String::_Reserve_t(), first.size() + second.size());
    result.append(first);
    result.append(second);
    return result;
}
}

namespace {
extern CCharString EASY_KEY;
CCharString FABLE_FASTCALL MD5(const CCharString&);
std::string FABLE_FASTCALL StdEncryptString(const std::string&, const std::string&);
std::string FABLE_FASTCALL StdDecryptString(const std::string&, const std::string&);
}


// STLport byte fills at 004110D0 use the character overloads of fill and fill_n.
template<> inline void std::vector<char>::_M_fill_insert(char* position, size_type n, const char& x)
{
    if (n) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= n) {
            char x_copy = x;
            const size_type elems_after = this->_M_finish - position;
            char* old_finish = this->_M_finish;
            if (elems_after > n) {
                char* first = old_finish - n;
                if (old_finish != first) memmove(old_finish, first, old_finish - first);
                this->_M_finish += n;
                int remaining = old_finish - n - position;
                if (remaining > 0) memmove(old_finish - remaining, position, remaining);
                memset(position, (unsigned char)x_copy, n);
            } else {
                memset(this->_M_finish,(unsigned char)x_copy,n-elems_after);
                this->_M_finish += n-elems_after;
                if (old_finish != position) memmove(this->_M_finish, position, old_finish - position);
                this->_M_finish += elems_after;
                memset(position, (unsigned char)x_copy, old_finish - position);
            }
        } else {
            __true_type t;
            this->_M_insert_overflow(position,x,t,n,false);
        }
    }
}

void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*) throw();

namespace {
// Fidelity: same-TU strong definition of 0x0055E040 (retail TU-mate)
void FABLE_FASTCALL EncipherMorsel(unsigned long* v,unsigned long* w,unsigned long* k)
{
    unsigned long v0=v[0],v1=v[1];
    unsigned long sum=0,delta=0x9e3779b9;
    for(unsigned long i=0;i<32;++i) {
        v0+=(((v1<<4)^(v1>>5))+v1)^(sum+k[sum&3]);
        sum+=delta;
        v1+=(((v0<<4)^(v0>>5))+v0)^(sum+k[(sum>>11)&3]);
    }
    w[0]=v0;w[1]=v1;
}
// Fidelity: same-TU strong definition of 0x0055E180 (retail TU-mate)
void FABLE_FASTCALL DecipherMorsel(unsigned long* v,unsigned long* w,unsigned long* k)
{
    unsigned long v0=v[0],v1=v[1];
    unsigned long sum=0xc6ef3720,delta=0x9e3779b9;
    for(unsigned long i=0;i<32;++i) {
        v1-=(((v0<<4)^(v0>>5))+v0)^(sum+k[(sum>>11)&3]);
        sum-=delta;
        v0-=(((v1<<4)^(v1>>5))+v1)^(sum+k[sum&3]);
    }
    w[0]=v0;w[1]=v1;
}
// Fidelity: same-TU strong definition of 0x0055E2C0 (retail TU-mate)
int FABLE_FASTCALL CryptoMatic(const char* data,char* enc,int dlen,const char* key,char mode)
{
    unsigned long keylength=strlen(key);
    char* keycopy=new char[keylength+1];
    unsigned long cryptuse[4]={0xaa9c,0xccc3,0xffcf,0xbbbb};
    long t=4;
    int retsize=0;
    memset(keycopy,0,keylength+1);
    for(int c=0;c<keylength;++c)
        keycopy[c]=(char)(key[keylength-c]*(c+1));
    cryptuse[2]=keycopy[1];
    cryptuse[3]=keycopy[0];
    cryptuse[1]=keycopy[2];
    cryptuse[0]=keycopy[3];
    char* oldenc=enc;
    if(mode==0) {
        memcpy(enc,&dlen,4);enc+=4;
    } else {
        memcpy(&retsize,data,4);
        if(retsize>dlen+10) {
            int mod=dlen%8;
            dlen-=mod;
            retsize=dlen;
        }
        data+=4;dlen-=4;
    }
    for(;dlen>=8;dlen-=8) {
        if(mode==0)EncipherMorsel((unsigned long*)data,(unsigned long*)enc,cryptuse);
        else DecipherMorsel((unsigned long*)data,(unsigned long*)enc,cryptuse);
        data+=8;enc+=8;
        if(t>=keylength)t=0;
        cryptuse[0]+=keycopy[t++];
        if(t>=keylength)t=0;
        cryptuse[1]-=keycopy[t++];
        if(t>=keylength)t=0;
        cryptuse[2]+=keycopy[t++];
        if(t>=keylength)t=0;
        cryptuse[3]-=keycopy[t++];
    }
    if(dlen>0) {
        unsigned long indata[2]={0,0};
        memcpy(indata,data,dlen);
        if(mode==0)EncipherMorsel(indata,(unsigned long*)enc,cryptuse);
        else DecipherMorsel(indata,(unsigned long*)enc,cryptuse);
        enc+=8;
    }
    if(mode==0)retsize=enc-oldenc;
    delete[] keycopy;
    return retsize;
}
// Fidelity: same-TU strong definition of 0x00560920 (retail TU-mate)
int FABLE_FASTCALL Explode(std::string& input,const std::string& token,std::vector<std::string>& ret)
{
    unsigned int startpos=0;
    unsigned int findpos=0;
    do {
        findpos=input.find(token,startpos);
        if(findpos!=std::string::npos) {
            std::string newstr=input.substr(startpos,findpos-startpos);
            ret.push_back(newstr);
            startpos=findpos+token.length();
        }
    } while(findpos!=std::string::npos);
    return ret.size();
}
}

int CTCHeroOnlineScoreboard::GetDecryptedDocument(std::vector<std::string>& r_decrypted_lines,const CCharString& key)
{
    unsigned long docsize=PHttp->GetDocumentSize();
    if(docsize!=0) {
        std::vector<char> encrypted_doc(docsize+1,0);
        PHttp->GetDocument(&encrypted_doc[0]);
        std::vector<char> decrypted_doc;
        decrypted_doc.resize(docsize+1);
        int decrypted_size=CryptoMatic(&encrypted_doc[0],&decrypted_doc[0],docsize,key.GetAsCharArray(),1);
        std::string doc_str(decrypted_doc.begin(),decrypted_doc.end());
        int count=Explode(doc_str,std::string("\r\n"),r_decrypted_lines);
        return count;
    }
    return -1;
}


namespace {
int FABLE_FASTCALL CryptoMatic(const char*,char*,int,const char*,char);
int FABLE_FASTCALL EncryptBuffer(const char* source,char* destination,int length,const char* key) {
    return CryptoMatic(source,destination,length,key,0);
}
std::string FABLE_FASTCALL StdEncryptString(const std::string& string_to_encrypt,const std::string& key) {
    std::vector<char> target_buffer(string_to_encrypt.size()+32,0);
    int enc_len=EncryptBuffer(string_to_encrypt.c_str(),&target_buffer[0],string_to_encrypt.size(),key.c_str());
    std::string ret_std_string(&target_buffer[0],&target_buffer[0]+enc_len);
    return ret_std_string;
}
int FABLE_FASTCALL DecryptBuffer(const char* source,char* destination,int length,const char* key) {
    return CryptoMatic(source,destination,length,key,1);
}
std::string FABLE_FASTCALL StdDecryptString(const std::string& string_to_decrypt,const std::string& key) {
    std::vector<char> target_buffer(string_to_decrypt.size(),0);
    int dec_len=DecryptBuffer(string_to_decrypt.c_str(),&target_buffer[0],string_to_decrypt.size(),key.c_str());
    std::string ret_std_string(&target_buffer[0],&target_buffer[0]+dec_len);
    return ret_std_string;
}
}

CTCHeroOnlineScoreboard::ENetworkSuccess CTCHeroOnlineScoreboard::UpdateTransaction()
{
    using namespace LHNetworkLib;
    switch (TaskState) {
    case ETS_UNSTARTED: return ENS_FAILURE;
    case ETS_SENDING_REQUEST_ASYNC: {
        LHHttpHeaderStatus HttpHeader;
        LH_HTTP_STATUS request = PHttp->SendRequestAsync();
        if (request == HTTP_STATUS_REQUEST_SENT) {
            TaskState = ETS_RECEIVING_HEADER_ASYNC;
        } else {
            if (request == HTTP_STATUS_NO_REQUEST_PREPARED || request == HTTP_STATUS_SENDING_ERROR) return ENS_FAILURE;
            return ENS_PENDING;
        }
    }
    case ETS_RECEIVING_HEADER_ASYNC: {
        LHHttpHeaderStatus HttpHeader;
        LH_HTTP_STATUS request = PHttp->ReceiveHeaderAsync();
        if (request == HTTP_STATUS_RECEIVING_ERROR || request == HTTP_STATUS_TIMEOUT_ERROR) return ENS_FAILURE;
        if (request != HTTP_STATUS_HEADER_RECEIVED) return ENS_PENDING;
        PHttp->GetHeader(HttpHeader);
        if (HttpHeader.hsServerCode != HTTP_DOCUMENT_FOUND) return ENS_FAILURE;
        TaskState = ETS_RECEIVING_DOCUMENT_ASYNC;
    }
    case ETS_RECEIVING_DOCUMENT_ASYNC: {
        LH_HTTP_STATUS request = PHttp->ReceiveDocumentAsync();
        if (request == HTTP_STATUS_DOCUMENT_RECEIVED) {
            TaskState = ETS_UNSTARTED;
            return ENS_SUCCESS;
        }
        if (request == HTTP_STATUS_RECEIVING_ERROR || request == HTTP_STATUS_TIMEOUT_ERROR) return ENS_FAILURE;
        return ENS_PENDING;
    }
    }
    return ENS_FAILURE;
}

CTCHeroOnlineScoreboard::ENetworkSuccess CTCHeroOnlineScoreboard::LogOnToRegistrationServer(CCharString& error_message_tag)
{
    if (TaskState == ETS_UNSTARTED || TaskState == ETS_PREPARING) {
        std::string req((GetGameIdent() + "LOG_ON_TO_REG_SERVER\r\n" + GetSignedInUserName() + "\r\n" + MD5(GetSignedInUserName().GetAsLowerCase() + GetSignedInPassword().GetAsLowerCase()) + "\r\n").GetAsCharArray());
        req = req + StdEncryptString(GetCDKey().GetAsCharArray(), MD5(GetSignedInPassword().GetAsLowerCase()).GetAsCharArray()) + std::string("\r\n");
        if (ResetAndOpenHTTPAndPrepareRequest(req, EASY_KEY, true) == ENS_FAILURE) {
            error_message_tag = "TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK";
            return ENS_FAILURE;
        }
    }
    ENetworkSuccess result = UpdateTransaction();
    if (result == ENS_SUCCESS) {
        std::vector<std::string> lines;
        int count = GetDecryptedDocument(lines, EASY_KEY);
        if (count > 0) {
            CCharString user_id_str(lines[0].c_str());
            CCharString return_code_str(lines[1].c_str());
            long return_code = GFCharStringToInt(return_code_str);
            std::string sessionid_encrypted(lines[2]);
            if (return_code == 0) {
                std::string sessionid = StdDecryptString(sessionid_encrypted, GetCDKey().GetAsCharArray());
                SessionID = sessionid.c_str();
            }
            else
                SessionID = "0";
            switch (return_code) {
            case -9:
            case -2:
            case -1:
                error_message_tag = "TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK";
                return ENS_FAILURE;
            case -5:
            case -4:
            case -3:
                error_message_tag = "TEXT_GUI_SCOREBOARD_ERROR_LOGIN_FAILURE";
                return ENS_FAILURE;
            case -7:
                error_message_tag = "TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK";
                return ENS_FAILURE;
            case -8:
                error_message_tag = "TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK";
                return ENS_FAILURE;
            }
        }
        PHttp.reset();
    } else {
        error_message_tag = "TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK";
    }
    return result;
}