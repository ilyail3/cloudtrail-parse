#include <iostream>
#include "rapidjson/reader.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace std;
using namespace rapidjson;

struct MyHandler {
    bool records_start = false;
    int array_depth = -1;
    int object_depth = 0;

    StringBuffer s;
    Writer<StringBuffer> writer;

    MyHandler(){
        writer.Reset(s);
    }

    bool rel(){
        return records_start && array_depth >= 0 && object_depth >= 0;
    }

    bool Null() {
        if(rel())
            writer.Null();


        return true;
    }
    bool Bool(bool b) {
        if(rel())
            writer.Bool(b);


        return true;
    }
    bool Int(int i) {
        if(rel())
            writer.Int(i);


        return true;
    }
    bool Uint(unsigned u) {
        if(rel())
            writer.Uint(u);


        return true;
    }
    bool Int64(int64_t i) {
        if(rel())
            writer.Int64(i);


        return true;
    }
    bool Uint64(uint64_t u) {
        if(rel())
            writer.Uint64(u);


        return true;
    }
    bool Double(double d) {
        if(rel())
            writer.Double(d);

        return true;
    }
    bool RawNumber(const char* str, SizeType length, bool copy) {
        if(rel())
            writer.RawNumber(str, length, copy);

        return true;
    }
    bool String(const char* str, SizeType length, bool copy) {
        if(rel())
            writer.String(str, length, copy);

        return true;
    }
    bool StartObject() {
        if(records_start) {
            if(rel())
                writer.StartObject();

            object_depth++;
        }

        return true;
    }
    bool Key(const char* str, SizeType length, bool copy) {
        //cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        //return true;

        if(!records_start){
            if(length == 7 && memcmp(str, "Records", 7) == 0)
                records_start = true;
        } else if(rel()) {
            writer.Key(str, length, copy);
        }

        return true;
    }
    bool EndObject(SizeType memberCount) {
        if(records_start) {
            object_depth--;

            if(rel())
                writer.EndObject(memberCount);

            //cout << "obj: " << object_depth << " arr:" << array_depth << endl;
            if(object_depth == 0 and array_depth == 0) {
                //cout << endl << "NEW OBJECT" << endl << endl;
                ObjectComplete();
            }
        }

        return true;
    }
    bool StartArray() {
        if(records_start) {
            if(rel())
                writer.StartArray();

            array_depth++;
        }

        return true;
    }
    bool EndArray(SizeType elementCount) {
        if(records_start) {
            array_depth--;

            if(rel())
                writer.EndArray(elementCount);
        }

        return true;
    }

    void ObjectComplete(){
        cout << s.GetString() << endl;

        s.Clear();
        // Reset is required after Clear, since we're adopting a new root
        writer.Reset(s);
    }

    void Close(){
        if(s.GetSize() > 0)
            ObjectComplete();
    }
};

int main() {
    FILE* fh = fopen("/home/ilya/Downloads/cloudtrails.json","r");
    char buffer[1000];

    MyHandler handler;
    FileReadStream readStream(fh, buffer, 1000);
    Reader reader;

    reader.Parse(readStream, handler);
    handler.Close();

    fclose(fh);

    cout << "Hello, World!" << endl;
    return 0;
}