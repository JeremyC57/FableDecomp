#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum EParsedItemType { PARSED_ITEM_SYMBOL=1, PARSED_ITEM_STRING=2 };
class CCharString {
public:
    const char* data;
    CCharString(const char* s=""):data(s){}
    bool operator!=(const char* s) const { return strcmp(data,s)!=0; }
    bool operator==(const char* s) const { return strcmp(data,s)==0; }
};
class CParsedItem {
public:
    EParsedItemType type;
    CCharString text;
    EParsedItemType GetType() const { return type; }
    const CCharString& GetString() const { return text; }
    const CCharString& GetSymbol() const { return text; }
};
class CStringParser {
    char text[256];
    char* next;
    unsigned long line;
public:
    CStringParser(const char* input):line(0) { strcpy(text,input); next=strtok(text," "); }
    unsigned long GetLine() const { return line; }
    bool ReadNextItem(CParsedItem& item) {
        if(!next) return false;
        ++line;
        item.text=next;
        item.type=strchr("{},;=!?",*next)?PARSED_ITEM_SYMBOL:PARSED_ITEM_STRING;
        next=strtok(0," ");
        return true;
    }
};
class CSymbolMap;
class CPersistContext {
public:
    CStringParser* parser;
    const CSymbolMap* symbols;
    CPersistContext(CStringParser& p,void*,bool load,void*,const CSymbolMap* map,void*):parser(&p),symbols(map) { if(!load || !map) abort(); }
};
template<class T> class CPersistTraits {
public:
    void TransferIn(CPersistContext& c,CStringParser& p,T& out) {
        if(c.parser!=&p) abort();
        CParsedItem item;
        if(!p.ReadNextItem(item)) abort();
        out=(T)atol(item.GetString().data);
    }
};
class CSymbolMap {
public:
    const char* CurrentFilename;
    char symbols[256],errors[256];
    CSymbolMap():CurrentFilename("test") { symbols[0]=errors[0]=0; }
    void AddSymbolDirty(const CCharString& group,const CCharString& key,long v) {
        sprintf(symbols+strlen(symbols),"%s/%s=%ld ",group.data,key.data,v);
    }
    void Error(const char* filename,long line,const char* msg,...) {
        if(filename!=CurrentFilename) abort();
        sprintf(errors+strlen(errors),"%ld %s|",line,msg);
    }
    bool ParseEnum(CStringParser&);
};
bool CSymbolMap::ParseEnum(CStringParser& parser)
{
    CParsedItem parser_item;
    if (parser.ReadNextItem(parser_item) == true) {
    CCharString symbol_type_name;
    if (parser_item.GetType() == PARSED_ITEM_STRING) {
        symbol_type_name = parser_item.GetString();
        if (!parser.ReadNextItem(parser_item)) {
            Error(CurrentFilename, parser.GetLine(), "Unexpected EOF while reading enum");
            return false;
        }
    } else
        symbol_type_name = "NULL";
    if (parser_item.GetType() == PARSED_ITEM_SYMBOL && !(parser_item.GetSymbol() != "{")) {
    long current_val = 0;
    while (parser.ReadNextItem(parser_item)) {
        if (parser_item.GetType() == PARSED_ITEM_SYMBOL && parser_item.GetSymbol() == "}") {
            if (!parser.ReadNextItem(parser_item)) {
                Error(CurrentFilename, parser.GetLine(), "Unexpected EOF in enum def line");
                return false;
            }
            if (parser_item.GetType() == PARSED_ITEM_SYMBOL && parser_item.GetSymbol() == ";")
                break;
        }
        if (parser_item.GetType() != PARSED_ITEM_STRING)
            Error(CurrentFilename, parser.GetLine(), "Expected string in enum");
        {
        CCharString symbol_name(parser_item.GetString());
        if (!parser.ReadNextItem(parser_item)) {
            Error(CurrentFilename, parser.GetLine(), "Unexpected EOF in enum");
            return false;
        }
        if (parser_item.GetType() == PARSED_ITEM_SYMBOL && parser_item.GetSymbol() == ",") {
            AddSymbolDirty(symbol_type_name, symbol_name, current_val);
        } else if (parser_item.GetType() == PARSED_ITEM_SYMBOL && parser_item.GetSymbol() == "}") {
            AddSymbolDirty(symbol_type_name, symbol_name, current_val);
            if (!parser.ReadNextItem(parser_item)) {
                Error(CurrentFilename, parser.GetLine(), "Unexpected EOF in enum");
                return false;
            }
            if (parser_item.GetType() != PARSED_ITEM_SYMBOL || parser_item.GetSymbol() != ";")
                Error(CurrentFilename, parser.GetLine(), "Expecting ';' in enum");
            break;
        } else if (parser_item.GetType() == PARSED_ITEM_SYMBOL && parser_item.GetSymbol() == "=") {
            CPersistContext persist_context(parser, 0, true, 0, this, 0);
            CPersistTraits<long> val_traits;
            val_traits.TransferIn(persist_context, parser, current_val);
            AddSymbolDirty(symbol_type_name, symbol_name, current_val);
            if (!parser.ReadNextItem(parser_item)) {
                Error(CurrentFilename, parser.GetLine(), "Unexpected EOF in enum");
                return false;
            }
            if (parser_item.GetType() == PARSED_ITEM_SYMBOL) {
                if (parser_item.GetSymbol() == ",") {
            } else if (parser_item.GetSymbol() == "}") {
                if (!parser.ReadNextItem(parser_item)) {
                    Error(CurrentFilename, parser.GetLine(), "Unexpected EOF in enum");
                    return false;
                }
                if (parser_item.GetType() != PARSED_ITEM_SYMBOL || parser_item.GetSymbol() != ";")
                    Error(CurrentFilename, parser.GetLine(), "Expecting ';' in enum");
                break;
            } else
                Error(CurrentFilename, parser.GetLine(), "Expecting ',' '=' or '}' in enum");
            } else
                Error(CurrentFilename, parser.GetLine(), "Expecting ',' '=' or '}' in enum");
        } else
            Error(CurrentFilename, parser.GetLine(), "Expecting ',' '=' or '}' in enum");
        }
        ++current_val;
    }
    return true;
    }
    Error(CurrentFilename, parser.GetLine(), "Expecting a brace in enum");
    return false;
    }
    Error(CurrentFilename, parser.GetLine(), "Cannot read Definition Type name");
    return false;
}
static bool check(const char* input,bool success,const char* symbols,const char* errors) {
    CSymbolMap map;
    CStringParser parser(input);
    bool result=map.ParseEnum(parser);
    if(success==result && !strcmp(symbols,map.symbols) && !strcmp(errors,map.errors)) return true;
    printf("%s: got %d [%s] [%s]\n",input,(int)result,map.symbols,map.errors); return false;
}
int main() {
    if(!check("",false,"","0 Cannot read Definition Type name|")) return 1;
    if(!check("Enum",false,"","1 Unexpected EOF while reading enum|")) return 2;
    if(!check("Enum ?",false,"","2 Expecting a brace in enum|")) return 3;
    if(!check("{ } ;",true,"","")) return 4;
    if(!check("Colour { Red , Blue = 17 , Green } ;",true,"Colour/Red=0 Colour/Blue=17 Colour/Green=18 ","")) return 5;
    if(!check("{ A = -3 } ;",true,"NULL/A=-3 ","")) return 6;
    if(!check("{ A ,",true,"NULL/A=0 ","")) return 7;
    if(!check("{ A",false,"","2 Unexpected EOF in enum|")) return 8;
    if(!check("{ A }",false,"NULL/A=0 ","3 Unexpected EOF in enum|")) return 9;
    if(!check("{ A } Bad",true,"NULL/A=0 ","4 Expecting ';' in enum|")) return 10;
    if(!check("{ A = 8",false,"NULL/A=8 ","4 Unexpected EOF in enum|")) return 11;
    if(!check("{ A = 8 }",false,"NULL/A=8 ","5 Unexpected EOF in enum|")) return 12;
    if(!check("{ A = 8 } !",true,"NULL/A=8 ","6 Expecting ';' in enum|")) return 13;
    if(!check("{ A = 8 ! B } ;",true,"NULL/A=8 NULL/B=9 ","5 Expecting ',' '=' or '}' in enum|")) return 14;
    if(!check("{ A = 8 Bad B } ;",true,"NULL/A=8 NULL/B=9 ","5 Expecting ',' '=' or '}' in enum|")) return 15;
    if(!check("{ A ! B } ;",true,"NULL/B=1 ","3 Expecting ',' '=' or '}' in enum|")) return 16;
    if(!check("{ ! , B } ;",true,"NULL/!=0 NULL/B=1 ","2 Expected string in enum|")) return 17;
    if(!check("{ }",false,"","2 Unexpected EOF in enum def line|")) return 18;
    if(!check("{ } B } ;",true,"NULL/B=0 ","")) return 19;
    puts("009e55a0_TEST PASS"); return 0;
}